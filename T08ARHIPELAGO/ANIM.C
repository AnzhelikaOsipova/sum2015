/* FILENAME: ANIM.C
 * PROGRAMMER: AO5
 * PURPOSE: Animation system module.
 * LAST UPDATE: 10.06.2015
 */

#include "anim.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm")

/* Получение значения оси джойстика */
#define AO5_GET_AXIS_VALUE(Axis) \
  (2.0 * (ji.dw ## Axis ## pos - jc.w ## Axis ## min) / (jc.w ## Axis ## max - jc.w ## Axis ## min) - 1.0)

/* Системный контекст анимации */
ao5ANIM AO5_Anim;

/* Данные для синхронизации по времени */
static INT64
  TimeFreq,  /* единиц измерения в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время прошлого кадра */
  TimePause, /* время простоя в паузе */
  TimeFPS;   /* время для замера FPS */
static INT
  FrameCounter; /* счетчик кадров */

/* Сохраненные мышиные координаты */
static INT
  AO5_MouseOldX, AO5_MouseOldY;

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL AO5_AnimInit( HWND hWnd )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;
  PIXELFORMATDESCRIPTOR pfd = {0};

  memset(&AO5_Anim, 0, sizeof(ao5ANIM));
  AO5_Anim.hWnd = hWnd;
  AO5_Anim.hDC = GetDC(hWnd);

  /* Инициализируем буфер кадра */
  AO5_Anim.W = 30;
  AO5_Anim.H = 30;
  AO5_Anim.NumOfUnits = 0;

  /*** Инициализация OpenGL ***/

  /* описываем формат точки */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(AO5_Anim.hDC, &pfd);
  DescribePixelFormat(AO5_Anim.hDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(AO5_Anim.hDC, i, &pfd);

  /* создаем контекст построения */
  AO5_Anim.hGLRC = wglCreateContext(AO5_Anim.hDC);
  /* делаем текущими контексты */
  wglMakeCurrent(AO5_Anim.hDC, AO5_Anim.hGLRC);

  /* инициализируем расширения */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(AO5_Anim.hGLRC);
    ReleaseDC(AO5_Anim.hWnd, AO5_Anim.hDC);
    memset(&AO5_Anim, 0, sizeof(ao5ANIM));
    return FALSE;
  }

  glActiveTexture(GL_TEXTURE0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glActiveTexture(GL_TEXTURE1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  /* Инициализация таймера */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  AO5_Anim.IsPause = FALSE;
  FrameCounter = 0;

  /* Инициализация ввода */
  GetCursorPos(&pt);
  ScreenToClient(AO5_Anim.hWnd, &pt);
  AO5_MouseOldX = pt.x;
  AO5_MouseOldY = pt.y;
  GetKeyboardState(AO5_Anim.KeysOld);
  return TRUE;
} /* End of 'AO5_AnimInit' function */

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AO5_AnimClose( VOID )
{
  INT i;

  /* Освобождение памяти из-под объектов анимации */
  for (i = 0; i < AO5_Anim.NumOfUnits; i++)
  {
    AO5_Anim.Units[i]->Close(AO5_Anim.Units[i], &AO5_Anim);
    free(AO5_Anim.Units[i]);
    AO5_Anim.Units[i] = NULL;
  }

  /* делаем текущими контексты */
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(AO5_Anim.hGLRC);
  /* Удаляем объекты GDI */
  ReleaseDC(AO5_Anim.hWnd, AO5_Anim.hDC);
} /* End of 'AO5_AnimClose' function */

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AO5_AnimResize( INT W, INT H )
{
  /* Сохранение размера */
  AO5_Anim.W = W;
  AO5_Anim.H = H;

  glViewport(0, 0, W, H);

  /* корректировка параметров проецирования */
  if (W > H)
    AO5_RndWp = (DBL)W / H * 3, AO5_RndHp = 3;
  else
    AO5_RndHp = (DBL)H / W * 3, AO5_RndWp = 3;

  AO5_RndMatrProj = MatrFrustum(-AO5_RndWp / 2, AO5_RndWp / 2,
                                -AO5_RndHp / 2, AO5_RndHp / 2,
                                AO5_RndProjDist, 100000);

} /* End of 'AO5_AnimResize' function */

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AO5_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;

  /*** Обновление таймера ***/
  QueryPerformanceCounter(&li);
  AO5_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  AO5_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  if (!AO5_Anim.IsPause)
    AO5_Anim.DeltaTime = AO5_Anim.GlobalDeltaTime;
  else
  {
    TimePause += li.QuadPart - TimeOld;
    AO5_Anim.DeltaTime = 0;
  }

  AO5_Anim.Time = (DBL)(li.QuadPart - TimePause - TimeStart) / TimeFreq;

  /* вычисляем FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    AO5_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* время "прошлого" кадра */
  TimeOld = li.QuadPart;

  /*** Обновление ввода ***/

  /* Клавиатура */
  GetKeyboardState(AO5_Anim.Keys);
  for (i = 0; i < 256; i++)
    AO5_Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    AO5_Anim.KeysClick[i] = AO5_Anim.Keys[i] && !AO5_Anim.KeysOld[i];
  memcpy(AO5_Anim.KeysOld, AO5_Anim.Keys, sizeof(AO5_Anim.KeysOld));

  /* Мышь */
  /* колесо */
  AO5_Anim.MsWheel = AO5_MouseWheel;
  AO5_MouseWheel = 0;
  /* абсолютная позиция */
  GetCursorPos(&pt);
  ScreenToClient(AO5_Anim.hWnd, &pt);
  AO5_Anim.MsX = pt.x;
  AO5_Anim.MsY = pt.y;
  /* относительное перемещение */
  AO5_Anim.MsDeltaX = pt.x - AO5_MouseOldX;
  AO5_Anim.MsDeltaY = pt.y - AO5_MouseOldY;
  AO5_MouseOldX = pt.x;
  AO5_MouseOldY = pt.y;

  /* Джойстик */
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* получение общей информации о джостике */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* получение текущего состояния */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Кнопки */
        memcpy(AO5_Anim.JButsOld, AO5_Anim.JButs, sizeof(AO5_Anim.JButs));
        for (i = 0; i < 32; i++)
          AO5_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          AO5_Anim.JButsClick[i] = AO5_Anim.JButs[i] && !AO5_Anim.JButsOld[i];

        /* Оси */
        AO5_Anim.JX = AO5_GET_AXIS_VALUE(X);
        AO5_Anim.JY = AO5_GET_AXIS_VALUE(Y);
        if (jc.wCaps & JOYCAPS_HASZ)
          AO5_Anim.JZ = AO5_GET_AXIS_VALUE(Z);
        if (jc.wCaps & JOYCAPS_HASU)
          AO5_Anim.JU = AO5_GET_AXIS_VALUE(U);
        if (jc.wCaps & JOYCAPS_HASV)
          AO5_Anim.JV = AO5_GET_AXIS_VALUE(V);
        if (jc.wCaps & JOYCAPS_HASR)
          AO5_Anim.JR = AO5_GET_AXIS_VALUE(R);

        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            AO5_Anim.JPOV = 0;
          else
            AO5_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }

  /* опрос на изменение состояний объектов */
  for (i = 0; i < AO5_Anim.NumOfUnits; i++)
    AO5_Anim.Units[i]->Response(AO5_Anim.Units[i], &AO5_Anim);

  /* очистка фона */
  glClearColor(0, 0, 0, 1);
  glClearDepth(1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* рисование объектов */
  for (i = 0; i < AO5_Anim.NumOfUnits; i++)
  {

    AO5_RndMatrWorld = MatrIdentity();

    AO5_Anim.Units[i]->Render(AO5_Anim.Units[i], &AO5_Anim);
  }
  glFinish();
  FrameCounter++;
} /* End of 'AO5_AnimRender' function */

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AO5_AnimCopyFrame( VOID )
{
  /* вывод буфера кадра */
  SwapBuffers(AO5_Anim.hDC);
} /* End of 'AO5_AnimCopyFrame' function */

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       AO5UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AO5_AnimAddUnit( ao5UNIT *Unit )
{
  if (AO5_Anim.NumOfUnits < AO5_MAX_UNITS)
  {
    AO5_Anim.Units[AO5_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &AO5_Anim);
  }
} /* End of 'AO5_AnimAddUnit' function */


/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AO5_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* текущий режим */
  static RECT SaveRC;               /* сохраненный размер */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* сохраняем старый размер окна */
    GetWindowRect(AO5_Anim.hWnd, &SaveRC);

    /* определяем в каком мониторе находится окно */
    hmon = MonitorFromWindow(AO5_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* получаем информацию для монитора */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* переходим в полный экран */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(AO5_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(AO5_Anim.hWnd, HWND_TOPMOST,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* восстанавливаем размер окна */
    SetWindowPos(AO5_Anim.hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'AO5_AnimFlipFullScreen' function */

/* Функция выхода из анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AO5_AnimDoExit( VOID )
{
  PostMessage(AO5_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'AO5_AnimDoExit' function */

/* Функция установки паузы анимации.
 * АРГУМЕНТЫ:
 *   - флаг паузы:
 *       BOOL NewPauseFlag;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AO5_AnimSetPause( BOOL NewPauseFlag )
{
  AO5_Anim.IsPause = NewPauseFlag;
} /* End of 'AO5_AnimSetPause' function */

/* END OF 'ANIM.C' FILE */

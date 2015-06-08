/* FILENAME: ANIM.C
 * PROGRAMMER: VG4
 * PURPOSE: Animation system module.
 * LAST UPDATE: 08.06.2015
 */

#include "anim.h"

/* Системный контекст анимации */
static ao5ANIM AO5_Anim;
/* Данные для синхронизации по времени */
static INT64
  TimeFreq,  /* единиц измерения в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время прошлого кадра */
  TimePause, /* время простоя в паузе */
  TimeFPS;   /* время для замера FPS */
static INT
  FrameCounter; /* счетчик кадров */

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AO5_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;

  AO5_Anim.hWnd = hWnd;
  /* Инициализируем буфер кадра */
  AO5_Anim.hDC = CreateCompatibleDC(hDC);
  AO5_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  SelectObject(AO5_Anim.hDC, AO5_Anim.hBmFrame);
  AO5_Anim.W = 30;
  AO5_Anim.H = 30;
  AO5_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  //LARGE_INTEGER li;
  /* Инициализация таймера */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  AO5_Anim.IsPause = FALSE;
  FrameCounter = 0;
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

  /* Удаляем объекты GDI */
  DeleteObject(AO5_Anim.hBmFrame);
  DeleteDC(AO5_Anim.hDC);
} /* End of 'AO5_AnimClose' function */

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AO5_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(AO5_Anim.hWnd);

  /* Перевыделение памяти у буфера кадра */
  DeleteObject(AO5_Anim.hBmFrame);
  AO5_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(AO5_Anim.hDC, AO5_Anim.hBmFrame);

  /* Сохранение размера */
  AO5_Anim.W = W;
  AO5_Anim.H = H;

  ReleaseDC(AO5_Anim.hWnd, hDC);
} /* End of 'AO5_AnimResize' function */

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AO5_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;

  if (GetAsyncKeyState('P') & 0x8000)
    AO5_AnimSetPause(TRUE);
  if (GetAsyncKeyState('O') & 0x8000)
    AO5_AnimSetPause(FALSE);
  /* опрос на изменение состояний объектов */
  for (i = 0; i < AO5_Anim.NumOfUnits; i++)
    AO5_Anim.Units[i]->Response(AO5_Anim.Units[i], &AO5_Anim);

  /* очистка фона */
  SelectObject(AO5_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(AO5_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(AO5_Anim.hDC, RGB(0, 0, 0));
  Rectangle(AO5_Anim.hDC, 0, 0, AO5_Anim.W + 1, AO5_Anim.H + 1);

  /* рисование объектов */
  for (i = 0; i < AO5_Anim.NumOfUnits; i++)
  {
    SelectObject(AO5_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(AO5_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(AO5_Anim.hDC, RGB(255, 255, 255));
    SetDCPenColor(AO5_Anim.hDC, RGB(0, 0, 0));

    AO5_Anim.Units[i]->Render(AO5_Anim.Units[i], &AO5_Anim);
  }
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
    static CHAR Buf[100];

    sprintf(Buf, "FPS: %.5f", AO5_Anim.FPS);
    SetWindowText(AO5_Anim.hWnd, Buf);

    AO5_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* время "прошлого" кадра */
  TimeOld = li.QuadPart;

  FrameCounter++;
} /* End of 'AO5_AnimRender' function */

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AO5_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* вывод кадра */
  hDC = GetDC(AO5_Anim.hWnd);
  BitBlt(hDC, 0, 0, AO5_Anim.W, AO5_Anim.H, AO5_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(AO5_Anim.hWnd, hDC);
} /* End of 'AO5_AnimCopyFrame' function */

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       ao5UNIT *Unit;
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
} /* End of 'VG4_AnimSetPause' function */


/* END OF 'ANIM.C' FILE */

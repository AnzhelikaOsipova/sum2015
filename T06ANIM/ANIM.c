/* FILENAME: ANIM.C
 * PROGRAMMER: VG4
 * PURPOSE: Animation system module.
 * LAST UPDATE: 08.06.2015
 */

#include "anim.h"

/* ��������� �������� �������� */
static ao5ANIM AO5_Anim;
/* ������ ��� ������������� �� ������� */
static INT64
  TimeFreq,  /* ������ ��������� � ������� */
  TimeStart, /* ����� ������ �������� */
  TimeOld,   /* ����� �������� ����� */
  TimePause, /* ����� ������� � ����� */
  TimeFPS;   /* ����� ��� ������ FPS */
static INT
  FrameCounter; /* ������� ������ */

/* ������� ������������� ��������.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 * ������������ ��������: ���.
 */
VOID AO5_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;

  AO5_Anim.hWnd = hWnd;
  /* �������������� ����� ����� */
  AO5_Anim.hDC = CreateCompatibleDC(hDC);
  AO5_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  SelectObject(AO5_Anim.hDC, AO5_Anim.hBmFrame);
  AO5_Anim.W = 30;
  AO5_Anim.H = 30;
  AO5_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  //LARGE_INTEGER li;
  /* ������������� ������� */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  AO5_Anim.IsPause = FALSE;
  FrameCounter = 0;
} /* End of 'AO5_AnimInit' function */

/* ������� ��������������� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AO5_AnimClose( VOID )
{
  INT i;

  /* ������������ ������ ��-��� �������� �������� */
  for (i = 0; i < AO5_Anim.NumOfUnits; i++)
  {
    AO5_Anim.Units[i]->Close(AO5_Anim.Units[i], &AO5_Anim);
    free(AO5_Anim.Units[i]);
    AO5_Anim.Units[i] = NULL;
  }

  /* ������� ������� GDI */
  DeleteObject(AO5_Anim.hBmFrame);
  DeleteDC(AO5_Anim.hDC);
} /* End of 'AO5_AnimClose' function */

/* ������� ��������� ��������� ������� ������� ������.
 * ���������:
 *   - ����� ������ ������� ������:
 *       INT W, H;
 * ������������ ��������: ���.
 */
VOID AO5_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(AO5_Anim.hWnd);

  /* ������������� ������ � ������ ����� */
  DeleteObject(AO5_Anim.hBmFrame);
  AO5_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(AO5_Anim.hDC, AO5_Anim.hBmFrame);

  /* ���������� ������� */
  AO5_Anim.W = W;
  AO5_Anim.H = H;

  ReleaseDC(AO5_Anim.hWnd, hDC);
} /* End of 'AO5_AnimResize' function */

/* ������� ���������� ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AO5_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;

  if (GetAsyncKeyState('P') & 0x8000)
    AO5_AnimSetPause(TRUE);
  if (GetAsyncKeyState('O') & 0x8000)
    AO5_AnimSetPause(FALSE);
  /* ����� �� ��������� ��������� �������� */
  for (i = 0; i < AO5_Anim.NumOfUnits; i++)
    AO5_Anim.Units[i]->Response(AO5_Anim.Units[i], &AO5_Anim);

  /* ������� ���� */
  SelectObject(AO5_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(AO5_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(AO5_Anim.hDC, RGB(0, 0, 0));
  Rectangle(AO5_Anim.hDC, 0, 0, AO5_Anim.W + 1, AO5_Anim.H + 1);

  /* ��������� �������� */
  for (i = 0; i < AO5_Anim.NumOfUnits; i++)
  {
    SelectObject(AO5_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(AO5_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(AO5_Anim.hDC, RGB(255, 255, 255));
    SetDCPenColor(AO5_Anim.hDC, RGB(0, 0, 0));

    AO5_Anim.Units[i]->Render(AO5_Anim.Units[i], &AO5_Anim);
  }
  /*** ���������� ������� ***/
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

  /* ��������� FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    static CHAR Buf[100];

    sprintf(Buf, "FPS: %.5f", AO5_Anim.FPS);
    SetWindowText(AO5_Anim.hWnd, Buf);

    AO5_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* ����� "��������" ����� */
  TimeOld = li.QuadPart;

  FrameCounter++;
} /* End of 'AO5_AnimRender' function */

/* ������� ������ ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AO5_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* ����� ����� */
  hDC = GetDC(AO5_Anim.hWnd);
  BitBlt(hDC, 0, 0, AO5_Anim.W, AO5_Anim.H, AO5_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(AO5_Anim.hWnd, hDC);
} /* End of 'AO5_AnimCopyFrame' function */

/* ������� ���������� � ������� ������� ��������.
 * ���������:
 *   - ����������� ������ ��������:
 *       ao5UNIT *Unit;
 * ������������ ��������: ���.
 */
VOID AO5_AnimAddUnit( ao5UNIT *Unit )
{
  if (AO5_Anim.NumOfUnits < AO5_MAX_UNITS)
  {
    AO5_Anim.Units[AO5_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &AO5_Anim);
  }
} /* End of 'AO5_AnimAddUnit' function */


/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ���������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AO5_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* ������� ����� */
  static RECT SaveRC;               /* ����������� ������ */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* ��������� ������ ������ ���� */
    GetWindowRect(AO5_Anim.hWnd, &SaveRC);

    /* ���������� � ����� �������� ��������� ���� */
    hmon = MonitorFromWindow(AO5_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* �������� ���������� ��� �������� */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* ��������� � ������ ����� */
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
    /* ��������������� ������ ���� */
    SetWindowPos(AO5_Anim.hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'AO5_AnimFlipFullScreen' function */

/* ������� ������ �� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AO5_AnimDoExit( VOID )
{
  PostMessage(AO5_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'AO5_AnimDoExit' function */

/* ������� ��������� ����� ��������.
 * ���������:
 *   - ���� �����:
 *       BOOL NewPauseFlag;
 * ������������ ��������: ���.
 */
VOID AO5_AnimSetPause( BOOL NewPauseFlag )
{
  AO5_Anim.IsPause = NewPauseFlag;
} /* End of 'VG4_AnimSetPause' function */


/* END OF 'ANIM.C' FILE */

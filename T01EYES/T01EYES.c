/* T01FWIN.C
 * ������ �������� ������ ��������� ��� WinAPI.
 * �������� � ��������� ����������� ����.
 */

#include <windows.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/* ��� ������ ���� */
#define WND_CLASS_NAME "My window class"

/* ������ ������ */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );

/* ������� ������� ���������.
 *   - ���������� ���������� ����������:
 *       HINSTANCE hInstance;
 *   - ���������� ����������� ���������� ����������
 *     (�� ������������ � ������ ���� NULL):
 *       HINSTANCE hPrevInstance;
 *   - ��������� ������:
 *       CHAR *CmdLine;
 * ������������ ��������:
 *   (INT) ��� �������� � ������������ �������.
 *   0 - ��� ������.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  /* HINSTANCE hIns = LoadLibrary("shell32"); */

  /* ����������� ������ ���� */
  wc.style = CS_VREDRAW | CS_HREDRAW; /* ����� ����: ��������� ��������������
                                       * ��� ��������� ������������� ���
                                       * ��������������� ��������
                                       * ��� ����� CS_DBLCLKS ��� ����������
                                       * ��������� �������� ������� */
  wc.cbClsExtra = 0; /* �������������� ���������� ���� ��� ������ */
  wc.cbWndExtra = 0; /* �������������� ���������� ���� ��� ���� */
  wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 0));
  wc.hCursor = LoadCursor(NULL, IDC_HAND); /* �������� ������� (����������) */
  wc.hIcon = LoadIcon(NULL, IDI_ASTERISK); /* �������� ����������� (���������) */
  wc.hInstance = hInstance; /* ���������� ����������, ��������������� ����� */
  wc.lpszMenuName = NULL; /* ��� ������� ���� */
  wc.lpfnWndProc = MyWindowFunc; /* ��������� �� ������� ��������� */
  wc.lpszClassName = WND_CLASS_NAME;

  /* ����������� ������ � ������� */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  /* �������� ���� */
  hWnd =
    CreateWindow(WND_CLASS_NAME,    /* ��� ������ ���� */
      "Title",                      /* ��������� ���� */
      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,          /* ����� ���� - ���� ������ ���� */
      CW_USEDEFAULT, CW_USEDEFAULT, /* ������� ���� (x, y) - �� ��������� */
      800, 800, /* ������� ���� (w, h) - �� ��������� */
      NULL,                         /* ���������� ������������� ���� */
      NULL,                         /* ���������� ������������ ���� */
      hInstance,                    /* ���������� ���������� */
      NULL);                        /* ��������� �� �������������� ��������� */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /* ������ ����� ��������� ���� */
  while (GetMessage(&msg, NULL, 0, 0))
  {
    /* ��������� ��������� �� ���������� */
    TranslateMessage(&msg);
    /* �������� ��������� � ������� ���� */
    DispatchMessage(&msg);
  }

  return msg.wParam;
} /* End of 'WinMain' function */

#define sqr(X) ((X) * (X))

/* ��������� ����.
 * ���������:
 *   - ���������� ��������� ���������:
 *       HDC hDC;
 *   - ������� ����:
 *       INT W, H;
 *   - ������� ����:
 *       INT Xc, Yc;
 * ������������ ��������: ���.
 */
VOID DrawEye( HDC hDC, INT W, INT H, INT Xc, INT Yc, INT PosX, INT PosY)
{
  FLOAT
    len = sqrt(sqr(Xc - (PosX + W / 2)) + sqr(Yc - (PosY + H / 2))),
    co = (Xc - (PosX + W / 2)) / len, si = (Yc - (PosY + H / 2)) / len;
  INT l = 30, x, y;

  l = len;
  if (l > W / 2 - W / 8)
    l = W / 2 - W / 8;
  x = W / 2 + co * l;
  y = H / 2 + si * l;


  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 255, 255));
  Ellipse(hDC, PosX, PosY, PosX + W, PosY + H);
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(2, 55, 5));
  Ellipse(hDC, x - W / 8 + PosX, y - H / 8 + PosY, x + W / 8 + PosX, y + H / 8 + PosY);

} /* End of 'DrawEye' function */


/* ������� ��������� ��������� ����.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 *   - ����� ��������� (��. WM_***):
 *       UINT Msg;
 *   - �������� ��������� ('word parameter'):
 *       WPARAM wParam;
 *   - �������� ��������� ('long parameter'):
 *       LPARAM lParam;
 * ������������ ��������:
 *   (LRESULT) - � ����������� �� ���������.
 */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  POINT pt;
  PAINTSTRUCT ps;
  static INT w, h;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 111, 50, NULL);
    return 0;

  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    return 0;

  case WM_TIMER:
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;

  case WM_COMMAND:
    if (LOWORD(wParam) == 1)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;

  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you shure to exit from program?",
          "Exit", MB_YESNO | MB_ICONQUESTION) == IDNO)
       return 0;
    break;

  case WM_ERASEBKGND:
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    SelectObject(hDC, GetStockObject(NULL_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC, RGB(255, 200, 30));
    Rectangle(hDC, 0, 0, w + 1, h + 1);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    DrawEye(hDC, 200, 200, pt.x, pt.y, 100, 100);
    DrawEye(hDC, 200, 200, pt.x, pt.y, w - 300, 100);
    /*
    Ellipse(hDC, 10, 10, w - 10, h - 10);
    Ellipse(hDC, 10 + x, h / 2 - 10, x - 10, h / 2 + 10);
    */
    EndPaint(hWnd, &ps);
    return 0;

  case WM_DESTROY:
    KillTimer(hWnd, 111);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'T01FWIN.C' FILE */

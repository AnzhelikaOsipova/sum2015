/* T01FWIN.C
 * Пример базового макета программы под WinAPI.
 * Создание и поддержка простейшего окна.
 */

#include <windows.h>

/* Имя класса окна */
#define WND_CLASS_NAME "My window class"

/* Ссылка вперед */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );

/* Главная функция программы.
 *   - дескриптор экземпляра приложения:
 *       HINSTANCE hInstance;
 *   - дескриптор предыдущего экземпляра приложения
 *     (не используется и должно быть NULL):
 *       HINSTANCE hPrevInstance;
 *   - командная строка:
 *       CHAR *CmdLine;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) код возврата в операционную систему.
 *   0 - при успехе.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  /* HINSTANCE hIns = LoadLibrary("shell32"); */

  /* Регистрация класса окна */
  wc.style = CS_VREDRAW | CS_HREDRAW; /* Стиль окна: полностью перерисовывать
                                       * при изменении вертикального или
                                       * горизонтального размеров
                                       * еще можно CS_DBLCLKS для добавления
                                       * отработки двойного нажатия */
  wc.cbClsExtra = 0; /* Дополнительное количество байт для класса */
  wc.cbWndExtra = 0; /* Дополнительное количество байт для окна */
  wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 0));
  wc.hCursor = LoadCursor(NULL, IDC_HAND); /* Загрузка курсора (системного) */
  wc.hIcon = LoadIcon(NULL, IDI_ASTERISK); /* Загрузка пиктограммы (системной) */
  wc.hInstance = hInstance; /* Дескриптор приложения, регистрирующего класс */
  wc.lpszMenuName = NULL; /* Имя ресурса меню */
  wc.lpfnWndProc = MyWindowFunc; /* Указатель на функцию обработки */
  wc.lpszClassName = WND_CLASS_NAME;

  /* Регистрация класса в системе */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  /* Создание окна */
  hWnd =
    CreateWindow(WND_CLASS_NAME,    /* Имя класса окна */
      "Title",                      /* Заголовок окна */
      WS_OVERLAPPEDWINDOW,          /* Стили окна - окно общего вида */
      CW_USEDEFAULT, CW_USEDEFAULT, /* Позиция окна (x, y) - по умолчанию */
      CW_USEDEFAULT, CW_USEDEFAULT, /* Размеры окна (w, h) - по умолчанию */
      NULL,                         /* Дескриптор родительского окна */
      NULL,                         /* Дескриптор загруженного меню */
      hInstance,                    /* Дескриптор приложения */
      NULL);                        /* Указатель на дополнительные параметры */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /* Запуск цикла сообщений окна */
  while (GetMessage(&msg, NULL, 0, 0))
    /* Передача сообщений в функцию окна */
    DispatchMessage(&msg);

  return 30;
} /* End of 'WinMain' function */

/* Функция обработки сообщения окна.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 *   - номер сообщения (см. WM_***):
 *       UINT Msg;
 *   - параметр сообшения ('word parameter'):
 *       WPARAM wParam;
 *   - параметр сообшения ('long parameter'):
 *       LPARAM lParam;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (LRESULT) - в зависимости от сообщения.
 */
#define sqr(X) ((X) * (X))

/*VOID DrawEye( HWND hWnd, HDC hDC, INT W, INT H, INT Xc, INT Yc )
{
  POINT pt;
  FLOAT
    len;// = sqrt(sqr(Xc - W / 2) + sqr(Yc - H / 2)),
    //co = (Xc - W / 2) / len, si = (Yc - H / 2) / len;
  INT l = 30, x, y;

  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);
  
  l = len;
  if (l > W / 2 - W / 8)
    l = W / 2 - W / 8;
  x = W / 2 + co * l;
  y = H / 2 + si * l;  
  len = (H / 8) / sqrt(sqr(pt.x) + sqr(pt.y));
  x = Xc * len - H / 8;
  y = Yc * len - H / 8;

  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 255, 255));
  Ellipse(hDC, Xc - W / 2, Yc - H / 2, Xc + W / 2, Yc + H / 2);
  //SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(0, 0, 0));
  Ellipse(hDC, x - H / 8, y - H / 8, x + H / 8, y + H / 8);

}*/ /* End of 'DrawEye' function */
void DrawEye( HDC hDC, int left, int top, int right, int bottom )
{
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(0, 0, 0));
  Ellipse(hDC, left, top, right, bottom);
} /* End of 'DrawEye' function */


LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  static INT w, h;
  INT x, y;
  POINT pt; 
  INT CEyeF = 240, CEyeFy = 240, CEyeS = 560, CEyeSy = 240, R = 65;
  INT x1, y1, x2, y2; /* The coordinats of center */

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
    hDC = GetDC(hWnd);
    //DrawEye(hWnd, hDC, 100, 100, 150, 150);
    //r = 30;
    //xs = 145 + h / 4;
    //ys = 5 + h / 4;
    //Ellipse(hDC, 150, 10, h / 2 + 140, h / 2);
    //Ellipse(hDC, w - h / 2 - 140, 10, w - 10 - 140, h / 2);
    
    //SetDCBrushColor(hDC, RGB(0, 0, 0));
    //SelectObject(hDC, GetStockObject(DC_BRUSH));
    //Ellipse(hDC, h / 4 - 10 - r, h / 4 - 10 + r, h / 4 - 10 + r, h / 4 - 10 - r);
    //Ellipse(hDC, w - ((w - 10) - (w - h / 2)) / 2 - 10 - r, h / 4 - 10 + r, w - ((w - 10) - (w - h / 2)) / 2 - 10 + r, h / 4 - 10 - r);
    //Ellipse(hDC, h / 4 - r, h / 4 - r, h / 4 + r, h / 4 - r);

    //k = r / sqrt(pt.x * pt.x + pt.y * pt.y * 1.0);
    //(pt.y - r) < 10 ? ((T = 10), (B = 10 + 2 * r)) : ((pt.y + r) > (h / 2) ? ((B = h / 2), (T = h / 2 - 2 * r)) : ((T = pt.y - r), (B = pt.y + r)));
    //(pt.x + r) > (h / 2) ? ((R = h / 2), (L = h / 2 - 2 * r)) : ((pt.x - r) < 10 ? ((L = 10), (R = 10 + 2 * r)) : ((R = pt.x + r), (L = pt.x - r)));  
    
    //Ellipse(hDC, h / 4 - 10 - r + pt.x * k, h / 4 - 10 + r + pt.y * k, h / 4 - 10 + r + pt.x * k, h / 4 - 10 - r + pt.y * k);

    //(pt.x + r) > (w - 10) ? ((R = w - 10), (L = w - 10 - 2 * r)) : ((pt.x -  r) < (w - h / 2) ? ((L = (w - h / 2)), (R = w - h / 2 + 2 * r)) : ((R = pt.x + r), (L = pt.x - r)));
    //Ellipse(hDC, L, T, R, B);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    x = pt.x;
    y = pt.y;

    Ellipse(hDC, 160, 160, 321, 321);
    Ellipse(hDC, 480, 160, 641, 321);

    x1 = R * abs(CEyeF - x) / (INT)sqrt(sqr(CEyeF - x) + sqr(CEyeFy - y));
    y1 = R * abs(CEyeFy - y) / (INT)sqrt(sqr(CEyeF - x) + sqr(CEyeFy - y));
    x2 = R * abs(CEyeS - x) / (INT)sqrt(sqr(CEyeS - x) + sqr(CEyeSy - y));
    y2 = R * abs(CEyeSy - y) / (INT)sqrt(sqr(CEyeS - x) + sqr(CEyeSy - y));

    if (x > CEyeF && x < CEyeS)
      x2 = -x2;
    if (x < CEyeF)
      x1 = -x1, x2 = -x2;
    if (y < CEyeFy)
      y1 = -y1, y2 = -y2;

    SelectObject(hDC, GetStockObject(DC_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC, RGB(0, 0, 0));
    SetDCPenColor(hDC, RGB(0, 0, 0));
    DrawEye(hDC, CEyeF + x1 - 16, CEyeFy + y1 - 16, CEyeF + x1 + 16, CEyeFy + y1 + 16);
    DrawEye(hDC, CEyeS + x2 - 16, CEyeSy + y2 - 16, CEyeS + x2 + 16, CEyeSy + y2 + 16);


    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, 111);
    PostMessage(hWnd, WM_QUIT, 0, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */
/* END OF 'T01FWIN.C' FILE */

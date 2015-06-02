#include <windows.h>

void main(void)
{
  if(MessageBox(NULL, "Text", "Mess", MB_YESNO | MB_ICONQUESTION) == IDYES)
    MessageBox(NULL, "You Pressed Yes", "Mess", MB_OK);
  else
    MessageBox(NULL, "You Pressed No", "Mess", MB_OK);

}
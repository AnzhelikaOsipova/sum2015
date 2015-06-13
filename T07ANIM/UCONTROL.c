/* FILENAME: UCONTRL.C
 * PROGRAMMER: AO5
 * PURPOSE: Control unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"

/* Тип представления мяча */
typedef struct tagao5UNIT_CTRL
{
  AO5_UNIT_BASE_FIELDS;
  HFONT hFnt; /* Шрифт для вывода FPS */
} ao5UNIT_CTRL;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT_CTRL *Uni;
 *   - указатель на контекст анимации:
 *       AO5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitInit( ao5UNIT_CTRL *Uni, ao5ANIM *Ani )
{
  Uni->hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE,
    FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
    VARIABLE_PITCH, "Bookman Old Style");
} /* End of 'AO5_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT_CTRL *Uni;
 *   - указатель на контекст анимации:
 *       AO5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitClose( ao5UNIT_CTRL *Uni, ao5ANIM *Ani )
{
  DeleteObject(Uni->hFnt);
} /* End of 'AO5_AnimUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT_CTRL *Uni;
 *   - указатель на контекст анимации:
 *       AO5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitResponse( ao5UNIT_CTRL *Uni, ao5ANIM *Ani )
{
  if (Ani->Keys[VK_ESCAPE])
    AO5_AnimDoExit();
  if (Ani->KeysClick['F'])
    AO5_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    AO5_AnimSetPause(!Ani->IsPause);
} /* End of 'AO5_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT_CTRL *Uni;
 *   - указатель на контекст анимации:
 *       AO5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitRender( ao5UNIT_CTRL *Uni, ao5ANIM *Ani )
{
  HFONT hFntOld = SelectObject(Ani->hDC, Uni->hFnt);
  static DBL count = 30;
  static CHAR Buf[1000];

  count += Ani->GlobalDeltaTime;
  if (count > 1)
  {
    count = 0;
    sprintf(Buf, "FPS: %.3f", Ani->FPS);
    SetWindowText(Ani->hWnd, Buf);
  }
} /* End of 'AO5_AnimUnitRender' function */

/* Функция создания объекта анимации "управление".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (AO5UNIT *) указатель на созданный объект анимации.
 */
ao5UNIT * AO5_UnitControlCreate( VOID )
{
  ao5UNIT_CTRL *Uni;

  if ((Uni = (VOID *)AO5_AnimUnitCreate(sizeof(ao5UNIT_CTRL))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)AO5_AnimUnitInit;
  Uni->Close = (VOID *)AO5_AnimUnitClose;
  Uni->Response = (VOID *)AO5_AnimUnitResponse;
  Uni->Render = (VOID *)AO5_AnimUnitRender;
  return (ao5UNIT *)Uni;
} /* End of 'AO5_UnitBallCreate' function */

/* END OF 'UCONTRL.C' FILE */

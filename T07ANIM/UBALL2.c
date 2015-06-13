/* FILENAME: UBALL.C
 * PROGRAMMER: AO5
 * PURPOSE: Reactangle unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"

/* Тип представления мяча */
typedef struct tagao5UNIT_BALL2
{
  AO5_UNIT_BASE_FIELDS;

  VEC Pos;     /* Позиция мяча */
  DWORD Color; /* Цвет мяча */
  DBL
    Amplitude,  /* Амплитуда */
    PhaseShift, /* Сдвиг по фазе */
    ScaleShift; /* Масштаб времени */
} ao5UNIT_BALL2;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT_BALL2 *Uni;
 *   - указатель на контекст анимации:
 *       ao5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitInit( ao5UNIT_BALL2 *Uni, ao5ANIM *Ani )
{
  Uni->Pos = VecSet(rand() % 1000, rand() % 700, 0);
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);
  Uni->PhaseShift = rand() % 3000;
  Uni->ScaleShift = 5 + 0.30 * rand() / RAND_MAX;
  Uni->Amplitude = 30 + 59.0 * rand() / RAND_MAX;
} /* End of 'AO5_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT_BALL2 *Uni;
 *   - указатель на контекст анимации:
 *       ao5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitClose( ao5UNIT_BALL2 *Uni, ao5ANIM *Ani )
{
} /* End of 'AO5_AnimUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT_BALL2 *Uni;
 *   - указатель на контекст анимации:
 *       ao5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitResponse( ao5UNIT_BALL2 *Uni, ao5ANIM *Ani )
{
} /* End of 'AO5_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT_BALL2 *Uni;
 *   - указатель на контекст анимации:
 *       ao5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitRender( ao5UNIT_BALL2 *Uni, ao5ANIM *Ani )
{
  DBL shift = Uni->Amplitude * fabs(sin(Uni->ScaleShift * (DBL)clock() / CLOCKS_PER_SEC + Uni->PhaseShift));
  
  SetDCBrushColor(Ani->hDC, Uni->Color);
  Rectangle(Ani->hDC, Uni->Pos.X - 10 - shift, Uni->Pos.Y - 5 + shift, Uni->Pos.X + 10 + shift, Uni->Pos.Y + 5 - shift);
} /* End of 'AO5_AnimUnitRender' function */

/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ao5UNIT *) указатель на созданный объект анимации.
 */
ao5UNIT * AO5_UnitBallCreate2( VOID )
{
  ao5UNIT_BALL2 *Uni;

  if ((Uni = (VOID *)AO5_AnimUnitCreate(sizeof(ao5UNIT_BALL2))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)AO5_AnimUnitInit;
  Uni->Close = (VOID *)AO5_AnimUnitClose;
  Uni->Response = (VOID *)AO5_AnimUnitResponse;
  Uni->Render = (VOID *)AO5_AnimUnitRender;
  return (ao5UNIT *)Uni;
} /* End of 'AO5_UnitBallCreate' function */

/* END OF 'UBALL.C' FILE */

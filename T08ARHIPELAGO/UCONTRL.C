/* FILENAME: UCONTRL.C
 * PROGRAMMER: AO5
 * PURPOSE: Control unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include <stdio.h>

#include "anim.h"

/* Тип представления мяча */
typedef struct tagao5UNIT_CTRL
{
  AO5_UNIT_BASE_FIELDS;
  HFONT hFnt; /* Шрифт для вывода FPS */
  VEC CPos, Pos, At;
  FLT Head, Omega, V;
} ao5UNIT_CTRL;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT_CTRL *Uni;
 *   - указатель на контекст анимации:
 *       ao5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitInit( ao5UNIT_CTRL *Uni, ao5ANIM *Ani )
{
  Uni->hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE,
    FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
    VARIABLE_PITCH, "Bookman Old Style");
  /* начальные параметры корабля */
  Uni->Pos = VecSet(0, 0, 0);
  Uni->CPos = VecSet(100, 30, 100);
  Uni->V = 0;
  Uni->Head = 0;
  Uni->Omega = 0;
} /* End of 'AO5_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT_CTRL *Uni;
 *   - указатель на контекст анимации:
 *       ao5ANIM *Ani;
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
 *       ao5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitResponse( ao5UNIT_CTRL *Uni, ao5ANIM *Ani )
{
  VEC Move, Dir, At;
  if (Ani->Keys[VK_ESCAPE])
    AO5_AnimDoExit();
  if (Ani->KeysClick['F'])
    AO5_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    AO5_AnimSetPause(!Ani->IsPause);
  /* позиционирование корабля */
  //Uni->Omega = -3000 * Ani->JX * Ani->DeltaTime;
  //Uni->Head += -3 * 30 * Ani->JR * Ani->DeltaTime;
  Dir = VectorTransform(VecSet(0, 0, 1), MatrRotateY(Uni->Head));
  //Uni->V += -3 * Ani->JY * Ani->DeltaTime;
  //Uni->V *= max(1 - Ani->GlobalDeltaTime, 0);
  if(Ani->Keys['D'])
    Uni->Head -= 1;
  if(Ani->Keys['A'])
    Uni->Head += 1;
  if(Ani->Keys['W'])
    Uni->V = -5;
  else if(Ani->Keys['S'])
    Uni->V = 5;
  else
    Uni->V = 0;
  Uni->Pos = VecAddVec(Uni->Pos, VecMulNum(Dir, Uni->V * Ani->DeltaTime)); 

  Uni->Pos = VecAddVec(Uni->Pos, VecMulNum(VectorTransform(Dir, MatrRotateY(-90)), Uni->V));
  //Uni->Pos.Y += 30 * (Ani->JButs[1] - Ani->JButs[2]) * Ani->DeltaTime; 

  Uni->At = VecSubVec(Uni->Pos, VecMulNum(Dir, 100));
  Uni->At.Y += 10;
  Move = VecSubVec(Uni->At, Uni->CPos);
  Uni->CPos = VecAddVec(Uni->CPos, VecMulNum(Move, Ani->DeltaTime));
  AO5_RndMatrView = MatrView(Uni->CPos,
                             Uni->Pos,
                             VecSet(0, 1, 0)); 
  AO5_RndMatrWorld = MatrMulMatr(MatrMulMatr(MatrRotateY(Uni->Head), MatrTranslate(Uni->Pos.X, Uni->Pos.Y, Uni->Pos.Z)), MatrScale(0.1, 0.1, 0.1)); 
} /* End of 'AO5_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT_CTRL *Uni;
 *   - указатель на контекст анимации:
 *       ao5ANIM *Ani;
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
 *   (ao5UNIT *) указатель на созданный объект анимации.
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

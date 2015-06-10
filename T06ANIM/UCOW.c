/* FILENAME: UBALL.C
 * PROGRAMMER: AO5
 * PURPOSE: Cow unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


#include "anim.h"
#include "render.h"

/* Тип представления мяча */
typedef struct tagao5UNIT_MODEL
{
  AO5_UNIT_BASE_FIELDS;
  ao5GOBJ Model;

} ao5UNIT_MODEL;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT_MODEL *Uni;
 *   - указатель на контекст анимации:
 *       ao5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

/* Draw object functioln.
 * ARGUMENTS:
 *   - object (*.OBJ) file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if loading is success, FALSE otherwise.
 */

/* END OF 'LOADOBJ.C' FILE */

static VOID AO5_AnimUnitInit( ao5UNIT_MODEL *Uni, ao5ANIM *Ani )
{
  AO5_RndGObjLoad( &Uni->Model, "cow.object" );
} /* End of 'AO5_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT_MODEL *Uni;
 *   - указатель на контекст анимации:
 *       ao5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitClose( ao5UNIT_MODEL *Uni, ao5ANIM *Ani )
{
  AO5_RndGObjFree( &Uni->Model );
} /* End of 'AO5_AnimUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT_MODEL *Uni;
 *   - указатель на контекст анимации:
 *       ao5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitResponse( ao5UNIT_MODEL *Uni, ao5ANIM *Ani )
{
  /*INT i;
  if(Ani->JPOV == 1 || Ani->Keys['W'] == 1)
    Uni->Y -= 10;
  if(Ani->JPOV == 3 || Ani->Keys['D'] == 1)
    Uni->X += 10;
  if(Ani->JPOV == 5 || Ani->Keys['S'] == 1)
    Uni->Y += 10;
  if(Ani->JPOV == 7 || Ani->Keys['A'] == 1)
    Uni->X -= 10;*/
  /*
  if(Ani->JZ == 1 || Ani->Keys['1'])
    for(i = 0; i < Uni->ObjNumOfV; i++)
      Uni->ObjV[i] = RotateX(Uni->ObjV[i], -1);
  if(Ani->JZ == -1 || Ani->Keys['2'])
    for(i = 0; i < Uni->ObjNumOfV; i++)
      Uni->ObjV[i] = RotateX(Uni->ObjV[i], 1);
  if(Ani->JX == 1 || Ani->Keys['3'])
    for(i = 0; i < Uni->ObjNumOfV; i++)
      Uni->ObjV[i] = RotateY(Uni->ObjV[i], 1);
  if(Ani->JX == -1 || Ani->Keys['4'])
    for(i = 0; i < Uni->ObjNumOfV; i++)
      Uni->ObjV[i] = RotateY(Uni->ObjV[i], -1);
  if(Ani->JY == 1 || Ani->Keys['5'])
    for(i = 0; i < Uni->ObjNumOfV; i++)
      Uni->ObjV[i] = RotateZ(Uni->ObjV[i], -1);
  if(Ani->JY == -1 || Ani->Keys['6'])
    for(i = 0; i < Uni->ObjNumOfV; i++)
      Uni->ObjV[i] = RotateZ(Uni->ObjV[i], 1);  */
  
} /* End of 'AO5_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT_MODEL *Uni;
 *   - указатель на контекст анимации:
 *       ao5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitRender( ao5UNIT_MODEL *Uni, ao5ANIM *Ani )
{
  SetDCBrushColor(Ani->hDC, RGB(255, 255, 255));

  AO5_RndMatrView = MatrView(VecSet(30, 30, 30),
                             VecSet(0, 0, 0),
                             VecSet(0, 1, 0));
  AO5_RndMatrWorld = 
    MatrMulMatr(MatrMulMatr(MatrMulMatr(MatrTranslate(Ani->JX * 59, Ani->JY * 88, 0), MatrScale(1, 1, 1)),
    MatrRotateY(30 * Ani->Time + Ani->JR * 180)),
    MatrTranslate(0, 0, 100 * Ani->JZ));
  //AO5_RndMatrProj = MatrFrustum(AO5_RndWp / 2, AO5_RndWp / 2, AO5_RndHp / 2, AO5_RndHp / 2, AO5_RndProjDist, 800);
  AO5_RndGObjDraw(&Uni->Model);
} /* End of 'AO5_AnimUnitRender' function */

/* Функция создания объекта анимации "модель".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ao5UNIT *) указатель на созданный объект анимации.
 */
ao5UNIT * AO5_UnitCowCreate( VOID )
{
  ao5UNIT_MODEL *Uni;

  if ((Uni = (VOID *)AO5_AnimUnitCreate(sizeof(ao5UNIT_MODEL))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)AO5_AnimUnitInit;
  Uni->Close = (VOID *)AO5_AnimUnitClose;
  Uni->Response = (VOID *)AO5_AnimUnitResponse;
  Uni->Render = (VOID *)AO5_AnimUnitRender;
  return (ao5UNIT *)Uni;
} /* End of 'AO5_UnitBallCreate' function */

/* END OF 'UBALL.C' FILE */

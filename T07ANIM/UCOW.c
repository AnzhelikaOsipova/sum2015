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

/* Тип представления модели */
typedef struct tagao5UNIT_MODEL
{
  AO5_UNIT_BASE_FIELDS;
  ao5GOBJ Model;
  INT X, Y, Z;
  INT RX, RY, RZ;
} ao5UNIT_MODEL;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT_MODEL *Uni;
 *   - указатель на контекст анимации:
 *       ao5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

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
  /*
  if(Ani->JPOV == 1 || Ani->Keys['W'] == 1)
    Uni->Y += 1;
  if(Ani->JPOV == 3 || Ani->Keys['D'] == 1)
    Uni->X += 1;
  if(Ani->JPOV == 5 || Ani->Keys['S'] == 1)
    Uni->Y -= 1;
  if(Ani->JPOV == 7 || Ani->Keys['A'] == 1)
    Uni->X -= 1;
  
  if(Ani->JZ == 1 || Ani->Keys['1'])
      Uni->RX = -1;
  if(Ani->JZ == -1 || Ani->Keys['2'])
      Uni->RX = 1;
  if(Ani->JX == 1 || Ani->Keys['3'])
      Uni->RY = 1;
  if(Ani->JX == -1 || Ani->Keys['4'])
      Uni->RY = -1;
  if(Ani->JY == 1 || Ani->Keys['5'])
      Uni->RZ = -1;
  if(Ani->JY == -1 || Ani->Keys['6']) 
     Uni->RZ = 1;  */
  
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
  INT i, j;

  AO5_RndMatrView = MatrView(VecSet(0, 0, 100),
                             VecSet(0, 0, 0),
                             VecSet(0, 1, 0));

  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);

  for (i = 0; i < 1; i++)
    for (j = 0; j < 1; j++)
    {    
      AO5_RndMatrWorld =
        MatrMulMatr(MatrMulMatr(MatrMulMatr(
          MatrTranslate(0, 0, 0),
          MatrScale(1, 1, 1)),
          MatrRotateY(30 * Ani->Time - 180)),
          MatrTranslate(j * 20.30, 0, i * 20.30 + 10));
      glColor3d(i & 1, j & 1, 1 - ((i & 1) + (j & 1)) / 2);
      AO5_RndGObjDraw(&Uni->Model);
    }
} /* End of 'AO5_AnimUnitRender' function */

/* Функция создания объекта анимации "модель".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ao5UNIT *) указатель на созданный объект анимации.
 */
ao5UNIT * AO5_UnitModelCreate( VOID )
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

/* END OF 'UMODEL.C' FILE */

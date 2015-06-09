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
#include "vec.h"

/* Тип представления мяча */
typedef struct tagao5UNIT_COW
{
  AO5_UNIT_BASE_FIELDS;

  VEC *ObjV; /* Vertex coordinates */
  INT ObjNumOfV; /* Number of model vertices */
  INT X, Y;

} ao5UNIT_COW;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT_COW *Uni;
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

static VOID AO5_AnimUnitInit( ao5UNIT_COW *Uni, ao5ANIM *Ani )
{
  FILE *F;
  INT nv = 0;
  static CHAR Buf[10000];

  /* Open file */
  if ((F = fopen("cow.object", "r")) == NULL)
     return;
  /* Count vertices */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
  }

  /* Allocate memory for data */
  if ((Uni->ObjV = malloc(sizeof(VEC) * nv)) == NULL)
  {
    fclose(F);
  }

  /* Read vertices */
  rewind(F);
  nv = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf",
        &Uni->ObjV[nv].X, &Uni->ObjV[nv].Y, &Uni->ObjV[nv].Z);        
      nv++;
    }
  }

  fclose(F);
  Uni->ObjNumOfV = nv;
  Uni->X = 300;
  Uni->Y = 300;
} /* End of 'AO5_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT_COW *Uni;
 *   - указатель на контекст анимации:
 *       ao5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitClose( ao5UNIT_COW *Uni, ao5ANIM *Ani )
{
} /* End of 'AO5_AnimUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT_COW *Uni;
 *   - указатель на контекст анимации:
 *       ao5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitResponse( ao5UNIT_COW *Uni, ao5ANIM *Ani )
{
  INT i;
  if(Ani->JPOV == 1 || Ani->Keys['W'] == 1)
    Uni->Y -= 10;
  if(Ani->JPOV == 3 || Ani->Keys['D'] == 1)
    Uni->X += 10;
  if(Ani->JPOV == 5 || Ani->Keys['S'] == 1)
    Uni->Y += 10;
  if(Ani->JPOV == 7 || Ani->Keys['A'] == 1)
    Uni->X -= 10;

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
      Uni->ObjV[i] = RotateZ(Uni->ObjV[i], 1);
  
} /* End of 'AO5_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT_COW *Uni;
 *   - указатель на контекст анимации:
 *       ao5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitRender( ao5UNIT_COW *Uni, ao5ANIM *Ani )
{
  int i;
  SetDCBrushColor(Ani->hDC, RGB(255, 255, 255));

  for (i = 0; i < Uni->ObjNumOfV; i++)
  {
    /* рисуем точку ObjV[i] */
    //Ellipse(Ani->hDC, Ani->MsX + Uni->ObjV[i].X * 50 - 2, Ani->MsY - Uni->ObjV[i].Y * 50 + 2, Ani->MsX + Uni->ObjV[i].X * 50 + 2, Ani->MsY - Uni->ObjV[i].Y * 50 - 2);
   Ellipse(Ani->hDC, Uni->X + Uni->ObjV[i].X * 50 - 2, Uni->Y - Uni->ObjV[i].Y * 50 + 2, Uni->X + Uni->ObjV[i].X * 50 + 2, Uni->Y - Uni->ObjV[i].Y * 50 - 2);
  }

} /* End of 'AO5_AnimUnitRender' function */

/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ao5UNIT *) указатель на созданный объект анимации.
 */
ao5UNIT * AO5_UnitCowCreate( VOID )
{
  ao5UNIT_COW *Uni;

  if ((Uni = (VOID *)AO5_AnimUnitCreate(sizeof(ao5UNIT_COW))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)AO5_AnimUnitInit;
  Uni->Close = (VOID *)AO5_AnimUnitClose;
  Uni->Response = (VOID *)AO5_AnimUnitResponse;
  Uni->Render = (VOID *)AO5_AnimUnitRender;
  return (ao5UNIT *)Uni;
} /* End of 'AO5_UnitBallCreate' function */

/* END OF 'UBALL.C' FILE */

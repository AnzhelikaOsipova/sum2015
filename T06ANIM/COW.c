/* FILENAME: UBALL.C
 * PROGRAMMER: VG4
 * PURPOSE: Bounce ball unit handle module.
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
  if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    AO5_AnimDoExit();
  if (GetAsyncKeyState('F') & 0x8000)
    AO5_AnimFlipFullScreen();
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
    if(Ani->IsPause == FALSE)
    {
      Uni->ObjV[i] = RotateY(Uni->ObjV[i], 1);
      Uni->ObjV[i] = RotateX(Uni->ObjV[i], 1);
      Uni->ObjV[i] = RotateZ(Uni->ObjV[i], 1);
    }
    Ellipse(Ani->hDC, 300 + Uni->ObjV[i].X * 50 - 2, 300 - Uni->ObjV[i].Y * 50 + 2, 300 + Uni->ObjV[i].X * 50 + 2, 300 - Uni->ObjV[i].Y * 50 - 2);
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

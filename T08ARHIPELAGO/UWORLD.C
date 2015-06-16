/* FILENAME: UWORLD.C
 * PROGRAMMER: AO5
 * PURPOSE: World view unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include <string.h>

#include "anim.h"


typedef struct tagao5UNIT_WORLD
{
  AO5_UNIT_BASE_FIELDS;

  ao5PRIM Sky; /* Модель для отображения неба */
  ao5PRIM Water; /* Модель для отображения воды */
  INT TextId;  /* Id текстуры */
} ao5UNIT_WORLD;
/* Функция по-умолчанию инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT *Uni;
 *   - указатель на контекст анимации:
 *       ao5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitInit( ao5UNIT_WORLD *Uni, ao5ANIM *Ani )
{
  AO5_MtlLib[1].TexId = AO5_TextureLoad("sky.bmp");
  AO5_MtlLib[2].TexId = AO5_TextureLoad("water3.bmp");
  Uni->Sky.Prog = AO5_ShaderLoad("SKY");
  Uni->Water.Prog = AO5_ShaderLoad("WATER");
  //AO5_PrimCreatePlane(&Uni->Water, VecSet(-10, 0, 10), VecSet(0, 0, -20), VecSet(20, 0, 0), 10, 10);
  AO5_PrimCreateHeightField(&Uni->Water, VecSet(-10, 0, 10), VecSet(0, 0, -20), VecSet(20, 0, 0), 1, "water10.bmp");
  AO5_PrimCreateSphere(&Uni->Sky, VecSet(0, 0, 0), 10, 100, 100);
  Uni->Sky.MtlNo = 1;
  Uni->Water.MtlNo = 2;
} /* End of 'AO5_AnimUnitInit' function */

/* Функция по-умолчанию деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT *Uni;
 *   - указатель на контекст анимации:
 *       ao5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitClose( ao5UNIT_WORLD *Uni, ao5ANIM *Ani )
{
  AO5_ShaderFree(Uni->Sky.Prog);
  AO5_PrimFree(&Uni->Sky);
  AO5_ShaderFree(Uni->Water.Prog);
  AO5_PrimFree(&Uni->Water);
} /* End of 'AO5_AnimUnitClose' function */

/* Функция по-умолчанию обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT *Uni;
 *   - указатель на контекст анимации:
 *       ao5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitResponse( ao5UNIT_WORLD *Uni, ao5ANIM *Ani )
{
} /* End of 'AO5_AnimUnitResponse' function */

/* Функция по-умолчанию построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ao5UNIT *Uni;
 *   - указатель на контекст анимации:
 *       ao5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitRender( ao5UNIT_WORLD *Uni, ao5ANIM *Ani )
{
  static DBL time = 5;
  time += AO5_Anim.GlobalDeltaTime;
  if (time > 5)
    {
      time = 0;
      AO5_ShaderFree(Uni->Sky.Prog);
      Uni->Sky.Prog = AO5_ShaderLoad("SKY");
      AO5_ShaderFree(Uni->Water.Prog);
      Uni->Water.Prog = AO5_ShaderLoad("WATER");
    }
  AO5_PrimDraw(&Uni->Sky);
  AO5_PrimDraw(&Uni->Water);
} /* End of 'AO5_AnimUnitRender' function */

/* Функция создания мира анимации.
 * АРГУМЕНТЫ:
 *   - размер структуры объекта анимации:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ao5UNIT *) указатель на созданный объект анимации.
 */
ao5UNIT * AO5_UnitWorldCreate( VOID )
{
  ao5UNIT *Uni;

  if ((Uni = (VOID *)AO5_AnimUnitCreate(sizeof(ao5UNIT_WORLD))) == NULL)
    return NULL;
  /* заполняем поля по-умолчанию */
  Uni->Init = AO5_AnimUnitInit;
  Uni->Close = AO5_AnimUnitClose;
  Uni->Response = AO5_AnimUnitResponse;
  Uni->Render = AO5_AnimUnitRender;
  return Uni;
} /* End of 'AO5_AnimUnitCreate' function */

/* END OF 'UWORLD.C' FILE */

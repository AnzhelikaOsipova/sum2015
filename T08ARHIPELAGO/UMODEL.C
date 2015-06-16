/* FILENAME: UMODEL.C
 * PROGRAMMER: AO5
 * PURPOSE: Ship model view unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include "anim.h"

/* Тип представления мяча */
typedef struct tagao5UNIT_MODEL
{
  AO5_UNIT_BASE_FIELDS;

  ao5GEOM Model; /* Модель для отображения */
  INT TextId;  /* Id текстуры */
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
  Uni->Model.Prog = AO5_ShaderLoad("SHIP");
  AO5_GeomLoad(&Uni->Model, "WarShip.g3d");
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
  INT i;
  AO5_ShaderFree(Uni->Model.Prog);
  Uni->Model.Prog = 0;
  for(i = 0; i < Uni->Model.NumOfPrimitives; i++)
        Uni->Model.Prims[i].Prog = Uni->Model.Prog;
  AO5_GeomFree(&Uni->Model);
} /* End of 'AO5_AnimUnitClose' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       AO5UNIT_MODEL *Uni;
 *   - указатель на контекст анимации:
 *       AO5ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AO5_AnimUnitRender( ao5UNIT_MODEL *Uni, ao5ANIM *Ani )
{
  INT i;
  static FLT x = 1, y = 0, z = 0, pov = 0;
  static DBL time = 5;
  AO5_RndMatrView = MatrView(VecSet(0, 30, 100),
                             VecSet(0, 10, 0),
                             VecSet(0, 1, 0));
  time += AO5_Anim.GlobalDeltaTime;
  if (time > 5)
    {
      time = 0;
      AO5_ShaderFree(Uni->Model.Prog);
      Uni->Model.Prog = AO5_ShaderLoad("SHIP");
      for(i = 0; i < Uni->Model.NumOfPrimitives; i++)
        Uni->Model.Prims[i].Prog = Uni->Model.Prog;
    }
  
  if (Ani->KeysClick['X'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Z'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);
  if (Ani->Keys['W'])
    z -= 1;
  if (Ani->Keys['S'])
    z += 1;
  if (Ani->Keys['A'])
    pov += 1;
  if (Ani->Keys['D'])
    pov -= 1;
  
  AO5_RndMatrWorld = MatrMulMatr(MatrMulMatr(MatrTranslate(0, -90, z), MatrRotateY(pov)), MatrScale(0.1, 0.1, 0.1)); 
  AO5_GeomDraw(&Uni->Model);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);
} /* End of 'AO5_AnimUnitRender' function */

/* Функция создания объекта анимации "корабль".
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
  Uni->Render = (VOID *)AO5_AnimUnitRender;
  return (ao5UNIT *)Uni;
} /* End of 'AO5_UnitModelCreate' function */

/* END OF 'UMODEL.C' FILE */

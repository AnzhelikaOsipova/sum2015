/* FILENAME: UMODEL.C
 * PROGRAMMER: AO5
 * PURPOSE: Model view unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include "anim.h"

/* Тип представления мяча */
typedef struct tagao5UNIT_MODEL
{
  AO5_UNIT_BASE_FIELDS;

  ao5GEOM Model; /* Модель для отображения */
  ao5GEOM Geom; /* Модель для отображения */
  ao5PRIM Pr;
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
  ao5VERTEX V[]= 
  {
    {{0, 0, 0}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{1, 0, 0}, {5, 0}, {0, 0, 1}, {1, 0, 1, 1}},
    {{0, 1, 0}, {0, 5}, {0, 0, 1}, {1, 1, 0, 1}},
    {{1, 1, 0}, {5, 5}, {0, 0, 1}, {1, 1, 0, 1}},
  };
  INT I[] = {0, 1, 2, 2, 1, 3};
  BYTE txt[2][2][3] =
  {
    {{255, 255, 255}, {0, 0, 0}},
    {{0, 0, 0}, {255, 255, 255}}
  };

  /* загружаем текстуру */
  //Uni->TextId = AO5_TextureLoad("M.BMP");

  AO5_PrimCreate(&Uni->Pr, AO5_PRIM_TRIMESH, 4, 6, V, I);

  //AO5_RndPrimMatrConvert = MatrRotateX(-90);
  //AO5_GeomLoad(&Uni->Model, "Models//Nissan_Pathfinder//Nissan_Pathfinder.g3d");
  AO5_GeomLoad(&Uni->Model, "Models//Avent//avent.g3d");
  //AO5_GeomLoad(&Uni->Model, "Models//BMW_M3_GTR.g3d");
  //AO5_GeomLoad(&Uni->Model, "WarShip.g3d");


  //AO5_GeomLoad(&Uni->Geom, "X6.G3D");
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
  AO5_GeomFree(&Uni->Model);
  //AO5_GeomFree(&Uni->Geom);
  AO5_PrimFree(&Uni->Pr);
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
  static FLT x = 1, y = 0, z = 0, pov = 0;
  
  AO5_RndMatrView = MatrView(VecSet(0, 3, 100),
                             VecSet(0, 0, 0),
                             VecSet(0, 1, 0));
  
  if (Ani->KeysClick['X'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Z'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);
  if (Ani->Keys['W'])
    z -= 0.1;
  if (Ani->Keys['S'])
    z += 0.1;
  if (Ani->Keys['A'])
    pov += 1;
  if (Ani->Keys['D'])
    pov -= 1;
  AO5_RndMatrWorld = MatrMulMatr(MatrRotateY(pov),MatrTranslate(0, 0, z));
  
  AO5_GeomDraw(&Uni->Model);
  AO5_RndMatrWorld = MatrMulMatr(MatrMulMatr(MatrRotateZ(30 * sin(Ani->Time * 3.0)), MatrRotateY(30 * Ani->Time)), MatrScale(5, 5, 5));
 // AO5_GeomDraw(&Uni->Geom);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);
  AO5_PrimDraw(&Uni->Pr);
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
  Uni->Render = (VOID *)AO5_AnimUnitRender;
  return (ao5UNIT *)Uni;
} /* End of 'AO5_UnitModelCreate' function */

/* END OF 'UMODEL.C' FILE */

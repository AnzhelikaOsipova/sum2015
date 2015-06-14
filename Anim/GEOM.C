/* FILENAME: GEOM.C
 * PROGRAMMER: AO5
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 13.06.2015
 */

#include <stdio.h>
#include <stdlib.h>

#include "render.h"

/* Функция добавления примитива к геометрическому объекту.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       ao5GEOM *G;
 *   - указатель на добавляемый примитив:
 *       ao5PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного примитива в массиве (-1 при ошибке).
 */
INT AO5_GeomAddPrim( ao5GEOM *G, ao5PRIM *Prim )
{
  ao5PRIM *new_bulk;

  /* Выделяем память под новый массив примитивов */
  if ((new_bulk = malloc(sizeof(ao5PRIM) * (G->NumOfPrimitives + 1))) == NULL)
    return -1;

  /* Копируем старые примитивы */
  if (G->Prims != NULL)
  {
    memcpy(new_bulk, G->Prims, sizeof(ao5PRIM) * G->NumOfPrimitives);
    free(G->Prims);
  }
  /* указываем на новый массив примитивлв */
  G->Prims = new_bulk;

  /* Сохраняем новый элемент */
  G->Prims[G->NumOfPrimitives] = *Prim;
  return G->NumOfPrimitives++;
} /* End of 'AO5_GeomAddPrim' function */

/* Функция освобождения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       ao5GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AO5_GeomFree( ao5GEOM *G )
{
  INT i;

  if (G->Prims != NULL)
  {
    for (i = 0; i < G->NumOfPrimitives; i++)
      AO5_PrimFree(&G->Prims[i]);
    free(G->Prims);
  }
  memset(G, 0, sizeof(ao5GEOM));
} /* End of 'AO5_GeomFree' function */

/* Функция отображения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       ao5GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AO5_GeomDraw( ao5GEOM *G )
{
  INT i, loc;

  for (i = 0; i < G->NumOfPrimitives; i++)
  {
    glUseProgram(AO5_RndProg);
    loc = glGetUniformLocation(AO5_RndProg, "PartNo");
    if (loc != -1)
      glUniform1f(loc, i);
    glUseProgram(0);
    AO5_PrimDraw(&G->Prims[i]);
  }
} /* End of 'AO5_GeomDraw' function */

/* Функция загрузки геометрического объекта из G3D файла.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       ao5GEOM *G;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе - FALSE.
 */
BOOL AO5_GeomLoad( ao5GEOM *G, CHAR *FileName )
{
  FILE *F;
  INT i, j, n;
  CHAR Sign[4];
  MATR M;
  static CHAR MtlName[300];

  memset(G, 0, sizeof(ao5GEOM));
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  M = MatrTranspose(MatrInverse(AO5_RndPrimMatrConvert));

  /* читаем сигнатуру */
  fread(Sign, 1, 4, F);
  if (*(DWORD *)Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }

  /* читаем количество примитивов в объекте */
  fread(&n, 4, 1, F);
  fread(MtlName, 1, 300, F);

  /* читаем примитивы */
  for (i = 0; i < n; i++)
  {
    INT nv, ni, *Ind;
    ao5VERTEX *Vert;
    ao5PRIM P;

    /* читаем количество вершин и индексов */
    fread(&nv, 4, 1, F);
    fread(&ni, 4, 1, F);
    /* читаем имя материала текущего примитива */
    fread(MtlName, 1, 300, F);

    /* выделяем память под вершины и индексы */
    if ((Vert = malloc(sizeof(ao5VERTEX) * nv + sizeof(INT) * ni)) == NULL)
      break;
    Ind = (INT *)(Vert + nv);

    /* читаем данные */
    fread(Vert, sizeof(ao5VERTEX), nv, F);
    /* конвертируем геометрию */
    for (j = 0; j < nv; j++)
    {
      Vert[j].P = PointTransform(Vert[j].P, AO5_RndPrimMatrConvert);
      Vert[j].N = VectorTransform(Vert[j].N, M);
    }
    fread(Ind, sizeof(INT), ni, F);

    /* заносим в примитив */
    AO5_PrimCreate(&P, AO5_PRIM_TRIMESH, nv, ni, Vert, Ind);

    free(Vert);

    /* добавляем примитив к объекту */
    AO5_GeomAddPrim(G, &P);
  }
  fclose(F);
  AO5_RndPrimMatrConvert = MatrIdentity();
  return TRUE;
} /* End of 'AO5_GeomDraw' function */

/* END OF 'GEOM.C' FILE */

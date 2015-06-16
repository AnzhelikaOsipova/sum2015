/* FILENAME: PRIM.C
 * PROGRAMMER: AO5
 * PURPOSE: Primitive handle module.
 * LAST UPDATE: 13.06.2015
 */

#include "anim.h"
#include "render.h"
#include "image.h"

/* Матрица изменения примитива при создании */
MATR AO5_RndPrimMatrConvert = AO5_UNIT_MATR;

/* Цвет новых объектов по умолчанию */
COLOR AO5_RndPrimDefaultColor = {1, 1, 1, 1};

/* Функция создания примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       ao5PRIM *Prim;
 *   - тип примитива (AO5_PRIM_***):
 *       ao5PRIM_TYPE Type;
 *   - количество вершин и индексов:
 *       INT NoofV, NoofI;
 *   - массив вершин:
 *       ao5VERTEX *Vertices;
 *   - массив индексов:
 *       INT *Indices;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
VOID AO5_PrimCreate( ao5PRIM *Prim, ao5PRIM_TYPE Type,
                     INT NoofV, INT NoofI, ao5VERTEX *Vertices, INT *Indices)
{
  Prim->Type = Type;
  Prim->NumOfI = NoofI;
  /* Создаем буфера OpenGL */
  glGenVertexArrays(1, &Prim->VA);
  glGenBuffers(1, &Prim->VBuf);
  glGenBuffers(1, &Prim->IBuf);

  /* делаем активным массив вершин */
  glBindVertexArray(Prim->VA);
  /* делаем активным буфер вершин */
  glBindBuffer(GL_ARRAY_BUFFER, Prim->VBuf);
  /* сливаем данные */
  glBufferData(GL_ARRAY_BUFFER, sizeof(ao5VERTEX) * NoofV, Vertices, GL_STATIC_DRAW);
  /* делаем активным буфер индексов */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Prim->IBuf);
  /* сливаем данные */
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NoofI, Indices, GL_STATIC_DRAW);

  /* задаем порядок данных */
  /*                    layout,
   *                       количество компонент,
   *                          тип,
   *                                    надо ли нормировать,
   *                                           размер в байтах одного элемента буфера,
   *                                                           смещение в байтах до начала данных */
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(ao5VERTEX), (VOID *)0); /* позиция */
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(ao5VERTEX), (VOID *)sizeof(VEC)); /* текстура */
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(ao5VERTEX), (VOID *)(sizeof(VEC) + sizeof(ao5UV))); /* нормаль */
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(ao5VERTEX), (VOID *)(sizeof(VEC) * 2 + sizeof(ao5UV))); /* цвет */

  /* включаем нужные аттрибуты (layout) */
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  /* отключаемся от массива вершин */
  glBindVertexArray(0);
} /* End of 'AO5_PrimCreate' function */

/* Функция удаления примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       ao5PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AO5_PrimFree( ao5PRIM *Prim )
{
  /* делаем активным массив вершин */
  glBindVertexArray(Prim->VA);
  /* "отцепляем" буфера */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Prim->VBuf);
  glDeleteBuffers(1, &Prim->IBuf);
  /* делаем неактивным массив вершин */
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Prim->VA);

  memset(Prim, 0, sizeof(ao5PRIM));
} /* End of 'AO5_PrimFree' function */

/* Функция отрисовки примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       ao5PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AO5_PrimDraw( ao5PRIM *Prim )
{
  INT loc;
  MATR M;

  AO5_RndMatrWorldViewProj = MatrMulMatr(MatrMulMatr(AO5_RndMatrWorld, AO5_RndMatrView), AO5_RndMatrProj);

  /* оставлено для отлдадки, если нет шейдеров */
  glLoadMatrixf(AO5_RndMatrWorldViewProj.A[0]);

  /* рисуем треугольники */
  glBindVertexArray(Prim->VA);
  glUseProgram(Prim->Prog);

  loc = glGetUniformLocation(Prim->Prog, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AO5_RndMatrWorld.A[0]);
  loc = glGetUniformLocation(Prim->Prog, "MatrView");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AO5_RndMatrView.A[0]);
  loc = glGetUniformLocation(Prim->Prog, "MatrProj");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AO5_RndMatrProj.A[0]);
  loc = glGetUniformLocation(Prim->Prog, "MatrWVP");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AO5_RndMatrWorldViewProj.A[0]);

  M = MatrTranspose(MatrInverse(MatrMulMatr(AO5_RndMatrWorld, AO5_RndMatrView)));
  loc = glGetUniformLocation(Prim->Prog, "MatrWVInverse");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  M = MatrTranspose(MatrInverse(AO5_RndMatrWorld));
  loc = glGetUniformLocation(Prim->Prog, "MatrWInverse");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  M = MatrMulMatr(AO5_RndMatrWorld, AO5_RndMatrView);
  loc = glGetUniformLocation(Prim->Prog, "MatrWV");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  loc = glGetUniformLocation(Prim->Prog, "Time");
  if (loc != -1)
    glUniform1f(loc, AO5_Anim.Time);

  /* Применение материала */
  loc = glGetUniformLocation(Prim->Prog, "Ka");
  if (loc != -1)
    glUniform3fv(loc, 1, &AO5_MtlLib[Prim->MtlNo].Ka.X);
  loc = glGetUniformLocation(Prim->Prog, "Kd");
  if (loc != -1)
    glUniform3fv(loc, 1, &AO5_MtlLib[Prim->MtlNo].Kd.X);
  loc = glGetUniformLocation(Prim->Prog, "Ks");
  if (loc != -1)
    glUniform3fv(loc, 1, &AO5_MtlLib[Prim->MtlNo].Ks.X);
  loc = glGetUniformLocation(Prim->Prog, "Kp");
  if (loc != -1)
    glUniform1f(loc, AO5_MtlLib[Prim->MtlNo].Kp);
  loc = glGetUniformLocation(Prim->Prog, "Kt");
  if (loc != -1)
    glUniform1f(loc, AO5_MtlLib[Prim->MtlNo].Kt);

  loc = glGetUniformLocation(Prim->Prog, "IsTextureUse");
  if (AO5_MtlLib[Prim->MtlNo].TexId == 0)
    glUniform1f(loc, 0);
  else
  {
    glUniform1f(loc, 1);
    glBindTexture(GL_TEXTURE_2D, AO5_MtlLib[Prim->MtlNo].TexId);
  }

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(0xFFFFFFFF);
  if (Prim->Type == AO5_PRIM_GRID)
    glDrawElements(GL_TRIANGLE_STRIP, Prim->NumOfI, GL_UNSIGNED_INT, NULL);
  else
    glDrawElements(GL_TRIANGLES, Prim->NumOfI, GL_UNSIGNED_INT, NULL);

  glUseProgram(0);
  glBindVertexArray(0);
} /* End of 'AO5_PrimDraw' function */

/* Функция создания примитива плоскость.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       AO5PRIM *Prim;
 *   - угловая точка:
 *       VEC Loc;
 *   - касательные вектора-стороны:
 *       VEC Du, Dv;
 *   - разбиение:
 *       INT N, M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL AO5_PrimCreatePlane( ao5PRIM *Prim, VEC Loc, VEC Du, VEC Dv, INT N, INT M )
{
  INT i, j;
  VEC norm;
  INT *Ind, *iptr;
  ao5VERTEX *V, *ptr;

  memset(Prim, 0, sizeof(ao5PRIM));

  if ((V = malloc(sizeof(ao5VERTEX) * N * M +
                  sizeof(INT) * ((2 * M + 1) * (N - 1)))) == NULL)
    return FALSE;
  Ind = (INT *)(V + N * M);

  /* заполняем вершины */
  norm = VecNormalize(VecCrossVec(Du, Dv));
  for (ptr = V, i = 0; i < N; i++)
    for (j = 0; j < M; j++, ptr++)
    {
      ptr->P = VecAddVec(Loc,
                 VecAddVec(VecMulNum(Du, j / (M - 1.0)),
                           VecMulNum(Dv, i / (N - 1.0))));
      ptr->C = AO5_RndPrimDefaultColor;
      ptr->N = norm;
      ptr->T = UVSet(j / (M - 1.0), i / (N - 1.0));
    }

  /* заполняем индексы */
  for (iptr = Ind, i = 0; i < N - 1; i++)
  {
    for (j = 0; j < M; j++)
    {
      /* верхний */
      *iptr++ = i * M + j + M;
      /* нижний */
      *iptr++ = i * M + j;
    }
    /* сохраняем индекс разрыва примитива */
    *iptr++ = 0xFFFFFFFF;
  }

  AO5_PrimCreate(Prim, AO5_PRIM_GRID, M * N, (2 * M + 1) * (N - 1), V, Ind);

  free(V);

  return TRUE;
} /* End of 'AO5_PrimCreatePlane' function */

/* Функция создания примитива сфера.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       AO5PRIM *Prim;
 *   - центр сферы:
 *       VEC С;
 *   - радиус сферы:
 *       FLT R;
 *   - разбиение:
 *       INT N, M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL AO5_PrimCreateSphere( ao5PRIM *Prim, VEC C, FLT R, INT N, INT M )
{
  INT i, j;
  INT *Ind, *iptr;
  ao5VERTEX *V, *ptr;

  memset(Prim, 0, sizeof(ao5PRIM));

  if ((V = malloc(sizeof(ao5VERTEX) * N * M +
                  sizeof(INT) * ((2 * M + 1) * (N - 1)))) == NULL)
    return FALSE;
  Ind = (INT *)(V + N * M);

  /* заполняем вершины */
  for (ptr = V, i = 0; i < N; i++)
    for (j = 0; j < M; j++, ptr++)
    {
      DBL
        theta = PI * i / (N - 1.0), phi = 2 * PI * j / (M - 1.0),
        x = sin(theta) * sin(phi),
        y = cos(theta),
        z = sin(theta) * cos(phi);

      ptr->P = VecAddVec(C, VecSet(R * x, R * y, R * z));
      ptr->C = AO5_RndPrimDefaultColor;
      ptr->N = VecSet(x, y, z);
      ptr->T = UVSet(j / (M - 1.0), i / (N - 1.0));
    }

  /* заполняем индексы */
  for (iptr = Ind, i = 0; i < N - 1; i++)
  {
    for (j = 0; j < M; j++)
    {
      /* верхний */
      *iptr++ = i * M + j + M;
      /* нижний */
      *iptr++ = i * M + j;
    }
    /* сохраняем индекс разрыва примитива */
    *iptr++ = 0xFFFFFFFF;
  }

  AO5_PrimCreate(Prim, AO5_PRIM_GRID, M * N, (2 * M + 1) * (N - 1), V, Ind);

  free(V);

  return TRUE;
} /* End of 'AO5_PrimCreateSphere' function */

/* Функция создания примитива карта высот.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       AO5PRIM *Prim;
 *   - угловая точка:
 *       VEC Loc;
 *   - касательные вектора-стороны:
 *       VEC Du, Dv;
 *   - масштаб по высоте:
 *       DBL Scale;
 *   - имя файла с высотами:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL AO5_PrimCreateHeightField( ao5PRIM *Prim, VEC Loc, VEC Du, VEC Dv, DBL Scale, CHAR *FileName )
{
  INT i, j;
  VEC norm;
  INT *Ind, *iptr;
  ao5VERTEX *V, *ptr;
  INT N, M;
  IMAGE Img;

  memset(Prim, 0, sizeof(ao5PRIM));

  if (!ImageLoad(&Img, FileName))
    return FALSE;
  M = Img.W;
  N = Img.H;

  if ((V = malloc(sizeof(ao5VERTEX) * N * M +
                  sizeof(INT) * ((2 * M + 1) * (N - 1)))) == NULL)
    return FALSE;
  Ind = (INT *)(V + N * M);

  /* заполняем вершины */
  norm = VecNormalize(VecCrossVec(Du, Dv));
  for (ptr = V, i = 0; i < N; i++)
    for (j = 0; j < M; j++, ptr++)
    {
      DWORD icolor = ImageGetP(&Img, j, i);
      DBL
        r = ((icolor >> 16) & 0xFF) / 255.0,
        g = ((icolor >> 8) & 0xFF) / 255.0,
        b = ((icolor >> 0) & 0xFF) / 255.0,
        H = r * 0.30 + g * 0.59 + b * 0.11;

      ptr->P = VecAddVec(Loc,
                 VecAddVec(VecAddVec(
                             VecMulNum(Du, j / (M - 1.0)),
                             VecMulNum(Dv, i / (N - 1.0))),
                             VecMulNum(norm, H * Scale)));
      ptr->C = AO5_RndPrimDefaultColor;
      ptr->N = norm;
      ptr->T = UVSet(j / (M - 1.0), i / (N - 1.0));
    }
  ImageFree(&Img);
  /* заполняем индексы */
  for (iptr = Ind, i = 0; i < N - 1; i++)
  {
    for (j = 0; j < M; j++)
    {
      /* верхний */
      *iptr++ = i * M + j + M;
      /* нижний */
      *iptr++ = i * M + j;
    }
    /* сохраняем индекс разрыва примитива */
    *iptr++ = 0xFFFFFFFF;
  }

  /* Вычесление нормалей */
  
  /* обнуляем нормали всех вершин */
  for (i = 0; i < N * M; i++)
    V[i].N = VecSet(0, 0, 0);

  /* пробегаем по всем треугольникам */
  for (i = 0; i < N - 1; i++)
    for (j = 0; j < M - 1; j++)
    {
      ao5VERTEX
        *Vij = V + i * M + j,
        *Vi1j = V + (i + 1) * M + j,
        *Vij1 = V + i * M + j + 1,
        *Vi1j1 = V + (i + 1) * M + j + 1;

      /* верхний треугольник */
      norm = VecNormalize(VecCrossVec(VecSubVec(Vij->P, Vi1j->P), VecSubVec(Vi1j1->P, Vi1j->P)));
      Vij->N = VecAddVec(Vij->N, norm);
      Vi1j->N = VecAddVec(Vi1j->N, norm);
      Vi1j1->N = VecAddVec(Vi1j1->N, norm);

      /* нижний треугольник */
      norm = VecNormalize(VecCrossVec(VecSubVec(Vi1j1->P, Vij1->P), VecSubVec(Vij->P, Vij1->P)));
      Vij->N = VecAddVec(Vij->N, norm);
      Vij1->N = VecAddVec(Vij1->N, norm);
      Vi1j1->N = VecAddVec(Vi1j1->N, norm);
    }

  /* нормируем нормали всех вершин */
  for (i = 0; i < N * M; i++)
    V[i].N = VecNormalize(V[i].N);

  AO5_PrimCreate(Prim, AO5_PRIM_GRID, M * N, (2 * M + 1) * (N - 1), V, Ind);

  free(V);

  return TRUE;
} /* End of 'AO5_PrimCreatePlane' function */


/* END OF 'PRIM.C' FILE */

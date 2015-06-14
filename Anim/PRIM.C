/* FILENAME: PRIM.C
 * PROGRAMMER: AO5
 * PURPOSE: Primitive handle module.
 * LAST UPDATE: 13.06.2015
 */

#include "anim.h"
#include "render.h"

/* Матрица изменения примитива при создании */
MATR AO5_RndPrimMatrConvert = AO5_UNIT_MATR;

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
  glUseProgram(AO5_RndProg);

  loc = glGetUniformLocation(AO5_RndProg, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AO5_RndMatrWorld.A[0]);
  loc = glGetUniformLocation(AO5_RndProg, "MatrView");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AO5_RndMatrView.A[0]);
  loc = glGetUniformLocation(AO5_RndProg, "MatrProj");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AO5_RndMatrProj.A[0]);
  loc = glGetUniformLocation(AO5_RndProg, "MatrWVP");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AO5_RndMatrWorldViewProj.A[0]);

  M = MatrTranspose(MatrInverse(MatrMulMatr(AO5_RndMatrWorld, AO5_RndMatrView)));
  loc = glGetUniformLocation(AO5_RndProg, "MatrWVInverse");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  M = MatrMulMatr(AO5_RndMatrWorld, AO5_RndMatrView);
  loc = glGetUniformLocation(AO5_RndProg, "MatrWV");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  loc = glGetUniformLocation(AO5_RndProg, "Time");
  if (loc != -1)
    glUniform1f(loc, AO5_Anim.Time);

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
 *       ao5PRIM *Prim;
 *   - касательные вектора-стороны:
 *       VEC Du, Dv;
 *   - разбиение:
 *       INT N, M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL AO5_PrimCreatePlane( ao5PRIM *Prim, VEC Du, VEC Dv, INT N, INT M )
{
} /* End of 'AO5_PrimCreatePlane' function */

/* Функция создания примитива сфера.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       ao5PRIM *Prim;
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
} /* End of 'AO5_PrimCreateSphere' function */

/* END OF 'PRIM.C' FILE */

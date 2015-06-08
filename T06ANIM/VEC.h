/* FILE NAME: VEC.h
*  PROGRAMMER: AO5
*  DATE: 06.06.2015
*  PURPOSE: Animation drawing*/


#ifndef _VEC_H_
#define _VEC_H_

#pragma warning(disable: 4244)

#include <math.h>
#define PI 3.14159265358979323846
#define DEGREE2RADIANL 0.01745329251994329576L
#define D2R(A) ((A) * PI / 180.0)

static long double MultiplierDegree2Radian = DEGREE2RADIANL;

/* базовый вещественный тип */
typedef double DBL;

/* тип для вектора в простанстве */
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

/* тип для матрицы - массив в структуре */
typedef struct tagMATR
{
  DBL A[4][4];
} MATR;

__inline MATR MatrIdentity( VOID )
{
  MATR M =
  {
    {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };

  return M;
}

__inline MATR MatrTranslate( DBL Dx, DBL Dy, DBL Dz )
{
  MATR M =
  {
    {
      { 1,  0,  0, 0},
      { 0,  1,  0, 0},
      { 0,  0,  1, 0},
      {Dx, Dy, Dz, 1}
    }
  };

  return M;
}

__inline MATR MatrScale( DBL Sx, DBL Sy, DBL Sz )
{
  MATR M =
  {
    {
      {Sx,  0,  0, 0},
      { 0, Sy,  0, 0},
      { 0,  0, Sz, 0},
      { 0,  0,  0, 1}
    }
  };

  return M;
}

__inline MATR MatrRotateX( DBL AngleDegree )
{
  DBL si = sin(D2R(AngleDegree)), co = cos(D2R(AngleDegree));
  MATR M =
  {
    {
      { 1,    0,   0, 0},
      { 0,   co,  si, 0},
      { 0,  -si,  co, 0},
      { 0,    0,   0, 1}
    }
  };

  return M;
}

__inline MATR MatrRotateY( DBL AngleDegree )
{
  DBL si = sin(D2R(AngleDegree)), co = cos(D2R(AngleDegree));
  MATR M =
  {
    {
      { co,  0,  -si, 0},
      {  0,  1,    0, 0},
      { si,  0,   co, 0},
      {  0,  0,    0, 1}
    }
  };

  return M;
}

__inline MATR MatrRotateZ( DBL AngleDegree )
{
  DBL si = sin(D2R(AngleDegree)), co = cos(D2R(AngleDegree));
  MATR M =
  {
    {
      {  co,  si, 0, 0},
      { -si,  co, 0, 0},
      {   0,   0, 1, 0},
      {   0,   0, 0, 1}
    }
  };

  return M;
}

__inline MATR MatrRotate( DBL AngleDegree, DBL X, DBL Y, DBL Z )
{
  DBL a, si, co, len;
  MATR M;

  /* correct source parameters */
  a = D2R(AngleDegree);
  si = sin(a);
  co = cos(a);
  len = X * X + Y * Y + Z * Z;
  if (len != 0 && len != 1)
    len = sqrt(len), X /= len, Y /= len, Z /= len;

  /* Build quaternion matrix */
  X *= si;
  Y *= si;
  Z *= si;

  M.A[0][0] = 1 - 2 * (Y * Y + Z * Z);
  M.A[0][1] = 2 * X * Y - 2 * co * Z;
  M.A[0][2] = 2 * co * Y + 2 * X * Z;
  M.A[0][3] = 0;

  M.A[1][0] = 2 * X * Y + 2 * co * Z;
  M.A[1][1] = 1 - 2 * (X * X + Z * Z);
  M.A[1][2] = 2 * Y * Z - 2 * co * X;
  M.A[1][3] = 0;

  M.A[2][0] = 2 * X * Z - 2 * co * Y;
  M.A[2][1] = 2 * co * X + 2 * Y * Z;
  M.A[2][2] = 1 - 2 * (X * X + Y * Y);
  M.A[2][3] = 0;

  M.A[3][0] = 0;
  M.A[3][1] = 0;
  M.A[3][2] = 0;
  M.A[3][3] = 1;
  return M;
}

__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR M;
  int i, j, k;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (M.A[i][j] = 0, k = 0; k < 4; k++)
        M.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return M;
}

__inline MATR MatrTranspose( MATR M )
{
  MATR M2;
  int i, j;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      M2.A[i][j] = M.A[j][i];
  return M2;
}

__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                   DBL A21, DBL A22, DBL A23,
                   DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 +
        -A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
}

__inline DBL MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
}

__inline MATR MatrInverse( MATR M )
{
  MATR r;
  DBL det = MatrDeterm(M);
  int p[4][3] =
  {
    {1, 2, 3},
    {0, 2, 3},
    {0, 1, 3},
    {0, 1, 2}
  }, i, j;

  if (det == 0)
    return MatrIdentity();
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)  
      r.A[j][i] = (1 - (i + j) % 2 * 2) *
        MatrDeterm3x3(M.A[p[i][0]][p[j][0]], M.A[p[i][0]][p[j][1]], M.A[p[i][0]][p[j][2]],
                      M.A[p[i][1]][p[j][0]], M.A[p[i][1]][p[j][1]], M.A[p[i][1]][p[j][2]],
                      M.A[p[i][2]][p[j][0]], M.A[p[i][2]][p[j][1]], M.A[p[i][2]][p[j][2]]) / det;

  return r;
}

/* Векторы */

__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC r = {X, Y, Z};
  return r;
}

__inline VEC VecAddVec( VEC A, VEC B )
{
  return VecSet(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
}

__inline VEC VecSubVec( VEC A, VEC B )
{
  return VecSet(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
}

__inline VEC VecMulNum( VEC A, DBL N )
{
  return VecSet(A.X * N, A.Y * N, A.Z * N);
}
__inline VEC VecDivNum( VEC A, DBL N )
{
  return VecSet(A.X / N, A.Y / N, A.Z / N);
}

__inline VEC VecNeg( VEC A )
{
  return VecSet(-A.X, -A.Y, -A.Z);
}

__inline DBL VecDotVec( VEC A, VEC B )
{
  return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
}

__inline VEC VecCrossVec( VEC A, VEC B )
{
  return VecSet(A.Y * B.Z - A.Z * B.Y, A.Z * B.X - A.X * B.Z, A.X * B.Y - A.Y * B.X);
}

__inline DBL VecLen2( VEC V )
{
  return V.X * V.X + V.Y * V.Y + V.Z * V.Z;
}

__inline DBL VecLen( VEC V )
{
  return sqrt(V.X * V.X + V.Y * V.Y + V.Z * V.Z);
}

__inline VEC VecNormalize( VEC V )
{
  return VecSet(V.X / VecLen(V), V.Y / VecLen(V), V.Z / VecLen(V));
}

__inline VEC PointTransform( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
}
__inline VEC VectorTransform( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);

}

__inline VEC RotateZ( VEC V, DBL AngleDegree )
{
  DBL sine, cosine, tmp;
  _asm{
    fld AngleDegree
    fmul MultiplierDegree2Radian
    fsincos
    fstp cosine
    fstp sine
  }
  tmp = V.X * cosine - V.Y * sine;
  V.Y = V.X * sine + V.Y * cosine;
  V.X = tmp;
  return V;
}
__inline VEC RotateX( VEC V, DBL AngleDegree )
{
  DBL sine, cosine, tmp;
  _asm{
    fld AngleDegree
    fmul MultiplierDegree2Radian
    fsincos
    fstp cosine
    fstp sine
  }
  tmp = V.Y * sine + V.Z * cosine;
  V.Y = V.Y * cosine - V.Z * sine;
  V.Z = tmp; 
  return V;
}

__inline VEC RotateY( VEC V, DBL AngleDegree )
{
  DBL sine, cosine, tmp;
  _asm{
    fld AngleDegree
    fmul MultiplierDegree2Radian
    fsincos
    fstp cosine
    fstp sine
  }
  tmp = V.X * sine + V.Z * cosine;
  V.X = V.X * cosine - V.Z * sine;
  V.Z = tmp; 
  return V;
}

#endif
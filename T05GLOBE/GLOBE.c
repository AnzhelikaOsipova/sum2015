/* FILE NAME: T05GLOBE
*  PROGRAMMER: AO5
*  DATE: 05.06.2015
*  PURPOSE: WinAPI windowed application sample*/


#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "globe.h"
#include "image.h"
#include "def.h"

#define N (30 * 10)
#define M (30 * 20)

static VEC Grid[N][M];

/* ������� �������� ����� ������ ��� X */
VEC RotateX( VEC P, DOUBLE AngleDegree )
{
  DOUBLE a = AngleDegree * Pi / 180, si = sin(a), co = cos(a);
  VEC r;

  r.X = P.X;
  r.Y = P.Y * co - P.Z * si;
  r.Z = P.Y * si + P.Z * co;
  return r;
} /* End of 'RotateX' function */

/* ������� ���������� ������������ */
VEC VecCrossVec( VEC A, VEC B )
{
  VEC r;

  r.X = A.Y * B.Z - A.Z * B.Y;
  r.Y = A.Z * B.X - A.X * B.Z;
  r.Z = A.X * B.Y - A.Y * B.X;
  return r;
} /* End of 'VecCrossVec' function */

/* ������� ��������� �������� */
VEC VecSubVec( VEC A, VEC B )
{
  VEC r;

  r.X = A.X - B.X;
  r.Y = A.Y - B.Y;
  r.Z = A.Z - B.Z;
  return r;
} /* End of 'VecSubVec' function */

/* ��������� ���������������� */
VOID DrawQuad( HDC hDC, VEC P0, VEC P1, VEC P2, VEC P3, INT W, INT H )
{
  POINT pnts[4];  

  pnts[0].x = P0.X + W / 2;
  pnts[0].y = -P0.Y + H / 2;

  pnts[1].x = P1.X + W / 2;
  pnts[1].y = -P1.Y + H / 2;

  pnts[2].x = P2.X + W / 2;
  pnts[2].y = -P2.Y + H / 2;

  pnts[3].x = P3.X + W / 2;
  pnts[3].y = -P3.Y + H / 2;
  
  /* back-face culling */
  if ((pnts[0].x - pnts[1].x) * (pnts[0].y + pnts[1].y) +
      (pnts[1].x - pnts[2].x) * (pnts[1].y + pnts[2].y) +
      (pnts[2].x - pnts[3].x) * (pnts[2].y + pnts[3].y) +
      (pnts[3].x - pnts[0].x) * (pnts[3].y + pnts[0].y) < 0)
    return;

  Polygon(hDC, pnts, 4);
} /* End of 'DrawQuad' function */

VOID GlobeBuild(VOID)
{
  DOUBLE theta, phi, t = clock() / (DOUBLE)CLOCKS_PER_SEC;
  INT i, j, r = 300;
  //srand(30);
  for(i = 0; i < N; i++)
  {
    theta = i / (N - 1.0) * Pi;
    for(j = 0; j < M; j++)
    {
      phi = j / (M - 1.0) * 2 * Pi + t * 2;
      Grid[i][j].X = r * sin(theta) * sin(phi);
      Grid[i][j].Y = r * cos(theta);
      Grid[i][j].Z = r * sin(theta) * cos(phi);
      //Grid[i][j] = RotateX(Grid[i][j], sin(t * 1) * 50);
    }
  }
} /* End of 'GlobeBuild' function */


VOID GlobeDraw(HDC hDC, INT w, INT h)
{
  INT i, j, ix, iy;
  srand(30);
  if(Globe.hBm == NULL)
    ImageLoad(&Globe, "globe.bmp");
  for(i = 0; i < N - 1; i++)
    for(j = 0; j < M - 1; j++)
    {
      ix = j * (Globe.W - 1) / (M - 1);
      iy = i * (Globe.H - 1) / (N - 1);
      SelectObject(hDC, GetStockObject(DC_BRUSH));
      SelectObject(hDC, GetStockObject(NULL_PEN));
      SetDCBrushColor(hDC, ImageGetP(&Globe, ix, iy));
      DrawQuad(hDC, Grid[i][j], Grid[i + 1][j], Grid[i + 1][j + 1], Grid[i][j + 1], w, h);
    }
} /* End of 'GlobeDraw' function */
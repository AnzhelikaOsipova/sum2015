/* FILE NAME: T05GLOBE
*  PROGRAMMER: AO5
*  DATE: 05.06.2015
*  PURPOSE: WinAPI windowed application sample*/



#ifndef _GLOBE_H_
#define _GLOBE_H_

#include "def.h"
#include "image.h"

#define Pi 3.14159265358979323846

/* Тип хранения координат точки в пространстве */
typedef struct
{
  DOUBLE X, Y, Z;
} VEC;

static IMAGE Globe;

VOID GlobeBuild(VOID);
VOID DrawQuad( HDC hDC, VEC P0, VEC P1, VEC P2, VEC P3, INT W, INT H );
VOID GlobeDraw(HDC hDC, INT w, INT h);

#endif

/* FILENAME: RENDER.H
 * PROGRAMMER: AO5
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 13.06.2015
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

/* ������� */
extern MATR
  AO5_RndMatrWorld,
  AO5_RndMatrView,
  AO5_RndMatrProj,
  AO5_RndMatrWorldViewProj;

/* ������� ��������� ��������� ��� �������� */
extern MATR AO5_RndPrimMatrConvert;

/* ��������� ������������� */
extern DBL
  AO5_RndWp, AO5_RndHp,    /* ������� ������� ������������� */
  AO5_RndProjDist;         /* ���������� �� ��������� �������� */

/* ������ �� ��������� */
extern UINT AO5_RndProg;

/* ������� �������� �������� ��� ����� ���������.
 * ���������:
 *   - ������� ����� �����:
 *       CHAR *FileNamePrefix;
 * ������������ ��������:
 *   (UINT) ����� ����������� ���������.
 */
UINT AO5_ShaderLoad( CHAR *FileNamePrefix );

/* ������� ����������� �������� ��� ���������.
 * ���������:
 *   - ����� ���������:
 *       UINT PrgId;
 * ������������ ��������: ���.
 */
VOID AO5_ShaderFree( UINT PrgId );

/***
 * ������ � �����������
 ***/

/* ��� ������� ��������� ����������� */
typedef struct tagao5MATERIAL
{
  /* ��� ��������� */
  CHAR Name[300];

  /* ������������ ��������� */
  VEC Ka, Kd, Ks;
  FLT Kp; /* ������� "����������" */

  /* ����������� ������������ */
  FLT Kt;

  /* �������� ����������� */
  INT TexId;
} ao5MATERIAL;

/* ���������� ������� ���������� */
#define AO5_MATERIAL_MAX 300
extern INT AO5_MtlLibSize;
extern ao5MATERIAL AO5_MtlLib[AO5_MATERIAL_MAX];

/* ������� ���������� ��������� � ����������.
 * ���������:
 *   - ��������� � ����������� ���������:
 *       ao5MATERIAL *Mtl;
 * ������������ ��������:
 *   (INT) ���������� ����� ������������ ��������� (0 ��� ������).
 */
INT AO5_MtlAdd( ao5MATERIAL *Mtl );

/* ������� ������ ��������� �� �����.
 * ���������:
 *   - ��� ���������:
 *       CHAR *Name;
 * ������������ ��������:
 *   (INT) ���������� ����� ���������� ��������� (0 ��� ������).
 */
INT AO5_MtlFind( CHAR *Name );

/* ������� �������� ��������� �� ����� (*.MTL).
 * ���������:
 *   - ��� ����� ���������:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (INT) ���������� ����������� ����������.
 */
INT AO5_MtlLoad( CHAR *FileName );

/***
 * ������ � �����������
 ***/

/* ���� ���������� */
typedef enum tagao5PRIM_TYPE
{
  AO5_PRIM_TRIMESH,  /* �������� - ����� ������������� */
  AO5_PRIM_GRID      /* ���������� ����� ������������� (����� triangle strip) */
} ao5PRIM_TYPE;

/* ��� �������� ��������� */
typedef struct tagao5PRIM
{
  ao5PRIM_TYPE Type; /* ��� ��������� (AO5_PRIM_***) */
  INT
    VA,          /* ������ ������ */
    VBuf, IBuf,  /* ������ ������ � �������� */
    NumOfI,      /* ���������� �������� ��� ������ ��������� */
    MtlNo;       /* ����� ��������� �� ���������� */
} ao5PRIM;

/* ��� �������� ���������� ��������� */
typedef struct tagao5UV
{
  FLT U, V; /* ���������� ���������� */
} ao5UV;

/* ������� ������� ���������� ���������.
 * ���������:
 *   - ����� ������� ���������:
 *       FLT U, V;
 * ������������ ��������:
 *   (AO5UV) ����������� ����������.
 */
__inline ao5UV UVSet( FLT U, FLT V )
{
  ao5UV UV = {U, V};

  return UV;
} /* End of 'UVSet' function */

/* ��� �������� ����� ������� ���������� */
typedef struct tagao5VERTEX
{
  VEC P;    /* ������� ������� */
  ao5UV T;  /* ���������� ���������� ������� */
  VEC N;    /* ������� � ������� */
  COLOR C;  /* ���� ������� */
} ao5VERTEX;

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       ao5PRIM *Prim;
 *   - ��� ��������� (AO5_PRIM_***):
 *       ao5PRIM_TYPE Type;
 *   - ���������� ������ � ��������:
 *       INT NoofV, NoofI;
 *   - ������ ������:
 *       ao5VERTEX *Vertices;
 *   - ������ ��������:
 *       INT *Indices;
 * ������������ ��������: ���.
 */
VOID AO5_PrimCreate( ao5PRIM *Prim, ao5PRIM_TYPE Type,
                     INT NoofV, INT NoofI, ao5VERTEX *Vertices, INT *Indices);

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       ao5PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID AO5_PrimFree( ao5PRIM *Prim );

/* ������� ��������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       ao5PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID AO5_PrimDraw( ao5PRIM *Prim );

/* ������� �������� ��������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       ao5PRIM *Prim;
 *   - ����������� �������-�������:
 *       VEC Du, Dv;
 *   - ���������:
 *       INT N, M;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL AO5_PrimCreatePlane( ao5PRIM *Prim, VEC Du, VEC Dv, INT N, INT M );

/* ������� �������� ��������� �����.
 * ���������:
 *   - ��������� �� ��������:
 *       ao5PRIM *Prim;
 *   - ����� �����:
 *       VEC �;
 *   - ������ �����:
 *       FLT R;
 *   - ���������:
 *       INT N, M;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL AO5_PrimCreateSphere( ao5PRIM *Prim, VEC C, FLT R, INT N, INT M );

/* ������� �������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       ao5PRIM *GObj;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE �����.
 */
BOOL AO5_PrimLoad( ao5PRIM *GObj, CHAR *FileName );

/***
 * ������ � ��������������� ���������
 ***/

/* �������������� ������ - ����� ���������� */
typedef struct tagao5GEOM
{
  INT NumOfPrimitives; /* ���������� ���������� */
  ao5PRIM *Prims;      /* ������ ���������� */
} ao5GEOM;

/* ������� ���������� ��������� � ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       ao5GEOM *G;
 *   - ��������� �� ����������� ��������:
 *       ao5PRIM *Prim;
 * ������������ ��������:
 *   (INT) ����� ������������ ��������� � ������� (-1 ��� ������).
 */
INT AO5_GeomAddPrim( ao5GEOM *G, ao5PRIM *Prim );

/* ������� ������������ ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       ao5GEOM *G;
 * ������������ ��������: ���.
 */
VOID AO5_GeomFree( ao5GEOM *G );

/* ������� ����������� ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       ao5GEOM *G;
 * ������������ ��������: ���.
 */
VOID AO5_GeomDraw( ao5GEOM *G );

/* ������� �������� ��������������� ������� �� G3D �����.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       ao5GEOM *G;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� - FALSE.
 */
BOOL AO5_GeomLoad( ao5GEOM *G, CHAR *FileName );

/* ������� �������� ��������.
 * ���������:
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (INT ) ������������� OpenGL ��� ��������.
 */
INT AO5_TextureLoad( CHAR *FileName );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */

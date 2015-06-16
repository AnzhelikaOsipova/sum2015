/* FILENAME: RENDER.C
 * PROGRAMMER: AO5
 * PURPOSE: Rendering system module.
 * LAST UPDATE: 10.06.2015
 */

#include <stdio.h>
#include <string.h>

#include "anim.h"
#include "render.h"

#pragma comment(lib, "glew32s")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

/* ������� */
MATR
  AO5_RndMatrWorld = AO5_UNIT_MATR,
  AO5_RndMatrView = AO5_UNIT_MATR,
  AO5_RndMatrProj = AO5_UNIT_MATR,
  AO5_RndMatrWorldViewProj = AO5_UNIT_MATR;

/* ��������� ������������� */
DBL
  AO5_RndWp = 3, AO5_RndHp = 3,     /* ������� ������� ������������� */
  AO5_RndProjDist = 5;              /* ���������� �� ��������� �������� */

/* ������� �������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       AO5PRIM *GObj;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE �����.
 */
BOOL AO5_PrimLoad( ao5PRIM *GObj, CHAR *FileName )
{
  FILE *F;
  ao5VERTEX *V;
  INT (*Facets)[3];
  INT nv = 0, nf = 0;
  static CHAR Buf[10000];

  memset(GObj, 0, sizeof(ao5PRIM));
  /* Open file */
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertices */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  /* Allocate memory for data */
  if ((V = malloc(sizeof(ao5VERTEX) * nv + sizeof(INT [3]) * nf)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  Facets = (INT (*)[3])(V + nv);
  memset(V, 0, sizeof(ao5VERTEX) * nv + sizeof(INT [3]) * nf);

  /* Read vertices */
  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%f%f%f",
        &V[nv].P.X, &V[nv].P.Y, &V[nv].P.Z);
      V[nv].C = ColorSet(Rnd0(), Rnd0(), Rnd0(), 1);
      nv++;
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT a, b, c;

      sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
              sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 ||
              sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
              sscanf(Buf + 2, "%i %i %i", &a, &b, &c);

      Facets[nf][0] = a - 1;
      Facets[nf][1] = b - 1;
      Facets[nf][2] = c - 1;
      nf++;
    }
  }
  fclose(F);
  AO5_PrimCreate(GObj, AO5_PRIM_TRIMESH, nv, nf * 3, V, (INT *)Facets);

  /* ����������� ����������� ������ */
  free(V);
  return TRUE;
} /* End of 'AO5_PrimLoad' function */

/* ������� �������� ��������.
 * ���������:
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (INT ) ������������� OpenGL ��� ��������.
 */
/* ������� �������� ��������.
 * ���������:
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (INT ) ������������� OpenGL ��� ��������.
 */
INT AO5_TextureLoad( CHAR *FileName )
{
  INT TexId = 0, i;
  HDC hMemDC, hMemDCDIB;
  BITMAP bm;
  HBITMAP hBm, hBmDIB;
  DWORD *Bits;
  BITMAPINFOHEADER bih = {0};
  /* ����� ��� ����������� ������� */
  static struct
  {
    CHAR Name[300]; /* ��� ����� */
    INT TexId;      /* ������������� ������� */
  } StoreTex[300];
  static INT StoreTexSize = 0;

  /* ���� � ��� ����������� */
  for (i = 0; i < StoreTexSize; i++)
    if (strcmp(FileName, StoreTex[i].Name) == 0)
      return StoreTex[i].TexId;

  /* ��������� ����������� �� ����� */
  hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBm == NULL)
    return 0;

  /* Create compatible context and select image into one */
  hMemDC = CreateCompatibleDC(AO5_Anim.hDC);
  hMemDCDIB = CreateCompatibleDC(AO5_Anim.hDC);
  SelectObject(hMemDC, hBm);

  /* Obtain image size */
  GetObject(hBm, sizeof(BITMAP), &bm);

  /* ������ DIB - device-independed-bitmap ������ */
  bih.biSize = sizeof(BITMAPINFOHEADER);
  bih.biWidth = bm.bmWidth;
  bih.biHeight = bm.bmHeight;
  bih.biBitCount = 32;
  bih.biCompression = BI_RGB;
  bih.biPlanes = 1;
  bih.biSizeImage = bm.bmWidth * bm.bmHeight * 4;

  hBmDIB = CreateDIBSection(NULL, (BITMAPINFO *)&bih, DIB_RGB_COLORS, &Bits, NULL, 0);
  SelectObject(hMemDCDIB, hBmDIB);

  /* ���������� � DIB �������� */
  BitBlt(hMemDCDIB, 0, 0, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY);

  /* ��������� ����� ������ */
  for (i = bm.bmWidth * bm.bmHeight - 1; i >= 0; i--)
    Bits[i] |= 0xFF000000;

  /* ��������� � ������ OpenGL */
  glGenTextures(1, &TexId);
  glBindTexture(GL_TEXTURE_2D, TexId);
  gluBuild2DMipmaps(GL_TEXTURE_2D, 4, bm.bmWidth, bm.bmHeight,
    GL_BGRA_EXT, GL_UNSIGNED_BYTE, Bits);
  glBindTexture(GL_TEXTURE_2D, 0);

  DeleteDC(hMemDC);
  DeleteDC(hMemDCDIB);
  DeleteObject(hBm);
  DeleteObject(hBmDIB);

  /* ��������� �� ������ */
  if (StoreTexSize < 300)
  {
    strncpy(StoreTex[StoreTexSize].Name, FileName, 300);
    StoreTex[StoreTexSize].TexId = TexId;
    StoreTexSize++;
  }
  return TexId;
} /* End of 'AO5_TextureLoadfunction */




/* END OF 'RENDER.C' FILE */


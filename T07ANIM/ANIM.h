/* FILENAME: ANIM.H
 * PROGRAMMER: AO5
 * PURPOSE: Animation system declaration module.
 * LAST UPDATE: 08.06.2015
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"

/* ������������ ���������� ��������� �������� */
#define AO5_MAX_UNITS 3000

/* ���������� ���� �������� �������� "������" */
typedef struct tagao5UNIT ao5UNIT;

/* ��������� �������� ��������� �������� */
typedef struct
{
  HWND hWnd;          /* ���� ������ */
  INT W, H;           /* ������ ���� ������ */
  HDC hDC;            /* �������� ���� ������ */
  HGLRC hGLRC;        /* �������� ���������� */

  /* ������ ��������� �������� � �� ���������� */
  ao5UNIT *Units[AO5_MAX_UNITS]; 
  INT NumOfUnits;
  
  /* ���������� ������������� */
  DBL
    Time,            /* ����� � �������� �� ������ �������� */
    GlobalTime,      /* ����� -"-, �� ��� ����� */
    DeltaTime,       /* ����������� ����� � �������� */
    GlobalDeltaTime, /* ����������� ����� � �������� ��� ����� */
    FPS;             /* ���������� ������ � ������� */
  BOOL
    IsPause;         /* ���� ����� */
  /* ���������� ����� */
  BYTE
    Keys[256],       /* �������� ������ ���������� � ���� */
    KeysOld[256],    /* �������� ������ �� ���������� ����� */
    KeysClick[256];  /* ����� ������������ ������� */
  INT
    MsDeltaX, MsDeltaY, /* ������������� ���������� ������� ���� */
    MsX, MsY,        /* ���������� ���������� ������� ���� */
    MsWheel;         /* ��������� ������ ���� */
  BYTE
    JButs[32],       /* �������� ������ ��������� */
    JButsOld[32],    /* �������� ������ ��������� �� ���������� ����� */
    JButsClick[32],  /* ����� ������������ ������� ������ ��������� */
    JPOV;            /* ������������� POV - 0..8 */
  DBL
    JX, JY, JZ, JR, JU, JV; /* ��� ��������� (-1.0 .. 1.0)*/

} ao5ANIM;
/* ��������� �������� �������� */
extern ao5ANIM AO5_Anim;
extern INT AO5_MouseWheel;
/* ������� ���� ������� ��������:
 * - ������ ��������� ��� ������������
 *     INT Size;
 * - ������������� ��������:
 *     INT Id;
 * - ��������� �� ������� �������������
 *     VOID (*Init)( ao5UNIT *Unit, ao5ANIM *Ani );
 * - ��������� �� ������� ���������������
 *     VOID (*Close)( ao5UNIT *Unit, ao5ANIM *Ani );
 * - ��������� �� ������� ���������� ����������� ����������
 *     VOID (*Response)( ao5UNIT *Unit, ao5ANIM *Ani );
 * - ��������� �� ������� ����������
 *     VOID (*Render)( ao5UNIT *Unit, ao5ANIM *Ani );
 */
#define AO5_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( ao5UNIT *Unit, ao5ANIM *Ani );       \
  VOID (*Close)( ao5UNIT *Unit, ao5ANIM *Ani );      \
  VOID (*Response)( ao5UNIT *Unit, ao5ANIM *Ani );   \
  VOID (*Render)( ao5UNIT *Unit, ao5ANIM *Ani )

/* ������� ��� ������� �������� */
struct tagao5UNIT
{
  AO5_UNIT_BASE_FIELDS;
};

/* ������� ������������� ��������.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 * ������������ ��������: ���.
 */
BOOL AO5_AnimInit( HWND hWnd );

/* ������� ��������������� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AO5_AnimClose( VOID );

/* ������� ��������� ��������� ������� ������� ������.
 * ���������:
 *   - ����� ������ ������� ������:
 *       INT W, H;
 * ������������ ��������: ���.
 */
VOID AO5_AnimResize( INT W, INT H );

/* ������� ���������� ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AO5_AnimRender( VOID );

/* ������� ������ ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AO5_AnimCopyFrame( VOID );

/* ������� ���������� � ������� ������� ��������.
 * ���������:
 *   - ����������� ������ ��������:
 *       ao5UNIT *Unit;
 * ������������ ��������: ���.
 */
VOID AO5_AnimAddUnit( ao5UNIT *Unit );

/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ���������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AO5_AnimFlipFullScreen( VOID );

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (ao5UNIT *) ��������� �� ��������� ������ ��������.
 */
ao5UNIT * AO5_AnimUnitCreate( INT Size );

/* ������� ������ �� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AO5_AnimDoExit( VOID );

/* ������� ��������� ����� ��������.
 * ���������:
 *   - ���� �����:
 *       BOOL NewPauseFlag;
 * ������������ ��������: ���.
 */
VOID AO5_AnimSetPause( BOOL NewPauseFlag );


#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */

/* FILENAME: UBALL.C
 * PROGRAMMER: VG4
 * PURPOSE: Bounce ball unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"

/* ��� ������������� ���� */
typedef struct tagao5UNIT_BALL
{
  AO5_UNIT_BASE_FIELDS;

  VEC Pos;     /* ������� ���� */
  DWORD Color; /* ���� ���� */
  DBL
    Amplitude,  /* ��������� */
    PhaseShift, /* ����� �� ���� */
    ScaleShift; /* ������� ������� */
} ao5UNIT_BALL;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ao5UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       ao5ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AO5_AnimUnitInit( ao5UNIT_BALL *Uni, ao5ANIM *Ani )
{
  Uni->Pos = VecSet(rand() % 1000, rand() % 700, 0);
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);
  Uni->PhaseShift = rand() % 3000;
  Uni->ScaleShift = 5 + 0.30 * rand() / RAND_MAX;
  Uni->Amplitude = 30 + 59.0 * rand() / RAND_MAX;
} /* End of 'AO5_AnimUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ao5UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       ao5ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AO5_AnimUnitClose( ao5UNIT_BALL *Uni, ao5ANIM *Ani )
{
} /* End of 'AO5_AnimUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ao5UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       ao5ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AO5_AnimUnitResponse( ao5UNIT_BALL *Uni, ao5ANIM *Ani )
{
  if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    AO5_AnimDoExit();
  if (GetAsyncKeyState('F') & 0x8000)
    AO5_AnimFlipFullScreen();
} /* End of 'AO5_AnimUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ao5UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       ao5ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AO5_AnimUnitRender( ao5UNIT_BALL *Uni, ao5ANIM *Ani )
{
  DBL shift = Uni->Amplitude * fabs(sin(Uni->ScaleShift * Ani->Time + Uni->PhaseShift));
  
  SetDCBrushColor(Ani->hDC, Uni->Color);
  Ellipse(Ani->hDC, Uni->Pos.X - 5, Uni->Pos.Y - 5 - shift, Uni->Pos.X + 5, Uni->Pos.Y + 5 - shift);
} /* End of 'AO5_AnimUnitRender' function */

/* ������� �������� ������� �������� "���".
 * ���������: ���.
 * ������������ ��������:
 *   (ao5UNIT *) ��������� �� ��������� ������ ��������.
 */
ao5UNIT * AO5_UnitBallCreate( VOID )
{
  ao5UNIT_BALL *Uni;

  if ((Uni = (VOID *)AO5_AnimUnitCreate(sizeof(ao5UNIT_BALL))) == NULL)
    return NULL;
  /* ��������� ���� */
  Uni->Init = (VOID *)AO5_AnimUnitInit;
  Uni->Close = (VOID *)AO5_AnimUnitClose;
  Uni->Response = (VOID *)AO5_AnimUnitResponse;
  Uni->Render = (VOID *)AO5_AnimUnitRender;
  return (ao5UNIT *)Uni;
} /* End of 'AO5_UnitBallCreate' function */

/* END OF 'UBALL.C' FILE */

/* FILENAME: UCONTRL.C
 * PROGRAMMER: AO5
 * PURPOSE: Control unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"

/* ��� ������������� ���� */
typedef struct tagao5UNIT_CTRL
{
  AO5_UNIT_BASE_FIELDS;
  HFONT hFnt; /* ����� ��� ������ FPS */
} ao5UNIT_CTRL;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ao5UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       AO5ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AO5_AnimUnitInit( ao5UNIT_CTRL *Uni, ao5ANIM *Ani )
{
  Uni->hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE,
    FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
    VARIABLE_PITCH, "Bookman Old Style");
} /* End of 'AO5_AnimUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ao5UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       AO5ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AO5_AnimUnitClose( ao5UNIT_CTRL *Uni, ao5ANIM *Ani )
{
  DeleteObject(Uni->hFnt);
} /* End of 'AO5_AnimUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ao5UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       AO5ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AO5_AnimUnitResponse( ao5UNIT_CTRL *Uni, ao5ANIM *Ani )
{
  if (Ani->Keys[VK_ESCAPE])
    AO5_AnimDoExit();
  if (Ani->KeysClick['F'])
    AO5_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    AO5_AnimSetPause(!Ani->IsPause);
} /* End of 'AO5_AnimUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ao5UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       AO5ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AO5_AnimUnitRender( ao5UNIT_CTRL *Uni, ao5ANIM *Ani )
{
  HFONT hFntOld = SelectObject(Ani->hDC, Uni->hFnt);
  static DBL count = 30;
  static CHAR Buf[1000];

  count += Ani->GlobalDeltaTime;
  if (count > 1)
  {
    count = 0;
    sprintf(Buf, "FPS: %.3f", Ani->FPS);
    SetWindowText(Ani->hWnd, Buf);
  }
} /* End of 'AO5_AnimUnitRender' function */

/* ������� �������� ������� �������� "����������".
 * ���������: ���.
 * ������������ ��������:
 *   (AO5UNIT *) ��������� �� ��������� ������ ��������.
 */
ao5UNIT * AO5_UnitControlCreate( VOID )
{
  ao5UNIT_CTRL *Uni;

  if ((Uni = (VOID *)AO5_AnimUnitCreate(sizeof(ao5UNIT_CTRL))) == NULL)
    return NULL;
  /* ��������� ���� */
  Uni->Init = (VOID *)AO5_AnimUnitInit;
  Uni->Close = (VOID *)AO5_AnimUnitClose;
  Uni->Response = (VOID *)AO5_AnimUnitResponse;
  Uni->Render = (VOID *)AO5_AnimUnitRender;
  return (ao5UNIT *)Uni;
} /* End of 'AO5_UnitBallCreate' function */

/* END OF 'UCONTRL.C' FILE */

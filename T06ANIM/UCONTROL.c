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
  RECT rc;
  static CHAR Buf[1000];

  SetTextColor(Ani->hDC, RGB(255, 255, 255));
  SetBkMode(Ani->hDC, TRANSPARENT);

  rc.left = 0;
  rc.top = 0;
  rc.right = Ani->W;
  rc.bottom = Ani->H;
  DrawText(Ani->hDC, Buf,
    sprintf(Buf,
      "FPS: %.3f\n"
      "J: %i %i %i, POV:%i\n"
      "%.5f %.5f %.5f %.5f %.5f %.5f",
      Ani->FPS,
      Ani->JButs[0], Ani->JButs[1], Ani->JButs[2], Ani->JPOV,
      Ani->JX, Ani->JY, Ani->JZ, Ani->JR, Ani->JU, Ani->JV),
    &rc,
    DT_TOP | DT_LEFT);

  SelectObject(Ani->hDC, hFntOld);
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

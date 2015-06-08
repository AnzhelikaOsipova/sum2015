/* FILENAME: UNIT.C
 * PROGRAMMER: VG4
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>

#include "anim.h"

/* ������� ��-��������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ao5UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       ao5ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AO5_AnimUnitInit( ao5UNIT *Uni, ao5ANIM *Ani )
{
} /* End of 'AO5_AnimUnitInit' function */

/* ������� ��-��������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ao5UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       ao5ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AO5_AnimUnitClose( ao5UNIT *Uni, ao5ANIM *Ani )
{
} /* End of 'AO5_AnimUnitClose' function */

/* ������� ��-��������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ao5UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       ao5ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AO5_AnimUnitResponse( ao5UNIT *Uni, ao5ANIM *Ani )
{
} /* End of 'AO5_AnimUnitResponse' function */

/* ������� ��-��������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ao5UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       ao5ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AO5_AnimUnitRender( ao5UNIT *Uni, ao5ANIM *Ani )
{
} /* End of 'AO5_AnimUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (ao5UNIT *) ��������� �� ��������� ������ ��������.
 */
ao5UNIT * AO5_AnimUnitCreate( INT Size )
{
  ao5UNIT *Uni;

  if (Size < sizeof(ao5UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* ��������� ���� ��-��������� */
  Uni->Size = Size;
  Uni->Init = AO5_AnimUnitInit;
  Uni->Close = AO5_AnimUnitClose;
  Uni->Response = AO5_AnimUnitResponse;
  Uni->Render = AO5_AnimUnitRender;
  return Uni;
} /* End of 'AO5_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */

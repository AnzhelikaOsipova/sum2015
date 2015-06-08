/* FILENAME: UNITS.H
 * PROGRAMMER: VG4
 * PURPOSE: Animation units samples declaration module.
 * LAST UPDATE: 08.06.2015
 */

#ifndef __UNITS_H_
#define __UNITS_H_

#include "def.h"


/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ao5UNIT *) указатель на созданный объект анимации.
 */
ao5UNIT * AO5_UnitBallCreate( VOID );
ao5UNIT * AO5_UnitBallCreate2( VOID );
ao5UNIT * AO5_UnitCowCreate( VOID );

#endif /* __UNITS_H_ */

/* END OF 'UNITS.H' FILE */

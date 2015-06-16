/* FILENAME: UNITS.H
 * PROGRAMMER: AO5
 * PURPOSE: Animation units samples declaration module.
 * LAST UPDATE: 10.06.2015
 */

#ifndef __UNITS_H_
#define __UNITS_H_

#include "def.h"

/* Функция создания объекта анимации "управление".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ao5UNIT *) указатель на созданный объект анимации.
 */
ao5UNIT * AO5_UnitControlCreate( VOID );

/* Функция создания объекта анимации "модель".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ao5UNIT *) указатель на созданный объект анимации.
 */
ao5UNIT * AO5_UnitModelCreate( VOID );

/* Функция создания мира анимации.
 * АРГУМЕНТЫ:
 *   - размер структуры объекта анимации:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ao5UNIT *) указатель на созданный объект анимации.
 */
ao5UNIT * AO5_UnitWorldCreate( IVOID );

#endif /* __UNITS_H_ */

/* END OF 'UNITS.H' FILE */

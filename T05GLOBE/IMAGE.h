/* FILE NAME: T05GLOBE
*  PROGRAMMER: AO5
*  DATE: 05.06.2015
*  PURPOSE: WinAPI windowed application sample*/


#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "def.h"

/* Image store data type */
typedef struct
{
  INT W, H;    /* Image size */
  HBITMAP hBm; /* Image handle */
  HDC hDC;     /* Image bitmap device context for drawing */
} IMAGE;

BOOL ImageLoad( IMAGE *Img, CHAR *FileName );
VOID ImageFree( IMAGE *Img );
DWORD ImageGetP( IMAGE *Img, INT X, INT Y );

#endif /* __IMAGE_H_ */

/* END OF 'IMAGE.H' FILE */

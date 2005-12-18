 /*
 				misc.h

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	EyE
*
*	Author:		E.BERTIN (IAP)
*
*	Contents:	global declarations.
*
*	Last modify:	22/08/2003
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#ifndef _FIELD_H_
#include "field.h"
#endif

/*----------------------- miscellaneous variables ---------------------------*/
char			gstr[MAXCHAR];

/*------------------------------- functions ---------------------------------*/

extern int		copyimage(fieldstruct *, PIXTYPE *, int,int,int,int);

extern float		hmedian(float *, int);

extern double		rndbl(void);

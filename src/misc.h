/*
*				misc.h
*
* Include for misc.c.
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	This file part of:	EyE
*
*	Copyright:		(C) 1998-2010 IAP/CNRS/UPMC
*				(C) 1997 ESO
*				(C) 1995,1996 Sterrewacht Leiden
*
*	Author:			Emmanuel Bertin (IAP)
*
*	License:		GNU General Public License
*
*	EyE is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
* 	(at your option) any later version.
*	EyE is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*	You should have received a copy of the GNU General Public License
*	along with EyE.  If not, see <http://www.gnu.org/licenses/>.
*
*	Last modified:		09/10/2010
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef _FIELD_H_
#include "field.h"
#endif

/*----------------------- miscellaneous variables ---------------------------*/
char			gstr[MAXCHAR];

/*------------------------------- functions ---------------------------------*/

extern int		copyimage(fieldstruct *, PIXTYPE *, int,int,int,int);

extern float		fqmedian(float *, int);

extern double		rndbl(void);

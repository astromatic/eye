/*
*				retina.h
*
* Include file for retina.c.
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

#ifndef _RETINA_H_
#define _RETINA_H_

/*----------------------------- Internal constants --------------------------*/

#define		MAX_LEARNRATE	50.0	/* default max. learn rate */

#define		DYNAMIC_RANGE	16	/* > ln(1e6) : enough for floats */
					/* must be > 0 */
#define		BINS_PER_SIGMA	4	/* number of bin per background RMS */
					/* must be > 2.0/DYNAMIC_RANGE */


/*--------------------------------- Macros ----------------------------------*/

/* Direct and inverse retina transfer functions */

#define	PIX_TO_RETINA(pix)	((pix)>0.0?log(1+(pix)):-log(1-(pix)))

#define	RETINA_TO_PIX(pix)	((pix)>0.0?exp(pix)-1:1-exp((-pix)))


/*------------------------------- structures --------------------------------*/

/* Retina info */
typedef struct structretina
  {
  int		ninputdim;		/* Dimensionality of input vector */
  int		ninput;			/* Total number of inputs */
  int		*inputsize;		/* Retina dimensions */
  float		*input;			/* Input array */
  struct	structbpann	*bpann;	/* BackProp neural net structure */
  }     retinastruct;


/*------------------------------- functions ---------------------------------*/

retinastruct	*new_retina(int *retisize, int nretidim,
			int *nn, int nnn, int nsampmax),
		*load_retina(char *filename, int nsampmax);

int		feed_retina(retinastruct *retina,
			fieldstruct **field, int nfield, int nsampmax);

void		end_retina(retinastruct *retina),
		save_retina(retinastruct *retina, char *filename),
		train_retina(retinastruct *retina, int niter,
			float lparam1, float lparam2);

#endif

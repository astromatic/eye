/*
*				back.h
*
* Include file for back.c.
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	This file part of:	AstrOmatic software
*
*	Copyrights:		(C) 1993,1998-2010 IAP/CNRS/UPMC
*				(C) 1994,1997 ESO
*				(C) 1995,1996 Sterrewacht Leiden
*
*	Author:			Emmanuel Bertin (IAP)
*
*	License:		GNU General Public License
*
*	AstrOmatic software is free software: you can redistribute it and/or
*	modify it under the terms of the GNU General Public License as
*	published by the Free Software Foundation, either version 3 of the
*	License, or (at your option) any later version.
*	AstrOmatic software is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*	You should have received a copy of the GNU General Public License
*	along with AstrOmatic software.
*	If not, see <http://www.gnu.org/licenses/>.
*
*	Last modified:		09/10/2010
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef _FIELD_H_
#include "field.h"
#endif

/*----------------------------- Internal constants --------------------------*/
#define	BACK_BUFSIZE		1048576		/* bkgnd buffer */
#define	BACK_MINGOODFRAC	0.5		/* min frac with good weights*/
#define	QUANTIF_NSIGMA		5		/* histogram limits */
#define	QUANTIF_NMAXLEVELS	4096		/* max nb of quantif. levels */
#define	QUANTIF_AMIN		4		/* min nb of "mode pixels" */

/* NOTES:
One must have:		BACK_BUFSIZE >= MAXPICSIZE
			0 < QUANTIF_NSIGMA <= 10
			QUANTIF_AMIN > 0
*/

/*------------------------------- structures --------------------------------*/
/* Background info */
typedef struct structback
  {
  float		mode, mean, sigma;	/* Background mode, mean and sigma */
  long		*histo;			/* Pointer to a histogram */
  int		nlevels;		/* Nb of histogram bins */
  float		qzero, qscale;		/* Position of histogram */
  float		lcut, hcut;		/* Histogram cuts */
  int		npix;			/* Number of pixels involved */
  }	backstruct;


/*------------------------------- functions ---------------------------------*/
extern void	backhisto(backstruct *, PIXTYPE *, size_t, int, int, int),
		backline(fieldstruct *, int, PIXTYPE *),
		backstat(backstruct *, PIXTYPE *, size_t, int, int, int),
		backrmsline(fieldstruct *, int, PIXTYPE *),
		end_back(fieldstruct *),
		filter_back(fieldstruct *),
		make_back(fieldstruct *);

extern float	backguess(backstruct *, float *, float *),
		*make_backspline(fieldstruct *, float *);



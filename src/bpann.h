/*
*				bpann.h
*
* Include file for bpann.c.
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	This file part of:	EyE
*
*	Copyright:		(C) 1993-2010 Emmanuel Bertin -- IAP/CNRS/UPMC
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

#ifndef _FITSCAT_H_
#include "fits/fitscat.h"
#endif

/*------------------------------- definitions -------------------------------*/
#define	SIGMOID(u)	((u)<15.0?((u)>-15.0?1/(1+exp(-(u))):0.0):1.0)
				/* In-line activation function */

/*---------------------------------- types ----------------------------------*/
typedef	float	NFLOAT;		/* Floating point units for neural data */

/*------------------------------- structures --------------------------------*/
typedef	struct structbpann
	{
	int	nlayers;		/* Number of "active" layers */
	int	*nn;			/* Nb of neurons per "active" layer */
/*------ The ANN itself */
	NFLOAT	**neuron;		/* Neuron array (layer,pos in layer) */
	NFLOAT	**weight;		/* Weight array (layer,pos in layer) */
	NFLOAT	**dweight;		/* Weight increment array */
	NFLOAT	**graderr;		/* Back-propagated error gradient */
	NFLOAT	**dgraderr;		/* Error gradient from previous iter */
	int	initseed;		/* Random seed for init (0=random!) */
/*------ Learn-stacks */
	int	nstack, nstackmax;	/* Current and max. nb of patterns */
	NFLOAT	*instack, *outstack;	/* Input and output pattern arrays */
	NFLOAT	*wstack;		/* Pattern weight array */
/*------ Inputs and output scaling */
	NFLOAT	*inbias, *inscale;	/* Input scaling */
	NFLOAT	*outbias, *outscale;	/* Output scaling */
/*----- Learning parameters */
	NFLOAT	learnc[10];		/* Learning coefficients */
	NFLOAT	err;			/* Estimate of the cost function */
	int	ntrain;			/* Total nb of trainings */
	int	nsweep;			/* Nb of passes through each block */
	int	linearoutflag;		/* Flag: 0 if outputs are non-linear */
	}	bpannstruct;


/*------------------------------ Prototypes ---------------------------------*/

bpannstruct	*loadtab_bpann(tabstruct *tab, int nstackmax),
		*new_bpann(int *nn, int nl, int nstackmax, int linearflag);

NFLOAT		train_bpann(bpannstruct *bpann, int restart);

int		feed_bpann(bpannstruct *bpann, NFLOAT *invec, NFLOAT *outvec,
			float w);

void		free_bpann(bpannstruct *bpann),
		play_bpann(bpannstruct *bpann, NFLOAT *invec, NFLOAT *outvec),
		savetab_bpann(bpannstruct *bpann,tabstruct *tab,char *bp_type),
		looptrain_bpann(bpannstruct *bpann, int niter);


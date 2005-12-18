/*
 				bpann.h

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	neural V2.0 routines
*
*	Author:		E.BERTIN, IAP & Leiden observatory
*
*	Contents:	type definitions for ANN (Perceptron) routines.
*
*	Last modify:	22/08/2003
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/


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


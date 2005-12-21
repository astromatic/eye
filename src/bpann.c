/*
 				bpann.c

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	Any back-propagation-ANN-oriented software
*
*	Author:		E.BERTIN (IAP)
*
*	Contents:	9 routines for neural network management.
*			Enhanced and optimized version of the 1993 routines.
*			This is the block-adaptive, conjugate-gradient version
*			of the original bp.c.
*
*	Requirements:	The LDACTools.
*
*	Last modify:	21/12/2005
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#ifdef HAVE_CONFIG_H
#include	"config.h"
#endif

#ifdef HAVE_MATHIMF_H
#include	<mathimf.h>
#else
#include	<math.h>
#endif
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<time.h>

#include	"define.h"
#include	"globals.h"
#include	"fits/fitscat.h"	/* LDACTools needed */
#include	"bpann.h"
#include	"misc.h"
#include	"prefs.h"

/******************************** new_bpann **********************************/
/*
Allocate memory for a BP-ANN structure (neurones, weights...).
Returns a pointer to the allocated ANN structure.
*/
bpannstruct	*new_bpann(int *nn, int nl, int nstackmax, int linearflag)

  {
   bpannstruct	*bpann;
   NFLOAT	*weight, norm;
   int		i,l,w, nweight;

/* It should at least be a linear Perceptron... */
  if (nl<2)
    return NULL;

/* Allocate memory for the pointers to each layer */
  QCALLOC(bpann, bpannstruct, 1);
  bpann->nlayers = nl;
  bpann->nstackmax = nstackmax;
  bpann->linearoutflag = linearflag;
  QMALLOC(bpann->nn, int, nl);
  QMALLOC(bpann->neuron, NFLOAT *, nl);
  QMALLOC(bpann->weight, NFLOAT *, nl-1);
  QMALLOC(bpann->dweight, NFLOAT *, nl-1);
  QMALLOC(bpann->graderr, NFLOAT *, nl-1);
  QMALLOC(bpann->dgraderr, NFLOAT *, nl-1);

/* "Learn-stacks" */
  QMALLOC(bpann->instack, NFLOAT, nn[0]*nstackmax);
  QMALLOC(bpann->outstack, NFLOAT, nn[nl-1]*nstackmax);
  QMALLOC(bpann->wstack, NFLOAT, nstackmax);

/* Input and output scaling */
  QMALLOC(bpann->inbias, NFLOAT, nn[0]);
  QMALLOC(bpann->inscale, NFLOAT, nn[0]);
  QMALLOC(bpann->outbias, NFLOAT, nn[nl-1]);
  QMALLOC(bpann->outscale, NFLOAT, nn[nl-1]);

/* By default, Input and Output are scaled to unity */
  for (i=0; i<nn[0]; i++)
    {
    bpann->inbias[i] = 0.0;
    bpann->inscale[i] = 1.0;
    }
  for (i=0; i<nn[nl-1]; i++)
    {
    bpann->outbias[i] = 0.0;
    bpann->outscale[i] = 1.0;
    }

/* Loop over the "true" layers */
  for (l=0; l<nl; l++)
    {
    bpann->nn[l] = nn[l];
    if (l<nl-1)
      {
/*---- The total number of weights includes biases */
      nweight = nn[l+1]*(1 + nn[l]);
      QCALLOC(bpann->dweight[l], NFLOAT, nweight); /* null "speed" at start */
      QMALLOC(bpann->graderr[l], NFLOAT, nweight);
      QCALLOC(bpann->dgraderr[l], NFLOAT, nweight);
      QMALLOC(bpann->weight[l], NFLOAT, nweight);
/*---- Weights are initialized to random values (uniform white noise) */
      weight = bpann->weight[l];
      norm = sqrt(nn[l]+1.0);
      if (bpann->initseed)
        srand((unsigned int)bpann->initseed);
      else
        srand((unsigned int)time(NULL));
      for (w=0; w<nweight; w++)
        *(weight++) = (2.0*rand()/RAND_MAX-1.0)/norm;

      QMALLOC(bpann->neuron[l], NFLOAT, nn[l]+1);
/*-- Initialize fake "bias" neurons to a constant value */
      bpann->neuron[l][nn[l]] = -1.0;
      }
    else
      QMALLOC(bpann->neuron[l], NFLOAT, nn[l]);	/* no bias in the last layer */
    }

  return bpann;
  }


/******************************** free_bpann *********************************/
/*
Free all memory modules allocated for a Back-Propagation ANN structure.
*/
void	free_bpann(bpannstruct *bpann)

  {
   int		i;

/* Loop over the "true" layers */
  for (i=0; i<bpann->nlayers-1; i++)
    {
    free(bpann->neuron[i]);
    free(bpann->weight[i]);
    if (bpann->dweight)
      free(bpann->dweight[i]);
    if (bpann->graderr)
      free(bpann->graderr[i]);
    if (bpann->dgraderr)
      free(bpann->dgraderr[i]);
    }

  free(bpann->neuron[i]);	/* Because of the input layer */

/* Then free pointers of pointers */
  free(bpann->neuron);
  free(bpann->weight);
  free(bpann->dweight);
  free(bpann->graderr);
  free(bpann->dgraderr);
  free(bpann->nn);

/* Free "learn-stacks" */
  free(bpann->instack);
  free(bpann->outstack);
  free(bpann->wstack);

/* Free input and output layers */
  free(bpann->inbias);
  free(bpann->inscale);
  free(bpann->outbias);
  free(bpann->outscale);

/* And finally free the ANN structure itself */
  free(bpann);

  return;
  }


/******************************** feed_bpann ********************************/
/*
Add a new pattern to the learn-stacks. Returns RETURN_ERROR if the learning
stack is full.
*/
int	feed_bpann(bpannstruct *bpann, NFLOAT *invec, NFLOAT *outvec, float w)
  {
   NFLOAT	*bias,*scale, *stack;
   int		i;

/* Input vector */
  bias = bpann->inbias;
  scale = bpann->inscale;
  stack = bpann->instack+bpann->nstack*bpann->nn[0];
  for (i=bpann->nn[0]; i--;)
    *(stack++) = *(scale++)**(invec++)+*(bias++);

/* Output vector */
  bias = bpann->outbias;
  scale = bpann->outscale;
  stack = bpann->outstack+bpann->nstack*bpann->nn[bpann->nlayers-1];
  for (i=bpann->nn[bpann->nlayers-1]; i--;)
    *(stack++) = *(scale++)**(outvec++)+*(bias++);

/* pattern weight vector */
   *(bpann->wstack+bpann->nstack) = (NFLOAT)w;

  return (++bpann->nstack == bpann->nstackmax)?RETURN_ERROR:RETURN_OK;
  }


/****************************** looptrain_bpann *****************************/
/*
Loop several times over the learn-stack and perform training.
*/
void	looptrain_bpann(bpannstruct *bpann, int niter)
  {
   NFLOAT	*wstack;
   double	 wsum;
   char		str[MAXCHAR];
   int		i;

  bpann->nsweep = niter;
  sprintf(str, "Learning started: %d patterns / %d iterations.\n",
	bpann->nstack, bpann->nsweep);
  NFPRINTF(OUTPUT,str);

/* Normalize weights */
  wsum = 0.0;
  wstack = bpann->wstack;
  for (i=bpann->nstack; i--;)
    wsum += *(wstack++);
  wsum /= bpann->nstack;
  if (wsum <= 0.0)
    error(EXIT_FAILURE,"*Error*: null pattern weight sum found","");
  wstack = bpann->wstack;
  for (i=bpann->nstack; i--;)
    *(wstack++) /= (NFLOAT)wsum;

  for (i=0; i<niter; i++)
    {
    sprintf(str, "Iterations: %d / Normalized error: %f",
	i+1, train_bpann(bpann, i));
    NFPRINTF(OUTPUT, str);
    }

  return;
  }


/******************************** train_bpann *******************************/
/*
Perform backpropagation learning on a learn-stack, using the RPROP algorithm.
Returns the quadratic error on the set of vector-pair.
*/
NFLOAT	train_bpann(bpannstruct *bpann, int restart)
  {
   double	outerr;
   NFLOAT	u, bperr, dir,
		*instack,*outstack, *neuroni,*neuronj, *weight,*dweight,
		*graderr,*dgraderr, *wstack, etam, etap, deltamin,deltamax, w;
   int		i,j,l,lp,nl,nlp, nweight, t, outflag;

  outerr = 0.0;
  instack = bpann->instack;
  outstack = bpann->outstack;
  wstack = bpann->wstack;

/* Loop over the pattern to compute the total error gradient vector */
  for (t=0; t<bpann->nstack; t++)
    {
    w = *(wstack++);

/*-- First step: a simple forward pass through the network */
    for (l=0; l<bpann->nlayers; l++)
      {
      neuronj = bpann->neuron[l];
      if (l)
        {
        lp = l-1;
        weight = bpann->weight[lp];
        outflag = (!bpann->linearoutflag) || (l < bpann->nlayers-1);
        for (j=bpann->nn[l]; j--;)
          {     /* note we don't touch the "bias" neuron (=-1) */
          neuroni = bpann->neuron[lp];
          u = *(weight++)**(neuroni++);
          for (i=bpann->nn[lp]; i--;)   /* The last one is the bias */
            u += *(weight++)**(neuroni++);
          *(neuronj++) = outflag?SIGMOID(u):u;
          }
        }
      else
        {
        memcpy(neuronj, instack, bpann->nn[0]*sizeof(NFLOAT));
        instack += bpann->nn[0];
        }
      }

/*-- Second step: back-propagation of the output error */
    for (l=bpann->nlayers-1; l; l--)
      {
      lp = l-1;
      nlp = bpann->nn[lp] + 1;
      nl = bpann->nn[l];
      neuroni = bpann->neuron[lp];
      if (l==bpann->nlayers-1)
        {
        neuronj = bpann->neuron[l];
        for (j=nl; j--;)
          {
          bperr = *neuronj-*(outstack++);
          if (bpann->linearoutflag)
            *(neuronj++) = bperr * w;
          else
	    {
            *(neuronj) *= (1-*neuronj) * bperr * w;
            neuronj++;
            }
          outerr += (double)bperr*bperr * w;
          }
        }

/*-- This time the inner loop concerns the downstream layer */
      for (i=0; i<nlp; neuroni++)       /* The last one is the bias */
        {
        bperr = 0.0;
        neuronj = bpann->neuron[l];
        weight = bpann->weight[lp]+i;
        graderr = bpann->graderr[lp]+i;
        for (j=nl; j--; weight+=nlp, graderr+=nlp)
          {
          bperr += *weight**neuronj;
          if (!t)
            *graderr = 0.0;
          *graderr += *neuroni**(neuronj++);
          }
/*------ We use the neuron array to store the back-propagated error */
        if ((++i != nlp) && lp)
          *neuroni *= (1-*neuroni)*bperr;
        }
      }
    }

/* Now compute inertia and update dgraderr */
  etam = 0.5;
  etap = 1.2;
  deltamin = 1e-6;
  deltamax = bpann->learnc[1];
  for (l=0; l<bpann->nlayers-1; l++)
    {
    graderr = bpann->graderr[l];
    dgraderr = bpann->dgraderr[l];
    weight = bpann->weight[l];
    dweight = bpann->dweight[l];
    nweight = bpann->nn[l+1] * (1 + bpann->nn[l]);
    for (i=nweight; i--; dweight++)
      {
      if (!restart)
        *dweight = bpann->learnc[0];
      if ((dir = *graderr**dgraderr)>=0.0)
        {
        if (dir!=0.0 && (*dweight*=etap)>deltamax)
          *dweight = deltamax;
        *(weight++) += *graderr>0.0?-*dweight:*dweight;
        *(dgraderr++) = *(graderr++);
        }
      else
        {
        if ((*dweight*=etam)<deltamin)
          *dweight = deltamin;
        *(dgraderr++) = 0.0;
        graderr++;
        weight++;
        }
      }
    }

  bpann->err = outerr = sqrt(outerr/bpann->nstack);
  bpann->ntrain += bpann->nstack;

  return (NFLOAT)bpann->err;
  }


/******************************** play_bpann *********************************/
/*
Single forward pass through the ANN.
*/
void	play_bpann(bpannstruct *bpann, NFLOAT *invec, NFLOAT *outvec)
  {
   NFLOAT	u, *bias,*scale, *neuroni,*neuronj, *weight;
   int		i,j,l,lp,ll;

  bias = bpann->inbias;
  scale = bpann->inscale;
  neuronj = bpann->neuron[0];
  for (i=bpann->nn[0]; i--;)
    *(neuronj++) = *(scale++)**(invec++)+*(bias++);
  bias = bpann->outbias;
  scale = bpann->outscale;
  ll = bpann->nlayers-1;
  for (l=1; l<bpann->nlayers; l++)
    {
    neuronj = bpann->neuron[l];
    lp = l-1;
    weight = bpann->weight[lp];
    for (j=bpann->nn[l]; j--;)
      {       /* note we don't touch the "bias" neuron (=-1) */
      neuroni = bpann->neuron[lp];
      u = *(weight++)**(neuroni++);
      for (i=bpann->nn[lp]; i--;)     /* The last one is the bias */
        u += *(weight++)**(neuroni++);
      if (l == ll)
        {
        *neuronj = bpann->linearoutflag?u:SIGMOID(u);
        *(outvec++) = (*(neuronj++)-*(bias++))/(*(scale++));
        }
      else
        *(neuronj++) = SIGMOID(u);
      }
    }

  return;
  }


/******************************* loadtab_bpann *******************************/
/*
Load the complete ANN structure (using the LDACTools).
*/
bpannstruct	*loadtab_bpann(tabstruct *tab, int nstackmax)
  {
   bpannstruct	*bpann;
   keystruct	*key;
   char		*head, str[80];
   int		l, nweight;

/* Allocate memory for the ANN structure */
  QCALLOC(bpann, bpannstruct, 1);

/* Load important scalars (which are stored as FITS keywords) */
  head = tab->headbuf;
  if (fitsread(head, "BPNLAYER", &bpann->nlayers, H_INT, T_LONG) != RETURN_OK)
    goto headerror;
  if (fitsread(head, "BPLEARN1",&bpann->learnc[0],H_FLOAT,T_FLOAT)!=RETURN_OK)
    goto headerror;
  if (fitsread(head, "BPLEARN2",&bpann->learnc[1],H_FLOAT,T_FLOAT)!=RETURN_OK)
    goto headerror;
  if (fitsread(head, "BPNTRAIN",&bpann->ntrain,H_INT,T_LONG)!=RETURN_OK)
    goto headerror;
  if (fitsread(head, "BPLINEAR",&bpann->linearoutflag,H_INT,T_LONG)!=RETURN_OK)
    bpann->linearoutflag = 0;

/* Load all vectors!! */
  read_keys(tab, NULL, NULL, 0, NULL);
/* Now interpret the result */
  if (!(key = name_to_key(tab, "INPUT_BIAS"))) goto headerror;
  bpann->inbias = key->ptr;
  key->ptr = 0;	/* In order not to free it when the cat is no longer needed */
  if (!(key = name_to_key(tab, "INPUT_SCALE"))) goto headerror;
  bpann->inscale = key->ptr; key->ptr = 0;
  if (!(key = name_to_key(tab, "OUTPUT_BIAS"))) goto headerror;
  bpann->outbias = key->ptr; key->ptr = 0;
  if (!(key = name_to_key(tab, "OUTPUT_SCALE"))) goto headerror;
  bpann->outscale = key->ptr; key->ptr = 0;
  if (!(key = name_to_key(tab, "NNEUR_PER_LAYER"))) goto headerror;
  bpann->nn = key->ptr; key->ptr = 0;
  QMALLOC(bpann->neuron, NFLOAT *, bpann->nlayers);
  QMALLOC(bpann->weight, NFLOAT *, bpann->nlayers-1);
  if (nstackmax)
    {
    QMALLOC(bpann->dweight, NFLOAT *, bpann->nlayers-1);
    QMALLOC(bpann->graderr, NFLOAT *, bpann->nlayers-1);
    QMALLOC(bpann->dgraderr, NFLOAT *, bpann->nlayers-1);
/*-- "Learn-stacks" */
    QMALLOC(bpann->instack, NFLOAT, bpann->nn[0]*nstackmax);
    QMALLOC(bpann->outstack, NFLOAT, bpann->nn[bpann->nlayers-1]*nstackmax);
    QMALLOC(bpann->wstack, NFLOAT, nstackmax);
    bpann->nstackmax = nstackmax;
    }
  for (l=0; l<bpann->nlayers-1; l++)
    {
    QMALLOC(bpann->neuron[l], NFLOAT, bpann->nn[l]+1);
    bpann->neuron[l][bpann->nn[l]] = -1.0;
    sprintf(str, "WEIGHT_LAYER%d", l+1);
    if (!(key = name_to_key(tab, str))) goto headerror;
      bpann->weight[l] = key->ptr; key->ptr = 0;
    if (nstackmax) /* if learn mode: null "speed" at start */
      {
      nweight = bpann->nn[l+1]*(1 + bpann->nn[l]);
      QCALLOC(bpann->dweight[l], NFLOAT, nweight);
      QMALLOC(bpann->graderr[l], NFLOAT, nweight);
      QCALLOC(bpann->dgraderr[l], NFLOAT, nweight);
      }
    }

  QMALLOC(bpann->neuron[l], NFLOAT, bpann->nn[l]); /* no bias in this layer */

  return bpann;

headerror:
  error(EXIT_FAILURE, "*Error*: incorrect BP-ANN header in file", "");

  return NULL;
  }


/******************************* savetab_bpann *******************************/
/*
Save the complete ANN structure (using the LDACTools).
*/
void	savetab_bpann(bpannstruct *bpann, tabstruct *tab, char *bp_type)
  {
   keystruct	*key;
   char		*head, str[80];
   int		l;

  head = tab->headbuf;
/* Add and write important scalars as FITS keywords */
  fitsadd(head, "BPNLAYER", "Total number of layers (incl. input)");
  fitswrite(head, "BPNLAYER", &bpann->nlayers, H_INT, T_LONG);
  fitsadd(head, "BPLEARN1", "Learning coefficient 1");
  fitswrite(head, "BPLEARN1", &bpann->learnc[0], H_FLOAT, T_FLOAT);
  fitsadd(head, "BPLEARN2", "Learning coefficient 2");
  fitswrite(head, "BPLEARN2", &bpann->learnc[1], H_FLOAT, T_FLOAT);
  fitsadd(head, "BPNTRAIN", "Number of training samples so far");
  fitswrite(head, "BPNTRAIN", &bpann->ntrain, H_INT, T_LONG);
  fitsadd(head, "BPERROR ", "RMS error during last training run");
  fitswrite(head, "BPERROR ", &bpann->err, H_FLOAT, T_FLOAT);
  fitsadd(head, "BPLINEAR", "Output-layer linearity flag");
  fitswrite(head, "BPLINEAR", &bpann->linearoutflag, H_INT, T_LONG);
  fitsadd(head, "BPTYPE", "Neural network type");
  fitswrite(head, "BPTYPE", bp_type, H_STRING, T_STRING);

/* Create and fill the arrays */
  key = new_key("NNEUR_PER_LAYER");
  key->naxis = 1;
  QMALLOC(key->naxisn, int, key->naxis);
  key->naxisn[0] = bpann->nlayers;
  key->nobj = 1;
  strcat(key->comment, "Number of neurons for each layer (incl. input)");
  key->htype = H_INT;
  key->ttype = T_LONG;
  key->nbytes = key->naxisn[0]*t_size[T_LONG];
  key->ptr = bpann->nn;
  add_key(key, tab, 0);

  key = new_key("INPUT_BIAS");
  key->naxis = 1;
  QMALLOC(key->naxisn, int, key->naxis);
  key->naxisn[0] = bpann->nn[0];
  key->nobj = 1;
  strcat(key->comment, "ANN input = input*INPUT_SCALE+INPUT_BIAS");
  key->htype = H_FLOAT;
  key->ttype = T_FLOAT;
  key->nbytes = key->naxisn[0]*t_size[T_FLOAT];
  key->ptr = bpann->inbias;
  add_key(key, tab, 0);

  key = new_key("INPUT_SCALE");
  key->naxis = 1;
  QMALLOC(key->naxisn, int, key->naxis);
  key->naxisn[0] = bpann->nn[0];
  key->nobj = 1;
  strcat(key->comment, "ANN input = input*INPUT_SCALE+INPUT_BIAS");
  key->htype = H_FLOAT;
  key->ttype = T_FLOAT;
  key->nbytes = key->naxisn[0]*t_size[T_FLOAT];
  key->ptr = bpann->inscale;
  add_key(key, tab, 0);

  key = new_key("OUTPUT_BIAS");
  key->naxis = 1;
  QMALLOC(key->naxisn, int, key->naxis);
  key->naxisn[0] = bpann->nn[bpann->nlayers-1];
  key->nobj = 1;
  strcat(key->comment, "ANN output = output*OUTPUT_SCALE+OUTPUT_BIAS");
  key->htype = H_FLOAT;
  key->ttype = T_FLOAT;
  key->nbytes = key->naxisn[0]*t_size[T_FLOAT];
  key->ptr = bpann->outbias;
  add_key(key, tab, 0);

  key = new_key("OUTPUT_SCALE");
  key->naxis = 1;
  QMALLOC(key->naxisn, int, key->naxis);
  key->naxisn[0] = bpann->nn[bpann->nlayers-1];
  key->nobj = 1;
  strcat(key->comment, "ANN output = output*OUTPUT_SCALE+OUTPUT_BIAS");
  key->htype = H_FLOAT;
  key->ttype = T_FLOAT;
  key->nbytes = key->naxisn[0]*t_size[T_FLOAT];
  key->ptr = bpann->outscale;
  add_key(key, tab, 0);

  for (l=0; l<bpann->nlayers-1; l++)
    {
    sprintf(str, "WEIGHT_LAYER%d", l+1);
    key = new_key(str);
    key->naxis = 1;
    QMALLOC(key->naxisn, int, key->naxis);
    key->naxisn[0] =  bpann->nn[l+1]*(1 + bpann->nn[l]);
    key->nobj = 1;
    strcat(key->comment, "Weight vector for this layer");
    key->htype = H_FLOAT;
    key->ttype = T_FLOAT;
    key->nbytes = key->naxisn[0]*t_size[T_FLOAT];
    key->ptr = bpann->weight[l];
    add_key(key, tab, 0);
    }

  return;
  }


 /*
 				retina.c

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	EyE
*
*	Author:		E.BERTIN (IAP), C.MARMO (IAP)
*
*	Contents:	Handling of retinae.
*
*	Last modify:	22/12/2005
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#ifdef HAVE_CONFIG_H
#include	"config.h"
#endif

#ifdef HAVE_MATHIMF_H
#include	<mathimf.h>
#else
#endif
#include	<math.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"define.h"
#include	"globals.h"
#include	"bpann.h"
#include	"field.h"
#include	"fits/fitscat.h"
#include	"misc.h"
#include	"prefs.h"
#include	"retina.h"
#include	"readimage.h"

/******************************* new_retina **********************************/
/*
Prepare a new retina.
*/
retinastruct	*new_retina(int *retisize, int nretidim,
			int *nn, int nnn, int nsampmax)

  {
   retinastruct	*retina;
   int		i, *size;

  QMALLOC(retina, retinastruct, 1);
  retina->ninputdim = nretidim;
  QMALLOC(retina->inputsize, int, retina->ninputdim);

  retina->ninput = 1;
  size = retina->inputsize;
  for (i=nretidim; i--;)
    retina->ninput *= (*(size++)=*(retisize++));

  QMALLOC(retina->input, float, retina->ninput);
  nn[0] = retina->ninput;

  retina->bpann = new_bpann(nn, prefs.nnn_size, nsampmax, 1);

  return retina;
  }


/******************************** load_retina ********************************/
/*
Read a retina file.
*/
retinastruct      *load_retina(char *filename, int nsampmax)

  {
#define FILTEST(x) \
	if (x != RETURN_OK) \
	  error(EXIT_FAILURE, "*Error*: RETINA header in ", filename)

   retinastruct	*retina;
   catstruct	*fcat;
   tabstruct	*ftab;
   char		str[82];
   int		i;

  QMALLOC(retina, retinastruct, 1);
/* We first map the catalog */
  if (!(fcat = read_cat(filename)))
    error(EXIT_FAILURE, "*Error*: retina file not found: ", filename);
/* Test if the requested table is present */
  if (!(ftab = name_to_tab(fcat,"BP-ANN", 0)))
    error(EXIT_FAILURE, "*Error*: no BP-ANN info found in ", filename);
  FILTEST(fitsread(ftab->headbuf,"BPTYPE  ", gstr,H_STRING,T_STRING));
  if (strcmp(gstr, "RETINA"))
    error(EXIT_FAILURE, "*Error*: not a retina in ", filename);
  FILTEST(fitsread(ftab->headbuf,"RENAXIS ", &retina->ninputdim,H_INT,T_LONG));
  retina->ninput = 1;
  for (i=0; i<retina->ninputdim; i++)
    {
    sprintf(str, "RENAXIS%1d", i+1);
    FILTEST(fitsread(ftab->headbuf, str, retina->inputsize + i, H_INT,T_LONG));
    retina->ninput *= retina->inputsize[i];
    }

  retina->bpann = loadtab_bpann(ftab, nsampmax);
  if (retina->ninput != retina->bpann->nn[0])
    error(EXIT_FAILURE, "*Error*: retina incompatible with the ANN in ",
	filename);

  QMALLOC(retina->input, float, retina->ninput);

  close_cat(fcat);
  free_cat(&fcat,1);

  return retina;
  }


/******************************* save_retina *********************************/
/*
Save the complete ANN structure (using the LDACTools).
*/
void	save_retina(retinastruct *retina, char *filename)
  {
   catstruct	*cat;
   tabstruct	*tab;
   char		*head, str[82];
   int		i;

/* Create the catalog stuff */
  cat = new_cat(1);
  init_cat(cat);
  tab = new_tab("BP-ANN");
  add_tab(tab, cat, 0);
  head = tab->headbuf;
/* Add and write important scalars as FITS keywords */
  fitsadd(head, "RENAXIS ", "Number of retina dimensions");
  fitswrite(head, "RENAXIS ", &retina->ninputdim, H_INT, T_LONG);
  for (i=0; i<retina->ninputdim; i++)
    {
    sprintf(str, "RENAXIS%1d", i+1);
    fitsadd(head, str, "Retina pixels along the this dimension");
    fitswrite(head, str, retina->inputsize + i, H_INT, T_LONG);
    }

/* The "pure" BP-ANN part */
  savetab_bpann(retina->bpann, tab, "RETINA");

/* Then, just save everything */
  save_cat(cat, filename);

/* Free memory but don't touch my arrays!! */
  blank_keys(tab);
  free_cat(&cat, 1);

  return;
  }


/******************************* end_retina **********************************/
/*
Delete a retina.
*/
void	end_retina(retinastruct *retina)

  {
  free_bpann(retina->bpann);
  free(retina->input);
  free(retina->inputsize);
  free(retina);

  return;
  }


/****************************** train_retina *********************************/
/*
Launch retina training.
*/
void	train_retina(retinastruct *retina, int niter,
			float lparam1, float lparam2)

  {
  if (lparam1 && lparam2)
    {
    retina->bpann->learnc[0] = lparam1;
    retina->bpann->learnc[1] = lparam2;
    }

  looptrain_bpann(retina->bpann, niter);

  return;
  }


/******************************* feed_retina *********************************/
/*
Build a multi-dimensional histogram for weighting image-image learning,
and use it to select or weight appropriately image samples feeding the
neural net.
*/
int	feed_retina(retinastruct *retina, fieldstruct **field, int nfield,
			int nsampmax)

  {
   fieldstruct	**fieldt;
   catstruct	*cat;
   tabstruct	*tab;
   char		str[80];
   PIXTYPE	**scan, **scant, 
		pix;
   float	*input,
		sig, binoff, quota, frac;

   int		*bin,*bint, *step,*stept, *fcurpos,*fcurpost,
		index, nbin;
   int		i,x,y, w,h, histow, noccup, xmin,xmax,ymin,ymax;

  w = (*field)->width;
  h = (*field)->height;
  sig = (*field)->backsig;
  NFPRINTF(OUTPUT, "Histogramming...");
/* Init scan counters */
  histow = 2*DYNAMIC_RANGE*BINS_PER_SIGMA;	/* twice: pos + neg. values */
  binoff = ((histow-1)/2)+0.5;
  QMALLOC(step, int, nfield);
  QMALLOC(fcurpos, int, nfield);
  nbin = 1;
  stept = step;
  fieldt = field;
  fcurpost = fcurpos;
  for (i=nfield; i--; fieldt++)
    {
    (*fieldt)->y = (*fieldt)->stripy = (*fieldt)->ymin
	= (*fieldt)->stripylim = (*fieldt)->stripysclim = 0;
    *(stept++) = nbin;
    nbin *= histow;

/* Save current position in file */
    QFTELL(*(fcurpost++), (*fieldt)->cat->file, (*fieldt)->filename);
    }

/* Allocate memory space for the multi-dimensional histogram */
  QCALLOC(bin, int, nbin);
  QMALLOC(scan, PIXTYPE *, nfield);


  xmin = prefs.lim[0]>0? prefs.lim[0]:0;
  xmax = prefs.lim[2]>0? prefs.lim[2]:w;
  ymin = prefs.lim[1]>0? prefs.lim[1]:0;
  ymax = prefs.lim[3]>0? prefs.lim[3]:h;

/* Go line-per-line */
  for (y=0; y<h; y++)
    {
    fieldt = field;
    scant = scan;
    for (i=nfield; i--; fieldt++)
      {
      (*fieldt)->stripy = ((*fieldt)->y=y)%(*fieldt)->stripheight;
      *(scant++) = ((*fieldt)->stripy==(*fieldt)->stripysclim)?
		load_strip((*fieldt))
		: &(*fieldt)->strip[(*fieldt)->stripy*(*fieldt)->width];
      }
    if (y>=ymin && y<ymax)
      for (x=0; x<w; x++)
        {
        index = 0;
        scant = scan;
        stept = step;
        fieldt = field;
        for (i=nfield; i--; fieldt++)
          {
          pix = *((*scant)++)/sig;
          scant++;
          index += *(stept++)*(int)(BINS_PER_SIGMA*PIX_TO_RETINA(pix)+binoff);
          }
        if (x>=xmin && x<xmax && index>=0 && index<nbin)
          bin[index]++;
        }
    }

/* Count occupied bins */
  NFPRINTF(OUTPUT, "Counting occupied bins...");
  bint = bin;
  for (noccup=0,index=nbin; index--;)
    if (*(bint++))
      noccup++;
  quota = (float)nsampmax/noccup;

/* Init scan counters once more */
  sprintf(str, "Feeding the pattern stack (%d max. more, %5.1f%% free)",
	nsampmax, 100.0 - 100.0*retina->bpann->nstack/retina->bpann->nstackmax);
  NFPRINTF(OUTPUT, str);
  fieldt = field;
  fcurpost = fcurpos;
  for (i=nfield; i--; fieldt++)
    {
    (*fieldt)->y = (*fieldt)->stripy = (*fieldt)->ymin
	= (*fieldt)->stripylim = (*fieldt)->stripysclim = 0;

/* Go back to the original position in file*/
    QFSEEK((*fieldt)->cat->file, *(fcurpost++), SEEK_SET, (*fieldt)->filename);
    }

/* 2nd pass line-by-line */
  for (y=0; y<h; y++)
    {
    fieldt = field;
    scant = scan;
    for (i=nfield; i--; fieldt++)
      {
      (*fieldt)->stripy = ((*fieldt)->y=y)%(*fieldt)->stripheight;
      *(scant++) = ((*fieldt)->stripy==(*fieldt)->stripysclim)?
		load_strip((*fieldt))
		: &(*fieldt)->strip[(*fieldt)->stripy*(*fieldt)->width];
      }
    for (x=0; x<w; x++)
      {
      index = 0;
      scant = scan;
      stept = step;
      fieldt = field;
      for (i=nfield; i--; fieldt++)
        {
        pix = *((*scant)++)/sig;
        scant++;
        pix = PIX_TO_RETINA(pix);
        index += *(stept++)*(int)(BINS_PER_SIGMA*pix+binoff);
        }
      if (y>=ymin && y<ymax && x>=xmin && x<xmax && index>=0 && index<nbin)
        {
/*------ Apply pixel selection or weighting */
        if ((frac=quota/bin[index])>1.0 || rndbl()<frac)
          {
/*-------- *field is the first field in the list (in) */
          copyimage(*field, retina->input,
		retina->inputsize[0], retina->inputsize[1], x, y);
          input=retina->input;
          for (i=retina->ninput; i--; input++)
            *input = PIX_TO_RETINA(*input/sig);
/*-------- pix is the current pixel from the last field in the list (out) */
          if (feed_bpann(retina->bpann, retina->input, &pix, frac>1.0?frac:1.0)
		== RETURN_ERROR)
            goto end_feed_retina;
          }
        }
      }
    }

end_feed_retina:

  if (prefs.check_type == CHECK_HISTOGRAM)
    {
    cat = new_cat(1);
    init_cat(cat);
    tab = cat->tab;
    tab->naxis = 2;	/* This is an image */
    QMALLOC(tab->naxisn, int, tab->naxis);
    tab->bitpix =  BP_LONG;
    tab->bytepix = t_size[T_LONG];
    tab->naxisn[0] = histow;
    tab->naxisn[1] = nbin/histow;
    tab->tabsize = tab->bytepix*tab->naxisn[0]*tab->naxisn[1];
    tab->bodybuf = (void *)bin;
    save_cat(cat, prefs.check_name);
    tab->bodybuf = NULL;
    free_cat(&cat, 1);
    }

/* Free memory */
  free(bin);
  free(scan);
  free(fcurpos);

  return retina->bpann->nstack;
  }



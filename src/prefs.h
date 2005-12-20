 /*
 				prefs.h

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	EyE
*
*	Author:		E.BERTIN (IAP)
*
*	Contents:	Include for prefs.c
*
*	Last modify:	20/12/2005
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#ifndef _FIELD_H_
#include "field.h"
#endif

#ifndef _PREFS_H_
#define _PREFS_H_

/*------------------------------- preferences -------------------------------*/
typedef struct
  {
  int		npair;					/* Nb of input pairs */
  char		prefs_name[MAXCHAR];			/* prefs filename*/
  char		retina_name[MAXCHAR];			/* retina filename*/
  int		retina_size[2];				/* retina size */
  int		nretina_size;				/* nb of params */
  int		nn_size[4];				/* neural net size */
  int		nnn_size;				/* nb of params */
  enum	{OUT_RELATIVE, OUT_ABSOLUTE, OUT_BINARY}
					out_type;	/* output type */
  enum	{LEARN_NONE, LEARN_NEW, LEARN_RESUME, LEARN_RESTART}
					learn_type;	/* learning type */
  double	learn_rate[2];				/* learning rate */
  int		nlearn_rate;				/* nb of params */
  int		niter;					/* nb of passes */
  int		nsamp_max;				/* nb of patterns */
/*----- background */
  int		subback_flag[MAXINFIELD];		/* subtraction flag */
  int		nsubback_flag;				/* nb of params */
  backenum	back_type[MAXINFIELD];			/* subtraction type */
  int           nback_type;				/* nb of params */
  int		back_size[2];				/* bkgnd mesh size */
  int		nback_size;				/* nb of params */
  int		back_fsize[2];				/* bkgnd filt. size */
  int		nback_fsize;				/* nb of params */
  double	back_default[MAXINFIELD];		/* Default background*/
  int		nback_default;				/* nb of params */
  double        back_fthresh;				/* Filter threshold */
  int		lim[4];					/* frame limits */
  int		nlim;					/* nb of params */
  enum	{CHECK_NONE, CHECK_FILTERED, CHECK_RESIDUALS, CHECK_HISTOGRAM}
					check_type;	/* check-image type */
  char		check_name[MAXCHAR];			/* check-image name */
/* Multithreading */
  int		nthreads;		/* Number of active threads */
  int		nnodes;			/* Number of nodes (for clusters) */  
/*----- miscellaneous */
  int		node_index;		/* Node index (for multiprocessing) */ 
  enum	{QUIET, NORM, FULL}		verbose_type;	/* display type */

/*----- customize */
  }	prefstruct;

prefstruct	prefs;

/*----------------------------- Internal constants --------------------------*/

#define		MAXLIST		32	/* max. nb of list members */

/*-------------------------------- protos -----------------------------------*/
extern int	cistrcmp(char *cs, char *ct, int mode);

extern void	dumpprefs(int state),
		readprefs(char *filename,char **argkey,char **argval,int narg),
		useprefs(void);

#endif



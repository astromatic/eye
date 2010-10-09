/*
*				prefs.h
*
* Include file for prefs.c.
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	This file part of:	EyE
*
*	Copyright:		(C) 1998-2010 IAP/CNRS/UPMC
*				(C) 1997 ESO
*				(C) 1995,1996 Sterrewacht Leiden
*
*	Authors:		Emmanuel Bertin (IAP)
*				Chiara Marmo (IAP)
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
/*----- miscellaneous */
  enum	{QUIET, NORM, FULL}		verbose_type;	/* display type */
  char		sdate_start[12];		/* EyE start date */
  char		stime_start[12];		/* EyE start time */
  char 		sdate_end[12];			/* EyE end date */
  char		stime_end[12];			/* EyE end time */
  int		time_diff;			/* Execution time */

/*----- customize */
  }	prefstruct;

prefstruct	prefs;

/*----------------------------- Internal constants --------------------------*/

#define		MAXCHARL	16384	/* max. nb of chars in a string list */
#define		MAXLIST		32	/* max. nb of list members */
#define		MAXLISTSIZE	2000000	/* max size of list */

/*-------------------------------- protos -----------------------------------*/
extern char	*list_to_str(char *listname);
extern int	cistrcmp(char *cs, char *ct, int mode);

extern void	dumpprefs(int state),
		readprefs(char *filename,char **argkey,char **argval,int narg),
		useprefs(void);

#endif



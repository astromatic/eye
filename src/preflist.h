 /*
 				preflist.h

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	EyE
*
*	Author:		E.BERTIN (IAP)
*
*	Contents:	Keywords for the configuration file.
*
*	Last modify:	22/08/2003
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include "key.h"

#ifndef _PREFS_H_
#include "prefs.h"
#endif

#ifndef _RETINA_H_
#include "retina.h"
#endif

#ifdef	USE_THREADS
#define	THREADS_PREFMAX	THREADS_NMAX
#else
#define	THREADS_PREFMAX	65535
#endif

int idummy;

pkeystruct key[] =
 {
  {"BACK_DEFAULT", P_FLOATLIST, prefs.back_default, 1,7, -BIG, BIG,
   {""}, 1, MAXINFIELD, &prefs.nback_default},
  {"BACK_FILTTHRESH", P_FLOAT, &prefs.back_fthresh, 0,0, -BIG, BIG},
  {"BACK_SIZE", P_INTLIST, prefs.back_size, 1,2000000000, 0.0,0.0,
   {""}, 1, MAXINFIELD, &prefs.nback_size},
  {"BACK_FILTERSIZE", P_INTLIST, prefs.back_fsize, 1,7, 0.0,0.0,
   {""}, 1, MAXINFIELD, &prefs.nback_fsize},
  {"BACK_TYPE", P_KEYLIST, prefs.back_type, 0,0, 0.0,0.0,
   {"AUTO", "MANUAL", ""},
   1, MAXINFIELD, &prefs.nback_type},
  {"BUFFER_MAXSIZE", P_INT, &prefs.nsamp_max, 1,2000000000},
  {"CHECKIMAGE_NAME", P_STRING, prefs.check_name},
  {"CHECKIMAGE_TYPE", P_KEY, &prefs.check_type, 0,0, 0.0,0.0,
   {"NONE", "FILTERED", "RESIDUALS", "HISTOGRAM"}},
  {"FRAME_LIMITS", P_INTLIST, prefs.lim, 1, 2000000000, 0.0,0.0, {""},
     1,4, &prefs.nlim},
  {"LEARNING_TYPE", P_KEY, &prefs.learn_type, 0,0, 0.0,0.0,
   {"NONE","NEW", "RESUME", "RESTART"}},
  {"LEARNING_RATE", P_FLOATLIST, prefs.learn_rate, 0,0, 0.0, 1e6, {""},
     1,2, &prefs.nlearn_rate},
  {"NN_SIZE", P_INTLIST, &prefs.nn_size[1], 1, 1024, 0.0,0.0, {""},
     1,3, &prefs.nnn_size},
  {"NNODES", P_INT, &prefs.nnodes, 1, 65535},
  {"NODE_INDEX", P_INT, &prefs.node_index, -1, 65534},
  {"NTHREADS", P_INT, &prefs.nthreads, 1, THREADS_PREFMAX},
  {"PASSES", P_INT, &prefs.niter, 1,2000000000},
  {"RETINA_NAME", P_STRING, prefs.retina_name},
  {"RETINA_SIZE", P_INTLIST, prefs.retina_size, 1, 1024, 0.0,0.0, {""},
     1,2, &prefs.nretina_size},
  {"SUBTRACT_BACK", P_BOOLLIST, prefs.subback_flag, 0,0, 0.0,0.0,
   {""}, 1, MAXINFIELD, &prefs.nsubback_flag},
  {"VERBOSE_TYPE", P_KEY, &prefs.verbose_type, 0,0, 0.0,0.0,
   {"QUIET","NORMAL","FULL",""}},
  {""}
 };

char			keylist[sizeof(key)/sizeof(pkeystruct)][16];
const char		notokstr[] = {" \t=,;\n\r\""};

char *default_prefs[] =
 {
"# Default configuration file for " BANNER " " MYVERSION,
"# EB " DATE,
"#",
"#-------------------------------- Retina -------------------------------------",
" ",
"RETINA_NAME		default.ret	# Name of the file containing retina weights",
"RETINA_SIZE		5,5		# Retina size: <size> or <width>,<height>",
"#---------------------------- Neural Network ---------------------------------",
" ",
"LEARNING_TYPE		NEW             # Can be one of NONE, NEW, RESUME or RESTART",
"LEARNING_RATE		0.1, 50.0	# <learn rate> or <learn rate>,<max. learn rate>",
" ",
"NN_SIZE		12,8,1		# Number of neurons per layer (max. 4 layers)",
"PASSES			100000000	# Number of pattern presentations",
"BUFFER_MAXSIZE		200000		# Maximum number of different patterns used",
" ",
"#--------------------------- Background subtraction ---------------------------",
" ",
"SUBTRACT_BACK		Y		# Subtraction sky background (Y/N)?",
"					# (all or for each image)",
" ",
"BACK_TYPE		AUTO		# \"AUTO\" or \"MANUAL\"",
"					# (all or for each image)",
"BACK_DEFAULT		0.0		# Default background value in MANUAL",
"					# (all or for each image)",
"BACK_SIZE		128		# Background mesh size (pixels)",
"					# (all or for each image)",
"BACK_FILTERSIZE		3		# Background map filter range (meshes)",
"					# (all or for each image)",
"*BACK_FILTTHRESH	0.0		# Threshold above which the background-",
"*					# map filter operates",
" ",
"#------------------------------ Check Image ----------------------------------",
" ",
"CHECKIMAGE_TYPE	NONE		# may be one of NONE, FILTERED, RESIDUALS",
"					# or HISTOGRAM",
"CHECKIMAGE_NAME	check.fits	# Filename for the check-image",
" ",
"#------------------------------ Miscellaneous ---------------------------------",
" ",
"VERBOSE_TYPE		NORMAL		# \"QUIET\",\"NORMAL\" or \"FULL\"",
"*NNODES		1		# Number of nodes (for clusters)",
"*NODE_INDEX		0		# Node index (for clusters)",
" ",
#ifdef USE_THREADS
"NTHREADS		2		# Number of simultaneous threads for",
"					# the SMP version of SWarp",
#else
"NTHREADS		1		# 1 single thread",
#endif
  ""
 };


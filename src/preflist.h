/*
*				preflist.h
*
* Configuration keyword definition.
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
  {"FRAME_LIMITS", P_INTLIST, prefs.lim, -1, 2000000000, 0.0,0.0, {""},
     1,4, &prefs.nlim},
  {"LEARNING_TYPE", P_KEY, &prefs.learn_type, 0,0, 0.0,0.0,
   {"NONE","NEW", "RESUME", "RESTART"}},
  {"LEARNING_RATE", P_FLOATLIST, prefs.learn_rate, 0,0, 0.0, 1e6, {""},
     1,2, &prefs.nlearn_rate},
  {"NN_SIZE", P_INTLIST, &prefs.nn_size[1], 1, 1024, 0.0,0.0, {""},
     1,3, &prefs.nnn_size},
  {"NPASSES", P_INT, &prefs.niter, 1,2000000000},
  {"NTHREADS", P_INT, &prefs.nthreads, 0, THREADS_PREFMAX},
  {"RETINA_NAME", P_STRING, prefs.retina_name},
  {"RETINA_SIZE", P_INTLIST, prefs.retina_size, 1, 1024, 0.0,0.0, {""},
     1,2, &prefs.nretina_size},
  {"SUBTRACT_BACK", P_BOOLLIST, prefs.subback_flag, 0,0, 0.0,0.0,
   {""}, 1, MAXINFIELD, &prefs.nsubback_flag},
  {"VERBOSE_TYPE", P_KEY, &prefs.verbose_type, 0,0, 0.0,0.0,
   {"QUIET","NORMAL","FULL",""}},
  {""}
 };

char			keylist[sizeof(key)/sizeof(pkeystruct)][32];
const char		notokstr[] = {" \t=,;\n\r\""};

char *default_prefs[] =
 {
"# Default configuration file for " BANNER " " MYVERSION,
"# EB " DATE,
"#",
"#-------------------------------- Retina -------------------------------------",
" ",
"RETINA_NAME            default.ret     # File containing retina weights",
"RETINA_SIZE            5,5             # Retina size: <size> or <width>,<height>",
" ",
"#---------------------------- Neural Network ---------------------------------",
" ",
"LEARNING_TYPE          NEW             # NONE, NEW, RESUME or RESTART",
"LEARNING_RATE          0.1, 50.0       # <learn rate> or",
"                                       # <learn rate>,<max. learn rate>",
"NN_SIZE                12,8,1          # Neurons per layer (max. 3 layers)",
"NPASSES                100             # Nb of passes through the training set",
"BUFFER_MAXSIZE         200000          # Max.number of different patterns used",
" ",
"#--------------------------- Background subtraction ---------------------------",
" ",
"SUBTRACT_BACK          Y               # Subtract sky background (Y/N)?",
"                                       # (all or for each image)",
"*BACK_TYPE              AUTO            # \"AUTO\" or \"MANUAL\"",
"                                       # (all or for each image)",
"*BACK_DEFAULT           0.0             # Default background value in MANUAL",
"                                       # (all or for each image)",
"BACK_SIZE              128             # Background mesh size (pixels)",
"                                       # (all or for each image)",
"BACK_FILTERSIZE        3               # Background map filter range (meshes)",
"                                       # (all or for each image)",
"*BACK_FILTTHRESH        0.0             # Threshold above which the background-",
"*                                       # map filter operates",
" ",
"#------------------------------ Check Image ----------------------------------",
" ",
"CHECKIMAGE_TYPE        NONE            # may be one of NONE, or HISTOGRAM",
"CHECKIMAGE_NAME        check.fits      # Filename for the check-image",
" ",
"#------------------------------ Miscellaneous ---------------------------------",
" ",
"FRAME_LIMITS           -1              # xmin, ymin, xmax, ymax of rectangular",
"                                       # area to use (-1 is the whole frame)",
"VERBOSE_TYPE           NORMAL          # \"QUIET\",\"NORMAL\" or \"FULL\"",
#ifdef USE_THREADS
"NTHREADS               0               # Number of simultaneous threads for",
"                                       # the SMP version of EyE",
#else
"NTHREADS               1               # 1 single thread",
#endif
  ""
 };


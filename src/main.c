/*
*				main.c
*
* Parse the commande line.
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	This file part of:	EyE
*
*	Copyright:		(C) 1995-2010 Emmanuel Bertin -- IAP/CNRS/UPMC
*				& Chiara Marmo -- IAP/CNRS
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

#ifdef HAVE_CONFIG_H
#include	"config.h"
#endif

#include	<ctype.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"define.h"
#include	"globals.h"
#include	"fits/fitscat.h"
#include	"prefs.h"

#define		SYNTAX \
"eye -i Input_image(s) -o Output_image(s) [-<keyword> <value>]\n" \
" [-c <configuration_file>]\n" \
"> to dump a default configuration file: eye -d \n" \
"> to dump a default extended configuration file: eye -dd"

/********************************** main ************************************/

int	main(int argc, char *argv[])
  {
   char		**argkey, **argval;
   int		a,abi,aei,nai, abo,aeo,nao, narg, opt, opt2;

  if (argc<2)
    {
    fprintf(OUTPUT, "\n         %s  version %s (%s)\n", BANNER,MYVERSION,DATE);
    fprintf(OUTPUT, "\nCopyright %s\n", COPYRIGHT);
    fprintf(OUTPUT, "Written by %s\n", AUTHORS);
    fprintf(OUTPUT, "\nVisit %s\n", WEBSITE);
    fprintf(OUTPUT, "\n%s\n", DISCLAIMER);
    error(EXIT_SUCCESS, "SYNTAX: ", SYNTAX);
    }

  QMALLOC(argkey, char *, argc);
  QMALLOC(argval, char *, argc);

/* default parameters */
  abi=aei=abo=aeo=nai=nao=0;
  narg = 0;
  strcpy(prefs.prefs_name, "eye.conf");

  for (a=1; a<argc; a++)
    {
    if (*(argv[a]) == '-')
      {
      opt = (int)argv[a][1];
      if (strlen(argv[a])<4 || opt == '-')
        {
	opt2 = (int)tolower((int)argv[a][2]);
        if (opt == '-')
	  {
	  opt = opt2;
          opt2 = (int)tolower((int)argv[a][3]);
	  }
        switch(opt)
          {
          case 'c':
            if (a<(argc-1))
              strcpy(prefs.prefs_name, argv[++a]);
            break;
          case 'd':
            dumpprefs(opt2=='d' ? 1 :0);
            exit(EXIT_SUCCESS);
            break;
          case 'i':
            for(abi = ++a; (a<argc) && (*argv[a]!='-'); a++);
            aei = a--;
            nai = aei - abi;
            break;
          case 'o':
            for(abo = ++a; (a<argc) && (*argv[a]!='-'); a++);
            aeo = a--;
            nao = aeo - abo;
            break;
          case 'v':
            printf("%s version %s (%s)\n", BANNER,MYVERSION,DATE);
            exit(EXIT_SUCCESS);
            break;
          case 'h':
          default:
            error(EXIT_SUCCESS,"SYNTAX: ", SYNTAX);
          }
	}
      else
        {
        argkey[narg] = &argv[a][1];
        argval[narg++] = argv[++a];
        }       
      }
    else
      error(EXIT_SUCCESS,"SYNTAX: ", SYNTAX);
    }

  if (nao!=nai)
    error(EXIT_FAILURE,
	"*Error*: the number of input and output images is different!", "");
  prefs.npair = nai;
  readprefs(prefs.prefs_name, argkey, argval, narg);
  useprefs();
  free(argkey);
  free(argval);

  makeit(argv+abi, argv+abo, nai);

  NFPRINTF(OUTPUT, "");
  NPRINTF(OUTPUT, "> All done (in %d s)\n", prefs.time_diff);

  exit(EXIT_SUCCESS);
  return 0;
  }

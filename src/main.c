 /*
 				main.c

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	EyE
*
*	Author:		E.BERTIN, (IAP)
*
*	Contents:	parsing of the command line.
*
*	Last modify:	19/08/2003
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#ifdef HAVE_CONFIG_H
#include	"config.h"
#endif

#include	<ctype.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#ifdef HAVE_MPI
#include        <mpi.h>
#endif

#include	"define.h"
#include	"globals.h"
#include	"fits/fitscat.h"
#include	"prefs.h"

#define		SYNTAX \
"eye -i Input_image(s) -o Output_image(s) [-<keyword> <value>]\n" \
" [-c <configuration_file>]\n" \
" or, to dump a default configuration file:\n" \
"eye -d \n"

/********************************** main ************************************/

int	main(int argc, char *argv[])
  {
   static char	prefsname[MAXCHAR];
   char		**argkey, **argval;
   int		a,abi,aei,nai, abo,aeo,nao, narg, opt, opt2;

#ifdef HAVE_MPI
  MPI_Init (&argc,&argv);
#endif

  if (argc<2)
    {
    fprintf(OUTPUT, "\n         %s  version %s (%s)\n", BANNER,MYVERSION,DATE);
    fprintf(OUTPUT, "\nby %s\n", COPYRIGHT);
    fprintf(OUTPUT, "visit %s\n", WEBSITE);
    error(EXIT_SUCCESS, "SYNTAX: ", SYNTAX);
    }

  QMALLOC(argkey, char *, argc);
  QMALLOC(argval, char *, argc);

/* default parameters */
  abi=aei=abo=aeo=nai=nao=0;
  narg = 0;
  strcpy(prefs.prefs_name, "default.eye");

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
              strcpy(prefsname, argv[++a]);
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
            exit(0);
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

  readprefs(prefsname, argkey, argval, narg);
  useprefs();
  free(argkey);
  free(argval);

  if (nao!=nai)
    error(EXIT_FAILURE,
	"*Error*: the number of input and output images is different!", "");

#ifdef HAVE_MPI
  MPI_Comm_size(MPI_COMM_WORLD,&prefs.nnodes);
  MPI_Comm_rank(MPI_COMM_WORLD,&prefs.node_index);
#endif

  makeit(argv+abi, argv+abo, nai);

  NFPRINTF(OUTPUT, "All done");
  NPRINTF(OUTPUT, "\n");

#ifdef HAVE_MPI
  MPI_Finalize();
#endif

  exit(EXIT_SUCCESS);
  return 0;
  }
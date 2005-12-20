 /*
 				makeit.c

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	EyE
*
*	Author:		E.BERTIN (IAP)
*
*	Contents:	Main loop.
*
*	Last modify:	20/12/2005
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#ifdef HAVE_CONFIG_H
#include	"config.h"
#endif

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<time.h>

#ifdef HAVE_MPI
#include	<mpi.h>
#endif

#include	"define.h"
#include	"globals.h"
#include	"fits/fitscat.h"
#include	"prefs.h"
#include	"retina.h"

/********************************** makeit ***********************************/
/*
*/
void	makeit(char **inputnames, char **outputnames, int nb)

  {
   retinastruct	*retina;
   fieldstruct	*in, *out, *fields[2];
   catstruct	*cati, *cato;
   tabstruct	*tabi, *tabo;
   time_t	thetime, thetime2;
   struct tm	*tm;
   char		str1[16], str2[16];
   int		i,j,k,n, nstack=0;

/* Processing start date and time */
  thetime = time(NULL);
  tm = localtime(&thetime);
  sprintf(prefs.sdate_start,"%04d-%02d-%02d",
        tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday);
  sprintf(prefs.stime_start,"%02d:%02d:%02d",
        tm->tm_hour, tm->tm_min, tm->tm_sec);

  NFPRINTF(OUTPUT, "");
  QPRINTF(OUTPUT,
        "----- %s %s started on %s at %s with %d thread%s\n\n",
                BANNER,
                MYVERSION,
                prefs.sdate_start,
                prefs.stime_start,
                prefs.nthreads,
                prefs.nthreads>1? "s":"");

  NFPRINTF(OUTPUT, "Initializing the retina structure...");
  if (prefs.learn_type == LEARN_NEW)
    retina = new_retina(prefs.retina_size, prefs.nretina_size,
	prefs.nn_size, prefs.nnn_size, prefs.nsamp_max);
  else
    retina = load_retina(prefs.retina_name, prefs.nsamp_max);
  NFPRINTF(OUTPUT, "Examining frames...");
  n = 0;
  for (i=0; i<nb; i++)
    {
    if (!(cati=read_cat(inputnames[i])))
      {
      sprintf(gstr, "*Error*: %s not found", inputnames[i]);
      error(EXIT_FAILURE, gstr,"");
      }
    if (!(cato=read_cat(outputnames[i])))
      {
      sprintf(gstr, "*Error*: %s not found", outputnames[i]);
      error(EXIT_FAILURE, gstr,"");
      }
/*-- Examine all extensions */
    tabi = cati->tab;
    tabo = cato->tab;
    k = -1;
    for (j=0; j<cati->ntab; j++)
      {
      if (tabi->naxis && (!(tabi->tfields && tabi->bitpix==8 && tabi->pcount)))
        {
        while (++k<cato->ntab)
	  {
          if (tabo->naxis && (!(tabo->tfields && tabo->bitpix==8 &&
				tabo->pcount)))
            break;
          tabo = tabo->nexttab;
          }
        if (k >= cato->ntab)
	  {
          warning("Missing extensions in ", cato->filename);
          tabi = tabi->nexttab;
          break;
          }

/*------ Prepare image reading */
        in = load_field(inputnames[i], j, i);
        out = load_field(outputnames[i], k, i);
        NPRINTF(OUTPUT, "\n");
        if (in->width!=out->width || in->height!=out->height)
          error(EXIT_FAILURE, "*Error*: image size different from input in ",
	    outputnames[i]);
        if (cati->ntab>1)
          sprintf(str1, "[%d/%d]", j , cati->ntab-1);
        else
          *str1 = '\0';
        if (cato->ntab>1)
          sprintf(str2, "[%d/%d]", k , cato->ntab-1);
        else
          *str2 = '\0';
        fprintf(OUTPUT, "Input-pair #%4d: %.30s%s & %.30s%s",
		n+1,
		in->rfilename, str1,
		out->rfilename, str2);
/*------ Put it in the learning-list */
        fields[0] = in;
        fields[1] = out;
        nstack = feed_retina(retina, fields, 2, prefs.nsamp_max);
        end_field(in);
        end_field(out);
        n++;
        }
      tabi = tabi->nexttab;
      }

    free_cat(&cati, 1);
    free_cat(&cato, 1);
    if (nstack == prefs.nsamp_max)
      {
      warning("Pattern stack maximum reached in ", inputnames[i]);
      break;
      }
    }

/* Actual training */
  if (!nstack)
    error(EXIT_FAILURE, "*Error*: pattern stack empty","");

  if (prefs.learn_type != LEARN_NONE)
    {
    train_retina(retina, prefs.niter/nstack,
	(float)prefs.learn_rate[0], (float)prefs.learn_rate[1]);
    NFPRINTF(OUTPUT, "Saving retina...");
    save_retina(retina, prefs.retina_name);
    }

  end_retina(retina);

/* Processing end date and time */
  thetime2 = time(NULL);
  tm = localtime(&thetime2);
  sprintf(prefs.sdate_end,"%04d-%02d-%02d",
        tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday);
  sprintf(prefs.stime_end,"%02d:%02d:%02d",
        tm->tm_hour, tm->tm_min, tm->tm_sec);
  prefs.time_diff = difftime(thetime2, thetime);

  return;
  }


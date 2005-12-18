 /*
 				field.c

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	EyE
*
*	Author:		E.BERTIN (IAP)
*
*	Contents:	Handling of field structures.
*
*	Last modify:	22/08/2003
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#ifdef HAVE_CONFIG_H
#include	"config.h"
#endif

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"define.h"
#include	"globals.h"
#include	"fits/fitscat.h"
#include	"back.h"
#include	"field.h"
#include	"prefs.h"

/****** load_field ************************************************************
PROTO	fieldstruct *load_field(char *filename, int frameno)
PURPOSE	Initialize a field structure (in read mode)
INPUT	File name,
	FITS extension number in file (0=primary).
OUTPUT	The new field pointer if OK, NULL otherwise.
NOTES	-.
AUTHOR	E. Bertin (IAP)
VERSION	22/08/2003
 ***/
fieldstruct	*load_field(char *filename, int frameno, int fieldno)

  {
   fieldstruct	*field;
   tabstruct	*tab;
   int		i;

/* First allocate memory for the new field (and nullify pointers) */
  QCALLOC(field,fieldstruct, 1);
  field->flags = FIELD_READ;
  field->frameno = frameno;
  strcpy (field->filename, filename);
/* A short, "relative" version of the filename */
  if (!(field->rfilename = strrchr(field->filename, '/')))
    field->rfilename = field->filename;
  else
    field->rfilename++;

/* Check the image exists and read important info (image size, etc...) */
  field->cat = read_cat(filename);
  if (!field->cat)
    error(EXIT_FAILURE, "*Internal Error*: Can't read ", filename);
  tab=field->cat->tab;

/* Select the desired FITS extension (if frameno != 0) */
  if (frameno >= field->cat->ntab)
    error(EXIT_FAILURE, "*Internal Error*: FITS extension unavailable in ",
		filename);
  for (i=frameno; i--;)
    tab = tab->nexttab;
  field->tab = tab;

  if (tab->naxis<1)
    error(EXIT_FAILURE, "*Error*: Zero-dimensional table in ", filename);

/* Force data to be at least 2D */
  if (tab->naxis<2)
    {
    tab->naxis = 2;
    QREALLOC(tab->naxisn, int, 2);
    tab->naxisn[1] = 1;
    }

/* Set field width and field height (the latter can be "virtual") */
  field->width = tab->naxisn[0];
  field->height = 1;
  for (i=1; i<tab->naxis; i++)
    field->height *= tab->naxisn[i];

/*-- Background */
  field->backw = prefs.back_size[fieldno]<field->width ?
					prefs.back_size[fieldno]
					: field->width;
  field->backh = prefs.back_size[fieldno]<field->height ?
					prefs.back_size[fieldno]
					: field->height;
  field->nbackp = field->backw * field->backh;
  if ((field->nbackx = (field->width-1)/field->backw + 1) < 1)
    field->nbackx = 1;
  if ((field->nbacky = (field->height-1)/field->backh + 1) < 1)
    field->nbacky = 1;
  field->nback = field->nbackx * field->nbacky;
  field->nbackfx = field->nbackx>1 ? prefs.back_fsize[fieldno] : 1;
  field->nbackfy = field->nbacky>1 ? prefs.back_fsize[fieldno] : 1;
/* Set the back_type flag if absolute background is selected */
  field->back_type = prefs.back_type[fieldno];
  field->backdefault = prefs.back_default[fieldno];

/* Now make the background map */
  make_back(field);

/* Prepare the image buffer */
  field->stripheight = prefs.retina_size[1];
  field->stripmargin = (prefs.retina_size[1]-1)/2;

  if (!(field->strip=(PIXTYPE *)malloc(field->stripheight*field->width
	*sizeof(PIXTYPE))))
    error(EXIT_FAILURE,"Not enough memory for the image buffer in ",
	field->rfilename);

  NPRINTF(OUTPUT, "    Background: %.7g   RMS: %.7g\n",
		field->backmean, field->backsig);

  return field;
  }


/****** end_field ************************************************************
PROTO	void end_field(fieldstruct *field)
PURPOSE	Free a field structure.
INPUT	field structure pointer.
OUTPUT	-.
NOTES	-.
AUTHOR	E. Bertin (IAP)
VERSION	20/08/2003
 ***/
void	end_field(fieldstruct *field)

  {
/* Check first that a tab structure is present */
  if (field->tab)
    {
/*-- End memory mapping (if allocated) */
    free_body(field->tab);

/*-- Close cat if not already done */
    if (field->cat)
      free_cat(&field->cat, 1);
    field->tab = NULL;
    }

  end_back(field);
  field->pix = NULL;
  free(field);

  return;
  }

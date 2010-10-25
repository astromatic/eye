/*
*				field.c
*
* Handle fields (aka images).
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	This file part of:	EyE
*
*	Copyright:		(C) 1995-2010 Emmanuel Bertin -- IAP/CNRS/UPMC
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
PROTO	fieldstruct *load_field(char *filename, int frameno, int fieldno)
PURPOSE	Initialize a field structure (in read mode)
INPUT	File name,
	FITS extension number in file (0=primary).
OUTPUT	The new field pointer if OK, NULL otherwise.
NOTES	-.
AUTHOR	E. Bertin (IAP)
VERSION	20/12/2005
 ***/
fieldstruct	*load_field(char *filename, int frameno, int fieldno)

  {
   char		str[512];
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

  sprintf(str,"Examining Image %s", field->rfilename);
  NFPRINTF(OUTPUT, str);

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
  field->npix = field->width*field->height;

/*-- Background */
  field->backw = prefs.back_size[0]<field->width ?
					prefs.back_size[0]
					: field->width;
  field->backh = prefs.back_size[1]<field->height ?
					prefs.back_size[1]
					: field->height;
  field->nbackp = field->backw * field->backh;
  if ((field->nbackx = (field->width-1)/field->backw + 1) < 1)
    field->nbackx = 1;
  if ((field->nbacky = (field->height-1)/field->backh + 1) < 1)
    field->nbacky = 1;
  field->nback = field->nbackx * field->nbacky;
  field->nbackfx = field->nbackx>1 ? prefs.back_fsize[0] : 1;
  field->nbackfy = field->nbacky>1 ? prefs.back_fsize[1] : 1;
/* Set the back_type flag if absolute background is selected */
  field->back_type = prefs.back_type[fieldno];
  field->backdefault = prefs.back_default[fieldno];
  printinfo_field(field);

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


/****** printinfo_field ******************************************************
PROTO	void printinfo_field(fieldstruct *field)
PURPOSE	Print info about a field
INPUT	Pointer to the field.
OUTPUT	-.
NOTES	-.
AUTHOR	E. Bertin (IAP)
VERSION	16/08/2003
 ***/
void	printinfo_field(fieldstruct *field)

  {  
/* Information about the file */
  if (field->frameno)
      sprintf(gstr, "[%d]", field->frameno);
    else
      *gstr ='\0';
  QPRINTF(OUTPUT, "%s%s \"%.20s\"  %dx%d  %d bits (%s)\n",
        field->rfilename, gstr, *field->ident? field->ident: "no ident",
        field->width, field->height, field->tab->bytepix*8,
        field->tab->bitpix>0?
                (field->tab->compress_type!=COMPRESS_NONE ?
                        "compressed":"integers") : "floats");

  return;
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

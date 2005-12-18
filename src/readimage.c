/*
 				readimage.c

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	EyE
*
*	Author:		E.BERTIN (IAP)
*
*	Contents:	functions for input of image data.
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
#include	"prefs.h"
#include	"readimage.h"

/******************************* load_strip **********************************/
/*
Load a new strip of pixel data into the buffer.
*/
PIXTYPE	*load_strip(fieldstruct *field)

  {
   tabstruct	*tab;
   PIXTYPE	*data, *datat, *back;
   int		x, y, w;

  w = field->width;
  if (!field->cat || !(tab=field->cat->tab))
    return NULL;

  if (!field->y)
    {
/*- First strip */
     int	nbpix;

    nbpix = w*field->stripheight;
    read_body(tab, field->strip, (size_t)nbpix);
    data = field->strip;
    if (prefs.subback_flag)
      for (y=0; y<field->stripheight; y++, data += w)
      {
/*---- This is the only place where one can pick-up safely the current bkg */
      backline(field, y, field->backline);
      datat = data;
      back = field->backline;
      for (x=w; x--; )
        *(datat++) -= *(back++);
      }

    field->ymax = field->stripheight;
    if (field->ymax < field->height)
      field->stripysclim = field->stripheight - field->stripmargin;
    }
  else
    {
/*- other strips */
    data = field->strip + field->stripylim*w;
/*---- copy to Check-image the "oldest" line before it is replaced */
    read_body(tab, data, (size_t)w);
/*-- Interpolate and subtract the background at current line */
    if (prefs.subback_flag)
      {
      backline(field, field->ymax, field->backline);
      datat = data;
      back = field->backline;
      for (x=w; x--; )
        *(datat++) -= *(back++);
      }
/*---- Check-image stuff */
    field->stripylim = (++field->ymin)%field->stripheight;
    if ((++field->ymax)<field->height)
      field->stripysclim = (++field->stripysclim)%field->stripheight;
    }

  return field->strip + field->stripy*w;
  }


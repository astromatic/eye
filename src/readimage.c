/*
*				readimage.c
*
* Image I/O functions.
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


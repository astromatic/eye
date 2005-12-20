 /*
 				field.h

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

#ifndef _FITSCAT_H_
#include "fits/fitscat.h"
#endif

#ifndef _FIELD_H_
#define _FIELD_H_

#define		MAXINFIELD	8192	/* Maximum number of input files */

/*------------------------------ field flags --------------------------------*/
#define		DETECT_FIELD	0x01	/* Detection */
#define		MEASURE_FIELD	0x02	/* Measurement */
#define		FLAG_FIELD	0x04	/* Flagging */
#define		RMS_FIELD	0x08	/* Weighting with std deviations */
#define		VAR_FIELD	0x10	/* Weighting with variances */
#define		WEIGHT_FIELD	0x20	/* Weighting with weights */
#define		BACKRMS_FIELD	0x40	/* Weighting from a backrms matrix */

#define		FIELD_READ	0x100	/* Field available for read access */
#define		FIELD_WRITE	0x200	/* Field available for read access */

/*--------------------------------- typedefs --------------------------------*/
typedef enum	{BACK_RELATIVE, BACK_ABSOLUTE}	backenum;

typedef struct field
  {
  char		filename[MAXCHAR];	/* image filename */
  char		*rfilename;		/* pointer to the reduced image name */
  char		ident[MAXCHAR];		/* field identifier (read from FITS) */
  catstruct	*cat;			/* cat structure */
  tabstruct	*tab;			/* tab structure */
/* ---- main image parameters */
  int		frameno;		/* pos in Multi-extension FITS file */
  int		width, height;		/* x,y size of the field */
  size_t	npix;			/* total number of pixels */
/* ---- background parameters */
  float		*back;			/* ptr to the background map in mem */
  float		*dback;			/* ptr to the background deriv. map */
  float		*sigma;			/* ptr to the sigma map */
  float		*dsigma;		/* Ptr to the sigma deriv. map */
  int		backw, backh;		/* x,y size of a bkgnd mesh */
  int		nbackp;			/* total nb of pixels per bkgnd mesh */
  int		nbackx, nbacky;		/* x,y number of bkgnd meshes */
  int		nback;			/* total number of bkgnd meshes */
  int		nbackfx, nbackfy;	/* x,y size of bkgnd filtering mask */
  double       	backdefault;		/* default background value */
  double       	backmean;		/* median bkgnd value in image */
  double       	backsig;		/* median bkgnd rms in image */
  double	sigfac;			/* scaling RMS factor (for WEIGHTs) */
  PIXTYPE	*pix;			/* pixel data */
  PIXTYPE	*backline;		/* current interpolated bkgnd line */
  backenum     	back_type;		/* background type */
  int		y;			/* y current position in field */
  int		ymin;			/* y limit (lowest accessible) */
  int		ymax;			/* y limit (highest accessible+1) */
  PIXTYPE	*strip;			/* pointer to the image buffer */
  int		stripheight;		/* height  of a strip (in lines) */
  int		stripmargin;		/* number of lines in margin */
  int		stripstep;		/* number of lines at each read */
  int		stripy;			/* y position in buffer */
  int		stripylim;		/* y limit in buffer */
  int		stripysclim;		/* y scroll limit in buffer */
/* ---- astrometric parameters */
  int		flags;			/* flags defining the field type */
/* ---- image interpolation */
  PIXTYPE	weight_thresh;		/* interpolation threshold */
  }	fieldstruct;

/*------------------------------- functions ---------------------------------*/

extern fieldstruct	*load_field(char *filename, int frameno, int fieldno);

extern void		end_field(fieldstruct *field),
			printinfo_field(fieldstruct *field);

#endif

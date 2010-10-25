#
#				Makefile.am
#
# Process this file with automake to generate a Makefile
#
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#
#	This file part of:	EyE
#
#	Copyright:		(C) 2002-2010 Emmanuel Bertin -- IAP/CNRS/UPMC
#
#	License:		GNU General Public License
#
#	EyE is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
# 	(at your option) any later version.
#	EyE is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#	You should have received a copy of the GNU General Public License
#	along with EyE.  If not, see <http://www.gnu.org/licenses/>.
#
#	Last modified:		09/10/2010
#
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

SUBDIRS			= fits
bin_PROGRAMS		= eye
eye_SOURCES		= back.c bpann.c field.c main.c makeit.c \
			  misc.c prefs.c readimage.c retina.c \
			  back.h bpann.h define.h field.h globals.h key.h \
			  misc.h preflist.h prefs.h readimage.h retina.h \
			  types.h
eye_LDADD		= $(top_builddir)/src/fits/libfits.a
DATE=`date +"%Y-%m-%d"`


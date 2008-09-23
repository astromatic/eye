# Program Makefile for EyE
# Copyright (C) 2003 Emmanuel Bertin.
SUBDIRS			= fits
bin_PROGRAMS		= eye
eye_SOURCES		= back.c bpann.c field.c main.c makeit.c \
			  misc.c prefs.c readimage.c retina.c \
			  back.h bpann.h define.h field.h globals.h key.h \
			  misc.h preflist.h prefs.h readimage.h retina.h \
			  types.h
eye_LDADD		= $(top_builddir)/src/fits/libfits.a
DATE=`date +"%Y-%m-%d"`


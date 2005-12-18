# Main Makefile for EyE
# Copyright (C) 2003 Emmanuel Bertin.
AUTOMAKE_OPTIONS	= foreign no-dependencies
SUBDIRS			= man src
EXTRA_DIST		= doc AUTHORS BUGS ChangeLog HISTORY INSTALL README \
			  THANKS acx_pthread.m4 acx_prog_cc_optim.m4 \
			  acx_mpi.m4 \
			  $(PACKAGE).spec.in \
			  $(PACKAGE)-tpx.spec.in man/$(PACKAGE).1.in
RPM_ROOTDIR		= /usr/src/RPM
RPM_SRCDIR		= $(RPM_ROOTDIR)/SOURCES
RPM_ARCH		= i586
RPM_RPMDIR		= $(RPM_ROOTDIR)/RPMS/$(RPM_ARCH)
rpm:	dist
	cp -f $(PACKAGE)-$(VERSION).tar.gz $(RPM_SRCDIR)
	rpmbuild -bb --clean $(PACKAGE).spec

rpm-tpx:	dist
	cp -f $(PACKAGE)-$(VERSION).tar.gz $(RPM_SRCDIR)
	rpmbuild -bb --clean $(PACKAGE).spec $(PACKAGE)-tpx.spec


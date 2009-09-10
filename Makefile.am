# Main Makefile for EyE
# Copyright (C) 2003-2009 Emmanuel Bertin (IAP/CNRS/UPMC)
AUTOMAKE_OPTIONS	= foreign no-dependencies
SUBDIRS			= man src
EXTRA_DIST		= AUTHORS BUGS ChangeLog COPYRIGHT HISTORY INSTALL \
			  README THANKS \
			  doc/eye.pdf \
			  acx_pthread.m4 acx_prog_cc_optim.m4
RPM_ROOTDIR		= `rpmbuild --nobuild -E %_topdir`
RPM_SRCDIR		= $(RPM_ROOTDIR)/SOURCES
dist-hook:
	rm -rf `find $(distdir) -name .svn`
rpm:	dist
	cp -f $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz $(RPM_SRCDIR)
	rpmbuild -ba --clean --nodeps $(PACKAGE_NAME).spec

rpm-icc:	dist
	cp -f $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz $(RPM_SRCDIR)
	USE_ICC="1" rpmbuild -ba --clean --nodeps $(PACKAGE_NAME).spec

rpm-best:	dist
	cp -f $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz $(RPM_SRCDIR)
	USE_BEST="1" rpmbuild -ba --clean --nodeps $(PACKAGE_NAME).spec


# Main Makefile for EyE
# Copyright (C) 2003-2006 Emmanuel Bertin.
AUTOMAKE_OPTIONS	= foreign no-dependencies
SUBDIRS			= man src
EXTRA_DIST		= AUTHORS BUGS ChangeLog HISTORY INSTALL README \
			  THANKS acx_pthread.m4 acx_prog_cc_optim.m4
			  $(PACKAGE).spec.in man/$(PACKAGE).1.in
RPM_ROOTDIR		= /usr/src/RPM
RPM_SRCDIR		= $(RPM_ROOTDIR)/SOURCES
rpm:	dist
	cp -f $(PACKAGE)-$(VERSION).tar.gz $(RPM_SRCDIR)
	rpmbuild -ba --clean $(PACKAGE).spec

rpm-opteron:	dist
	cp -f $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz $(RPM_SRCDIR)
	GCCFLAGS="-O3 -g -funroll-loops -fomit-frame-pointer -Wall -march=opteron" rpmbuild -ba --target=x86_64 --clean $(PACKAGE_NAME).spec                                                            

rpm-athlon:	dist
	cp -f $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz $(RPM_SRCDIR)
	GCCFLAGS="-O3 -g -funroll-loops -fomit-frame-pointer -Wall -m32 -march=i686 -msse -mfpmath=sse -mtune=athlon" rpmbuild -ba --target=i686 --clean $(PACKAGE_NAME).spec

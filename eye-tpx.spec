%define name eye
%define version 1.2b1
%define release 1tpx

Summary: resample and combine astronomical FITS images (Multi-Processing version)
Name: %{name}
Version: %{version}
Release: %{release}
Source0: ftp://ftp.iap.fr/pub/from_users/bertin/%{name}/%{name}-%{version}.tar.gz
URL: http://terapix.iap.fr/soft/%{name}/
License: LGPL
Group: Sciences/Astronomy
BuildRoot: %{_tmppath}/%{name}-buildroot
Prefix: %{_prefix}

%description
SWarp is a program that resamples and coadd FITS images to any arbitrary astrometric projection recognized by the WCS standard.


%prep
%setup -q

%build
./configure --enable-icc --enable-static --enable-threads --prefix=$RPM_BUILD_ROOT/usr/local/ --mandir=$RPM_BUILD_ROOT/usr/local/man/

make

%install
make install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
/usr/local/bin/eye
/usr/local/man/man1/eye.1
/usr/local/man/manx/eye.x
%doc AUTHORS ChangeLog COPYING HISTORY INSTALL README THANKS doc/swarp.ps

%changelog
* Sun Dec 18 2005 Emmanuel Bertin <bertin@iap.fr>
- Automatic RPM rebuild
* Wed Jul 09 2003 Emmanuel Bertin <bertin@iap.fr>
- RPM build for V2.0b6
* Tue Apr 29 2003 Emmanuel Bertin <bertin@iap.fr>
- RPM build for V1.42
* Wed Mar 05 2003 Emmanuel Bertin <bertin@iap.fr>
- RPM build for V1.40
* Sat Jan 18 2003 Emmanuel Bertin <bertin@iap.fr>
- RPM build for V1.39
* Thu Nov 28 2002 Emmanuel Bertin <bertin@iap.fr>
- RPM build for V1.38
* Thu May 09 2002 Emmanuel Bertin <bertin@iap.fr>
- First RPM build

# end of file

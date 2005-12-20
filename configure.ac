# configure.ac for EyE
# (C) E.Bertin 2003-2005
# Process this file with autoconf to produce a configure script.
# First, disable the annoying config.cache
define([AC_CACHE_LOAD],)
define([AC_CACHE_SAVE],)

# This is your standard Bertin source code...
AC_PREREQ(2.53)
AC_INIT(eye, 1.2b, [bertin@iap.fr])
AC_CONFIG_SRCDIR(src/makeit.c)
AC_CONFIG_AUX_DIR(autoconf)
AM_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE
date=`date +%Y-%m-%d`
date2=`date +"%a %b %d %Y"`
date3=`date +"%B %Y"`
AC_DEFINE_UNQUOTED(DATE, "$date", [Archive creation date])
AC_SUBST(PACKAGER, "Emmanuel Bertin")
AC_SUBST(DATE2, "$date2")
AC_SUBST(DATE3, "$date3")

# Include macros
sinclude(acx_mpi.m4)
sinclude(acx_pthread.m4)
sinclude(acx_prog_cc_optim.m4)

# Display pakage and version number
AC_MSG_RESULT([*********** Configuring:  $PACKAGE $VERSION ($date) **********])

# Initialize the list of compilers to consider
cclist="cc gcc"

# Backup and reset the input CFLAGS and LDFLAGS
mycflags="$CFLAGS"
CFLAGS=""
myldflags="$LDFLAGS"
LDFLAGS=""

# Provide special option for the Linux Intel C compiler
AC_MSG_CHECKING([for Linux Intel C compiler mode])
AC_ARG_ENABLE(icc,
	[AC_HELP_STRING([--enable-icc],
	[Enable special mode for compilation with the Intel compiler \
(off by default)])],
        use_icc="yes"
        cclist="icc $cclist"
	AC_MSG_RESULT([yes]),
        use_icc="no"
	AC_MSG_RESULT([no]))

# Provide special option for the Assure debugger
AC_MSG_CHECKING([for Assure debugger mode])
AC_ARG_ENABLE(assure,
	[AC_HELP_STRING([--enable-assure],
	[Enable special mode for compilation with the Assure debugger \
(off by default)])],
        use_assure="yes"
        cclist="assuretcc $cclist"
	AC_MSG_RESULT([yes]),
        use_assure="no"
	AC_MSG_RESULT([no]))

# Provide special option for the Insure++ debugger
AC_MSG_CHECKING([for Insure++ debugger mode])
AC_ARG_ENABLE(insure,
	[AC_HELP_STRING([--enable-insure],
	[Enable special mode for compilation with the Insure++ debugger \
(off by default)])],
        use_insure="yes"
        cclist="insure $cclist"
	AC_MSG_RESULT([yes]),
        use_insure="no"
	AC_MSG_RESULT([no]))

# Provide special option for the e-fence malloc checker
AC_MSG_CHECKING([for e-fence mode])
AC_ARG_ENABLE(efence,
	[AC_HELP_STRING([--enable-efence],
	[Enable linking with the -e-fence library (off by default)])],
        use_efence="yes"
	AC_MSG_RESULT([yes]),
        use_efence="no"
	AC_MSG_RESULT([no]))

# Provide special option for gprof profiling
AC_MSG_CHECKING([for gprof profiler mode])
AC_ARG_ENABLE(gprof,
	[AC_HELP_STRING([--enable-gprof],
	[Enable special mode for compilation with the gprof profiler \
(off by default)])],
        use_gprof="yes"
	AC_MSG_RESULT([yes]),
        use_gprof="no"
	AC_MSG_RESULT([no]))

# Provide special option for the MPI version
AC_MSG_CHECKING([for MPI mode])
AC_ARG_ENABLE(mpi,
	[AC_HELP_STRING([--enable-mpi],
	[Enable MPI parallel processing (off by default)])],
		use_mpi="yes"
		AC_MSG_RESULT([yes]),
      		use_mpi="no"
		AC_MSG_RESULT([no]),
	use_mpi="no"
	AC_MSG_RESULT([no]))

# Enable static linking
AC_MSG_CHECKING([static linking])
AC_ARG_ENABLE(static,
	[AC_HELP_STRING([--enable-static],
	[Enable static linking \
(off by default)])],
        use_static="yes"
	AC_MSG_RESULT([yes]),
        use_static="no"
	AC_MSG_RESULT([no]))

# Checks for programs.
# GCC is chosen last because it is likely to yield less optimized code
AC_PROG_CC([$cclist])

# Compile with MPI option
if test "$use_mpi" = "yes"; then
ACX_MPI([AC_DEFINE(HAVE_MPI,1,[Define if you have the MPI library.])]
	CC="$MPICC",
      	use_mpi="no")
fi

# C Compiler: Check that it is ANSI C
AM_PROG_CC_STDC
# C Compiler: Check that it is POSIX-compliant
AC_ISC_POSIX
ACX_PROG_CC_OPTIM
AC_PROG_RANLIB

# Checks for libraries.
AC_CHECK_LIB(m, sin)

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(fcntl.h sys/mman.h time.h unistd.h)
if test $use_icc = "yes" -a $CC = "icc"; then
AC_CHECK_HEADERS(mathimf.h)
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_SIGNAL
AC_TYPE_UID_T
AC_CHECK_TYPES([unsigned long long, long long])
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MMAP
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([atexit getenv gethostname memcpy memmove memset mmap time])

# Set flags for multithreading
AC_ARG_ENABLE(threads,
	[AC_HELP_STRING([--enable-threads@<:@=<max_number_of_threads>@:>@],
		[Enable multhreading (off by default)])],
    if test "$enableval" = "yes"; then
      n_pthreads=16
    else
      n_pthreads=$enableval
    fi
    use_pthreads=yes,
    n_pthreads=1
    use_pthreads=no
    )
AC_DEFINE_UNQUOTED(THREADS_NMAX, $n_pthreads,[Maximum number of POSIX threads])

if test "$use_pthreads" == "yes"; then
# Actions to complete in case of multhreading
  AC_MSG_CHECKING([for multithreading])
  AC_MSG_RESULT([maximum of $n_pthreads thread(s)])
  AC_DEFINE(USE_THREADS, 1, [Triggers multhreading])
# CC, CFLAGS and LIBS are system and compiler-dependent
  ACX_PTHREAD
  CC="$PTHREAD_CC"
  CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
  LIBS="$PTHREAD_LIBS $LIBS"
fi

# Check support for large files
AC_SYS_LARGEFILE
AC_FUNC_FSEEKO

# Special options for the Insure debugger
if test $use_insure = "yes" -a $CC = "insure"; then
  CFLAGS="$CFLAGS -g"
  use_static="no"
fi

# Special optimization options for the INTEL C compiler
if test $use_icc = "yes" -a $CC = "icc"; then
  CFLAGS="$CFLAGS -O3 -axiMKW -ipo -ipo_obj -unroll"
  LIBS="-limf $LIBS"
fi

# Special options for the Assure debugger
rootsrcdir=`pwd`
if test $use_assure = "yes" -a $CC = "assuretcc"; then
  CFLAGS="-WApname=$rootsrcdir/$PACKAGE"
  LDFLAGS="-WApname=$rootsrcdir/$PACKAGE"
fi

# Link with e-fence option
if test "$use_efence" = "yes"; then
  LIBS="-lefence $LIBS"
fi

# Link with gprof option
if test "$use_gprof" = "yes"; then
  if test "$use_icc" = "yes"; then
    CFLAGS="$CFLAGS -pq"
  else
    CFLAGS="$CFLAGS -pg"
  fi
  use_static="no"
fi

if test "$use_static" = "yes"; then
  LDFLAGS="$LDFLAGS -static"
fi

# Link with MPI option
if test "$use_mpi" = "yes"; then
  LIBS="$MPILIBS $LIBS"
fi

# Override automatic CFLAGS and LDFLAGS with those of user
if test -n "$mycflags"; then
CFLAGS="$mycflags"
fi
if test -n "$myldflags"; then
LDFLAGS="$myldflags"
fi

# Display compiler and linker flags
AC_MSG_RESULT([I will compile using:  $CC $CFLAGS])
AC_MSG_RESULT([I will link using:     $CC $LDFLAGS $LIBS])

AC_CONFIG_FILES([Makefile man/Makefile src/Makefile src/fits/Makefile] eye.spec eye-tpx.spec man/eye.1)
AC_OUTPUT

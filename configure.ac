dnl Process this file with autoconf to produce a configure script.
dnl
dnl     MAGNETAR
dnl     Copyright (C) 2013 Ben Asselstine
dnl
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; version 2 or later of the License.   
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
dnl 02110-1301, USA.


dnl Initializations:
AC_INIT([magnetar],[0.0.1],[http://sv.nongnu.org/bugs/?group=magnetar])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE
AC_PREREQ([2.69])
AM_CONFIG_HEADER(config.h:config.in)
AC_CONFIG_SRCDIR([xsc.C])

dnl Set the language to C++
AC_LANG([C++])

dnl Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL

dnl Checks for simple libraries.
AC_CHECK_LIB(m, sin)

dnl Checks for X11 include and library directories.
AC_PATH_X
AC_PATH_XTRA

CPPFLAGS="$CPPFLAGS $X_CFLAGS"
LDFLAGS="$LDFLAGS $X_LDFLAGS $X_LIBS"

if test "x$no_x" = "xyes"; then 
    AC_MSG_ERROR([
*** X libraries or include files not found. Check 'config.log' for 
*** more details.])
fi

PKG_CHECK_MODULES(SDL, [sdl >= 1.2.1])
AC_SUBST(SDL_LIBS)
AC_SUBST(SDL_CFLAGS)

eval MAGNETAR_DATADIR="$datadir/magnetar"
AC_SUBST(MAGNETAR_DATADIR)

AC_CHECK_LIB(SDL_mixer, Mix_OpenAudio, , 
    AC_MSG_ERROR([*** libSDL_mixer not found. Check 'config.log' for more details.]),

    $X_EXTRA_LIBS)
AC_CHECK_LIB(X11, XOpenDisplay, , 
    AC_MSG_ERROR([*** libX11 not found. Check 'config.log' for more details.]),
    $X_EXTRA_LIBS)
AC_CHECK_LIB(Xpm, XpmReadFileToPixmap, , 
    AC_MSG_ERROR([*** libXpm not found. Check 'config.log' for more details.]),
    $X_EXTRA_LIBS)

AC_CHECK_LIB(GL, glXChooseVisual, , 
    AC_MSG_ERROR([*** libGL not found. Check 'config.log' for more details.]),
    $X_EXTRA_LIBS)

dnl Checks for header files.
AC_CHECK_HEADERS(time.h)
AC_CHECK_HEADERS(sys/time.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_TIME

dnl Checks for library functions.
AC_DIAGNOSE([obsolete],[your code may safely assume C89 semantics that RETSIGTYPE is void.
Remove this warning and the `AC_CACHE_CHECK' when you adjust the code.])dnl
AC_CACHE_CHECK([return type of signal handlers],[ac_cv_type_signal],[AC_COMPILE_IFELSE(
[AC_LANG_PROGRAM([#include <sys/types.h>
#include <signal.h>
],
		 [return *(signal (0, 0)) (0) == 1;])],
		   [ac_cv_type_signal=int],
		   [ac_cv_type_signal=void])])
AC_DEFINE_UNQUOTED([RETSIGTYPE],[$ac_cv_type_signal],[Define as the return type of signal handlers
		    (`int' or `void').])


AC_CHECK_FUNCS(gettimeofday, true, AC_MSG_ERROR([
*** gettimeofday() not found.  Check 'config.log' for more details.
*** If it really doesn't exist you'll need to find another high resolution
*** clock function and modify timing.h use it.]))

AC_CHECK_FUNCS(usleep, true, AC_MSG_ERROR([
*** usleep() not found.  Check 'config.log' for more details.
*** If it really doesn't exist you'll need to find another high resolution
*** sleep function and modify timing.h to use it.]))

dnl fall back on rand, srand if these don't exist (or configure screws up)
AC_CHECK_FUNCS(random srandom, true, false)

dnl Output:
AC_CONFIG_FILES([Makefile sounds/Makefile])
AC_OUTPUT

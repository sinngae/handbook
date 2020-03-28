########## Autoconf ########
# Process this file with autoconf to produce a configure script

AC_PREREQ([2.69])
AC_INIT([auto_test],[1.0],[zqren@foxmail.com])

AC_SUBST([PACKAGE_RELEASE],[1.0],[zqren@foxmail.com])

#AM_INIT_AUTOMAKE(auto_test, 1.0)

AC_CONFIG_SRCDIR([src/auto_test.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile
		 src/Makefile])

AM_INIT_AUTOMAKE

## check for typedefs, structures, add compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

## check for library functions.
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset socket])

AC_OUTPUT

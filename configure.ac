#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

PACKAGE=chumbradiod
VERSION=0.1
AC_PREREQ(2.59)
AC_INIT(FULL-PACKAGE-NAME, VERSION, BUG-REPORT-ADDRESS)
AC_CONFIG_SRCDIR([src])
AM_CONFIG_HEADER(config.h)

AM_INIT_AUTOMAKE($PACKAGE,$VERSION)

AM_MAINTAINER_MODE

AC_CANONICAL_HOST
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_INSTALL
AC_C_BIGENDIAN

# Checks for programs.
AC_PROG_CXX

AC_HEADER_STDC
AC_CHECK_HEADERS(sys/stat.h chumby_httpd/chumby_http_server.h)

AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_LANG_RESTORE

# Checks for libraries.

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	])

# Checks for library functions.
AC_OUTPUT

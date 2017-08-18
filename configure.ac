AC_INIT([oxfs], [0.0.1], [jjohn@2ion.de])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_PROG_CC

AC_CONFIG_SRCDIR([src/oxfs.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])

AC_CHECK_FUNCS([error_at_line])

AC_CANONICAL_BUILD

PKG_CHECK_MODULES([LIBTALLOC], [talloc])

AC_OUTPUT

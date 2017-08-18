#ifndef LERROR_H
#define LERROR_H

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <talloc.h>

#include "config.h"

void lerror(const char *file_name, const char *func_name, unsigned int file_line, int status,
    int errnum, FILE *stream, const char *msg, ...);

#ifdef HAVE_ERROR_AT_LINE
#include <error.h>
#define LERROR(status, errnum, ...) error_at_line((status), (errnum), \
        (__func__), (__LINE__), __VA_ARGS__)
#else
#define LERROR(status, errnum, ...) lerror((__FILE__), (__func__), (__LINE__), \
    (status), (errnum), stderr, __VA_ARGS__)
#endif

#define GUARD_NULL(v) \
  if((v) == NULL)       \
    LERROR(EXIT_FAILURE, errno, "GUARD_NULL(%s)", #v);

#endif

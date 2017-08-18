#pragma once

#include <stdint.h>
#include <talloc.h>
#include "lerror.h"

typedef struct {
  char *url;
  char *mountpoint;
  char *pinentry_hook;
} OXParams;

typedef struct {
  TALLOC_CTX *root_ctx;
} OXState;

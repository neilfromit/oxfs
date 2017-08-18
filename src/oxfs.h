#pragma once

#include <stdint.h>
#include <talloc.h>
#include "lerror.h"

enum {
  OX_PINENTRY_READLNE,
  OX_PINENTRY_HOOK,
};

typedef struct {
  char *mountpoint;
  char *pinentry_hook;
  char *url;
  char *username;
  char *password;
  int pinentry_mode;
} OXParams;

typedef struct {
  TALLOC_CTX *root_ctx;
} OXState;

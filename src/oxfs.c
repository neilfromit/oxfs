#include <getopt.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdint.h>
#include <stdlib.h>
#include <talloc.h>
#include <unistd.h>
#include <string.h>

#include "oxfs.h"

static void ox_usage(void);
static void ox_cmdline(int, char**, OXState*, OXParams*);

void ox_usage(void)
{
  puts("Usage: oxfs [-h] [-H pinentry-hook] url mount-point");
}

void ox_cmdline(int argc, char **argv, OXState *st, OXParams *pm)
{
  int opt;

  while ((opt = getopt(argc, argv, "H:h")) != -1)
  {
    switch (opt)
    {
      case 'h':
        ox_usage();
        exit(EXIT_SUCCESS);
      case 'H':
        pm->pinentry_hook = talloc_strdup(st->root_ctx, optarg);
        pm->pinentry_mode = OX_PINENTRY_HOOK;
        break;
      case '?':
        ox_usage();
        exit(EXIT_FAILURE);
    }
  }

  if (argc - optind < 2) {
    LERROR(EXIT_FAILURE, 0, "Missing URL and/or mount point");
  }

  pm->url = talloc_strdup(st->root_ctx, argv[optind++]);
  pm->mountpoint = talloc_strdup(st->root_ctx, argv[optind]);

  /* Process pinentry hook */

  switch (pm->pinentry_mode)
  {
    case OX_PINENTRY_READLNE:
      {
        char *p;

        if(p = readline("User> ")) {
          pm->username = talloc_strdup(st->root_ctx, (const char*)p);
          free(p);
        }

        if(p = readline("Password> ")) {
          pm->password = talloc_strdup(st->root_ctx, (const char*)p);
          free(p);
        }
      }
      break;
    case OX_PINENTRY_HOOK:
      if(strcmp(pm->pinentry_hook, "@ENV") == 0) {
        {
          char *p;
          if(p = getenv("OXFS_USERNAME"))
            pm->username = talloc_strdup(st->root_ctx, (const char*)p);
          else
            LERROR(0, 0, "warning: OXFS_USERNAME is empty");

          if(p = getenv("OXFS_PASSWORD"))
            pm->password = talloc_strdup(st->root_ctx, (const char*)p);
          else
            LERROR(0, 0, "warning: OXFS_PASSWORD is empty");
        }
      } else {
        /* Run hooker */
      }
      break;
  }
}

int main(int argc, char** argv)
{
  OXState state = {0};
  OXParams params = {
    .mountpoint = NULL,
    .password = NULL,
    .pinentry_hook = NULL,
    .pinentry_mode = OX_PINENTRY_READLNE,
    .url = NULL,
    .username = NULL,
  };

  state.root_ctx = talloc_autofree_context();
  GUARD_NULL(state.root_ctx);

  ox_cmdline(argc, argv, &state, &params);
  
  return EXIT_SUCCESS;
}

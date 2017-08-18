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

static void usage(void);

void usage(void)
{
  puts("Usage: oxfs [-h] [-H pinentry-hook] url mount-point");
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
  int opt;

  /* Memory tree */

  state.root_ctx = talloc_autofree_context();
  GUARD_NULL(state.root_ctx);

  /* CLI parser */

  while ((opt = getopt(argc, argv, "H:h")) != -1)
  {
    switch (opt)
    {
      case 'h':
        usage();
        return EXIT_SUCCESS;
      case 'H':
        params.pinentry_hook = talloc_strdup(state.root_ctx, optarg);
        params.pinentry_mode = OX_PINENTRY_HOOK;
        break;
    }
  }

  if (argc - optind < 2) {
    LERROR(EXIT_FAILURE, 0, "Missing URL and/or mount point");
  }

  params.url = talloc_strdup(state.root_ctx, argv[optind++]);
  params.mountpoint = talloc_strdup(state.root_ctx, argv[optind]);

  /* Process pinentry hook */

  switch (params.pinentry_mode)
  {
    case OX_PINENTRY_READLNE:
      {
        char *p;

        if(p = readline("User> ")) {
          params.username = talloc_strdup(state.root_ctx, (const char*)p);
          free(p);
        }

        if(p = readline("Password> ")) {
          params.password = talloc_strdup(state.root_ctx, (const char*)p);
          free(p);
        }
      }
      break;
    case OX_PINENTRY_HOOK:
      if(strcmp(params.pinentry_hook, "@ENV") == 0) {
        {
          char *p;
          if(p = getenv("OXFS_USERNAME"))
            params.username = talloc_strdup(state.root_ctx, (const char*)p);
          if(p = getenv("OXFS_PASSWORD"))
            params.password = talloc_strdup(state.root_ctx, (const char*)p);
        }
      } else {
        /* execute hooker and read stdout */
      }
      break;
  }

  return EXIT_SUCCESS;
}

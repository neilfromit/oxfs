#include <stdlib.h>
#include <stdint.h>
#include <talloc.h>

#include "oxfs.h"

int main(int argc, char** argv)
{
  OXState state = {0};
  OXParams params = {0};

  state.root_ctx = talloc_new(NULL);
  GUARD_NULL(state.root_ctx);

  return EXIT_SUCCESS;
}

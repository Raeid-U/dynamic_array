#include "da.h"
#include "lumber.h"

int main(int argc, char *argv[]) {
  logf("=== Implementation Start: %s ===\n\n", __progname);
  dynamic_array *da = da_init();

  da_append_string("this is a string\0", da);

  da_print_string(da);

  da_pop(da);
  da_pop(da);

  da_print_string(da);

  da_print_verbose(da);
  da_destroy(da);
  return 0;
}

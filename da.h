typedef struct {
  int size;
  int pos;
  char *content;
} dynamic_array;

void da_zero(dynamic_array *da);

dynamic_array *da_init();

void da_destroy(dynamic_array *da);

int da_check_health(dynamic_array *da);

void da_print_verbose(dynamic_array *da);

void da_print(dynamic_array *da);

void da_print_string(dynamic_array *da);

void da_resize(dynamic_array *da);

void da_append(char c, dynamic_array *da);

void da_pop(dynamic_array *da);

char da_nth_lookup(int index, dynamic_array *da);

void da_slice(int start, int end, dynamic_array *da);

void da_append_string(char *c, dynamic_array *da);

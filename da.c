#include "da.h"
#include "lumber.h"
#include <stdlib.h>
#include <string.h>

#define DYNAMIC_ARRAY_BASE_SIZE 10
#define DYNAMIC_ARRAY_TYPE char

void da_zero(dynamic_array *da) {
  if (da == NULL) {
    log("Zero Error: Dynamic Array supplied is NULL\n");
    return;
  }

  int sz = da->size;

  for (int i = 0; i < sz; i++) {
    da->content[i] = 0;
  }
  da->pos = 0;
}

dynamic_array *da_init() {
  dynamic_array *da = malloc(sizeof(dynamic_array));

  if (da == NULL) {
    log("Initialization Error: Unable to allocate memory for a new Dynamic "
        "Array\n");
    return NULL;
  }

  da->size = DYNAMIC_ARRAY_BASE_SIZE;
  da->pos = '\0';

  da->content = malloc(da->size * sizeof(DYNAMIC_ARRAY_TYPE));

  if (da->content == NULL) {
    log("Initialization Error: Unable to allocate memory for a new Dynamic "
        "Array Content\n");
    free(da);

    return NULL;
  }

  da_zero(da);

  return da;
}

void da_destroy(dynamic_array *da) {
  if (da == NULL) {
    log("Destruction Error: Dynamic Array supplied is NULL\n");
    return;
  }

  free(da->content);
  free(da);
}

int da_check_health(dynamic_array *da) {
  if (da == NULL) {
    log("Health Error: Dynamic Array supplied is NULL\n");
    return -1;
  }

  if (da->pos == da->size) {
    log("Health Warning: Dynamic Array is running out of Memory\n");
    return 1;
  }

  if (da->content == NULL) {
    log("Health Error: Dynamic Array content is Corrupted\n");
    return -1;
  }

  if (da->pos > da->size) {
    log("Health Error: Dynamic Array indexing is Corrupted\n");
    return -1;
  }

  if (da->size <= 0) {
    log("Health Error: Dynamic Array sizing is Corrupted\n");
    return -1;
  }

  logf("Health Check: Dynamic Array @ %p is Healthy\n", da);
  return 0;
}

void da_print_verbose(dynamic_array *da) {
  if (da_check_health(da) < 0) {
    log("Print Error: Cannot print an unhealthy Dynamic Array\n");
    return;
  }

  printf("--- Dynamic Array @ %p ---\n", da);
  printf("> Size: %d Bytes\n", da->size);
  printf("> Used: %d Bytes\n", da->pos);
  printf("\n");
  printf("[");

  for (int i = 0; i < da->pos - 1; i++) {
    printf("%c, ", *(da->content + i));
  }

  if (da->pos - 1 >= 0) {
    printf("%c", *(da->content + da->pos - 1));
  }

  printf("]\n");
  printf("---\n");
}

void da_print(dynamic_array *da) {
  if (da_check_health(da) < 0) {
    log("Print Error: Cannot print an unhealthy Dynamic Array\n");
    return;
  }

  printf("[");

  for (int i = 0; i < da->pos - 1; i++) {
    printf("%c, ", *(da->content + i));
  }

  if (da->pos - 1 >= 0) {
    printf("%c", *(da->content + da->pos - 1));
  }

  printf("]\n");
}

void da_print_string(dynamic_array *da) {
  if (da_check_health(da) < 0) {
    log("Print Error: Cannot print an unhealthy Dynamic Array\n");
    return;
  }

  int pos = da->pos;
  for (int i = 0; i < pos; i++) {
    printf("%c", da->content[i]);
  }

  printf("\n");
}

void da_resize(dynamic_array *da) {
  char *content = da->content;

  da->size *= 2;

  da->content = malloc(da->size * sizeof(DYNAMIC_ARRAY_TYPE));

  for (int i = 0; i < da->size / 2; i++) {
    da->content[i] = content[i];
  }

  free(content);
}

void da_append(char c, dynamic_array *da) {
  int health = da_check_health(da);
  if (health > 0) {
    da_resize(da);
  } else if (health < 0) {
    log("Append Error: Cannot append to an unhealthy Dynamic Array\n");
  }

  da->content[da->pos] = c;
  ++da->pos;
}

void da_pop(dynamic_array *da) {
  int health = da_check_health(da);
  if (health > 0) {
    da_resize(da);
  } else if (health < 0) {
    log("Pop Error: Cannot pop from an unhealthy Dynamic Array\n");
  }

  da->content[da->pos] = 0;

  --da->pos;

  if (da->pos < da->size / 2) {
    da_optimize(da);
  }
}

char da_nth_lookup(int index, dynamic_array *da) {
  int health = da_check_health(da);
  if (health < 0) {
    log("Lookup Error: Cannot lookup an index of an unhealthy Dynamic Array\n");
    return '\0';
  }

  if (index >= da->pos) {
    logf(
        "Lookup Error: invalid index, Dynamic Array @ %p has only %d indices\n",
        da, da->pos - 1);
    return '\0';
  }

  return da->content[index];
}

void da_slice(int start, int end, dynamic_array *da) {
  int health = da_check_health(da);
  if (health < 0) {
    log("Slice Error: Cannot slice an unhealthy Dynamic Array\n");
    return;
  }

  if (end < start) {
    log("Slice Error: Invalid Boundaries, starting index > end index\n");
    return;
  }

  if (end >= da->size) {
    log("Slice Error: Invalid Boundaries, ending index beyond size limit\n");
    return;
  }

  int sz = da->size;
  char *old = malloc(sz * sizeof(DYNAMIC_ARRAY_TYPE *));

  for (int i = 0; i < sz; i++) {
    old[i] = da->content[i];
  }

  da_zero(da);

  for (int i = start; i < end + 1; i++) {
    da_append(old[i], da);
  }

  free(old);
}

void da_append_string(char *c, dynamic_array *da) {
  int sz = strlen(c);

  for (int i = 0; i < sz; i++) {
    da_append(c[i], da);
  }
}

void da_optimize(dynamic_array *da) {
  char *content = da->content;
  int sz = da->size;
  int pos = da->pos;

  da->content = malloc(DYNAMIC_ARRAY_BASE_SIZE * sizeof(DYNAMIC_ARRAY_TYPE));
  da->size = DYNAMIC_ARRAY_BASE_SIZE;
  da->pos = 0;

  da_zero(da);
  for (int i = 0; i < pos; i++) {
    da_append(content[i], da);
  }

  free(content);
}

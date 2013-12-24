#include <stdio.h>
#include "rdtsc.h"
#include "stddev.h"

#define RUNS 1000

#define NUM_LINES 10000
#define LINE_SIZE 128
#define PG_SIZE 4096

int access_mem(int align) {
  static char a[2 * NUM_LINES * PG_SIZE];
  int sum = 0;
  uint64_t tsc_before, tsc_after, tsc;
  uint64_t offset;

  int i, j;
  for (i = 0; i < NUM_LINES * PG_SIZE; i++) {
    a[i] = i % 17;
  }

  // Warmup
  offset = 0;
  for (j = 0; j < NUM_LINES; j++) {
    offset += PG_SIZE + align;
    sum += a[offset];
  }

  RDTSC_START(tsc_before);
  for (i = 0; i < RUNS; i++) {
    offset = 0;
    for (j = 0; j < NUM_LINES; j++) {
      offset += PG_SIZE + align;
      sum += a[offset];
    }
  }
  RDTSC_START(tsc_after);
  tsc = tsc_after - tsc_before;

  printf("Total time: %llu \n", tsc);

  return sum;
}

int main() {
  printf("Sum: %i\n", access_mem(0));  
  printf("Sum: %i\n", access_mem(LINE_SIZE));  
  return 0;
}

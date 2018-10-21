#ifndef MAP_DEFINED
#define MAP_DEFINED
typedef struct raw_map_t map_t;

#include "input.h"
#include <mpi.h>

struct raw_map_t {
    int num_stations;
    int** stations;
};

void map_init(input_t* input);
void print_map(map_t* map);
void map_cleanup(map_t* map);
#endif

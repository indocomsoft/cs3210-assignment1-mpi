#ifndef MAP_DEFINED
#define MAP_DEFINED
typedef struct raw_map_t map_t;
typedef struct raw_edge_map_t edge_map_t;

#define EDGE_MAP_NULL_ID -1

#include <mpi.h>

#include "input.h"
#include "slave.h"

struct raw_map_t {
    int num_stations;
    int** stations;
};

struct raw_edge_map_t {
    int num_stations;
    int** edges;
};

void map_init(input_t* input);
void map_print(map_t* map);
void map_cleanup(map_t* map);
void edge_map_init(edge_map_t* edge_map, map_t* map);
void edge_map_print(edge_map_t* edge_map);
void edge_map_get_slave_meta(edge_map_t* edge_map, int my_id, int res[2]);
void map_broadcast_receive(map_t* map, int source);
void map_broadcast_send(map_t* map, int source);
#endif

#ifndef SPAWN_DEFINED
#define SPAWN_DEFINED

typedef struct raw_spawn_info_t spawn_info_t;

#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "line.h"
#include "map.h"
#include "queue.h"
#include "station_stat.h"

struct raw_spawn_info_t {
    int line_id;
    int count;
    int num_trains;
    bool forward;
};

bool spawn_get_spawn_info(spawn_info_t spawn_infos[6], line_t* lines[3], int src, int dst);
int spawn_train(spawn_info_t spawn_infos[6], train_t trains[6]);

#endif

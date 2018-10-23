#ifndef SLAVE_DEFINED
#define SLAVE_DEFINED

#define SLAVE_META_SIZE 2
#define SLAVE_META_SRC 0
#define SLAVE_META_DST 1

#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "line.h"
#include "map.h"
#include "queue.h"
#include "spawn.h"
#include "station_stat.h"

void slave(int my_id, int slaves, MPI_Comm comm);
int get_train_next_station(train_t* train, int station_id, line_t* lines[LINE_NUM_LINES]);
void enqueue_train_for_departure(train_t train, queue_t* exit_queue);

#endif

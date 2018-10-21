#ifndef SLAVE_DEFINED
#define SLAVE_DEFINED

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
int get_train_next_edge(train_t* train, int station_id, line_t* lines[3]);
void enqueue_train_for_departure(train_t train, queue_t* exit_queue);

#endif

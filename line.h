#ifndef LINE_DEFINED
#define LINE_DEFINED

#define LINE_NUM_LINES 3

#define LINE_GREEN 0
#define LINE_YELLOW 1
#define LINE_BLUE 2

typedef struct raw_line_t line_t;

#include "input.h"
#include "station_stat.h"
#include <mpi.h>

struct raw_line_t {
    int num_stations;
    int* stations;
    station_stat_t* stats;
    char id;
    int num_trains;
    int start_train_id;
};

void line_init(line_t*, input_t*);
void line_print(line_t*);
void line_print_stats(line_t*);
void line_broadcast_receive(line_t* lines[LINE_NUM_LINES], int source);
void line_broadcast_send(line_t** lines, int source);
void line_broadcast_receive_one(line_t* line, int source);
void line_broadcast_send_one(line_t* line, int source);

/**
 * Returns the index of a given station_id.
 *
 * Returns -1 if station_id not part of this line.
 */
int line_find_station_index(line_t* line, int station_id);

#endif

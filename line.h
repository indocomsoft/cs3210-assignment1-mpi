#ifndef LINE_DEFINED
#define LINE_DEFINED

#define LINE_NUM_LINES 3

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
void line_broadcast_receive(line_t* lines[3], int source);
void line_broadcast_send(line_t** lines, int source);
void line_broadcast_receive_one(line_t* line, int source);
void line_broadcast_send_one(line_t* line, int source);

#endif

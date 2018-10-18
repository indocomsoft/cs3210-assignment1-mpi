#ifndef LINE_DEFINED
#define LINE_DEFINED
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

line_t* line_broadcast_receive(int source);
void line_broadcast_send(line_t* line, int source);

#endif

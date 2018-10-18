#ifndef INPUT_DEFINED
#define INPUT_DEFINED

typedef struct raw_input_t input_t;

#include "line.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATION_NAME_LENGTH 100

struct raw_input_t {
    int num_stations;
    char** station_names;
    int** stations;
    double* popularity;
    line_t* line_green;
    line_t* line_yellow;
    line_t* line_blue;
    int ticks;
    int* num_trains;
    int total_trains;
};

void input_init(input_t*);
void input_print(input_t*);
void input_cleanup(input_t*);
void input_print_stats(input_t*);

#endif

#include "line.h"

#define LINE_NUM_INT_FIELDS 3

void line_broadcast_receive(line_t* line, int source)
{
    int int_buff[LINE_NUM_INT_FIELDS];

    // num_stations, num_trains, start_train_id
    MPI_Bcast((void*)&int_buff, LINE_NUM_INT_FIELDS, MPI_INT, source, MPI_COMM_WORLD);
    line->num_stations = int_buff[0];
    line->num_trains = int_buff[1];
    line->start_train_id = int_buff[2];

    line->stations = (int*)malloc(sizeof(int) * line->num_stations);

    //stations
    MPI_Bcast((void*)line->stations, line->num_stations, MPI_INT, source, MPI_COMM_WORLD);

    // id
    MPI_Bcast((void*)&(line->id), 1, MPI_CHAR, source, MPI_COMM_WORLD);
}

void line_broadcast_send(line_t* line, int source)
{
    int int_buff[LINE_NUM_INT_FIELDS];

    int_buff[0] = line->num_stations;
    int_buff[1] = line->num_trains;
    int_buff[2] = line->start_train_id;

    // num_stations, num_trains, start_train_id
    MPI_Bcast((void*)&int_buff, LINE_NUM_INT_FIELDS, MPI_INT, source, MPI_COMM_WORLD);

    // stations
    MPI_Bcast((void*)line->stations, line->num_stations, MPI_INT, source, MPI_COMM_WORLD);

    // id
    MPI_Bcast((void*)&(line->id), 1, MPI_CHAR, source, MPI_COMM_WORLD);
}

void line_init(line_t* line, input_t* input)
{
    int i, j;
    char* str_buf = (char*)malloc(sizeof(char) * MAX_STATION_NAME_LENGTH * input->num_stations);
    line->stations = (int*)malloc(sizeof(int) * input->num_stations);

    fgets(str_buf, MAX_STATION_NAME_LENGTH * input->num_stations, stdin);
    i = 0;
    const char* delim = "\n,";
    char* next;

    for (next = strtok(str_buf, delim); next != NULL; next = strtok(NULL, delim)) {
        j = 0;
        while (j < input->num_stations) {
            if (strcmp(input->station_names[j], next) == 0)
                break;
            j++;
        }
        line->stations[i] = j;
        i++;
    }
    line->num_stations = i;
    free(str_buf);

    line->stats = (station_stat_t*)malloc(sizeof(station_stat_t) * line->num_stations);
    for (i = 0; i < line->num_stations; i++) {
        station_stat_init(&line->stats[i]);
    }
}

void line_print(line_t* line)
{
    int i;
    for (i = 0; i < line->num_stations; i++) {
        printf("%d ", line->stations[i]);
    }
    printf("\n");
}

void line_print_stats(line_t* line)
{
    double sum_avg_time = 0, sum_max_time = 0, sum_min_time = 0;
    double avg_avg_time, avg_max_time, avg_min_time;
    int i;
    // Forward
    for (i = 0; i < line->num_stations; i++) {
        if (line->stats[i].num_door_opening[STATION_STAT_FORWARD] != 0) {
            sum_avg_time += line->stats[i].total_wait_time[STATION_STAT_FORWARD] / (double)line->stats[i].num_door_opening[STATION_STAT_FORWARD];
        }
        sum_max_time += line->stats[i].max_wait_time[STATION_STAT_FORWARD];
        sum_min_time += line->stats[i].min_wait_time[STATION_STAT_FORWARD];
#ifdef DEBUG
        printf("Line %c sum: %.1lf, %.1lf, %.1lf\n", line->id, sum_avg_time, sum_max_time, sum_min_time);
#endif
    }
    // Reverse
    for (i = 0; i < line->num_stations; i++) {
        if (line->stats[i].num_door_opening[STATION_STAT_REVERSE] != 0) {
            sum_avg_time += line->stats[i].total_wait_time[STATION_STAT_REVERSE] / (double)line->stats[i].num_door_opening[STATION_STAT_REVERSE];
        }
        sum_max_time += line->stats[i].max_wait_time[STATION_STAT_REVERSE];
        sum_min_time += line->stats[i].min_wait_time[STATION_STAT_REVERSE];
#ifdef DEBUG
        printf("Line %c sum: %.1lf, %.1lf, %.1lf\n", line->id, sum_avg_time, sum_max_time, sum_min_time);
#endif
    }
    avg_avg_time = sum_avg_time / (2 * (double)line->num_stations);
    avg_max_time = sum_max_time / (2 * (double)line->num_stations);
    avg_min_time = sum_min_time / (2 * (double)line->num_stations);
    printf("%d trains -> %.1lf, %.1lf, %.1lf\n", line->num_trains, avg_avg_time, avg_max_time, avg_min_time);
}

#include "input.h"

void input_init(input_t* input)
{
    int i, j;

    scanf("%d\n", &(input->num_stations));

    char* str_buf = (char*)malloc(sizeof(char) * MAX_STATION_NAME_LENGTH * input->num_stations);

    // STATION NAMES
    input->station_names = (char**)malloc(sizeof(char*) * input->num_stations);
    for (i = 0; i < input->num_stations; i++) {
        input->station_names[i] = (char*)malloc(sizeof(char) * MAX_STATION_NAME_LENGTH);
    }

    fgets(str_buf, MAX_STATION_NAME_LENGTH * input->num_stations, stdin);
    const char* delim = "\n,";
    char* next;
    i = 0;
    for (next = strtok(str_buf, delim); next != NULL; next = strtok(NULL, delim)) {
        strcpy(input->station_names[i], next);
        i++;
    }

    // STATIONS
    input->stations = (int**)malloc(sizeof(int*) * input->num_stations);
    for (i = 0; i < input->num_stations; i++) {
        input->stations[i] = (int*)malloc(sizeof(int) * input->num_stations);
    }

    for (i = 0; i < input->num_stations; i++) {
        fgets(str_buf, MAX_STATION_NAME_LENGTH * input->num_stations, stdin);
        j = 0;
        for (next = strtok(str_buf, " "); next != NULL; next = strtok(NULL, " ")) {
            input->stations[i][j] = atoi(next);
            j++;
        }
    }

    // POPULARITY
    input->popularity = (double*)malloc(sizeof(int*) * input->num_stations);
    fgets(str_buf, MAX_STATION_NAME_LENGTH * input->num_stations, stdin);
    i = 0;
    for (next = strtok(str_buf, delim); next != NULL; next = strtok(NULL, delim)) {
        sscanf(next, "%lf", &(input->popularity[i]));
        i++;
    }

    // GREEN LINE
    input->line_green = (line_t*)malloc(sizeof(line_t));
    line_init(input->line_green, input);
    input->line_green->id = 'g';

    // YELLOW LINE
    input->line_yellow = (line_t*)malloc(sizeof(line_t));
    line_init(input->line_yellow, input);
    input->line_yellow->id = 'y';

    // BLUE LINE
    input->line_blue = (line_t*)malloc(sizeof(line_t));
    line_init(input->line_blue, input);
    input->line_blue->id = 'b';

    // TICKS
    scanf("%d\n", &(input->ticks));

    // NUM TRAINS
    input->num_trains = (int*)malloc(sizeof(int) * 3);

    fgets(str_buf, MAX_STATION_NAME_LENGTH * input->num_stations, stdin);
    i = 0;
    for (next = strtok(str_buf, delim); next != NULL; next = strtok(NULL, delim)) {
        sscanf(next, "%d", &(input->num_trains[i]));
        i++;
    }
    input->line_green->num_trains = input->num_trains[0];
    input->line_green->start_train_id = 0;
    input->line_yellow->num_trains = input->num_trains[1];
    input->line_yellow->start_train_id = input->num_trains[0];
    input->line_blue->num_trains = input->num_trains[2];
    input->line_blue->start_train_id = input->num_trains[0] + input->num_trains[1];

    input->total_trains = input->line_blue->start_train_id + input->line_blue->num_trains;

    // CLEANUP
    free(str_buf);
}

void input_print(input_t* input)
{
    int i;
    int j;
    printf("%d\n", input->num_stations);
    for (i = 0; i < input->num_stations; i++) {
        printf("%s ", input->station_names[i]);
    }
    printf("\n");

    for (i = 0; i < input->num_stations; i++) {
        for (j = 0; j < input->num_stations; j++) {
            printf("%d ", input->stations[i][j]);
        }
        printf("\n");
    }

    for (i = 0; i < input->num_stations; i++) {
        printf("%.2lf ", input->popularity[i]);
    }
    printf("\n");

    line_print(input->line_green);
    line_print(input->line_yellow);
    line_print(input->line_blue);

    printf("%d\n", input->ticks);

    for (i = 0; i < 3; i++) {
        printf("%d ", input->num_trains[i]);
    }
    printf("\n");
}

void input_cleanup(input_t* input)
{
    int i;
    for (i = 0; i < input->num_stations; i++) {
        free(input->station_names[i]);
    }
    free(input->station_names);
    for (i = 0; i < input->num_stations; i++) {
        free(input->stations[i]);
    }
    free(input->stations);
    free(input->popularity);
    free(input->line_green->stations);
    free(input->line_green);
    free(input->line_yellow->stations);
    free(input->line_yellow);
    free(input->line_blue->stations);
    free(input->line_blue);
    free(input->num_trains);
    free(input);
}

void input_print_stats(input_t* input)
{
    // Green
    printf("green: ");
    line_print_stats(input->line_green);
    // Yellow
    printf("yellow: ");
    line_print_stats(input->line_yellow);
    // Blue
    printf("blue: ");
    line_print_stats(input->line_blue);
}

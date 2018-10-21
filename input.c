#include "input.h"

void input_init(input_t* input)
{
    int i;

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
    map_init(input);

    // POPULARITY
    input->popularity = (double*)malloc(sizeof(int*) * input->num_stations);
    fgets(str_buf, MAX_STATION_NAME_LENGTH * input->num_stations, stdin);
    i = 0;
    for (next = strtok(str_buf, delim); next != NULL; next = strtok(NULL, delim)) {
        sscanf(next, "%lf", &(input->popularity[i]));
        i++;
    }

    // GREEN LINE
    input->lines[0] = (line_t*)malloc(sizeof(line_t));
    line_init(input->lines[0], input);
    input->lines[0]->id = 'g';

    // YELLOW LINE
    input->lines[1] = (line_t*)malloc(sizeof(line_t));
    line_init(input->lines[1], input);
    input->lines[1]->id = 'y';

    // BLUE LINE
    input->lines[2] = (line_t*)malloc(sizeof(line_t));
    line_init(input->lines[2], input);
    input->lines[2]->id = 'b';

    // TICKS
    scanf("%d\n", &(input->ticks));

    // NUM TRAINS
    input->num_trains = (int*)malloc(sizeof(int) * LINE_NUM_LINES);

    fgets(str_buf, MAX_STATION_NAME_LENGTH * input->num_stations, stdin);
    i = 0;
    for (next = strtok(str_buf, delim); next != NULL; next = strtok(NULL, delim)) {
        sscanf(next, "%d", &(input->num_trains[i]));
        i++;
    }
    input->lines[0]->num_trains = input->num_trains[0];
    input->lines[0]->start_train_id = 0;
    input->lines[1]->num_trains = input->num_trains[1];
    input->lines[1]->start_train_id = input->num_trains[0];
    input->lines[2]->num_trains = input->num_trains[2];
    input->lines[2]->start_train_id = input->num_trains[0] + input->num_trains[1];

    input->total_trains = input->lines[2]->start_train_id + input->lines[2]->num_trains;

    // CLEANUP
    free(str_buf);
}

void input_print(input_t* input)
{
    int i;
    printf("%d\n", input->num_stations);
    for (i = 0; i < input->num_stations; i++) {
        printf("%s ", input->station_names[i]);
    }
    printf("\n");

    map_print(input->map);

    for (i = 0; i < input->num_stations; i++) {
        printf("%.2lf ", input->popularity[i]);
    }
    printf("\n");

    line_print(input->lines[0]);
    line_print(input->lines[1]);
    line_print(input->lines[2]);

    printf("%d\n", input->ticks);

    for (i = 0; i < LINE_NUM_LINES; i++) {
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
    map_cleanup(input->map);
    free(input->popularity);
    free(input->lines[0]->stations);
    free(input->lines[0]);
    free(input->lines[1]->stations);
    free(input->lines[1]);
    free(input->lines[2]->stations);
    free(input->lines[2]);
    free(input->num_trains);
    free(input);
}

void input_print_stats(input_t* input)
{
    // Green
    printf("green: ");
    line_print_stats(input->lines[0]);
    // Yellow
    printf("yellow: ");
    line_print_stats(input->lines[1]);
    // Blue
    printf("blue: ");
    line_print_stats(input->lines[2]);
}

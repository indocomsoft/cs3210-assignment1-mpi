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
    input->lines[LINE_GREEN] = (line_t*)malloc(sizeof(line_t));
    line_init(input->lines[LINE_GREEN], input);
    input->lines[LINE_GREEN]->id = 'g';

    // YELLOW LINE
    input->lines[LINE_YELLOW] = (line_t*)malloc(sizeof(line_t));
    line_init(input->lines[LINE_YELLOW], input);
    input->lines[LINE_YELLOW]->id = 'y';

    // BLUE LINE
    input->lines[LINE_BLUE] = (line_t*)malloc(sizeof(line_t));
    line_init(input->lines[LINE_BLUE], input);
    input->lines[LINE_BLUE]->id = 'b';

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
    input->lines[LINE_GREEN]->num_trains = input->num_trains[LINE_GREEN];
    input->lines[LINE_GREEN]->start_train_id = 0;
    input->lines[LINE_YELLOW]->num_trains = input->num_trains[LINE_YELLOW];
    input->lines[LINE_YELLOW]->start_train_id = input->num_trains[LINE_GREEN];
    input->lines[LINE_BLUE]->num_trains = input->num_trains[LINE_BLUE];
    input->lines[LINE_BLUE]->start_train_id = input->num_trains[LINE_GREEN] + input->num_trains[LINE_YELLOW];

    input->total_trains = input->lines[LINE_BLUE]->start_train_id + input->lines[LINE_BLUE]->num_trains;

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

    line_print(input->lines[LINE_GREEN]);
    line_print(input->lines[LINE_YELLOW]);
    line_print(input->lines[LINE_BLUE]);

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
    free(input->lines[LINE_GREEN]->stations);
    free(input->lines[LINE_GREEN]);
    free(input->lines[LINE_YELLOW]->stations);
    free(input->lines[LINE_YELLOW]);
    free(input->lines[LINE_BLUE]->stations);
    free(input->lines[LINE_BLUE]);
    free(input->num_trains);
    free(input);
}

void input_print_stats(input_t* input)
{
    // Green
    printf("green: ");
    line_print_stats(input->lines[LINE_GREEN]);
    // Yellow
    printf("yellow: ");
    line_print_stats(input->lines[LINE_YELLOW]);
    // Blue
    printf("blue: ");
    line_print_stats(input->lines[LINE_BLUE]);
}

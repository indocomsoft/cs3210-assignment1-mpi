#include "map.h"

void map_init(input_t* input)
{
    int i, j;
    char* next;

    char* str_buf = (char*)malloc(sizeof(char) * MAX_STATION_NAME_LENGTH * input->num_stations);

    input->map = (map_t*)malloc(sizeof(map_t));
    input->map->stations = (int**)malloc(sizeof(int*) * input->num_stations);

    for (i = 0; i < input->num_stations; i++) {
        input->map->stations[i] = (int*)malloc(sizeof(int) * input->num_stations);
    }

    for (i = 0; i < input->num_stations; i++) {
        fgets(str_buf, MAX_STATION_NAME_LENGTH * input->num_stations, stdin);
        j = 0;
        for (next = strtok(str_buf, " "); next != NULL; next = strtok(NULL, " ")) {
            input->map->stations[i][j] = atoi(next);
            j++;
        }
    }

    input->map->num_stations = input->num_stations;

    free(str_buf);
}

void print_map(map_t* map)
{
    int i, j;

    for (i = 0; i < map->num_stations; i++) {
        for (j = 0; j < map->num_stations; j++) {
            printf("%d ", map->stations[i][j]);
        }
        printf("\n");
    }
}

void map_cleanup(map_t* map)
{
    int i;

    for (i = 0; i < map->num_stations; i++) {
        free(map->stations[i]);
    }
    free(map);
}
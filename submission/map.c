#include "map.h"

void edge_map_init(edge_map_t* edge_map, map_t* map)
{
    int i, j, counter = 0;
    edge_map->num_stations = map->num_stations;
    edge_map->edges = (int**)malloc(sizeof(int*) * map->num_stations);
    for (i = 0; i < map->num_stations; i++) {
        edge_map->edges[i] = (int*)malloc(sizeof(int) * map->num_stations);
    }

    for (i = 0; i < map->num_stations; i++) {
        for (j = 0; j < map->num_stations; j++) {
            if (map->stations[i][j] != 0) {
                edge_map->edges[i][j] = counter;
                counter++;
            } else {
                edge_map->edges[i][j] = EDGE_MAP_NULL_ID;
            }
        }
    }
}

void edge_map_print(edge_map_t* edge_map)
{
    int i, j;

    for (i = 0; i < edge_map->num_stations; i++) {
        for (j = 0; j < edge_map->num_stations; j++) {
            printf("%d ", edge_map->edges[i][j]);
        }
        printf("\n");
    }
}

void edge_map_get_slave_meta(edge_map_t* edge_map, int my_id, int res[SLAVE_META_SIZE])
{
    int i, j;
    for (i = 0; i < edge_map->num_stations; i++) {
        for (j = 0; j < edge_map->num_stations; j++) {
            if (edge_map->edges[i][j] == my_id) {
                res[SLAVE_META_SRC] = i;
                res[SLAVE_META_DST] = j;
                return;
            }
        }
    }
}

void map_broadcast_receive(map_t* map, int source)
{
    int i;
    MPI_Bcast((void*)&(map->num_stations), 1, MPI_INT, source, MPI_COMM_WORLD);
    map->stations = (int**)malloc(sizeof(int*) * map->num_stations);

    for (i = 0; i < map->num_stations; i++) {
        map->stations[i] = (int*)malloc(sizeof(int) * map->num_stations);
    }
    int* map_buf = (int*)malloc(sizeof(int) * map->num_stations * map->num_stations);

    MPI_Bcast((void*)map_buf, map->num_stations * map->num_stations, MPI_INT, source, MPI_COMM_WORLD);

    for (i = 0; i < map->num_stations; i++) {
        memcpy(map->stations[i], map_buf + i * map->num_stations, map->num_stations * sizeof(int));
    }

    free(map_buf);
}

void map_broadcast_send(map_t* map, int source)
{
    int i;

    MPI_Bcast((void*)&(map->num_stations), 1, MPI_INT, source, MPI_COMM_WORLD);

    int* map_buf = (int*)malloc(sizeof(int) * map->num_stations * map->num_stations);
    for (i = 0; i < map->num_stations; i++) {
        memcpy(map_buf + i * map->num_stations, map->stations[i], map->num_stations * sizeof(int));
    }

    MPI_Bcast((void*)map_buf, map->num_stations * map->num_stations, MPI_INT, source, MPI_COMM_WORLD);

    free(map_buf);
}

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

void map_print(map_t* map)
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

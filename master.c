#include "master.h"

#define MASTER_ID slaves

typedef struct raw_train_status_t train_status_t;
typedef struct raw_master_state_t master_state_t;

struct raw_train_status_t {
    int id;
    int line_id;
    bool is_travelling;
    bool spawned;
    int edge_id;
    int station_id;
};

struct raw_master_state_t {
    input_t* input;
    train_status_t* stats;
    edge_map_t* edge_map;
    timekeeper_t* station_timekeepers;
    int** edge_to_src_dst;
    int** train_line_id_index;
    int slaves;
    int time;
};

void train_statuses_init(train_status_t* stat, input_t* input)
{
    int counter = 0;
    for (int i = 0; i < LINE_NUM_LINES; i++) {
        for (int j = 0; j < input->num_trains[i]; i++) {
            stat[counter].id = j;
            stat[counter].line_id = i;
            stat[counter].is_travelling = false;
            stat[counter].spawned = false;
            stat[counter].edge_id = -1;
            stat[counter].station_id = -1;
            counter++;
        }
    }
}

void receive_commstats(master_state_t* state)
{
    int i, station_id, duration, start, train_idx;
    for (i = 0; i < state->slaves; i++) {
        commstat_t commstat;
        int tag = commstat_master_recv(&commstat, i);
        while (tag != COMMSTAT_END_COMM) {
            train_idx = state->train_line_id_index[commstat.train_line_id][commstat.train_id];
            state->stats[train_idx].spawned = true;
            switch (tag) {
            case COMMSTAT_START_MOVING:
                state->stats[train_idx].is_travelling = true;
                state->stats[train_idx].edge_id = i;
                break;
            case COMMSTAT_ARRIVED:
                station_id = state->edge_to_src_dst[i][SLAVE_META_DST];
                state->stats[train_idx].is_travelling = false;
                state->stats[train_idx].station_id = station_id;
                duration = (int)ceil((double)state->input->popularity[station_id] * (rand() % 10 + 1));
                start = timekeeper_increase_by(&state->station_timekeepers[station_id], duration, state->time);
                commstat_master_send_ready_time(start + duration, i);
                station_stat_open_door(state->input->lines[commstat.train_line_id]->stats, start, duration, commstat.travelling_forward);
                break;
            }
            tag = commstat_master_recv(&commstat, i);
        }
    }
}

void print_time_tick_stats(master_state_t* state)
{
    int i;
    train_status_t* stat;
    printf("%d: ", state->time);
    for (i = 0; i < state->input->total_trains; i++) {
        stat = &state->stats[i];
        if (stat->spawned) {
            printf("%c%d-", state->input->lines[stat->line_id]->id, stat->id);
            if (stat->is_travelling) {
                printf("s%d->s%d, ", state->edge_to_src_dst[stat->edge_id][SLAVE_META_SRC], state->edge_to_src_dst[stat->edge_id][SLAVE_META_DST]);
            } else {
                printf("s%d, ", stat->station_id);
            }
        }
    }
}

void station_timekeepers_init(timekeeper_t* timekeepers, input_t* input)
{
    int i;
    for (i = 0; i < input->num_stations; i++) {
        timekeeper_init(&timekeepers[i]);
    }
}

int** build_edge_to_src_dst_memo_table(edge_map_t* edge_map, int slaves)
{
    int i;
    int** result = (int**)malloc(slaves * sizeof(int*));
    for (i = 0; i < slaves; i++) {
        result[i] = (int*)malloc(SLAVE_META_SIZE * sizeof(int));
        edge_map_get_slave_meta(edge_map, i, result[i]);
    }
    return result;
}

int** build_train_line_id_index_memo_table(input_t* input)
{
    int i, j, counter = 0;
    int** result = (int**)malloc(LINE_NUM_LINES * sizeof(int*));
    for (i = 0; i < LINE_NUM_LINES; i++) {
        result[i] = (int*)malloc(input->num_trains[i] * sizeof(int));
        for (j = 0; j < input->num_trains[i]; j++) {
            result[i][j] = counter;
            counter++;
        }
    }
    return result;
}

void master_state_init(master_state_t* state, input_t* input, int slaves)
{
    state->input = input;
    state->slaves = slaves;
    state->time = 0;

    edge_map_t* edge_map = (edge_map_t*)malloc(sizeof(edge_map_t));
    train_status_t* train_statuses = (train_status_t*)malloc(input->total_trains * sizeof(train_status_t));
    timekeeper_t* station_timekeepers = (timekeeper_t*)malloc(input->num_stations * sizeof(timekeeper_t));

    edge_map_init(edge_map, input->map);
    train_statuses_init(train_statuses, input);
    station_timekeepers_init(station_timekeepers, input);

    state->stats = train_statuses;
    state->edge_map = edge_map;
    state->station_timekeepers = station_timekeepers;

    state->edge_to_src_dst = build_edge_to_src_dst_memo_table(edge_map, slaves);
    state->train_line_id_index = build_train_line_id_index_memo_table(input);
}

void master(int my_id, int slaves)
{
    input_t* input = (input_t*)malloc(sizeof(input_t));
    input_init(input);
#ifdef DEBUG
    puts("Input:");
    input_print(input);
#endif

#ifdef DEBUG
    puts("Sending map");
#endif
    map_broadcast_send(input->map, MASTER_ID);
#ifdef DEBUG
    puts("Sent map");
#endif

#ifdef DEBUG
    puts("Sending lines");
#endif
    line_broadcast_send(input->lines, MASTER_ID);
#ifdef DEBUG
    puts("Sent lines");
#endif

    master_state_t state;
    master_state_init(&state, input, slaves);

    while (state.time < input->ticks) {
        receive_commstats(&state);
        print_time_tick_stats(&state);
        MPI_Barrier(MPI_COMM_WORLD);
        state.time++;
    }
    input_print_stats(input);
}

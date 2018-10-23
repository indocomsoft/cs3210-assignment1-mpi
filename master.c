#include "master.h"

#define MASTER_ID slaves

typedef struct raw_train_status_t train_status_t;
typedef struct raw_master_state_t master_state_t;

struct raw_train_status_t {
    int id;
    int line_id;
    bool is_travelling;
    int edge_id;
    int station_id;
};

struct raw_master_state_t {
    input_t* input;
    train_status_t* stat;
    edge_map_t* edge_map;
    timekeeper_t* station_timekeepers;
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
            stat[counter].edge_id = -1;
            stat[counter].station_id = -1;
            counter++;
        }
    }
}

int get_train_idx(input_t* input, int train_id, int train_line_id)
{
    int result = 0;
    int i;
    for (i = 0; i < train_line_id; i++) {
        result += input->num_trains[i];
    }
    return result + train_id;
}

void receive_commstats(master_state_t* state)
{
    int res[SLAVE_META_SIZE];
    int i, station_id, duration, new_time;
    for (i = 0; i < state->slaves; i++) {
        commstat_t commstat;
        int tag = commstat_master_recv(&commstat, i);
        while (tag != COMMSTAT_END_COMM) {
            int train_idx = get_train_idx(state->input, commstat.train_id, commstat.train_line_id);
            switch (tag) {
            case COMMSTAT_START_MOVING:
                state->stat[train_idx].is_travelling = true;
                state->stat[train_idx].edge_id = i;
                break;
            case COMMSTAT_ARRIVED:
                edge_map_get_slave_meta(state->edge_map, i, res);
                station_id = res[SLAVE_META_DST];
                state->stat[train_idx].is_travelling = false;
                state->stat[train_idx].station_id = station_id;
                duration = (int)ceil((double)state->input->popularity[station_id] * (rand() % 10 + 1));
                new_time = timekeeper_increase_by(&tstate->station_timekeepers[station_id], duration, state->time);
                commstat_master_send_ready_time(new_time, i);
                break;
            }
            tag = commstat_master_recv(&commstat, i);
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

    state->stat = train_statuses;
    state->edge_map = edge_map;
    state->station_timekeepers = station_timekeepers;
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
        MPI_Barrier(MPI_COMM_WORLD);
        state.time++;
    }
}

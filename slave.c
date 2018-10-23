#include "slave.h"
#include <unistd.h>

#define MASTER_ID slaves

void slave(int my_id, int slaves, MPI_Comm comm_slave)
{
    int i;

    map_t map;
    map_broadcast_receive(&map, MASTER_ID);

    line_t* lines[LINE_NUM_LINES];
    line_broadcast_receive(lines, MASTER_ID);

    edge_map_t edge_map;
    edge_map_init(&edge_map, &map);

    int slave_meta[SLAVE_META_SIZE];
    edge_map_get_slave_meta(&edge_map, my_id, slave_meta);

    int src = slave_meta[SLAVE_META_SRC];
    int dst = slave_meta[SLAVE_META_DST];
    int transit_time = map.stations[src][dst];
    // printf("edge id: %d, src: %d, dst: %d\n", my_id, src, dst);

    int num_to_receive = 0;
    for (i = 0; i < map.num_stations; i++) {
        if (map.stations[i][src] != 0) {
            num_to_receive++;
        }
    }

    spawn_info_t spawn_infos[LINE_NUM_LINES];
    train_t trains_spawned[LINE_NUM_LINES];
    bool should_spawn;
    should_spawn = spawn_get_spawn_info(spawn_infos, lines, src, dst);

    queue_t enter_queue;
    queue_t exit_queue;

    queue_init(&enter_queue);
    queue_init(&exit_queue);

    train_t current_train;
    current_train.id = TRAIN_NULL_ID;
    int current_train_done_time = 0;
    int current_time = 0;

    commstat_t commstat;

    while (true) {
        // If current train exists and is done
        if (current_train.id != TRAIN_NULL_ID && current_train_done_time == current_time) {
            enqueue_train_for_departure(current_train, &exit_queue, &commstat, dst, MASTER_ID);
        }

        // If no current train and there's a train waiting to enter
        if (current_train.id == TRAIN_NULL_ID && queue_is_empty(&enter_queue) == false) {
            pair_t enter_queue_head = queue_dequeue(&enter_queue);
            current_train = enter_queue_head.train;
            current_train_done_time = current_time + transit_time;
            // inform master
            commstat_init(&commstat, current_train.id, current_train.line_id, src, current_train.travelling_forward);
            commstat_report_start_moving(&commstat, MASTER_ID);
        }

        // Spawn trains and report to master if necessary
        if (should_spawn == true) {
            int spawned = spawn_trains(spawn_infos, trains_spawned);
            if (spawned == 0) {
                should_spawn = false;
            }
            for (i = 0; i < spawned; i++) {
                enqueue_train_for_departure(trains_spawned[i], &exit_queue, &commstat, dst, MASTER_ID);
            }
        }

        // If there is a train that can start queueing for edge at the next tick, send it
        if (queue_is_empty(&exit_queue) == false && queue_head(&exit_queue).time == current_time + 1) {
            train_t depart_train = queue_dequeue(&exit_queue).train;
            int next_station = get_train_next_station(&depart_train, dst, lines);
            int next_edge = edge_map.edges[dst][next_station];

            train_send(&depart_train, next_edge, comm_slave);
        }

        // Send done signal to all edges I can send trains to.
        for (i = 0; i < map.num_stations; i++) {
            if (map.stations[dst][i] != 0) {
                train_send(NULL, edge_map.edges[dst][i], comm_slave);
            }
        }
        // printf("%d\n", my_id);

        // Receive done signals from all edges that could possibly send trains to me
        int received = 0;
        while (received < num_to_receive) {
            train_t train_buf;
            int status = train_recv(&train_buf, MPI_ANY_SOURCE, comm_slave);
            if (status == TRAIN_GOT_TRAIN) {
                pair_t enter_pair;
                enter_pair.train = train_buf;
                queue_enqueue(&enter_queue, enter_pair);
            } else {
                received++;
            }
        }

        commstat_report_end_comm(MASTER_ID);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(&current_time, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
        if (current_time == SHUTDOWN)
            break;
        // report to master done
        // advance time
    }
}

void enqueue_train_for_departure(train_t train, queue_t* exit_queue, commstat_t* commstat, int station_id, int slaves)
{
    // request depart time from master;
    commstat_init(commstat, train.id, train.line_id, station_id, train.travelling_forward);
    int depart_time = commstat_report_arrived(commstat, MASTER_ID);
    pair_t done_pair;
    done_pair.time = depart_time;
    done_pair.train = train;
    queue_enqueue(exit_queue, done_pair);
}

int get_train_next_station(train_t* train, int station_id, line_t* lines[LINE_NUM_LINES])
{
    int i;
    line_t* line = lines[train->line_id];
    if (train->travelling_forward == true) {
        // If no need to u-turn
        for (i = 0; i < line->num_stations - 1; i++) {
            if (line->stations[i] == station_id) {
                return line->stations[i + 1];
            }
        }

        // Train just reached last station, need to u-turn, return second last station
        train->travelling_forward = false;
        return line->stations[line->num_stations - 2];
    } else {
        // If no need to u-turn
        for (i = line->num_stations - 1; i > 0; i--) {
            if (line->stations[i] == station_id) {
                return line->stations[i - 1];
            }
        }

        // Train just reached last station, need to u-turn, return second last station
        train->travelling_forward = true;
        return line->stations[1];
    }
}

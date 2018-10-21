#include "slave.h"
#include <unistd.h>

#define MASTER_ID slaves

void slave(int my_id, int slaves, MPI_Comm comm_slave)
{
    int i;

    map_t map;
    map_broadcast_receive(&map, MASTER_ID);

    line_t* lines[3];
    line_broadcast_receive(lines, MASTER_ID);

    edge_map_t edge_map;
    edge_map_init(&edge_map, &map);

    int slave_meta[2];
    edge_map_get_slave_meta(&edge_map, my_id, slave_meta);

    int src = slave_meta[0];
    int dst = slave_meta[1];
    int transit_time = map.stations[src][dst];
    printf("edge id: %d, src: %d, dst: %d\n", my_id, src, dst);

    int num_to_receive = 0;
    for (i = 0; i < map.num_stations; i++) {
        if (map.stations[i][src] != 0) {
            num_to_receive++;
        }
    }

    queue_t* enter_queue;
    queue_t* exit_queue;

    queue_init(enter_queue);
    queue_init(exit_queue);

    train_t current_train;
    current_train.id = TRAIN_NULL_ID;
    int current_train_done_time = 0;
    int current_time = 0;
    // Synchronise start

    // If current train exists and is done
    if (current_train.id != TRAIN_NULL_ID && current_train_done_time == current_time) {
        // request depart time from master;
        int depart_time = 0;
        pair_t done_pair;
        done_pair.time = depart_time;
        done_pair.train = current_train;
        queue_enqueue(exit_queue, done_pair);
        // update current_train_push_to_next_edge_time
    }

    // If no current train and there's a train waiting to enter
    if (current_train.id == TRAIN_NULL_ID && queue_is_empty(enter_queue) == false) {
        pair_t enter_queue_head = queue_dequeue(enter_queue);
        current_train = enter_queue_head.train;
        current_train_done_time = current_time + transit_time;
    }

    // If there is a train that needs to be sent to the next edge
    if (queue_is_empty(exit_queue) == false && queue_head(exit_queue).time == current_time) {
        train_t depart_train = queue_dequeue(exit_queue).train;
        int next_edge = get_train_next_edge(&depart_train, dst, lines);

        train_send(&depart_train, next_edge, comm_slave);
    }

    for (i = 0; i < map.num_stations; i++) {
        if (map.stations[dst][i] != 0) {
            train_send(NULL, edge_map.edges[dst][i], comm_slave);
        }
    }

    int received = 0;
    while (received < num_to_receive) {
        train_t train_buf;
        int status = train_recv(&train_buf, MPI_ANY_SOURCE, comm_slave);
        if (status == TRAIN_GOT_TRAIN) {
            pair_t enter_pair;
            enter_pair.train = train_buf;
            queue_enqueue(enter_queue, enter_pair);
        } else {
            received++;
        }
    }

    // report to master done
    // advance time
}

int get_train_next_edge(train_t* train, int station_id, line_t* lines[3])
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

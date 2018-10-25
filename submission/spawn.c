#include "spawn.h"

bool spawn_get_spawn_info(spawn_info_t spawn_infos[LINE_NUM_LINES], line_t* lines[LINE_NUM_LINES], int src, int dst)
{
    bool is_spawn = false;
    int i;
    line_t line;
    for (i = 0; i < LINE_NUM_LINES; i++) {
        line = *lines[i];
        spawn_infos[i].line_id = i;
        spawn_infos[i].base_train_id = line.start_train_id;
        spawn_infos[i].count = 0;

        if (line.stations[line.num_stations - 2] == src && line.stations[line.num_stations - 1] == dst) {
            // If odd number of trains for the line, extra train will travel forward
            spawn_infos[i].num_trains = line.num_trains - line.num_trains / 2;
            spawn_infos[i].forward = true;
            is_spawn = true;
        } else if (line.stations[1] == src && line.stations[0] == dst) {
            spawn_infos[i].num_trains = line.num_trains / 2;
            spawn_infos[i].forward = false;
            is_spawn = true;
        } else {
            spawn_infos[i].num_trains = 0;
        }
    }

    return is_spawn;
}

int spawn_trains(spawn_info_t spawn_infos[LINE_NUM_LINES], train_t trains[LINE_NUM_LINES])
{
    int spawned = 0;

    for (int i = 0; i < LINE_NUM_LINES; i++) {
        if (spawn_infos[i].num_trains > 0 && spawn_infos[i].count < spawn_infos[i].num_trains) {
            int train_id = spawn_infos[i].count * 2;
            if (spawn_infos[i].forward == false) {
                // trains spawned backwards shall be odd
                train_id += 1;
            }

            trains[spawned].id = train_id + spawn_infos[i].base_train_id;
            trains[spawned].line_id = i;
            trains[spawned].travelling_forward = spawn_infos[i].forward;
            spawn_infos[i].count++;
            spawned++;
        }
    }

    return spawned;
}

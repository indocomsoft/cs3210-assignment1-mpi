#include "spawn.h"

bool spawn_get_spawn_info(spawn_info_t spawn_infos[6], line_t* lines[3], int src, int dst)
{
    bool is_spawn = false;
    int i;
    line_t line;
    int idx;
    for (i = 0; i < 3; i++) {
        line = *lines[i];
        idx = 2 * i;
        if (line.stations[line.num_stations - 2] == src && line.stations[line.num_stations - 1] == dst) {
            spawn_infos[idx].line_id = i;
            spawn_infos[idx].count = 0;
            // If odd number of trains for the line, extra train will travel forward
            spawn_infos[idx].num_trains = line.num_trains - line.num_trains / 2;
            spawn_infos[idx].forward = true;
            is_spawn = true;
        } else {
            spawn_infos[idx].num_trains = 0;
        }

        if (line.stations[1] == src && line.stations[0] == dst) {
            spawn_infos[idx + 1].line_id = i;
            spawn_infos[idx + 1].count = 0;
            spawn_infos[idx + 1].num_trains = line.num_trains / 2;
            spawn_infos[idx + 1].forward = false;
            is_spawn = true;
        } else {
            spawn_infos[idx + 1].num_trains = 0;
        }
    }

    return is_spawn;
}

int spawn_trains(spawn_info_t spawn_infos[6], train_t trains[6])
{
    int spawned = 0;

    for (int i = 0; i < 6; i++) {
        if (spawn_infos[i].num_trains > 0 && spawn_infos[i].count < spawn_infos[i].num_trains) {
            int train_id = spawn_infos[i].count * 2;
            if (spawn_infos[i].forward == false) {
                // trains spawned backwards shall be odd
                train_id += 1;
            }

            trains[spawned].id = train_id;
            trains[spawned].line_id = i / 2;
            trains[spawned].travelling_forward = spawn_infos[i].forward;
            spawned++;
        }
    }

    return spawned;
}
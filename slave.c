#include "slave.h"
#include <unistd.h>

#define MASTER_ID slaves

void slave(int my_id, int slaves)
{
    line_t* lines[3];
    line_broadcast_receive(lines, MASTER_ID);

    map_t map;
    map_broadcast_receive(&map, MASTER_ID);
    int slave_meta[2];

    edge_map_t edge_map;
    edge_map_init(&edge_map, &map);
    edge_map_get_slave_meta(&edge_map, my_id, slave_meta);
    printf("my id: %d, src: %d, dst: %d\n", my_id, slave_meta[0], slave_meta[1]);
}

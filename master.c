#include "master.h"

#define MASTER_ID slaves

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

    edge_map_t edge_map;
    edge_map_init(&edge_map, input->map);
}

#include "slave.h"

#define MASTER_ID slaves

void slave(int my_id, int slaves)
{
    line_t* line = line_broadcast_receive(MASTER_ID);
    puts("received");
    line_print(line);
    puts("But the 13th amendment!!!");
}

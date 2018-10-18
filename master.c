#include "master.h"

#define MASTER_ID slaves

void master(int my_id, int slaves)
{
    input_t* input = (input_t*)malloc(sizeof(input_t));
    input_init(input);
#ifdef DEBUG
    input_print(input);
#endif
    line_print(input->line_blue);
    puts("sending");
    line_broadcast_send(input->line_blue, MASTER_ID);
    puts("I am the master!");
}

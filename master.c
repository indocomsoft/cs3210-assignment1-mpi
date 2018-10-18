#include "master.h"

void master()
{
    input_t* input = (input_t*)malloc(sizeof(input_t));
    input_init(input);
#ifdef DEBUG
    input_print(input);
#endif

    puts("I am the master!");
}

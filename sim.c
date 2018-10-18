#include "sim.h"

#define MASTER_ID slaves

int main(int argc, char** argv)
{
    int nprocs;
    int my_id, slaves;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    // One master and nprocs-1 slaves
    slaves = nprocs - 1;

    if (my_id == MASTER_ID) {
        fprintf(stderr, " +++ Process %d is master\n", my_id);
        master(my_id, slaves);
    } else {
        fprintf(stderr, " --- Process %d is slave\n", my_id);
        slave(my_id, slaves);
    }
    MPI_Finalize();
    return 0;
}

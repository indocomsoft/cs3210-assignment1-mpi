#include "sim.h"

#define MASTER_ID slaves

int myid, slaves;

int main(int argc, char** argv)
{
    int nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    // One master and nprocs-1 slaves
    slaves = nprocs - 1;

    if (myid == MASTER_ID) {
        fprintf(stderr, " +++ Process %d is master\n", myid);
        master();
    } else {
        fprintf(stderr, " --- Process %d is slave\n", myid);
        slave();
    }
    MPI_Finalize();
    return 0;
}

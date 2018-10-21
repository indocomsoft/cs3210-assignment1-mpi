#include "commstat.h"

#define MASTER_ID slaves
#define BUFFER_SIZE 2
#define TRAIN_ID 0
#define STATION_ID 1

void commstat_init(commstat_t* stat, int train_id, int station_id)
{
    stat->train_id = train_id;
    stat->station_id = station_id;
}

void commstat_report_end_comm(int slaves)
{
    int buffer[BUFFER_SIZE];
    MPI_Send(buffer, BUFFER_SIZE, MPI_INT, MASTER_ID, COMMSTAT_END_COMM, MPI_COMM_WORLD);
}

void commstat_report_start_moving(commstat_t* stat, int slaves)
{
    int buffer[BUFFER_SIZE];
    buffer[TRAIN_ID] = stat->train_id;
    buffer[STATION_ID] = stat->station_id;
    MPI_Send(buffer, BUFFER_SIZE, MPI_INT, MASTER_ID, COMMSTAT_START_MOVING, MPI_COMM_WORLD);
}

int commstat_report_arrived(commstat_t* stat, int slaves)
{
    int buffer[BUFFER_SIZE];
    buffer[TRAIN_ID] = stat->train_id;
    buffer[STATION_ID] = stat->station_id;
    int ready_time;
    MPI_Send(buffer, BUFFER_SIZE, MPI_INT, MASTER_ID, COMMSTAT_ARRIVED, MPI_COMM_WORLD);
    MPI_Recv(&ready_time, 1, MPI_INT, MASTER_ID, COMMSTAT_ARRIVED, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return ready_time;
}

void commstat_master_send_ready_time(int ready_time, int slave_id)
{
    MPI_Send(&ready_time, 1, MPI_INT, slave_id, COMMSTAT_ARRIVED, MPI_COMM_WORLD);
}

int commstat_master_recv(commstat_t* stat, int slave_id)
{
    int buffer[BUFFER_SIZE];
    MPI_Status status;
    MPI_Recv(buffer, BUFFER_SIZE, MPI_INT, slave_id, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    stat->train_id = buffer[TRAIN_ID];
    stat->station_id = buffer[STATION_ID];
    return status.MPI_TAG;
}

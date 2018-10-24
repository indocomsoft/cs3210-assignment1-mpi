#include "commstat.h"

#define MASTER_ID slaves
#define BUFFER_SIZE 4

#define TRAIN_ID 0
#define TRAIN_LINE_ID 1
#define STATION_ID 2
#define TRAVELLING_FORWARD 3

void commstat_init(commstat_t* stat, int train_id, int train_line_id, int station_id, bool travelling_forward)
{
    stat->train_id = train_id;
    stat->train_line_id = train_line_id;
    stat->station_id = station_id;
    stat->travelling_forward = travelling_forward;
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
    buffer[TRAIN_LINE_ID] = stat->train_line_id;
    buffer[STATION_ID] = stat->station_id;
    buffer[TRAVELLING_FORWARD] = (int)stat->travelling_forward;
    MPI_Send(buffer, BUFFER_SIZE, MPI_INT, MASTER_ID, COMMSTAT_START_MOVING, MPI_COMM_WORLD);
}

int commstat_report_arrived(commstat_t* stat, int slaves)
{
    int buffer[BUFFER_SIZE];
    buffer[TRAIN_ID] = stat->train_id;
    buffer[TRAIN_LINE_ID] = stat->train_line_id;
    buffer[STATION_ID] = stat->station_id;
    buffer[TRAVELLING_FORWARD] = (int)stat->travelling_forward;
    int ready_time;
    MPI_Send(buffer, BUFFER_SIZE, MPI_INT, MASTER_ID, COMMSTAT_ARRIVED, MPI_COMM_WORLD);
    MPI_Recv(&ready_time, 1, MPI_INT, MASTER_ID, COMMSTAT_ARRIVED, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return ready_time;
}

void commstat_master_send_ready_time(int ready_time, int slave_id)
{
    MPI_Send(&ready_time, 1, MPI_INT, slave_id, COMMSTAT_ARRIVED, MPI_COMM_WORLD);
}

void commstat_master_recv(commstat_t* stat, int res[COMMSTAT_RECV_NUM_FIELDS])
{
    int buffer[BUFFER_SIZE];
    MPI_Status status;
    MPI_Recv(buffer, BUFFER_SIZE, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    stat->train_id = buffer[TRAIN_ID];
    stat->train_line_id = buffer[TRAIN_LINE_ID];
    stat->station_id = buffer[STATION_ID];
    stat->travelling_forward = (bool)buffer[TRAVELLING_FORWARD];
    res[COMMSTAT_MPI_TAG] = status.MPI_TAG;
    res[COMMSTAT_MPI_SOURCE] = status.MPI_SOURCE;
}

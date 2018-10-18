#include "train.h"

#define TAG 0
#define TRAIN_NUM_FIELDS 3
#define TRAIN_ID 0
#define TRAIN_LINE_ID 1
#define TRAIN_TRAVELLING_FORWARD 2

void train_init(train_t* train, int id, int line_id, bool travelling_forward)
{
    train->id = id;
    train->line_id = line_id;
    train->travelling_forward = travelling_forward;
}

void train_send(train_t* train, int dest)
{
    int data[TRAIN_NUM_FIELDS];
    data[TRAIN_ID] = train->id;
    data[TRAIN_LINE_ID] = train->line_id;
    data[TRAIN_TRAVELLING_FORWARD] = (int)train->travelling_forward;
    MPI_Send(data, TRAIN_NUM_FIELDS, MPI_INT, dest, TAG, MPI_COMM_WORLD);
}

void train_recv(train_t* train, int source)
{
    int data[TRAIN_NUM_FIELDS];
    MPI_Recv(data, TRAIN_NUM_FIELDS, MPI_INT, source, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    train->id = data[TRAIN_ID];
    train->line_id = data[TRAIN_LINE_ID];
    train->travelling_forward = (bool)data[TRAIN_NUM_FIELDS];
}

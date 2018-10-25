#ifndef TRAIN_DEFINED
#define TRAIN_DEFINED

#include <mpi.h>
#include <stdbool.h>
#include <stddef.h>

#define TRAIN_NO_TRAIN 0
#define TRAIN_GOT_TRAIN 1

#define TRAIN_NULL_ID -1

typedef struct raw_train_t train_t;

struct raw_train_t {
    int id;
    int line_id;
    bool travelling_forward;
};

void train_init(train_t* train, int id, int line_id, bool travelling_forward);

/**
 * Set train to NULL to indicate that no train is sent.
 */
void train_send(train_t* train, int dest, MPI_Comm comm);

/**
 * Returns 0 if there is no train (train_t* train contains garbage values)
 * Returns 1 if a train is sent
 */
int train_recv(train_t* train, int source, MPI_Comm comm);
#endif

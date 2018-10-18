#ifndef TRAIN_DEFINED
#define TRAIN_DEFINED

#include <mpi.h>
#include <stdbool.h>

typedef struct raw_train_t train_t;

struct raw_train_t {
    int id;
    int line_id;
    bool travelling_forward;
};

void train_init(train_t* train, int id, int line_id, bool travelling_forward);

void train_send(train_t* train, int dest);

void train_recv(train_t* train, int source);
#endif

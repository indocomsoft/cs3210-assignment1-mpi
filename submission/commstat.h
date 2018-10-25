#ifndef COMMSTAT_DEFINED
#define COMMSTAT_DEFINED

#define COMMSTAT_END_COMM 0
#define COMMSTAT_START_MOVING 1
#define COMMSTAT_ARRIVED 2

#define COMMSTAT_RECV_NUM_FIELDS 2
#define COMMSTAT_MPI_TAG 0
#define COMMSTAT_MPI_SOURCE 1

#define SHUTDOWN -1

typedef struct raw_commstat_t commstat_t;

#include "timekeeper.h"

#include <mpi.h>
#include <stdbool.h>

struct raw_commstat_t {
    int train_id;
    int train_line_id;
    int station_id;
    bool travelling_forward;
};

void commstat_init(commstat_t* stat, int train_id, int train_line_id, int station_id, bool travelling_forward);

/**
 * For slaves.
 * Reports end of communication for this time tick to master.
 */
void commstat_report_end_comm(int slaves);

/**
 * For slaves.
 * Reports to master that train_id started moving from station_id.
 */
void commstat_report_start_moving(commstat_t* stat, int slaves);

/**
 * For slaves.
 * Reports to master that train_id arrived at station_id.
 *
 * Returns the time when train_id will be ready to leave.
 */
int commstat_report_arrived(commstat_t* stat, int slaves);

/**
 * For master.
 * Send the ready time to slave_id.
 */
void commstat_master_send_ready_time(int ready_time, int slave_id);

/**
 * For master.
 * Receive stat from slave_id.
 *
 * Returns either COMMSTAT_END_COMM, COMMSTAT_START_MOVING, or COMMSTAT_ARRIVED
 */
void commstat_master_recv(commstat_t* stat, int res[COMMSTAT_RECV_NUM_FIELDS]);

#endif

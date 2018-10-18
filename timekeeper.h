#ifndef TIMEKEEPER_DEFINED
#define TIMEKEEPER_DEFINED

// For max and min
#include "station_stat.h"

typedef struct timekeeper_t {
    int time;
} timekeeper_t;

/**
 * Initialises a timekeeper with time = 0
 */
void timekeeper_init(timekeeper_t* timekeeper);

/**
 * Increases the timekeeper by duration and returns the new value.
 *
 * This operation is atomic.
 */
int timekeeper_increase_by(timekeeper_t* timekeeper, int duration, int current_time);

#endif

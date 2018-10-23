#ifndef MASTER_DEFINED
#define MASTER_DEFINED

#include <math.h>
#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commstat.h"
#include "input.h"
#include "line.h"
#include "map.h"
#include "station_stat.h"

void master(int my_id, int slaves);

#endif

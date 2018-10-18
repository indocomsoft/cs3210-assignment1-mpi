#!/bin/sh

if [ $# -eq 0 ]; then
  echo Usage:
  echo ./run.sh \<number of processes\>
  exit 1
fi

physicalCpuCount=$([ $(uname) = 'Darwin' ] && 
                       sysctl -n hw.physicalcpu_max ||
                       lscpu -p | egrep -v '^#' | sort -u -t, -k 2,4 | wc -l)

if [ $1 -gt $physicalCpuCount ]; then
  mpirun -np $1 --oversubscribe bash -c "./sim < input"
else
  mpirun -np $1 bash -c "./sim < input"
fi

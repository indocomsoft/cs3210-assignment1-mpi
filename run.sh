#!/bin/sh

if [ $# -eq 0 ]; then
  echo Usage:
  echo ./run.sh \<number of slaves\>
  exit 1
fi

physicalCpuCount=$([ $(uname) = 'Darwin' ] &&
                       sysctl -n hw.physicalcpu_max ||
                       lscpu -p | egrep -v '^#' | sort -u -t, -k 2,4 | wc -l)


processes=$(($1 + 1))

if [ $processes -gt $physicalCpuCount ]; then
  mpirun -np $processes --oversubscribe bash -c "./sim < input"
else
  mpirun -np $processes bash -c "./sim < input"
fi

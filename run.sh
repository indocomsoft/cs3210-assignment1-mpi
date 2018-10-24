#!/bin/sh

if [ $# -eq 0 ]; then
  echo Usage:
  echo ./run.sh \<input file\>
  exit 1
fi

physicalCpuCount=$([ $(uname) = 'Darwin' ] &&
                       sysctl -n hw.physicalcpu_max ||
                       lscpu -p | egrep -v '^#' | sort -u -t, -k 2,4 | wc -l)

num_edges=$(cat $1 | head -n1)
processes=$(($num_edges * 2 + 1))
echo $processes

if [ $processes -gt $physicalCpuCount ]; then
  mpirun -np $processes --oversubscribe bash -c "./sim < $1"
else
  mpirun -np $processes bash -c "./sim < $1"
fi

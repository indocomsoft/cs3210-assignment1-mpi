# OpenMP
```bash
49787E@ubuntu-soctf-pdc-003:~$ time ./sim_thread_short < 64_21-21-22
total trains: 3000
Running simulation with 3000 threads
green: 1000 trains -> 36.9, 135.3, 5.6
yellow: 1000 trains -> 33.4, 148.9, 4.3
blue: 1000 trains -> 23.1, 90.7, 1.3

real	2m7.469s
user	12m52.816s
sys	6m0.956s
```
# OpenMPI
```bash
49787E@ubuntu-soctf-pdc-003:~$ time mpirun -np 65 -rankfile 32rankfile -machinefile machinefile bash -c "./sim_short <64_21-21-22"
green: 1000 trains -> 29.5, 91.5, 9.2
yellow: 1000 trains -> 25.7, 92.0, 4.8
blue: 1000 trains -> 19.0, 74.8, 1.4

real	0m42.253s
user	3m49.364s
sys	9m47.756s
```

# Local OpenMPI
```bash
49787E@ubuntu-soctf-pdc-003:~/cs3210-assignment1-mpi$ time ./run.sh ../64_21-21-22
green: 1000 trains -> 30.1, 95.7, 7.8
yellow: 1000 trains -> 26.0, 100.8, 4.2
blue: 1000 trains -> 19.1, 64.7, 0.8

real	0m2.833s
user	0m25.912s
sys	0m23.536s
```

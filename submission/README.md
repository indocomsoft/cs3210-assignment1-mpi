# CS3210 Assignment 1 Part 2

## How to compile
Make sure you have OpenMPI installed.

Then, just run:

```bash
make
```

### How to run
```
mpirun -np <number of edges + 1> bash -c "./sim < input_file"
```

In case (number of edges + 1) is higher then the physical cpu core count of your machine, add `--oversubscribe` option when running:
```bash
mpirun -np <number of edges + 1> --oversubscribe bash -c "./sim < input_file"
```

Use `machinefile` and `rankfile` just as how you would other MPI executables (using the `-machinefile` and `-rankfile` options on `mpirun`)

### Sample input file
We have included the sample input file in the assignment sheet as `input/sample`

Run it like so:
```bash
mpirun -np 17 --oversubscribe bash -c "./sim < input/sample"
```

Note that if your machine has more than 17 physical cpu core count, you do not need the `--oversubscribe` option.

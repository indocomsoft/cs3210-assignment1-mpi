# cs3210-assignment1-mpi

macOS kernel is weird -- might encounter problem with shared memory allocation.

Workaround: use `OMPI_MCA_btl=self,tcp` environment variable

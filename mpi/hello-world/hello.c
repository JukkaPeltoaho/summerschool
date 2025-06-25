#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {

    // TODO: say hello! in parallel
    MPI_Init(&argc, &argv);
 
    char name[MPI_MAX_PROCESSOR_NAME];
    int size, rank, len;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(name, &len);
  if (rank == 0) {
    printf("Number of MPI processes %d\n", size);
  }

  if (rank == (size-1)) {
    printf("I'm the last but not least: %d\n", rank);
  }

  if (rank == 42) {
    printf("I'm the Answer to the Ultimate Question of Life, the Universe, and Everything! %d\n", rank);
  }

  printf("Hello from rank %d of %d, node %s\n", rank, size, name);

  fflush(stdout);


    MPI_Finalize();
}

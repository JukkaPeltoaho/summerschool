#include <cstdio>
#include <cmath>
#include <mpi.h>

constexpr int n = 840;

int main(int argc, char** argv)
{
   int rank, ntasks;

   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   if (0 == rank) {
      printf("Computing approximation to pi with N=%d\n", n);
      printf("Using %d MPI processes\n", ntasks);
   }

   int chunksize = n / ntasks;
   int istart = rank * chunksize + 1;
   int istop = (rank + 1) * chunksize;

   // Handle possible uneven division
   int remainder = n % ntasks;
   if (remainder > 0) {
       if (rank < remainder) {
          // Assign this task one element more
          istart += rank;
          istop += rank + 1;
       } else {
          istart += remainder;
          istop += remainder;
       }
    }

   double localpi = 0.0;
   for (int i=istart; i <= istop; i++) {
     double x = (i - 0.5) / n;
     localpi += 1.0 / (1.0 + x*x);
   }

   double pi = 0;
   //MPI_Reduce(&localpi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
   MPI_Allreduce(MPI_IN_PLACE, &localpi, 1, MPI_DOUBLE, MPI_SUM,  MPI_COMM_WORLD);
   //if (0 == rank) {
      localpi *= 4.0 / n;
      printf("Approximate pi=%18.16f (exact pi=%10.8f) (rank= %d) \n", localpi, M_PI, rank);
   //}

   MPI_Finalize();
}

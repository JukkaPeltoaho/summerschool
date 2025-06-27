#include <cstdio>
#include <mpi.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    int my_id, omp_rank;
    int provided, required=MPI_THREAD_FUNNELED;

    printf("Thread support MPI_THREAD_FUNNELED :  %d\n", MPI_THREAD_FUNNELED);
    /* TODO: Initialize MPI with thread support. */
    MPI_Init_thread(&argc, &argv, required, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &omp_rank);

    printf("Thread support: %d\n", provided);
    /* TODO: Find out the MPI rank and thread ID of each thread and print
     *       out the results. */

#pragma omp parallel private(my_id)
    {
        my_id = omp_get_thread_num();
        printf("I'm thread %d in process %d\n", my_id, omp_rank);
    }

    /* TODO: Investigate the provided thread support level. */

    MPI_Finalize();
    return 0;
}

#include <cstdio>
#include <mpi.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    int my_id, tid_tag, mpi_rank, mpi_size;
    int provided, required=MPI_THREAD_MULTIPLE;
    int sendbuf, recvbuf;
    printf("Thread support MPI_THREAD_MULTIPLE :  %d\n", MPI_THREAD_MULTIPLE);

    MPI_Init_thread(&argc, &argv, required, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    printf("Thread support: %d\n", provided);

#pragma omp parallel private(my_id, tid_tag, recvbuf)
    {
        my_id = omp_get_thread_num();
        tid_tag = 20000 + my_id;

        MPI_Status status;
        if (mpi_rank == 0 ) {
            for (int i=1; i<mpi_size; i++) {
                MPI_Send(&my_id, 1, MPI_INT, i, tid_tag, MPI_COMM_WORLD);
            }
            printf("I'm thread %d, tag %d in process %d\n", my_id, tid_tag, mpi_rank);
        } else {
            MPI_Recv(&recvbuf, 1, MPI_INT, 0, tid_tag, MPI_COMM_WORLD, &status);
            printf("I'm thread %d, tag %d in process %d. I received %d.\n", my_id, tid_tag, mpi_rank, recvbuf);
        }

    }


    MPI_Finalize();
    return 0;
}

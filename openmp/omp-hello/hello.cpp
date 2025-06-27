#include <cstdio>
#include <omp.h>
#include <mpi.h>

int main()
{
    printf("Hello world!\n");
	int omp_rank, num_threads;
#pragma omp parallel
    {
		omp_rank = omp_get_thread_num();
		num_threads = omp_get_num_threads();
        printf("X: %d\n", omp_rank);
    }
	printf("Threadnum: %d\n", num_threads);
    return 0;
}

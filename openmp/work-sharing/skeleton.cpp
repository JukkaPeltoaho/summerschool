#include <cstdio>
#include <omp.h>

#define NX 102400

int main(void)
{
    double vecA[NX], vecB[NX], vecC[NX];

    int omp_rank;
    /* Initialization of the vectors */
    for (int i = 0; i < NX; i++) {
        vecA[i] = 1.0 / ((double)(NX - i));
        vecB[i] = vecA[i] * vecA[i];
    }

    /* TODO:
     *   Implement here a parallelized version of vector addition,
     *   vecC = vecA + vecB
     */

    #pragma omp parallel
    {
    #ifdef _OPENMP
        omp_rank = omp_get_num_threads();
    #else
        omp_rank = 0;
    #endif
    }

    printf("Hello world! by thread %d\n",
           omp_rank);


    #pragma omp parallel for
    {
        for (int i=0; i < NX; i++)
            vecC[i] = vecA[i] + vecB[i];
    }


    double sum = 0.0;
    /* Compute the check value */
    for (int i = 0; i < NX; i++) {
        sum += vecC[i];
    }
    printf("Reduction sum: %18.16f\n", sum);

    return 0;
}

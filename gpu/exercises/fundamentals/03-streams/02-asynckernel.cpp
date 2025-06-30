
/*
 * This code uses default stream
 * Task:
 * - Place kernel_{a,b,c} in separate streams and execute them asynchronously
 * - Validate that concurrency with `srun ... rocprof --hip-trace ./02-asynckernel.cpp`
 *   - Open chromium url chrome://tracing or https://ui.perfetto.dev, open file "results.json"
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../error_checking.hpp"

#include "helperfuns.h"

int main() {
  constexpr size_t N = 1<<10; // 1024 items

  constexpr int blocksize = 256;
  constexpr int gridsize =(N-1+blocksize)/blocksize;
  constexpr size_t N_bytes = N*sizeof(float);

  // Host & device pointers
  float *a; float *d_a;
  float *b; float *d_b;
  float *c; float *d_c;

  // Host allocations
  a = (float*) malloc(N_bytes);
  b = (float*) malloc(N_bytes);
  c = (float*) malloc(N_bytes);

  hipStream_t stream_a;
  hipStream_t stream_b;
  hipStream_t stream_c;
  HIP_ERRCHK(hipStreamCreate(&stream_a));
  HIP_ERRCHK(hipStreamCreate(&stream_b));
  HIP_ERRCHK(hipStreamCreate(&stream_c));

  // Device allocations
  HIP_ERRCHK(hipMallocAsync((void**)&d_a, N_bytes, stream_a));
  HIP_ERRCHK(hipMallocAsync((void**)&d_b, N_bytes, stream_b));
  HIP_ERRCHK(hipMallocAsync((void**)&d_c, N_bytes, stream_c));

  // warmup
  kernel_c<<<gridsize, blocksize>>>(d_a, N);
  HIP_ERRCHK(hipMemcpy(a, d_a, N_bytes/100, hipMemcpyDefault));
  HIP_ERRCHK(hipDeviceSynchronize());

  // Execute kernels in sequence
  kernel_a<<<gridsize, blocksize,0,stream_a>>>(d_a, N);
  HIP_ERRCHK(hipGetLastError());

  kernel_b<<<gridsize, blocksize,0,stream_b>>>(d_b, N);
  HIP_ERRCHK(hipGetLastError());

  kernel_c<<<gridsize, blocksize,0,stream_c>>>(d_c, N);
  HIP_ERRCHK(hipGetLastError());

  // Copy results back
  HIP_ERRCHK(hipMemcpyAsync(a, d_a, N_bytes, hipMemcpyDefault, stream_a));
  HIP_ERRCHK(hipMemcpyAsync(b, d_b, N_bytes, hipMemcpyDefault, stream_b));
  HIP_ERRCHK(hipMemcpy(c, d_c, N_bytes, hipMemcpyDefault));
  // Viimeinen kutsu ei async == erillinen syncronize kutsu

  for (int i = 0; i < 20; ++i) printf("%f ", a[i]);
  printf("\n");

  for (int i = 0; i < 20; ++i) printf("%f ", b[i]);
  printf("\n");

  for (int i = 0; i < 20; ++i) printf("%f ", c[i]);
  printf("\n");
  // Free device and host memory allocations
  HIP_ERRCHK(hipFree(d_a));
  HIP_ERRCHK(hipFree(d_b));
  HIP_ERRCHK(hipFree(d_c));
  free(a);
  free(b);
  free(c);
  HIP_ERRCHK(hipStreamDestroy(stream_a));
  HIP_ERRCHK(hipStreamDestroy(stream_b));
  HIP_ERRCHK(hipStreamDestroy(stream_c));
}

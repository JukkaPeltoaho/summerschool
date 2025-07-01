#include <iostream>
#include <sycl/sycl.hpp>
using namespace sycl;

int main() {
  // Set up queue on any available device
  queue q;

  // Initialize input and output memory on the host
  constexpr size_t N = 25600;
  std::vector<int> x(N),y(N);
  int a=4;
  std::fill(x.begin(), x.end(), 1);
  std::fill(y.begin(), y.end(), 2);

  {
    // Create buffers for the host data or allocate memory usinggUSM
    // If USM + malloc_device() is used add the copy operations
    sycl::buffer<int, 1> x_buf(x.data(), range<1>(N));
    sycl::buffer<int, 1> y_buf(y.data(), range<1>(N));
    // Submit the kernel to the queue
    q.submit([&](handler& cgh) {
      sycl::accessor y_acc{y_buf, cgh, sycl::read_write};
      sycl::accessor x_acc{x_buf, cgh, sycl::read_only};

      cgh.parallel_for(sycl::nd_range<1>(sycl::range<1>(((N+local_size-1)/local_size)*local_size), sycl::range<1>(local_size)), [=](sycl::nd_item<1> item) {
        auto idx=item.get_global_id(0);
        if(idx<N){ //to avoid out of bounds access
           y_acc[idx] = y_acc[idx] + a*x_acc[idx];
        }
      });
    });

    //Checking the result inside the scope of the buffers using host_accessors
    // Use host_accessor to read back the results from Ybuff
    {
      host_accessor h_accY(y_buf, sycl::read_only); // Read back data after kernel execution
      std::cout << "First few elements of Y after operation:" << std::endl;
      for (size_t i = 0; i < 10; ++i) {
        std::cout << "Y[" << i << "] = " << h_accY[i] << std::endl;
      }
    }
  }
  // If USM + malloc_device() is used add the copy operations
  // TODO
  // Check that all outputs match expected value

  // If USM is used free the device memory
  // TODO
  // Check that all outputs match expected value
  bool passed = std::all_of(y.begin(), y.end(),
                            [a](int val) { return val == a * 1 + 2; });
  std::cout << ((passed) ? "SUCCESS" : "FAILURE")
            << std::endl;
  return (passed) ? 0 : 1;
}

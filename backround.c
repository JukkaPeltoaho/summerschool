#include <stdio.h>
#include <stdlib.h>

// Function definition
double calculate_sum(int n, const double *array)
{
    // Declare a variable
    double sum = 0.0;

    // Calculate sum by looping over array
    for (int i = 0; i < n; ++i) {
        sum += array[i];
    }

    // Return sum
    return sum;
}

// Main function
int main(int argc, char *argv[])
{
    // Declare variables
    int n = 4;
    double sum;

    // Print
    printf("Hello n=%d\n", n);

    // Declare a fixed-length array
    double a[4] = {1.1, 2.2, 3.3, 4.4};

    // Call a function
    sum = calculate_sum(4, a);
    printf("Sum of a is %f\n", sum);

    // Control statement
    if (sum > 10) {
        printf("Sum is large\n");
    } else {
        printf("Sum is small\n");
    }

    // Allocate a dynamic array
    double *b = (double*)malloc(sizeof(double) * n);

    // Set the array values by looping over array
    for (int i = 0; i < n; ++i) {
        b[i] = 1.1 * i;
    }

    // Call a function
    sum = calculate_sum(n, b);
    printf("Sum of b is %f\n", sum);

    // Free memory allocation
    free(b);

    return 0;
}
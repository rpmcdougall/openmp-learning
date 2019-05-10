#include <stdio.h>
#include <omp.h>

/*
 * Hello World
 */
//int main() {
//    #pragma omp parallel
//    {
//        int ID = omp_get_thread_num();
//        printf("hello(%d)", ID);
//        printf("world(%d) \n", ID);
//    }
//
//    return 0;
//}

/*
 * Threading (Fork Join), Pi SPMD Strategy, with Synchronization
 */
static long num_steps = 100000;
double step;
#define NUM_THREADS 2
void main() {
    double pi = 0.0;
    step = 1.0/(double) num_steps;

    omp_set_num_threads(NUM_THREADS);
    double start_time = omp_get_wtime();
    #pragma omp parallel
    {
        int i, id, nthrds, nthreads;
        double x, sum;
        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();
        if (id == 0) nthreads = nthrds;
        for (i=0, sum = 0.0; i<num_steps; i=i+nthreads) {
            x = (i + 0.5) * step;
            sum += 4.0/(1.0 + x*x);
        }
        // Only one thread summing at a time into pi
        #pragma omp critical
            pi += sum * step;
    };

    double end_time = omp_get_wtime();
    double elapsed = end_time - start_time;

    printf("Result: %lf; " , pi);
    printf("Elapsed Parallel: %lf", elapsed);
}
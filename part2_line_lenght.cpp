#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <omp.h>
#include <stdlib.h>
#include <malloc.h>

/* assembly code to read the TSC */
static inline unsigned long long RDTSC()
{
  unsigned int hi, lo;
  __asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
  return ((unsigned long long)hi << 32) | lo;
}


unsigned long long a1,a2;

long long num_steps = 1000000000;
int cpu_count = 4;

int main(int argc, char* argv[])
{
    struct timespec start, stop;
    double start_time, stop_time;
    int repeats=0;
start:
    start_time = omp_get_wtime();
    clock_gettime(CLOCK_MONOTONIC, &start);

    double pi, sum=0.0;
    double step;
    long i;
    step = 1./(double)num_steps;
    double partial_sum[cpu_count];
//    double partial_sum[]

//    a1 = RDTSC();
//    omp_set_num_threads(cpu_count);

#pragma omp parallel shared(sum, num_steps, step, partial_sum)
    {
//        cpu_set_t cpu;
//        CPU_ZERO(&cpu);
//        CPU_SET(omp_get_thread_num(), &cpu);
//        double psum=0;

//        pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);

        int sum_index = omp_get_thread_num();
//        printf("%d \n", sched_getcpu());
        double x;

#pragma omp for
        for (i=0; i<num_steps; i++)
        {
            x = (i + .5)*step;
//            psum += 4.0/(1.+ x*x);
            partial_sum[sum_index] += 4.0/(1.+ x*x);
        }
#pragma omp critical
//        sum += psum;
        sum += partial_sum[sum_index];


    }
//    sum = suml[0] + suml[1];

    pi = sum*step;
    printf("Wartosc liczby PI wynosi %15.12f\n",pi);

//    a2 = RDTSC();
    clock_gettime(CLOCK_MONOTONIC, &stop);;
    stop_time = omp_get_wtime();

//    printf("Ticks: %ld\n", a2-a1);
    printf("Czas przetwarzania wynosi %f sekund albo %f sekund\n",((double)(stop.tv_nsec - start.tv_nsec)/1000000000.0)+(stop.tv_sec - start.tv_sec), stop_time-start_time);
    if (repeats > 0){
        --repeats;
        goto start;
    }
    return 0;
}

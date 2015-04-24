#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <omp.h>
#include <stdlib.h>
#include <malloc.h>

long long num_steps = 1000000000;
int cpu_count = 2;

int main(int argc, char* argv[])
{
    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    double pi, sum=0.0;
    double step;
    long i;
    step = 1./(double)num_steps;

    omp_set_num_threads(cpu_count);

//    double suml[cpu_count];
//    volatile double *suml = (volatile double*)malloc(100000);

    volatile double *suml;
    posix_memalign((void**)&suml, 64, 4*64);

#pragma omp parallel shared(suml)
    {
        cpu_set_t cpu;
        CPU_ZERO(&cpu);
        CPU_SET(omp_get_thread_num(), &cpu);

        pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);

        printf("%d \n", sched_getcpu());
        double x;
//        int nCpu =  omp_get_thread_num();
        int nCpu =  omp_get_thread_num() * (64/sizeof(double));
        suml[nCpu] = 0;
#pragma omp for
        for (i=0; i<num_steps; i++)
        {
            x = (i + .5)*step;
            suml[nCpu] = suml[nCpu] + 4.0/(1.+ x*x);
        }


#pragma omp atomic
        sum +=  suml[nCpu];

    }
//    sum = suml[0] + suml[1];

    pi = sum*step;
    printf("Wartosc liczby PI wynosi %15.12f\n",pi);

    clock_gettime(CLOCK_MONOTONIC, &stop);;


    printf("Czas przetwarzania wynosi %f sekund\n",((double)(stop.tv_nsec - start.tv_nsec)/1000000000.0)+(stop.tv_sec - start.tv_sec));
    return 0;
}

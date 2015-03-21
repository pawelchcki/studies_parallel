#include <iostream>
#include<stdio.h>
#include<omp.h>
#include <pthread.h>
using namespace std;

int main()
{
    int i;
    omp_set_num_threads(6);
    omp_lock_t lock;
    omp_unset_lock(&lock);
#pragma omp parallel
    {
        int th_id = omp_get_thread_num();
        cpu_set_t cpu;
        CPU_ZERO(&cpu);
        CPU_SET(th_id, &cpu);


        pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
//        omp_set_lock(&lock);
//        cout << "Thread number: " << omp_get_thread_num() << "\n";
//        cout.flush();
//        omp_unset_lock(&lock);
//    printf("Hello World\n");
#pragma omp for schedule(static, 1)
    for(i=0;i<6;i++){
        printf("Iter:%d %d\n",i, omp_get_thread_num());
//    omp_unset_lock(&lock);
    }
    printf("GoodBye World\n");
    }
    while (omp_get_thread_num() != 0){
    }
    return 0;
}


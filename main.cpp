#include <iostream>
#include<stdio.h>
#include<omp.h>

int main()
{
    int i;
    omp_set_num_threads(4);
#pragma omp parallel
    {
//    printf("Hello World\n");
    for(i=0;i<6;i++){
        printf("Iter:%d\n",i);
    }
    printf("GoodBye World\n");
    }
    return 0;
}


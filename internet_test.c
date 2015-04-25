/* $Id: false-sharing.c 14 2011-10-28 09:06:28Z goulas@gmail.com $
 *
 * Copyright 2011 George Goulas
 * Available from http://code.google.com/p/goulas-openmp-demos
 *
 * This file is part of goulas-openmp-demos.
 *
 * goulas-openmp-demos is free software: you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 3 of the 
 * License, or (at your option) any later version.
 *
 * goulas-openmp-demos is distributed in the hope that it will be 
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty 
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with goulas-openmp-demos.  
 * If not, see <http://www.gnu.org/licenses/>.
 * 
 */
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define SIZE 10000000
#define MAXEL 100
#define SAFE_MALLOC(v,t,n) if((v=(t*)malloc( (n)*sizeof(t)))==NULL) return EXIT_FAILURE;
#define SAFE_EXEC( f ) if( f==EXIT_FAILURE) return EXIT_FAILURE;
#define MAX_THREADS 8
int *a;

int allocate_vectors( void )
{
	SAFE_MALLOC(a, int, SIZE);
	return EXIT_SUCCESS;
}

void random_init( void )
{
	srand( time(NULL) );
	for( int i=0; i<SIZE; i++ ) {
		a[i] = rand() % MAXEL;
	}	
}

int main( int argc, char* argv[] )
{
	int i;
	int serial_sum;
	double start_time, end_time;

	printf("\nParallel sum\n\nSize %d\n", SIZE);

	SAFE_EXEC( allocate_vectors() );		
	random_init();
	
	// Serial alternative
	start_time = omp_get_wtime();
	serial_sum = 0;
	for( i=0; i<SIZE; i++ ) {
		serial_sum += a[i];
	}
	end_time = omp_get_wtime();
	printf("Serial run time: %f msec\n", (end_time-start_time)*1000);


	// Parallel alternative, false sharing
	int* partial_sum;
	SAFE_MALLOC( partial_sum, int, MAX_THREADS );
	memset( partial_sum, 0, MAX_THREADS*sizeof(int) );
	
	start_time = omp_get_wtime();
	#pragma omp parallel shared(a, partial_sum) private(i)
	{
		int nthreads = omp_get_num_threads();
		int id = omp_get_thread_num();
		for( i=id*SIZE/nthreads; i<(id+1)*SIZE/nthreads; i++) {
			partial_sum[id] += a[i];
		}
	}
	for(i=1;i<MAX_THREADS;i++)
		partial_sum[0] += partial_sum[i];
	end_time = omp_get_wtime();
	printf("Parallel run time, false sharing: %f msec\n", (end_time-start_time)*1000);

	printf("Comparing results: ");
	if( partial_sum[0] != serial_sum )
		printf("FAILURE\n");
	else
		printf("SUCCESS\n");



	// Parallel alternative, no false sharing
	start_time = omp_get_wtime();
	int sum=0;
	#pragma omp parallel shared(a, sum) private(i)
	{
		int nthreads = omp_get_num_threads();
		int id = omp_get_thread_num();
		int partial_sum;
		for( i=id*SIZE/nthreads; i<(id+1)*SIZE/nthreads; i++) {
			partial_sum += a[i];
		}
		#pragma omp critical
		sum += partial_sum;
	}
	end_time = omp_get_wtime();
	printf("Parallel run time, no false sharing: %f msec\n", (end_time-start_time)*1000);

	printf("Comparing results: ");
	if( partial_sum[0] != serial_sum )
		printf("FAILURE\n");
	else
		printf("SUCCESS\n");




	free(a);
	return EXIT_SUCCESS;
}

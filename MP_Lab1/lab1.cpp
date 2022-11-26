#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace std;

void writeFile(int thread_num, double value, std::ofstream *out) {
    
    (*out) << thread_num << ";" << value << std::endl;
    
}

void getTime(int threads_num, int count, int *array, int max, std::ofstream *out) {
    int64_t n = 10;
    double begin, end, sum = 0; 
    for (int j = 0; j < n; j++) {
        double begin, end; 
        begin = omp_get_wtime();

        #pragma omp parallel num_threads(threads_num) shared(array, count) reduction(max: max) 
        {   
            #pragma omp for
            for (int i = 0; i < count; i++) {
                for(int j=0; j<30; j++);
                if (array[i] > max) { 
                    max = array[i]; 
                }
            }
        }

        end = omp_get_wtime();
        double elapsed_time = end - begin;

        sum += elapsed_time;
    }
    
    writeFile(threads_num, sum/n, out);
}

void getCompares(int threads_num, int count, int *array, int max, std::ofstream *out) {
    int64_t compares = 0, n = 50;
    for (int j = 0; j < n; j++) {

        #pragma omp parallel num_threads(threads_num) shared(array, count, compares) reduction(max: max) 
        {   
            #pragma omp for
            for (int i = 0; i < count; i++) {
                compares++;
                if (array[i] > max) { 
                    max = array[i]; 
                }
            }
        }
    }
    
    writeFile(threads_num, compares/n, out);
}

void generateArray(int count, int *array) {
    for (int i = 0; i < count; i++) { 
        array[i] = rand(); 
    }
}

int main(int argc, char** argv) {
    const int count = 10000000;     ///< Number of array elements
    const int random_seed = 920215; ///< RNG seed

    int* array = 0;                 ///< The array we need to find the max in
    int  max   = -1;                ///< The maximal element
    /* Initialize the RNG */
    srand(random_seed);

    std::ofstream points_out("./points.txt"), compares_out("./compares.txt");
    if (!points_out.is_open())
        return 1;

    if (!compares_out.is_open())
        return 1;

    /* Generate the random array */
    array = (int*)malloc(count*sizeof(int));

    /* Find the maximal element */
    
    for (int i = 12; i > 0; i--) {
        generateArray(count, array);
        getTime(i, count, array, max, &points_out);
        //getCompares(i, count, array, max, &compares_out);
    }

    points_out.close();
    compares_out.close();
    
    return(0);
}   
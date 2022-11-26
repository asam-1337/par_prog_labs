#include <iostream>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <fstream>

void writeFile(int thread_num, double value, std::ofstream& out) {
    
    out << thread_num << ";" << value << std::endl;
    
}

void insertionSort(int a[], int n, int stride) {
    for (int j = stride; j < n; j += stride) {
        int key = a[j];
        int i = j - stride;
        while (i >= 0 && a[i] > key) {
            a[i + stride] = a[i];
            i -= stride;
        }
        a[i + stride] = key;
    }
}

void shellSort(int a[], int n, int threads)
{
    int i, m;

    for(m = n/2; m > 0; m /= 2)
    {
            #pragma omp parallel for num_threads(threads) shared(a, m, n) private(i) default(none)
            for(i = 0; i < m; i++)
                insertionSort(&(a[i]), n-i, m);
    }
}

int main(int argc, char** argv)
{
    int n = omp_get_num_procs();

    std::cout << n << std::endl;
    printf("%d \n", n);
    
    int count = 10000000; // Number of array elements
    const int threads = 12; // Number of parallel threads to use
    const int random_seed = 64; // RNG seed
    int** array = 0; // The array we need to find the max in
    srand(random_seed);
    
    // printf("OpenMP: %d;\n======\n", _OPENMP);
    
    std::ofstream out("./points.txt");
    if (!out.is_open()) {
        return 1;
    }

    array = (int**)malloc(threads * sizeof(int*));

    for (int i = 0; i < threads; i++) {
        array[i] = (int *) malloc((count) * sizeof(int));

        for (int j = 0; j < count; j++) {
            array[i][j] = rand();
        }
    }

    double start, finish;
    int *array2;
    
    for(int th = 0; th < threads; th++) {
        start = omp_get_wtime();

        array2 = array[th];

        for (int i = 0; i < 10; i++)
            shellSort(array2, count, th+1);

        finish = omp_get_wtime();
        writeFile(th, (finish - start) / 50, out);
    }

    out.close();
    return 0;
}
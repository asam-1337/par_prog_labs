#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <omp.h>
#include <fstream>

#define OMP_CANCELLATION 1

void writeFile(int thread_num, double value, std::ofstream &out) {
    
    out << thread_num << ";" << value << std::endl;
    
}

int main(int argc, char** argv)
{
    int n = omp_get_num_procs();

    printf("%d \n", n);
    
    const int count = 200000; ///< Number of array elements
    const int threads = 12; ///< Number of parallel threads to use
    const int random_seed = 64; ///< RNG seed
    const int rep = 10000;
    int target = 0;
    int index = -1;
    const int u = 5;
    int** array = 0; ///< The array we need to find the max in
    srand(time(NULL));
    
    // printf("OpenMP: %d;\n======\n", _OPENMP);
    
    std::ofstream out("./points.txt");
    if (!out.is_open())
        return 1;

    array = (int**)malloc(rep*sizeof(int*));

    for (int i = 0; i < rep; i++) {
        array[i] = (int *) malloc((count+1) * sizeof(int));
        for (int j = 0; j < count; j++) {
            array[i][j] = rand();
        }
        array[i][count] = rand() % count;
    }

    double start, finish;
    int *array2;
    
    for(int r = 1; r <= threads; r++) {
        start = omp_get_wtime();
        for (int t = 0; t < rep*u; t++) {
            array2 = array[t/u];
            target = array2[count];

            //printf("%d \n", target);

            #pragma omp parallel num_threads(r) shared(array2, count, target, index) default(none)
            {
                #pragma omp for
                for (int i = 0; i < count; i++) {
                    if (array2[i] == target) {
                        index = i;
                        #pragma omp cancel for
                    };
                }
            }

        //printf("%d %d \n", target, array2[index]);
        }

        finish = omp_get_wtime();
        printf("%d %lf\n", r, (finish - start) * 1000000 / rep);

        writeFile(r, (finish - start) * 1000000 / rep, out);
    }

    out.close();

    return 0;
}
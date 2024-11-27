#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

// Función optimizada de multiplicación de matrices con bloques
void ProductMat_Optimized(int n, int cache_size, double* A, double* B, double* C) {
    const int ELEMENT_S = sizeof(double);  // Tamaño de un elemento en bytes
    int BLOCK = cache_size / ELEMENT_S;  // Tamaño del bloque en elementos

    // Ajustar el tamaño del bloque para no exceder el tamaño de la matriz
    if (BLOCK > n) {
        BLOCK = n;
    }

    int i, j, k, jj, kk;

    // Multiplicación por bloques
    for (jj = 0; jj < n; jj += BLOCK) {
        for (kk = 0; kk < n; kk += BLOCK) {
            for (i = 0; i < n; i++) {  // Itera sobre las filas de A
                for (k = kk; k < kk + BLOCK && k < n; k++) {  // Bloque columna de A
                    double r = A[i * n + k];  // A[i][k]
                    for (j = jj; j < jj + BLOCK && j < n; j++) {  // Bloque fila de B
                        C[i * n + j] += r * B[k * n + j];  // C[i][j] += A[i][k] * B[k][j]
                    }
                }
            }
        }
    }
}

// Función para imprimir una matriz
void printMatrix(int n, double* M) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%8.2f ", M[i * n + j]);
        }
        printf("\n");
    }
    printf("\n");
}

FILE* fp;

int main(int argc, char* argv[]) {
    errno_t err;
    err = fopen_s(&fp, "ReportOptimized.txt", "a+");
    if (err != 0) {
        printf("The file 'ReportOptimized.txt' was not opened\n");
        return 0;
    }

    if (argc > 2) {
        int n = atoi(argv[1]);  
        int samples = atoi(argv[2]);  
        int cache_size = atoi(argv[3]); 

        double* A = (double*)malloc(n * n * sizeof(double));
        double* B = (double*)malloc(n * n * sizeof(double));
        double* C = (double*)malloc(n * n * sizeof(double));

        for (int i = 0; i < n * n; i++) A[i] = 3.0;
        for (int i = 0; i < n * n; i++) B[i] = 4.0;

        printf("ver\ttypeData\tISA\t#sample\tn\ttime(s)\tNormalized(ns)\n");

        for (int s = 0; s < samples; s++) {
            for (int i = 0; i < n * n; i++) C[i] = 0.0;

            clock_t start = clock();  // Inicia medición de tiempo

            ProductMat_Optimized(n, cache_size, A, B, C);

            clock_t end = clock();  // Termina medición de tiempo

            double seconds = (double)(end - start) / CLOCKS_PER_SEC;
            double N = n;
            double timeNormalized = (seconds * 1.0e9) / (N * N * N);  // ns

            printf("%s\t%03d\t%05d\t%2.4f\t%2.4f \n", "blocks\t d\t x64\t", s, n, seconds, timeNormalized);        
            fprintf_s(fp, "%s\t%05d\t%05d\t%2.4f\t %2.4f \n ", "blocks\t d\tx64\t", s, n, seconds, timeNormalized); 

        }

        if (argc > 4) {
            printMatrix(n, A);
            printMatrix(n, B);
            printMatrix(n, C);
        }

        free(A);
        free(B);
        free(C);
        fclose(fp);
    }
    else {
        printf("Usage: %s <matrix_size> <samples> [print_matrices]\n", argv[0]);
    }

    return 0;
}

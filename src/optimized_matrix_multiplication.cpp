#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstdio>

// Función optimizada de multiplicación de matrices con bloques
void ProductMat_Optimized(int n, int cacheBlockSize, double* A, double* B, double* C) {
    const int ELEMENT_SIZE = sizeof(double);  
    int BLOCK_SIZE = cacheBlockSize / ELEMENT_SIZE;  

    
    if (BLOCK_SIZE > n) {
        BLOCK_SIZE = n;
    }

    int i, j, k, jj, kk;

    
    for (jj = 0; jj < n; jj += BLOCK_SIZE) {
        for (kk = 0; kk < n; kk += BLOCK_SIZE) {
            for (i = 0; i < n; i++) {  // Itera sobre las filas de A
                for (k = kk; k < kk + BLOCK_SIZE && k < n; k++) {  // Bloque columna de A
                    double r = A[i * n + k];  // A[i][k]
                    for (j = jj; j < jj + BLOCK_SIZE && j < n; j++) {  // Bloque fila de B
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

int main(int argc, char* argv[]) {
    std::ofstream fp("ReportOptimized.txt", std::ios::app);
    if (!fp.is_open()) {
        std::cerr << "The file 'ReportOptimized.txt' was not opened\n";
        return 0;
    }

    if (argc > 2) {
        int n = std::atoi(argv[1]);  // Tamaño de la matriz
        int samples = std::atoi(argv[2]);  // Número de muestras
        int cacheBlockSize = std::atoi(argv[3]); // Tamaño de la cache

        double* A = new double[n * n];
        double* B = new double[n * n];
        double* C = new double[n * n];

        for (int i = 0; i < n * n; i++) A[i] = 3.0;
        for (int i = 0; i < n * n; i++) B[i] = 4.0;

        std::cout << "ver\ttypeData\tISA\t#sample\tn\ttime(s)\tNormalized(ns)\n";

        for (int s = 0; s < samples; s++) {
            for (int i = 0; i < n * n; i++) C[i] = 0.0;

            clock_t start = clock();  // Inicia medición de tiempo

            ProductMat_Optimized(n, cacheBlockSize, A, B, C);

            clock_t end = clock();  // Termina medición de tiempo

            double seconds = (double)(end - start) / CLOCKS_PER_SEC;
            double N = n;
            double timeNormalized = (seconds * 1.0e9) / (N * N * N);  // ns

            printf("%s\t%03d\t%05d\t%2.4f\t%2.4f \n", "blocks\t d\t x64\t", s, n, seconds, timeNormalized);        
            fp << "blocks\t d\tx64\t" << s << "\t" << n << "\t" << seconds << "\t" << timeNormalized << "\n";

        }

        if (argc > 4) {
            printMatrix(n, A);
            printMatrix(n, B);
            printMatrix(n, C);
        }

        delete[] A;
        delete[] B;
        delete[] C;
    } else {
        std::cout << "Usage: " << argv[0] << " <matrix_size> <samples> [print_matrices]\n";
    }

    return 0;
}

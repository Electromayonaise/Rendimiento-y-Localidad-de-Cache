#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstdio>

// Función optimizada de multiplicación de matrices con bloques
void ProductMat_Optimized(int n, int cacheBlockSize, double* A, double* B, double* C) {
    const int ELEMENT_SIZE = sizeof(double);  
    int bloqueSize = cacheBlockSize / ELEMENT_SIZE;  

    
    if (bloqueSize > n) {
        bloqueSize = n;
    }

    int fila, columna, bloqueFila, bloqueColumna;

    
    for (bloqueColumna = 0; bloqueColumna < n; bloqueColumna += bloqueSize) {
        for (bloqueFila = 0; bloqueFila < n; bloqueFila += bloqueSize) {
            for (fila = 0; fila < n; fila++) {  // Itera sobre las filas de A
                for (columna = bloqueFila; columna < bloqueFila + bloqueSize && columna < n; columna++) {  // Bloque columna de A
                    double resultado = A[fila * n + columna];  // A[fila][columna]
                    for (int k = bloqueColumna; k < bloqueColumna + bloqueSize && k < n; k++) {  // Bloque fila de B
                        C[fila * n + k] += resultado * B[columna * n + k];  // C[fila][k] += A[fila][columna] * B[columna][k]
                    }
                }
            }
        }
    }
}

// Función para imprimir una matriz
void printMatrix(int n, double* M) {
    for (int fila = 0; fila < n; fila++) {
        for (int columna = 0; columna < n; columna++) {
            printf("%8.2f ", M[fila * n + columna]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    std::ofstream archivoSalida("ReportOptimized.txt", std::ios::app);
    if (!archivoSalida.is_open()) {
        std::cerr << "The file 'ReportOptimized.txt' was not opened\n";
        return 0;
    }

    if (argc > 2) {
        int n = std::atoi(argv[1]);  // Tamaño de la matriz
        int muestras = std::atoi(argv[2]);  // Número de muestras
        int tamanoCache = std::atoi(argv[3]); // Tamaño de la cache

        double* A = new double[n * n];
        double* B = new double[n * n];
        double* C = new double[n * n];

        for (int i = 0; i < n * n; i++) A[i] = 3.0;
        for (int i = 0; i < n * n; i++) B[i] = 4.0;

        std::cout << "ver\ttypeData\tISA\t#sample\tn\ttime(s)\tNormalized(ns)\n";

        for (int s = 0; s < muestras; s++) {
            for (int i = 0; i < n * n; i++) C[i] = 0.0;

            clock_t inicio = clock();  // Inicia medición de tiempo

            ProductMat_Optimized(n, tamanoCache, A, B, C);

            clock_t fin = clock();  // Termina medición de tiempo

            double segundos = (double)(fin - inicio) / CLOCKS_PER_SEC;
            double N = n;
            double tiempoNormalizado = (segundos * 1.0e9) / (N * N * N);  // ns

            printf("%s\t%03d\t%05d\t%2.4f\t%2.4f \n", "blocks\t d\t x64\t", s, n, segundos, tiempoNormalizado);        
            archivoSalida << "blocks\t d\tx64\t" << s << "\t" << n << "\t" << segundos << "\t" << tiempoNormalizado << "\n";

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

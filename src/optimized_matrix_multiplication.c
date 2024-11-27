#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Configuración mediante macros
#define ELEMENT_SIZE sizeof(double)

// Función optimizada de multiplicación de matrices con bloques
void optimized_matrix_multiplication(int matrix_size, int cache_size, double* matrix_a, double* matrix_b, double* matrix_c) {
    int block_size = cache_size / ELEMENT_SIZE;

    // Ajustar el tamaño del bloque para no exceder el tamaño de la matriz
    if (block_size > matrix_size) {
        block_size = matrix_size;
    }

    for (int block_row = 0; block_row < matrix_size; block_row += block_size) {
        for (int block_col = 0; block_col < matrix_size; block_col += block_size) {
            for (int row = 0; row < matrix_size; row++) { 
                for (int inner = block_col; inner < block_col + block_size && inner < matrix_size; inner++) { 
                    double temp = matrix_a[row * matrix_size + inner]; 
                    for (int col = block_row; col < block_row + block_size && col < matrix_size; col++) { 
                        matrix_c[row * matrix_size + col] += temp * matrix_b[inner * matrix_size + col];
                    }
                }
            }
        }
    }
}

// Inicialización de una matriz
void initialize_matrix(int matrix_size, double* matrix, double value) {
    for (int i = 0; i < matrix_size * matrix_size; i++) {
        matrix[i] = value;
    }
}

// Imprimir una matriz
void display_matrix(int matrix_size, double* matrix) {
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            printf("%8.2f ", matrix[i * matrix_size + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    FILE* report_file = fopen("Optimized_Report.txt", "a+");
    if (!report_file) {
        perror("No se pudo abrir el archivo de reporte");
        return EXIT_FAILURE;
    }

    if (argc > 3) {
        int matrix_size = atoi(argv[1]);
        int sample_count = atoi(argv[2]);
        int cache_size = atoi(argv[3]);

        double* matrix_a = malloc(matrix_size * matrix_size * ELEMENT_SIZE);
        double* matrix_b = malloc(matrix_size * matrix_size * ELEMENT_SIZE);
        double* matrix_c = malloc(matrix_size * matrix_size * ELEMENT_SIZE);

        initialize_matrix(matrix_size, matrix_a, 3.0);
        initialize_matrix(matrix_size, matrix_b, 4.0);

        printf("Versión\tTamaño\tMuestra\tTiempo(s)\tTiempo(ns)\n");

        for (int sample = 0; sample_count > sample; sample++) {
            initialize_matrix(matrix_size, matrix_c, 0.0);

            clock_t start_time = clock();
            optimized_matrix_multiplication(matrix_size, cache_size, matrix_a, matrix_b, matrix_c);
            clock_t end_time = clock();

            double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
            double size = matrix_size;
            double normalized_time = (elapsed_time * 1.0e9) / (size * size * size);  // ns

            printf("Bloques\t%d\t%d\t%.6f\t%.4f\n", matrix_size, sample, elapsed_time, normalized_time);
            fprintf(report_file, "Bloques\t%d\t%d\t%.6f\t%.4f\n", matrix_size, sample, elapsed_time, normalized_time);
        }

        if (argc > 4) {
            display_matrix(matrix_size, matrix_a);
            display_matrix(matrix_size, matrix_b);
            display_matrix(matrix_size, matrix_c);
        }

        free(matrix_a);
        free(matrix_b);
        free(matrix_c);
    } else {
        printf("Uso: %s <tamaño_matriz> <muestras> <tamaño_cache> [imprimir_matrices]\n", argv[0]);
    }

    fclose(report_file);
    return EXIT_SUCCESS;
}

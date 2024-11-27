#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <iomanip>

// Configuración mediante constantes
constexpr size_t ELEMENT_SIZE = sizeof(double);

// Función optimizada de multiplicación de matrices con bloques
void optimized_matrix_multiplication(int matrix_size, int cache_size, 
                                     const std::vector<double>& matrix_a, 
                                     const std::vector<double>& matrix_b, 
                                     std::vector<double>& matrix_c) {
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
void initialize_matrix(int matrix_size, std::vector<double>& matrix, double value) {
    std::fill(matrix.begin(), matrix.end(), value);
}

// Imprimir una matriz
void display_matrix(int matrix_size, const std::vector<double>& matrix) {
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            std::cout << std::setw(8) << std::fixed << std::setprecision(2) 
                      << matrix[i * matrix_size + j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

int main(int argc, char* argv[]) {
    std::ofstream report_file("Optimized_Report.txt", std::ios::app);
    if (!report_file.is_open()) {
        std::cerr << "No se pudo abrir el archivo de reporte\n";
        return EXIT_FAILURE;
    }

    if (argc > 3) {
        int matrix_size = std::stoi(argv[1]);
        int sample_count = std::stoi(argv[2]);
        int cache_size = std::stoi(argv[3]);

        std::vector<double> matrix_a(matrix_size * matrix_size);
        std::vector<double> matrix_b(matrix_size * matrix_size);
        std::vector<double> matrix_c(matrix_size * matrix_size);

        initialize_matrix(matrix_size, matrix_a, 3.0);
        initialize_matrix(matrix_size, matrix_b, 4.0);

        std::cout << "Versión\tTamaño\tMuestra\tTiempo(s)\tTiempo(ns)\n";

        for (int sample = 0; sample_count > sample; sample++) {
            initialize_matrix(matrix_size, matrix_c, 0.0);

            auto start_time = std::chrono::high_resolution_clock::now();
            optimized_matrix_multiplication(matrix_size, cache_size, matrix_a, matrix_b, matrix_c);
            auto end_time = std::chrono::high_resolution_clock::now();

            double elapsed_time = std::chrono::duration<double>(end_time - start_time).count();
            double size = matrix_size;
            double normalized_time = (elapsed_time * 1.0e9) / (size * size * size);  // ns

            std::cout << "Bloques\t" << matrix_size << "\t" << sample << "\t" 
                      << std::fixed << std::setprecision(6) << elapsed_time << "\t" 
                      << std::fixed << std::setprecision(4) << normalized_time << "\n";
            report_file << "Bloques\t" << matrix_size << "\t" << sample << "\t" 
                        << std::fixed << std::setprecision(6) << elapsed_time << "\t" 
                        << std::fixed << std::setprecision(4) << normalized_time << "\n";
        }

        if (argc > 4) {
            display_matrix(matrix_size, matrix_a);
            display_matrix(matrix_size, matrix_b);
            display_matrix(matrix_size, matrix_c);
        }

    } else {
        std::cout << "Uso: " << argv[0] << " <tamaño_matriz> <muestras> <tamaño_cache> [imprimir_matrices]\n";
    }

    return EXIT_SUCCESS;
}

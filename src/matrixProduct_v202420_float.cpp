#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

// https://inst.eecs.berkeley.edu/~cs61c/fa12/labs/07/
/* To save you time, we are including all 6 variants of the loop ordering
   as separate functions and then calling them using function pointers.
   The reason for having separate functions that are nearly identical is
   to avoid counting any extraneous processing towards the computation
   time.  This includes I/O accesses (printf) and conditionals (if/switch).
   I/O accesses are slow and conditional/branching statements could
   unfairly bias results (lower cases in switches must run through more
   case statements on each iteration).
*/

void ProductMat_a(int n, float *A, float *B, float *C)
{
    int i, j, k;
    float sum;
    /* This is ijk loop order version. */
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            sum = 0;
            for (k = 0; k < n; k++)
            {
                sum += A[i + k * n] * B[k + j * n]; // C[i][j] += A[i][k] * B[k][j]; (a)Version ij k
            }
            C[i + j * n] += sum;
        }
    }
}

void ProductMat_b(int n, float *A, float *B, float *C)
{
    int i, j, k;
    float sum;
    /* This is jik loop order version. */
    for (j = 0; j < n; j++)
    {
        for (i = 0; i < n; i++)
        {
            sum = 0;
            for (k = 0; k < n; k++)
            {
                sum += A[i + k * n] * B[k + j * n]; // C[i][j] += A[i][k] * B[k][j]; (a)Version ij k
            }
            C[i + j * n] += sum;
        }
    }
}

void ProductMat_c(int n, float *A, float *B, float *C)
{
    int i, j, k;
    float r;
    /* This is jki loop order version. */
    for (j = 0; j < n; j++)
    {
        for (k = 0; k < n; k++)
        {
            r = B[k + j * n];
            for (i = 0; i < n; i++)
            {
                C[i + j * n] += A[i + k * n] * r; // C[i][j] += A[i][k]*B[k][j]; (c) Version jki
            }
        }
    }
}

void ProductMat_d(int n, float *A, float *B, float *C)
{
    int i, j, k;
    float r;
    /* This is kji loop order. */
    for (k = 0; k < n; k++)
    {
        for (j = 0; j < n; j++)
        {
            r = B[k + j * n];
            for (i = 0; i < n; i++)
            {
                C[i + j * n] += A[i + k * n] * r; // C[i][j] += A[i][k]*B[k][j];   (d) Version kji
            }
        }
    }
}

void ProductMat_e(int n, float *A, float *B, float *C)
{
    int i, j, k;
    float r;
    /* This is kij loop order version. */
    for (k = 0; k < n; k++)
        for (i = 0; i < n; i++)
        {
            r = A[i + k * n];
            for (j = 0; j < n; j++)
            {
                C[i + j * n] += r * B[k + j * n]; // C[i][j] += A[i][k]*B[k][j]; (e) Version kij
            }
        }
}
void ProductMat_f(int n, float *A, float *B, float *C)
{
    int i, j, k;
    float r;
    /* This is jki loop order version. */
    for (i = 0; i < n; i++)
    {
        for (k = 0; k < n; k++)
        {
            r = A[i + k * n];
            for (j = 0; j < n; j++)
            {
                C[i + j * n] += r * B[k + j * n]; // C[i][j] += A[i][k]*B[k][j]; // (f) Version ikj
            }
        }
    }
}

//****************************************************************************************************/
void PrintMat(int n, float *M)
{
    int i, j;

    for (j = 0; j < n; j++)
    {
        // printf("; \n");
        for (i = 0; i < n; i++)
            printf("%.3f ", M[i + j * n]);
        printf("; \n");
    }
    printf("\n\n");
}

//****************************************************************************************************/
FILE *fp;

int main(int argc, char *argv[])
{

    //  printf("Product Matrix\n");

    errno_t err;
    err = fopen_s(&fp, "ReportS1.txt", "a+");
    // err = fopen_s(&fp, "items.txt", "w+");

    if (err != 0)
    {
        printf("The file 'ReportS1.txt' was not opened\n");
        // err = fclose(fp);
        return 0;
    }

    if (argv[1])
    {
        // printf( " input arg = %s " , argv[1]);
        int n = atoi(argv[1]); // se ingresa el tama�o de la matriz
        // printf( " input n = %d " , n);
        int samples = atoi(argv[2]);

        float *A = (float *)malloc(n * n * sizeof(float));
        float *B = (float *)malloc(n * n * sizeof(float));
        float *C = (float *)malloc(n * n * sizeof(float));

        for (int i = 0; i < n * n; i++)
            A[i] = 3.0; // (float)rand() / 100;//;
        for (int i = 0; i < n * n; i++)
            B[i] = 4.0; // (float)rand() / 100;//;

        printf("ver\ttypeData\tISA\t#sample\tn\ttime(s)\tNormalized(ns)\n");
        for (int s = 0; s < samples; s++)
        {
            for (int i = 0; i < n * n; i++)
                C[i] = 0; //;

            clock_t start = clock(); // Start measuring time

            ProductMat_f(n, A, B, C); // select ProductMat_a, ProductMat_b, ProductMat_c, ProductMat_d, ProductMat_f

            clock_t end = clock(); // Stop measuring time and calculate the elapsed time

            double seconds = (double)(end - start) / CLOCKS_PER_SEC;
            double N = n;
            double timeNormalized = (seconds * 1.0e9) / (N * N * N); // ns

            printf("%s\t%03d\t%05d\t%2.4f\t%2.4f \n", "ver(f)\t f\t x64\t", s, n, seconds, timeNormalized);        // change the version here (ver(a), ver(b), ver(c), ver(d), ver(e), ver(f)) and ISA (x86, x64)
            fprintf_s(fp, "%s\t%05d\t%05d\t%2.4f\t %2.4f \n ", "ver(f)\tf\tx64\t", s, n, seconds, timeNormalized); /* write data to the file */
        }

        if (argv[3])
        {
            PrintMat(n, A);
            PrintMat(n, B);
            PrintMat(n, C);
        }
        free(A);
        free(B);
        free(C);
        fclose(fp); /* close the file*/
    }
    printf("\n");
    return 0;
}
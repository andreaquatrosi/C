#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 6
#define M 5

// struct
typedef struct Parameters {
    const char *input_file;
    const char *output_file;
} parameters;

// prototypes
parameters readParameters(int argc, char *argv[]);
double ***createMatrix(const char *nomeFile);
void printMatrix(double ***matrix);
void sparsify(double ***matrix);
double *collect(double ***matrix);
void writeToFile(const char *nomeFile, double *array);

// --
int main(int argc, char *argv[])
{
    parameters pars = readParameters(argc, argv);
    // printf("%s, %s", pars.input_file, pars.output_file);
    printf("** A **\n");
    double ***A = createMatrix(pars.input_file);
    printMatrix(A);

    printf("\n** sparsify **\n");
    sparsify(A);
    double *array = collect(A);

    printf("\n\n** array **\n");
    for(short i = 0; i < 18; i++){
        printf("%lf ", array[i]);
    }

    writeToFile(pars.output_file, array);

    for(short i = 0; i < N; i++){
        for(short j = 0; j < N; j++){
            free(A[i][j]);
        }
        free(A[i]);
    }
    free(A);

    free(array);

    return 0;
}

// functions
parameters readParameters(int argc, char *argv[]){
    if(argc != 3){
        fprintf(stderr, "Usage: %s <input.txt> <output.txt>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if(strlen(argv[1]) > 255 || strlen(argv[2]) > 255){
        fprintf(stderr, "<input.txt> <output.txt> name lenght must be in [1, 255]\n");
        exit(EXIT_FAILURE);
    }

    parameters pars;
    pars.input_file = strdup(argv[1]);
    pars.output_file = strdup(argv[2]);

    return pars;
}

double ***createMatrix(const char *nomeFile){
    FILE *fi = fopen(nomeFile, "r");
    if(!fi){
        perror("Error while opening the file!\n");
        exit(EXIT_FAILURE);
    }

    double ***matrix = (double ***)malloc(sizeof(double **) * N);
    for(short i = 0; i < N; i++){
        matrix[i] = (double **)malloc(sizeof(double *) * M);
    }

    for(short i = 0; i < N; i++){
        for(short j = 0; j < M; j++){
            matrix[i][j] = (double *) malloc(sizeof(double));
            if(fscanf(fi, "%lf", matrix[i][j]) != 1){
                break;
            }
        }
    }

    return matrix;
}

void printMatrix(double ***matrix){
    for(short i = 0; i < N; i++){
        for(short j = 0; j < M; j++){
            printf("%lf ", *matrix[i][j]);
        }
        printf("\n");
    }
}

void sparsify(double ***matrix){
    double sum = 0.0, media = 0.0;

    for(short i = 0; i < N; i++){
        for(short j = 0; j < M; j++){
            sum += *matrix[i][j];
        }
        media = sum / M;

        for(short j = 0; j < M; j++){
            if(*matrix[i][j] <= media){
                free(matrix[i][j]);
                matrix[i][j] = NULL;
            }
        }
        sum = media = 0.0;
    }
}

double *collect(double ***matrix){
    double *array = (double *) malloc(sizeof(double) * 18);

    short k = 0;
    for(short i = 0; i < N; i++){
        for(short j = 0; j < M; j++){
            if(matrix[i][j] != NULL){
                printf("%lf", *matrix[i][j]);
                array[k] = *matrix[i][j];
                k++;
            }
        }
    }

    return array;
}

void writeToFile(const char *nomeFile, double *array){
    FILE *fo = fopen(nomeFile, "w");
    if(!fo){
        perror("Error while opening the file!\n");
        exit(EXIT_FAILURE);
    }

    for(short i = 0; i < 18; i++){
        fprintf(fo, "%lf", array[i]);
    }

    fclose(fo);
}

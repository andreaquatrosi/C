#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_S_LENGHT 31

// struct
typedef struct Arguments {
    const char *nomeFile;
    double F;
} arguments;

typedef struct Nodo {
    float V;
    char P[MAX_S_LENGHT];
    struct Nodo *next;
    struct Nodo *prev;
} nodo;

typedef struct List {
    nodo *head;
    int size;
} list;

// functions
arguments readInput(int argc, char *argv[]){
    if(argc != 3){
        fprintf(stderr, "Usage: %s <input.txt> <F>", argv[0]);
        exit(EXIT_FAILURE);
    }
    if(atof(argv[2]) < 1.0 || atof(argv[2]) > 20.0){
        fprintf(stderr, "<F> must be in range [1.0, 20.0]\n");
        exit(EXIT_FAILURE);
    }

    arguments arg;
    arg.nomeFile = strdup(argv[1]);
    arg.F = atof(argv[2]);

    return arg;
}

void insertRecord(double f, char p[], list *lst){
    nodo *newNodo = malloc(sizeof(nodo));
    if(newNodo != NULL){
        strncpy(newNodo->P, p, sizeof(newNodo->P) - 1);
        newNodo->V = f;
        newNodo->next = newNodo->prev = NULL;
    }

    if(lst->head == NULL){
        lst->head = newNodo;
        lst->size++;
    } else {
        nodo *current = lst->head;
        while(current->next != NULL){ // scorro fino all'ultimo elemento della lista
            current = current->next;
        }

        current->next = newNodo; // anziché next = NULL --- next = newNodo (ultimo elemento inserito)
        newNodo->prev = current; // il precedente di newNodo è current (vecchio ultimo elemento della lista)
        lst->size++;
    }

    // printf("%s, %lf\n", newNodo->P, newNodo->V);
}

void buildList(const char *nomeFile, list *lst){
    FILE *fi = fopen(nomeFile, "r");
    if(!fi){
        perror("Error while opening the file!\n");
        exit(EXIT_FAILURE);
    }

    double f = 0.0;
    char s[MAX_S_LENGHT];
    while(fscanf(fi, "%lf\n%s\n", &f, s) == 2){
        insertRecord(f, s, lst);
    }
    fclose(fi);

    // SWAP bubblesort list
    int swapped;
    do{
        swapped = 0;
        char temp[MAX_S_LENGHT];
        nodo *current = lst->head;

        while(current != NULL && current->next != NULL){
            if(strcmp(current->P, current->next->P) > 0){
                // SWAP
                strncpy(temp, current->P, sizeof(temp) - 1);
                strncpy(current->P, current->next->P, sizeof(current->P) - 1);
                strncpy(current->next->P, temp, sizeof(current->next->P) - 1);
                swapped = 1;
            }
            current = current->next;
        }
    }while(swapped);
}

void elab(double f, list *lst){
    nodo *current = lst->head;
    while(current != NULL){
        if(current->V >= f){
            printf("%lf\n%s\n", current->V, current->P);
        }
        current = current->next;
    }
}

void printList(list *lst){
    nodo *current = lst->head;
    while(current != NULL){
        printf("%lf\n%s\n", current->V, current->P);
        current = current->next;
    }
}

int main(int argc, char *argv[]){
    arguments arg = readInput(argc, argv);
    // printf("%s, %lf", arg.nomeFile, arg.F);

    list lst;
    lst.head = NULL;
    buildList(arg.nomeFile, &lst);
    printList(&lst);
    printf("\n");
    elab(arg.F, &lst);

    return 0;
}

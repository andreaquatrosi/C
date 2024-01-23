#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_S_LENGHT 31

// struct
typedef struct Arguments {
    const char *nomeFile;
    int k;
} arguments;

typedef struct Nodo {
    char stringa[MAX_S_LENGHT];
    struct Nodo *next;
} nodo;

typedef struct Stack {
    nodo *head;
} stack;

// functions
arguments readInput(int argc, char *argv[]){
    if(argc != 3){
        fprintf(stderr, "Usage: %s <input.txt> <k>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if(atoi(argv[2]) < 5 || atoi(argv[2]) > 15){
        fprintf(stderr, "Usage: %s <k> must be in range [5-15]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    arguments arg;
    arg.nomeFile = strdup(argv[1]);
    arg.k = atoi(argv[2]);

    return arg;
}

void pushWord(char word[], stack *pila){
    nodo *newNodo = malloc(sizeof(nodo));
    if(newNodo != NULL){
        strncpy(newNodo->stringa, word, sizeof(newNodo->stringa) - 1);
        newNodo->next = NULL;
    } else {
        fprintf(stderr, "Error while allocating space.\n");
        exit(EXIT_FAILURE);
    }

    if(pila->head == NULL){
        pila->head = newNodo;
    } else {
        newNodo->next = pila->head;
        pila->head = newNodo;
    }
}

char *popWord(stack *pila){
    if(pila->head == NULL){
        fprintf(stderr, "Stack empty!\n");
        exit(EXIT_FAILURE);
    }

    nodo *temp = pila->head;
    pila->head = temp->next;

    char *word = strdup(temp->stringa);

    free(temp);

    return word;
}

void buildStack(const char *nomeFile, stack *pila){
    FILE *fi = fopen(nomeFile, "r");
    if(!fi){
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    char word[MAX_S_LENGHT];
    while(fscanf(fi, "%s", word) == 1){
        pushWord(word, pila);
    }
    fclose(fi);
}

char *sortArray(char word[]) {
    int len = strlen(word);

    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - i - 1; j++) {
            if (word[j] > word[j + 1]) {
                char temp = word[j];
                word[j] = word[j + 1];
                word[j + 1] = temp;
            }
        }
    }

    return word;
}

void printStack(stack *pila, int k) {
    char stringa[MAX_S_LENGHT];
    while (pila->head != NULL) {
        char *popped = popWord(pila);
        strncpy(stringa, popped, sizeof(stringa) - 1);
        free(popped);

        if (strlen(stringa) >= k) {
            printf("%s\n", sortArray(stringa));
        } else {
            printf("%s\n", stringa);
        }
    }
}

// --
int main(int argc, char *argv[]){
    arguments arg = readInput(argc, argv);
    // printf("%s, %d", arg.nomeFile, arg.k);
    stack pila;
    pila.head = NULL;
    buildStack(arg.nomeFile, &pila);
    printStack(&pila, arg.k);

    return 0;
}
// --

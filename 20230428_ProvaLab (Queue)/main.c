#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_S_LENGHT 31

// struct
typedef struct Arguments {
    const char *nomeFile;
    char a;
    char b;
} arguments;

typedef struct Nodo {
    char stringa[MAX_S_LENGHT];
    struct Nodo *next;
} nodo;

typedef struct Queue {
    nodo *head;
    nodo *tail;
} queue;

// functions
arguments readInput(int argc, char *argv[]){
    if(argc != 4){
        fprintf(stderr, "Usage: %s <input.txt> <a> <b>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if(argv[2][0] < 'a' || argv[2][0] > 'z' || argv[3][0] < 'a' || argv[3][0] > 'z'){
        fprintf(stderr, "<a> <b> must be in range [a, z]\n");
        exit(EXIT_FAILURE);
    } else if(argv[2][0] > argv[3][0]){
        fprintf(stderr, "<a> <= <b>\n");
        exit(EXIT_FAILURE);
    }

    arguments arg;
    arg.nomeFile = strdup(argv[1]);
    arg.a = argv[2][0];
    arg.b = argv[3][0];

    return arg;
}

void enqueue(char word[], queue *q){
    nodo *newNodo = malloc(sizeof(nodo));
    if(newNodo != NULL){
        strncpy(newNodo->stringa, word, sizeof(newNodo->stringa) - 1);
        newNodo->next = NULL;
    } else {
        fprintf(stderr, "Error while allocating the node.\n");
        exit(EXIT_FAILURE);
    }

    if(q->head == NULL){
        q->head = q->tail = newNodo;
    } else {
        q->tail->next = newNodo;
        q->tail = newNodo;
    }
}

char *deQueue(queue *q){
    if(q->head == NULL){
        fprintf(stderr, "The queue is empty!\n");
        exit(EXIT_FAILURE);
    }

    nodo *temp = q->head;
    q->head = temp->next;

    char *word = strdup(temp->stringa);

    free(temp);

    return word;
}

void buildQueue(const char *nomeFile, queue *q){
    FILE *fi = fopen(nomeFile, "r");
    if(!fi){
        perror("Error while opening the file!\n");
        exit(EXIT_FAILURE);
    }

    char word[MAX_S_LENGHT];
    while(fscanf(fi, "%s", word) == 1){
        enqueue(word, q);
    }
}

void elab(char stringa[], char a, char b){
    for(int i = 0; i < MAX_S_LENGHT; i++){
        if(stringa[i] >= a && stringa[i] <= b){
            stringa[i] = '0';
        }
    }
}

void printQueue(queue *q, char a, char b){
    nodo *temp = q->head;
    while(temp != NULL){
        elab(temp->stringa, a, b);
        printf("%s\n", temp->stringa);
        temp = temp->next;
    }
}

// --
int main(int argc, char *argv[]){
    arguments arg = readInput(argc, argv);
    // printf("%s, %c, %c", arg.nomeFile, arg.a, arg.b);
    queue q;
    q.head = q.tail = NULL;

    buildQueue(arg.nomeFile, &q);
    printQueue(&q, arg.a, arg.b);

    return 0;
}

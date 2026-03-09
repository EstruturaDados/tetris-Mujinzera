#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO 5

// Estrutura da Peça
typedef struct {
    char tipo;
    int id;
} Peca;

// Estrutura da Fila Circular
typedef struct {
    Peca itens[TAMANHO];
    int frente;
    int fim;
    int total;
} FilaCircular;

// Protótipos das funções
void inicializarFila(FilaCircular *f);
Peca gerarPeca();
void enfileirar(FilaCircular *f);
void desenfileirar(FilaCircular *f);
void exibirFila(FilaCircular *f);

int main() {
    srand(time(NULL)); // numeros aleatórios
    FilaCircular fila;
    int opcao;

    inicializarFila(&fila);

    do {
        printf("\n--- MENU TETRIS ---");
        exibirFila(&fila);
        printf("\n1. Remover Peça");
        printf("\n2. Inserir Nova Peça");
        printf("\n0. Sair");
        printf("\nEscolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: desenfileirar(&fila); break;
            case 2: enfileirar(&fila); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while(opcao != 0);

    return 0;
}

Peca gerarPeca() {
    static int proximoId = 100;
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.tipo = tipos[rand() % 4];
    p.id = proximoId++;
    return p;
}

void inicializarFila(FilaCircular *f) {
    f->frente = 0;
    f->fim = -1;
    f->total = 0;
    
    printf("Inicializando fila com 5 peças...\n");
    for(int i = 0; i < TAMANHO; i++) {
        enfileirar(f);
    }
}

void enfileirar(FilaCircular *f) {
    if (f->total == TAMANHO) {
        printf("\nA fila está cheia! Jogue uma peça primeiro.\n");
        return;
    }
    f->fim = (f->fim + 1) % TAMANHO;
    f->itens[f->fim] = gerarPeca();
    f->total++;
}

void desenfileirar(FilaCircular *f) {
    if (f->total == 0) {
        printf("\nFila vazia! Não há peças para jogar.\n");
        return;
    }
    Peca p = f->itens[f->frente];
    printf("\n>>> PEÇA [%c] (ID: %d) ENVIADA PARA O CAMPO!\n", p.tipo, p.id);

    f->frente = (f->frente + 1) % TAMANHO;
    f->total--;
}

void exibirFila(FilaCircular *f) {
    printf("\nFila Atual: [");
    if (f->total == 0) {
        printf("Vazia ");
    } else {
        for (int i = 0; i < f->total; i++) {
            int indice = (f->frente + i) % TAMANHO;
            printf("%c(id:%d) ", f->itens[indice].tipo, f->itens[indice].id);
        }
    }
    printf("]\n");
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

typedef struct {
    char tipo;
    int id;
} Peca;

// Estrutura da Fila Circular (Entrada de Peças)
typedef struct {
    Peca itens[TAM_FILA];
    int frente, fim, total;
} FilaCircular;

// Estrutura da Pilha Linear (Reserva)
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} PilhaReserva;

// --- FUNÇÕES DE APOIO ---
Peca gerarPeca() {
    static int proximoId = 100;
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p = {tipos[rand() % 4], proximoId++};
    return p;
}

void enfileirar(FilaCircular *f) {
    if (f->total < TAM_FILA) {
        f->fim = (f->fim + 1) % TAM_FILA;
        f->itens[f->fim] = gerarPeca();
        f->total++;
    }
}

Peca desenfileirar(FilaCircular *f) {
    Peca p = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->total--;
    enfileirar(f);
    return p;
}

void push(PilhaReserva *p, Peca item) {
    if (p->topo < TAM_PILHA - 1) {
        p->itens[++(p->topo)] = item;
        printf("\n[OK] Peca %c (ID:%d) guardada na reserva!", item.tipo, item.id);
    } else {
        printf("\n[ERRO] Reserva cheia! Use uma peca antes de guardar outra.");
    }
}

Peca pop(PilhaReserva *p) {
    return p->itens[(p->topo)--];
}

// --- INTERFACE ---
void exibirEstado(FilaCircular *f, PilhaReserva *p) {
    printf("\n========================================");
    printf("\nFila: [ ");
    for (int i = 0; i < f->total; i++) {
        int idx = (f->frente + i) % TAM_FILA;
        printf("%c(%d) ", f->itens[idx].tipo, f->itens[idx].id);
    }
    printf("]");
    
    printf("\nReserva : [ ");
    for (int i = 0; i <= p->topo; i++) {
        printf("%c(%d) ", p->itens[i].tipo, p->itens[i].id);
    }
    printf("] <- Topo");
    printf("\n========================================\n");
}

int main() {
    srand(time(NULL));
    FilaCircular fila = { .frente = 0, .fim = -1, .total = 0 };
    PilhaReserva pilha = { .topo = -1 };
    int opcao;

    for(int i = 0; i < TAM_FILA; i++) enfileirar(&fila);

    do {
        exibirEstado(&fila, &pilha);
        printf("1 - Jogar peca da fila\n2 - Reservar peca da fila\n3 - Usar peca reservada\n0 - Sair\nEscolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: {
                Peca j = desenfileirar(&fila);
                printf("\nJOGADA: %c (ID:%d)\n", j.tipo, j.id);
                break;
            }
            case 2:
                if (pilha.topo < TAM_PILHA - 1) {
                    push(&pilha, desenfileirar(&fila));
                } else {
                    printf("\n[AVISO] Pilha de reserva lotada!\n");
                }
                break;
            case 3:
                if (pilha.topo >= 0) {
                    Peca r = pop(&pilha);
                    printf("\nJOGADA DA RESERVA: %c (ID:%d)\n", r.tipo, r.id);
                } else {
                    printf("\nNenhuma peca na reserva!\n");
                }
                break;
        }
    } while(opcao != 0);

    return 0;
}
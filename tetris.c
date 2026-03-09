#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TAM_FILA 5
#define TAM_PILHA 3

typedef struct {
    char tipo;
    int id;
} Peca;

typedef struct {
    Peca itens[TAM_FILA];
    int frente, fim, total;
} FilaCircular;

typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} PilhaReserva;

typedef struct {
    FilaCircular fila;
    PilhaReserva pilha;
} EstadoJogo;

Peca gerarPeca() {
    static int proximoId = 100;
    char tipos[] = {'I', 'O', 'T', 'L'};
    return (Peca){tipos[rand() % 4], proximoId++};
}

void enfileirar(FilaCircular *f, Peca p) {
    if (f->total < TAM_FILA) {
        f->fim = (f->fim + 1) % TAM_FILA;
        f->itens[f->fim] = p;
        f->total++;
    }
}

Peca desenfileirar(FilaCircular *f) {
    Peca p = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->total--;
    return p;
}

void salvarEstado(FilaCircular *f, PilhaReserva *p, EstadoJogo *backup) {
    backup->fila = *f;
    backup->pilha = *p;
}

void desfazer(FilaCircular *f, PilhaReserva *p, EstadoJogo *backup) {
    *f = backup->fila;
    *p = backup->pilha;
    printf("\n[VOLTAR] Ultima ação desfeita com sucesso!\n");
}

void trocarTopo(FilaCircular *f, PilhaReserva *p) {
    if (f->total > 0 && p->topo >= 0) {
        Peca temp = f->itens[f->frente];
        f->itens[f->frente] = p->itens[p->topo];
        p->itens[p->topo] = temp;
        printf("\n[TROCA] Peças trocadas entre Fila e Pilha!\n");
    } else {
        printf("\n[ERRO] Ambas as estruturas precisam ter peças para trocar.\n");
    }
}

void inverterEstruturas(FilaCircular *f, PilhaReserva *p) {
    Peca temp[TAM_FILA];
    int n = f->total;
    for(int i = 0; i < n; i++) temp[i] = desenfileirar(f);
    for(int i = n - 1; i >= 0; i--) enfileirar(f, temp[i]);
    
    for(int i = 0; i <= p->topo / 2; i++) {
        Peca t = p->itens[i];
        p->itens[i] = p->itens[p->topo - i];
        p->itens[p->topo - i] = t;
    }
    printf("\n[INVERSAO] Fila e Pilha invertidas internamente!\n");
}

void exibir(FilaCircular *f, PilhaReserva *p) {
    printf("\n--- TABULEIRO ---");
    printf("\nFILA:  ");
    for (int i = 0; i < f->total; i++) 
        printf("%c(%d) ", f->itens[(f->frente + i) % TAM_FILA].tipo, f->itens[(f->frente + i) % TAM_FILA].id);
    
    printf("\nPILHA:");
    for (int i = 0; i <= p->topo; i++) 
        printf("%c(%d) ", p->itens[i].tipo, p->itens[i].id);
    printf("] <-\n---------------------------\n");
}

int main() {
    srand(time(NULL));
    FilaCircular fila = {0, -1, 0};
    PilhaReserva pilha = {-1};
    EstadoJogo backup;
    int op;

    for(int i=0; i<TAM_FILA; i++) enfileirar(&fila, gerarPeca());
    salvarEstado(&fila, &pilha, &backup);

    do {
        exibir(&fila, &pilha);
        printf("1.Jogar\n2.Reservar\n3.Usar Reserva\n4.Trocar\n5.Desfazer\n6.Inverter\n0.Sair\nEscolha: ");
        scanf("%d", &op);

        if (op >= 1 && op <= 4 || op == 6) salvarEstado(&fila, &pilha, &backup);

        switch(op) {
            case 1: printf("\nJOGOU: %c\n", desenfileirar(&fila).tipo); enfileirar(&fila, gerarPeca()); break;
            case 2: if(pilha.topo < TAM_PILHA-1) { 
                        Peca p = desenfileirar(&fila);
                        pilha.itens[++pilha.topo] = p;
                        enfileirar(&fila, gerarPeca());
                    } break;
            case 3: if(pilha.topo >= 0) printf("\nUSOU RESERVA: %c\n", pilha.itens[pilha.topo--].tipo); break;
            case 4: trocarTopo(&fila, &pilha); break;
            case 5: desfazer(&fila, &pilha, &backup); break;
            case 6: inverterEstruturas(&fila, &pilha); break;
        }
    } while(op != 0);
    return 0;
}
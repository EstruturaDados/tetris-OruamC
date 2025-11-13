// Tetris Stack – Fila Circular + Pilha de Reserva
// Nível: Intermediário (fila de peças futuras e pilha de reserva)
// Funcionalidades: inicializar fila, jogar peça, reservar peça (fila->pilha),
// usar peça reservada (pilha), manter a fila sempre cheia (auto-geração) e exibir estado.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --------- Definições e estruturas ---------
#define CAPACIDADE_FILA 5
#define CAPACIDADE_PILHA 3

// Representa uma peça do Tetris
typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;    // identificador único sequencial
} Peca;

// Fila circular
typedef struct {
    Peca dados[CAPACIDADE_FILA];
    int frente;   // índice do primeiro elemento
    int tras;     // próxima posição livre ao final
    int tamanho;  // quantidade de elementos na fila
} Fila;

// Pilha linear (reserva)
typedef struct {
    Peca dados[CAPACIDADE_PILHA];
    int tamanho; // quantidade de elementos (topo está em tamanho-1)
} Pilha;

// --------- Utilidades de I/O ---------
static void limparBufferEntrada(void) {
    int c; while ((c = getchar()) != '\n' && c != EOF) {}
}

// --------- Geração de peças ---------
// Gera automaticamente uma nova peça com tipo aleatório e id sequencial
static Peca gerarPeca(void) {
    static int proximoId = 0;
    static const char TIPOS[] = { 'I', 'O', 'T', 'L' };
    Peca p;
    p.nome = TIPOS[rand() % (int)(sizeof(TIPOS) / sizeof(TIPOS[0]))];
    p.id = proximoId++;
    return p;
}

// --------- Operações sobre a fila circular ---------
static void inicializarFila(Fila* f) {
    f->frente = 0;
    f->tras = 0;
    f->tamanho = 0;
}

static int filaVazia(const Fila* f) { return f->tamanho == 0; }
static int filaCheia(const Fila* f) { return f->tamanho == CAPACIDADE_FILA; }

// Insere no final (enqueue). Retorna 1 se OK, 0 se cheia
static int enqueue(Fila* f, Peca valor) {
    if (filaCheia(f)) return 0;
    f->dados[f->tras] = valor;
    f->tras = (f->tras + 1) % CAPACIDADE_FILA;
    f->tamanho++;
    return 1;
}

// Remove da frente (dequeue). Retorna 1 se OK, 0 se vazia
static int dequeue(Fila* f, Peca* removida) {
    if (filaVazia(f)) return 0;
    if (removida) *removida = f->dados[f->frente];
    f->frente = (f->frente + 1) % CAPACIDADE_FILA;
    f->tamanho--;
    return 1;
}

// Exibe a fila no formato [T 0] [O 1] ...
static void mostrarFila(const Fila* f) {
    printf("\nFila de pecas\n");
    if (filaVazia(f)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = 0, idx = f->frente; i < f->tamanho; i++, idx = (idx + 1) % CAPACIDADE_FILA) {
        const Peca* p = &f->dados[idx];
        printf("[%c %d] ", p->nome, p->id);
    }
    printf("\n");
}

// --------- Operações sobre a pilha ---------
static void inicializarPilha(Pilha* p) { p->tamanho = 0; }
static int pilhaVazia(const Pilha* p) { return p->tamanho == 0; }
static int pilhaCheia(const Pilha* p) { return p->tamanho == CAPACIDADE_PILHA; }

// Empilha (push). Retorna 1 se OK, 0 se cheia
static int push(Pilha* p, Peca valor) {
    if (pilhaCheia(p)) return 0;
    p->dados[p->tamanho] = valor;
    p->tamanho++;
    return 1;
}

// Desempilha (pop). Retorna 1 se OK, 0 se vazia
static int pop(Pilha* p, Peca* removida) {
    if (pilhaVazia(p)) return 0;
    p->tamanho--;
    if (removida) *removida = p->dados[p->tamanho];
    return 1;
}

static void mostrarPilha(const Pilha* p) {
    printf("Pilha de reserva  (Topo -> Base): ");
    if (pilhaVazia(p)) { printf("(vazia)\n"); return; }
    // Mostra do topo para a base
    for (int i = p->tamanho - 1; i >= 0; i--) {
        printf("[%c %d] ", p->dados[i].nome, p->dados[i].id);
    }
    printf("\n");
}

// Mantém a fila cheia gerando novas peças até capacidade
static void reabastecerFila(Fila* f) {
    while (f->tamanho < CAPACIDADE_FILA) {
        enqueue(f, gerarPeca());
    }
}

// --------- Programa principal ---------
int main(void) {
    srand((unsigned)time(NULL));

    Fila fila;
    inicializarFila(&fila);
    Pilha pilha;
    inicializarPilha(&pilha);

    // Inicializa a fila com um número fixo de elementos (5)
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    printf("Tetris Stack – Gerenciamento de Pecas (Fila + Pilha)\n");
    mostrarFila(&fila);
    mostrarPilha(&pilha);

    int opcao;
    do {
        printf("\nOpcoes:\n");
        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca (mover da fila para a pilha)\n");
        printf("3 - Usar peca reservada (remover da pilha)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) { limparBufferEntrada(); opcao = -1; }
        limparBufferEntrada();

        switch (opcao) {
            case 1: {
                // Jogar peça: remove da frente da fila
                Peca jogada;
                if (dequeue(&fila, &jogada)) {
                    printf("Jogou a peca [%c %d].\n", jogada.nome, jogada.id);
                } else {
                    printf("Fila vazia. Nao ha peca para jogar.\n");
                }
                // Reabastece a fila para manter cheia
                reabastecerFila(&fila);
                mostrarFila(&fila);
                mostrarPilha(&pilha);
                break;
            }
            case 2: {
                // Reservar peça: move frente da fila para topo da pilha
                if (pilhaCheia(&pilha)) {
                    printf("Pilha de reserva cheia. Nao e possivel reservar mais pecas.\n");
                } else if (filaVazia(&fila)) {
                    printf("Fila vazia. Nao ha peca para reservar.\n");
                } else {
                    Peca reservada;
                    dequeue(&fila, &reservada);
                    push(&pilha, reservada);
                    printf("Reservou a peca [%c %d] para a pilha.\n", reservada.nome, reservada.id);
                }
                // Reabastece a fila para manter cheia
                reabastecerFila(&fila);
                mostrarFila(&fila);
                mostrarPilha(&pilha);
                break;
            }
            case 3: {
                // Usar peça reservada: pop da pilha
                Peca usada;
                if (pop(&pilha, &usada)) {
                    printf("Usou a peca reservada [%c %d].\n", usada.nome, usada.id);
                } else {
                    printf("Pilha vazia. Nao ha peca reservada para usar.\n");
                }
                // Mantem fila cheia (se houver espaço)
                reabastecerFila(&fila);
                mostrarFila(&fila);
                mostrarPilha(&pilha);
                break;
            }
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}


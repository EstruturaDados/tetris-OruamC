// Tetris Stack – Fila Circular de Peças Futuras
// Nível: Básico (apenas Fila)
// Funcionalidades: inicializar fila, jogar peça (dequeue), inserir nova peça (enqueue), exibir estado.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --------- Definições e estruturas ---------
#define CAPACIDADE_FILA 5

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

// --------- Programa principal ---------
int main(void) {
    srand((unsigned)time(NULL));

    Fila fila;
    inicializarFila(&fila);

    // Inicializa a fila com um número fixo de elementos (5)
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    printf("Tetris Stack – Fila de Pecas Futuras\n");
    mostrarFila(&fila);

    int opcao;
    do {
        printf("\nOpcoes:\n");
        printf("1 - Jogar peca (dequeue)\n");
        printf("2 - Inserir nova peca (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) { limparBufferEntrada(); opcao = -1; }
        limparBufferEntrada();

        switch (opcao) {
            case 1: {
                Peca jogada;
                if (dequeue(&fila, &jogada)) {
                    printf("Jogou a peca [%c %d].\n", jogada.nome, jogada.id);
                } else {
                    printf("Fila vazia. Nao ha peca para jogar.\n");
                }
                mostrarFila(&fila);
                break;
            }
            case 2: {
                if (filaCheia(&fila)) {
                    printf("Fila cheia. Nao e possivel inserir nova peca.\n");
                } else {
                    Peca nova = gerarPeca();
                    enqueue(&fila, nova);
                    printf("Inserida nova peca [%c %d] ao final da fila.\n", nova.nome, nova.id);
                }
                mostrarFila(&fila);
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


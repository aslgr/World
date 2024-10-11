// A  LEF é uma lista que armazena eventos ordenados pelo membro tempo
// do tipo evento_t. Cada evento pode ter um tipo e armazenar em dados
// diferentes estruturas de dados que permitem descrever cada evento.

typedef  struct evento {
    int tempo;                              // Tempo lógico do evento.
    int tipo;                               // Inteiro para diferenciar os diferentes tipos de eventos.
    int tamanho;                            // Tamanho ocupado pela estrutura referente ao evento (dados).
    void *dados;                            // Estrutura de dados que representa o evento.
    void (*destroidados)(void *dados);      // Ponteiro para função que libera evento->dados.
} evento_t;

// nodo_t eh uma estrutura interna que representa cada nodo da LEF.
typedef struct nodo_lef {
    evento_t *evento;
    struct nodo_lef *prox;
} nodo_lef_t;

// Representação da LEF.
typedef struct lef {
    nodo_lef_t  *primeiro;
} lef_t;

// Cria uma LEF vazia.
lef_t *cria_lef();

// Destrói a LEF, sempre retorna NULL. 
// Para os eventos é necessário executar a função destroidados para liberar os dados 
// que representam o evento.
lef_t *destroi_lef(lef_t *l);

// Adiciona um evento na primeira posição da LEF. Uma nova cópia
// da estrutura evento será feita. 
// Retorna 1 em caso de sucesso, 0 caso contrário.
int adiciona_inicio_lef(lef_t *l, evento_t *evento);

// Adiciona um evento de acordo com o valor evento->tempo na LEF.
// Uma nova cópia da estrutura evento será feita.
// Retorna 1 em caso de sucesso, 0 caso contrario.
int adiciona_ordem_lef(lef_t *l, evento_t *evento);

// Retorna o primeiro evento da LEF. A responsabilidade por desalocar
// a memória associado é de quem chama essa função. 
evento_t *obtem_primeiro_lef(lef_t *l);

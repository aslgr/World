// Header da LibFila, capaz de enfileirar dados.

typedef struct nodo_f {
    int chave;                  // Inteiro armazenado pela fila.
    struct nodo_f *prox;        // Ponteiro para o próximo.
} nodo_f_t;

typedef struct fila {
    nodo_f_t *ini;              // Ponteiro para o início da fila.
    nodo_f_t *fim;              // Ponteiro para o fim da fila.
    int tamanho;                // Tamanho da fila (número de elementos).
} fila_t;

// Cria uma fila vazia e a retorna, se falhar retorna NULL.
fila_t *cria_fila();

// Remove todos os elementos da fila, libera o espaço e retorna NULL.
fila_t *destroi_fila(fila_t *f);

// Retorna 1 se a fila estiver vazia e 0 caso contrário.
int fila_vazia(fila_t *f);

// Retorna o tamanho da fila, isto é, o numero de elementos presentes nela.
int tamanho_fila(fila_t *f);

// Insere o elemento no final da fila.
// Retorna 1 se a operação foi bem sucedida e 0 caso contrário.
int insere_fila (fila_t *f, int elemento);

// Remove e armazena o primeiro elemento da fila.
// Retorna 1 se a operação foi bem sucedida e 0 caso contrário.
int retira_fila (fila_t *f, int *elemento);

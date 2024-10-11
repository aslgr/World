// Header da LibConjunto, capaz de realizar operações em conjuntos de dados.

typedef struct conjunto {
    int max;        // Tamanho máximo do vetor atualmente alocado.
    int card;       // Cardinalidade do conjunto.
    int ptr;        // Ponteiro para algum índice do vetor (iterador).
    int *v;         // Vetor de inteiros com no máximo "max" elementos.
} conjunto_t;

// Cria um conjunto vazio e o retorna, se falhar retorna NULL.
// "max" é o tamanho máximo do conjunto.
conjunto_t *cria_conjunto(int max);

// Remove todos os elementos do conjunto, libera o espaço e retorna NULL.
conjunto_t *destroi_conjunto(conjunto_t *c);

// Retorna 1 se o conjunto estiver vazio e 0 caso contrário.
int conjunto_vazio(conjunto_t *c);

// Retorna a cardinalidade do conjunto.
int cardinalidade(conjunto_t *c);

// Insere o elemento no conjunto, garantindo que não hajam repetições.
// Retorna 1 se a operação for bem sucedida, 0 se tenta inserir um elemento
// já presente no conjunto e -1 caso falhe por falta de espaço.
int insere_conjunto(conjunto_t *c, int elemento);

// Remove o elemento do conjunto, caso ele exista.
// Retorna 1 se a operação for bem sucedida e 0 se o elemento não existir.
int retira_conjunto(conjunto_t *c, int elemento);

// Retorna 1 se o elemento pertencer ao conjunto e 0 caso contrário.
int pertence(conjunto_t *c, int elemento);

// Retorna 1 se o conjunto "c1" estiver contido no conjunto "c2" e 0 caso contrário.
int contido(conjunto_t *c1, conjunto_t *c2);

// Retorna 1 se o conjunto "c1" for igual ao conjunto "c2" e 0 caso contrário.
int sao_iguais(conjunto_t *c1, conjunto_t *c2);

// Cria e retorna o endereço do conjunto diferença entre "c1" e "c2", nesta ordem.
// Retorna NULL se a operação falhar.
conjunto_t *cria_diferenca(conjunto_t *c1, conjunto_t *c2);

// Cria e retorna o endereço do conjunto interseção entre os conjuntos "c1" e "c2".
// Retorna NULL se a operação falhar.
conjunto_t *cria_intersecao(conjunto_t *c1, conjunto_t *c2);

// Cria e retorna o endereço do conjunto união entre os conjuntos "c1" e "c2".
// Retorna NULL se a operacao falhou.
conjunto_t *cria_uniao(conjunto_t *c1, conjunto_t *c2);

// Retorna uma cópia do conjunto "c", ou NULL caso não seja possível.
conjunto_t *cria_copia(conjunto_t *c);

// Cria um subconjunto com elementos aleatórios do conjunto "c".
// Se o conjunto for vazio, retorna um subconjunto vazio.
// Se n >= cardinalidade(c), retorna o próprio conjunto "c".
conjunto_t *cria_subconjunto(conjunto_t *c, int n);

// Imprime os elementos do conjunto em ordem crescente.
void imprime(conjunto_t *c);

// Retorna 1 se conseguir dobrar o tamanho máximo do conjunto e 0 caso contrário.
int redimensiona(conjunto_t *c);

// As funções abaixo permitem acessar elementos apontados pelo ponteiro "ptr".
// Este ponteiro pode ser inicializado e incrementado, viabilizando a implementação
// de um mecanismo iterador.

// Inicializa o contador que será usado na função incrementar_iterador.
void iniciar_iterador(conjunto_t *c);

// Incrementa o iterador e retorna o elemento apontado por ele.
// Retorna 0 se o conjunto acabar. 
int incrementar_iterador(conjunto_t *c, int *elemento);

// Escolhe um elemento qualquer do conjunto para ser removido e o remove.
// Não testa se o conjunto é vazio, deixa para a main fazer isso.
int retirar_um_elemento(conjunto_t *c);

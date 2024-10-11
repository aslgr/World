#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "LibConjunto.h"

conjunto_t *cria_conjunto(int max)
{
	conjunto_t *c;
	if ((c = (conjunto_t*)malloc(sizeof(conjunto_t))) == NULL)
		return NULL;

	if ((c->v = (int*)malloc(max*sizeof(int))) == NULL)
		return NULL;

	c->max = max;
	c->card = 0;

	return c;
}

conjunto_t *destroi_conjunto(conjunto_t *c)
{
	free(c->v);
	free(c);

	return NULL;
}

int conjunto_vazio(conjunto_t *c)
{
	if (cardinalidade(c) == 0)
		return 1;

	return 0;
}

int cardinalidade(conjunto_t *c)
{
	return c->card;
}

int insere_conjunto(conjunto_t *c, int elemento)
{
	if (cardinalidade(c) == c->max)
		return -1;
	
	if (pertence(c, elemento))
		return 0;

	c->v[c->card] = elemento;
	c->card++;

	return 1;
}

int retira_conjunto(conjunto_t *c, int elemento)
{
	if (!conjunto_vazio(c))
	{
		for (int i = 0; i < c->card; i++)
		{
			if (elemento == c->v[i])
			{
				for (int j = i; j < c->card-1; j++)
					c->v[j] = c->v[j+1];
				
				c->card--;
				return 1;
			}
		}
	}

	return 0;
}

int pertence(conjunto_t *c, int elemento)
{
	if (!conjunto_vazio(c))
	{
		for (int i = 0; i < c->card; i++)
		{
			if (elemento == c->v[i])
				return 1;
		}
	}

	return 0;
}

int contido(conjunto_t *c1, conjunto_t *c2)
{
	if (!conjunto_vazio(c1))
	{
		for (int i = 0; i < c1->card; i++)
		{
			if (!pertence(c2, c1->v[i]))
				return 0;
		}
	}

	return 1;
}

int sao_iguais(conjunto_t *c1, conjunto_t *c2)
{
	if (contido(c1, c2) && contido(c2, c1))
		return 1;
		
	return 0;
}

conjunto_t *cria_diferenca(conjunto_t *c1, conjunto_t *c2)
{
	conjunto_t *cj_diff;

	if ((cj_diff = cria_conjunto(c1->card)) == NULL)
		return NULL;
	
	if (sao_iguais(c1, c2) || conjunto_vazio(c1))
		return cj_diff;

	for (int i = 0; i < c1->card; i++)
	{
		if (!pertence(c2, c1->v[i]))
			insere_conjunto(cj_diff, c1->v[i]);
	}

	return cj_diff;
}

conjunto_t *cria_intersecao(conjunto_t *c1, conjunto_t *c2)
{
	conjunto_t *cj_interseccao;
	
	if ((cj_interseccao = cria_conjunto(c1->card)) == NULL)
		return NULL;

	if (conjunto_vazio(c1) || conjunto_vazio(c2))
		return cj_interseccao;
	
	for (int i = 0; i < c1->card; i++)
	{
		if (pertence(c2, c1->v[i]))
			insere_conjunto(cj_interseccao, c1->v[i]);
	}

	return cj_interseccao;
}

conjunto_t *cria_uniao(conjunto_t *c1, conjunto_t *c2)
{
	conjunto_t *cj_uniao;

	if ((cj_uniao = cria_conjunto(c1->card + c2->card)) == NULL)
		return NULL;
	
	if (!conjunto_vazio(c1))
	{
		for (int i = 0; i < c1->card; i++)
			insere_conjunto(cj_uniao, c1->v[i]);
	}
	
	if (!conjunto_vazio(c2))
	{
		for (int i = 0; i < c2->card; i++)
			insere_conjunto(cj_uniao, c2->v[i]);
	}

	return cj_uniao;
}

conjunto_t *cria_copia(conjunto_t *c)
{
	conjunto_t *cj_copia;

	if ((cj_copia = cria_conjunto(c->max)) == NULL)
		return NULL;

	if (!conjunto_vazio(c))
	{
		for (int i = 0; i < c->card; i++)
			insere_conjunto(cj_copia, c->v[i]);
	}

	return cj_copia;
}

conjunto_t *cria_subconjunto(conjunto_t *c, int n)
{
	conjunto_t *subconjunto;

	if ((subconjunto = cria_conjunto(c->max)) == NULL)
		return NULL;

	srand(time(NULL));

	if (n >= cardinalidade(c))
	{
		for (int i = 0; i < c->card; i++)
			insere_conjunto(subconjunto, c->v[i]);
	}
	else
	{
		int i = 0;
		while (i < n)
		{
			if (insere_conjunto(subconjunto, c->v[rand() % cardinalidade(c)]))
				i++;
		}
	}
	
	return subconjunto;
}

// Função que o qsort usa para comparar os elementos.
int comparar(const void *a, const void *b)
{
	int x = *(int*)a;
	int y = *(int*)b;
    return (x > y) - (x < y);
}

void imprime(conjunto_t *c)
{
	qsort(c->v, c->card, sizeof(int), comparar);
	
	for (int i = 0; i < c->card; i++)
		printf("%d ", c->v[i]);
	printf("\n");
}

int redimensiona(conjunto_t *c)
{
	c->max = c->max*2;

	if ((c->v = (int*)realloc(c->v, c->max*sizeof(int))) == NULL)
		return 0;

	return 1;
}

void iniciar_iterador(conjunto_t *c)
{
	c->ptr = -1;
}

int incrementar_iterador(conjunto_t *c, int *elemento)
{
	if (c->ptr >= c->card-1)
		return 0;

	c->ptr++;

	*elemento = c->v[c->ptr];
	return *elemento;
}

int retirar_um_elemento(conjunto_t *c)
{
	srand(time(NULL));
	
	int aux = c->v[rand() % cardinalidade(c)];
	retira_conjunto(c, aux);

	return aux;
}

#include <stdlib.h>
#include "LibFila.h"

fila_t *cria_fila()
{
	fila_t *f;
	if ((f = (fila_t*)malloc(sizeof(fila_t))) == NULL)
		return NULL;
	
	f->ini = f->fim = NULL;
	f->tamanho = 0;

	return f;
}

fila_t *destroi_fila(fila_t *f)
{
	nodo_f_t *ptr;
	while (!fila_vazia(f))
	{
		ptr = f->ini;
		f->ini = ptr->prox;

		free(ptr);

		f->tamanho--;
	}

	free(f);
	return NULL;
}

int fila_vazia(fila_t *f)
{
	if (tamanho_fila(f) == 0)
		return 1;

	return 0;
}

int tamanho_fila(fila_t *f)
{
	return f->tamanho;
}

int insere_fila(fila_t *f, int elemento)
{	
	nodo_f_t *ptr;
	if ((ptr = (nodo_f_t*)malloc(sizeof(nodo_f_t))) == NULL)
		return 0;
	
	ptr->chave = elemento;

	if (f->fim != NULL)
		f->fim->prox = ptr;
	else
		f->ini = ptr;
	
	f->fim = ptr;
	ptr->prox = NULL;

	f->tamanho++;

	return 1;
}

int retira_fila(fila_t *f, int *elemento)
{
	if (fila_vazia(f))
		return 0;

	nodo_f_t *ptr = f->ini;

	if (ptr == f->fim)
		f->fim = NULL;

	(*elemento) = ptr->chave;
	f->ini = ptr->prox;

	free(ptr);

	f->tamanho--;

	return 1;
}

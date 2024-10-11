#include <stdlib.h>
#include <string.h>
#include "LibLEF.h"

lef_t *cria_lef()
{
	lef_t *l;
	if ((l = (lef_t*)malloc(sizeof(lef_t))) == NULL)
		return NULL;

	l->primeiro = NULL;

	return l;
}

lef_t *destroi_lef(lef_t *l)
{
    nodo_lef_t *aux;
    
	while ((aux = l->primeiro) != NULL)
	{
		l->primeiro = l->primeiro->prox;
		
		aux->evento->destroidados(aux->evento->dados);
		free(aux->evento);
		free(aux);
	}

	free(l);
	return NULL;
}

evento_t *copia_evento(evento_t *evento)
{
	evento_t *copia_ev;
	if ((copia_ev = (evento_t*)malloc(sizeof(evento_t))) == NULL)
		return 0;
		
	copia_ev->tempo = evento->tempo;
    copia_ev->tipo = evento->tipo;
    copia_ev->tamanho = evento->tamanho;
    
    if ((copia_ev->dados = malloc(evento->tamanho)) == NULL)
        return 0;
        
    memcpy(copia_ev->dados, evento->dados, evento->tamanho);

	copia_ev->destroidados = evento->destroidados;

	return copia_ev;
}

int adiciona_inicio_lef(lef_t *l, evento_t *evento)
{
	nodo_lef_t *ptr;
	if ((ptr = (nodo_lef_t*)malloc(sizeof(nodo_lef_t))) == NULL)
		return 0;
	
	evento_t *copia_ev = copia_evento(evento);

	ptr->evento = copia_ev;
	ptr->prox = l->primeiro;
	l->primeiro = ptr;

	return 1;
}

int adiciona_ordem_lef(lef_t *l, evento_t *evento)
{
	if (l->primeiro == NULL)
	{
		if (adiciona_inicio_lef(l, evento))
			return 1;

		return 0;
	}
	
	nodo_lef_t *anterior_aux, *aux, *ptr;
	if ((ptr = (nodo_lef_t*)malloc(sizeof(nodo_lef_t))) == NULL)
		return 0;
		
	evento_t *copia_ev = copia_evento(evento);

	ptr->evento = copia_ev;
	anterior_aux = aux = l->primeiro;

	while ((aux != NULL) && (ptr->evento->tempo > aux->evento->tempo))
	{
		anterior_aux = aux;
		aux = aux->prox;
	}

	if (anterior_aux == aux)
	{	
	    ptr->prox = l->primeiro;
		l->primeiro = ptr;

		return 1;
	}

	anterior_aux->prox = ptr;
	ptr->prox = aux;

	return 1;
}

evento_t *obtem_primeiro_lef(lef_t *l)
{
	nodo_lef_t *aux;
	evento_t *evento;
	
	if ((aux = l->primeiro) != NULL)
	{
		evento = aux->evento;
		l->primeiro = aux->prox;
		
		free(aux);
		return evento;
	}

	return NULL;
}

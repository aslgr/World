#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "LibFila.h"
#include "LibConjunto.h"
#include "LibLEF.h"

/********************************************************************/

typedef struct struct_mundo {
	int tempo_atual; /*cada unidade é 15 minutos*/
	conjunto_t *rumores; /*conjunto de todos os rumores*/
	int n_pessoas; /*numero total de pessoas no mundo*/
	int n_locais; /*numero total de locais no mundo*/
	int n_rumores; /*numero total de rumores*/
	int tamanho_mundo; /*coordenadas x,y maximas*/
	int tempo_fim_do_mundo;
	} mundo;
	
typedef struct struct_pessoa {
    int extroversao;
    int paciencia;
    int idade;
    conjunto_t *crc; /*conjunto de rumores conhecidos*/
    } pessoa;
	
typedef struct struct_coordenadas {
    int x;
    int y;
	} coordenadas;
	
typedef struct struct_local {
	int lotacao_maxima;
	conjunto_t *pessoas_no_lugar; /*pessoas no local*/
	fila_t *fila; /*fila do local*/
	coordenadas *localizacao; /*localizacao do local*/
	} local;
	
typedef struct struct_dados_evento {
    int id_pessoa;
    int id_local;
    conjunto_t *cj_rumor;
	} dados_evento;

/********************************************************************/

int Menor(int a, int b)
{
	if (a <= b)
		return a;

	return b;
}

int n_rumores_disseminados(pessoa p[], int id_pessoa, int tpl)
{
    int nrd;
    
    if (!(((nrd = p[id_pessoa].extroversao/10) <= p[id_pessoa].crc->card) && (nrd <= tpl)))
        nrd = Menor(p[id_pessoa].crc->card, tpl);

    return nrd;
}

int tempo_permanencia(pessoa p[], int id_pessoa)
{
    int tpl;
    
    srand(time(NULL) * id_pessoa);
    if ( !((tpl = ( ((p[id_pessoa].paciencia)/10) + (rand() % 9)-2 )) > 1) )
        tpl = 1;
    
    return tpl;
}

int tempo_deslocamento(pessoa p[], local l[], int id_pessoa, int id_local, int id_local_destino)
{
    int tdl, velocidade, distancia, x, y;
    
    if(Menor(l[id_local].localizacao->x, l[id_local_destino].localizacao->x) 
        == l[id_local].localizacao->x)
    {
        x = l[id_local_destino].localizacao->x - l[id_local].localizacao->x;
    }
    else
        x = l[id_local].localizacao->x - l[id_local_destino].localizacao->x;
        
    if(Menor(l[id_local].localizacao->y, l[id_local_destino].localizacao->y) 
        == l[id_local].localizacao->y)
    {
        y = l[id_local_destino].localizacao->y - l[id_local].localizacao->y;
    }
    else
        y = l[id_local].localizacao->y - l[id_local_destino].localizacao->y;
        
    distancia = (x*x) + (y*y);
    distancia = sqrt(distancia);

    velocidade = 100 - (p[id_pessoa].idade - 40);
    
    tdl = distancia/velocidade;
    
    return tdl;
}

void finalizar_mundo(mundo *m, lef_t *LEF, pessoa p[], local l[])
{
    int id_pessoa, id_local;
    
    for (id_pessoa = 0; id_pessoa < m->n_pessoas; id_pessoa++)
	    destroi_conjunto(p[id_pessoa].crc);
	    
	for (id_local = 0; id_local < m->n_locais; id_local++)
	{
        free(l[id_local].localizacao);

	    destroi_fila(l[id_local].fila);
            
        destroi_conjunto(l[id_local].pessoas_no_lugar);
	}

    destroi_conjunto(m->rumores);
    free(m);
	
	destroi_lef(LEF);

    return;
}

evento_t *cria_evento (int id_pessoa, int id_local, int tempo, int tipo)
{
    evento_t *evento;
    if ((evento = (evento_t*)malloc(sizeof(evento_t))) == NULL)
        return NULL;
        
    evento->tempo = tempo;
    evento->tipo = tipo;
    evento->tamanho = sizeof(dados_evento);
    
    if ((evento->dados = (dados_evento*)malloc(evento->tamanho)) == NULL)
        return NULL;
        
    ((dados_evento*)evento->dados)->id_pessoa = id_pessoa;
    ((dados_evento*)evento->dados)->id_local = id_local;
    
    return evento;
}

int organizar_eventos(mundo *m, lef_t *LEF, pessoa p[], local l[])
{
    int id_pessoa, id_local, tempo, tpl, nrd, primeiro_fila, id_local_destino, tdl, i = 0;
    int pessoa_escuta[30];
    int rumores_disseminacao[m->n_rumores];
    
    evento_t *evento, *evento_aux_dis, *evento_aux_saida, *evento_aux_cheg;
    
    while (LEF->primeiro != NULL)
    {
        evento = obtem_primeiro_lef(LEF);
        
        if ((m->tempo_atual = evento->tempo) >= m->tempo_fim_do_mundo)
            evento->tipo = 4;
        
        id_pessoa = ((dados_evento*)evento->dados)->id_pessoa;
        id_local = ((dados_evento*)evento->dados)->id_local;

        srand(time(NULL) * i);

        switch (evento->tipo)
        {
            case (1) : /*CHEGADA*/
            
                if ((insere_conjunto(l[id_local].pessoas_no_lugar, id_pessoa)) == 1)
                {    /*se a pessoa foi inserida no local é pq tem lugar...*/
                
                    printf("%6d:CHEGA Pessoa %4d Local %2d ", m->tempo_atual, id_pessoa, id_local);
                    printf("(%2d/%2d), ENTRA\n", cardinalidade(l[id_local].pessoas_no_lugar)-1, l[id_local].lotacao_maxima);
                    
                    /*CALCULAR TEMPO DE PERMANENCIA NO LOCAL...*/
                    
                    tpl = tempo_permanencia(p, id_pessoa);
                    
                    /*CALCULAR NUMERO DE RUMORES A SEREM DISSEMINADOS*/
                    
                    nrd = n_rumores_disseminados(p, id_pessoa, tpl);
                    
                    /*******CRIAR EVENTO DE DISSEMINACAO********/
                    
                    tempo = m->tempo_atual + rand() % tpl;
                    
                    if ((evento_aux_dis = cria_evento(id_pessoa, id_local, tempo, 3)) == NULL)
                        return 0;
                        
                    /*CRIA CONJUNTO RUMOR COM BASE NO CRC DA PESSOA*/
                    
                    if ((((dados_evento *)evento_aux_dis->dados)->cj_rumor = cria_subconjunto(p[id_pessoa].crc, nrd)) == NULL)
                        return 0;
                        
                    adiciona_ordem_lef(LEF, evento_aux_dis);
                    
                    /*******CRIAR EVENTO DE SAIDA********/
                    
                    tempo = m->tempo_atual + tpl;
                    
                    if ((evento_aux_saida = cria_evento(id_pessoa, id_local, tempo, 2)) == NULL)
                        return 0;
                        
                    adiciona_ordem_lef(LEF, evento_aux_saida);
                    
                }
                else /*caso nao haja lugar*/
                {
                    if( ( (p[id_pessoa].paciencia/4) - tamanho_fila(l[id_local].fila) ) > 0 )
                    {
                        insere_fila (l[id_local].fila, id_pessoa);
    	                
    	                printf("%6d:CHEGA Pessoa %4d Local %2d ", m->tempo_atual, id_pessoa, id_local);
    	                printf("(%2d/%2d), ", cardinalidade(l[id_local].pessoas_no_lugar), l[id_local].lotacao_maxima);
    	                printf("FILA %2d\n", tamanho_fila(l[id_local].fila));
                    }
                    else /*caso a pessoa nao tenha paciencia para esperar na fila*/
                    {
                        printf("%6d:CHEGA Pessoa %4d Local %2d ", m->tempo_atual, id_pessoa, id_local);
                        printf("(%2d/%2d), DESISTE\n", cardinalidade(l[id_local].pessoas_no_lugar), l[id_local].lotacao_maxima);
                        
                        /*******CRIAR EVENTO DE SAIDA********/
                    
                        tempo = m->tempo_atual;
                        
                        if ((evento_aux_saida = cria_evento(id_pessoa, id_local, tempo, 2)) == NULL)
                            return 0;
                            
                        adiciona_inicio_lef(LEF, evento_aux_saida);
                    }
                }
                
            break;
            
            case (2) : /*SAIDA*/
            
                if ((retira_conjunto(l[id_local].pessoas_no_lugar, id_pessoa)) == 1)
                {
                    if ((retira_fila (l[id_local].fila, &primeiro_fila)) == 1)
                    {   /*se conseguiu retirar a primeira pessoa da fila...*/
                        
                        printf("%6d:SAIDA Pessoa %4d Local %2d ", m->tempo_atual, id_pessoa, id_local);
                        printf("(%2d/%2d), ", cardinalidade(l[id_local].pessoas_no_lugar)+1, l[id_local].lotacao_maxima);
                        printf("REMOVE FILA Pessoa %2d\n", primeiro_fila);
                        
                        /*******CRIAR EVENTO DE CHEGADA********/
                        
                        tempo = m->tempo_atual;
                        
                        if ((evento_aux_cheg = cria_evento(primeiro_fila, id_local, tempo, 1)) == NULL)
                            return 0;
                        
                        adiciona_inicio_lef(LEF, evento_aux_cheg);
                    }
                    else
                    {
                        printf("%6d:SAIDA Pessoa %4d Local %2d ", m->tempo_atual, id_pessoa, id_local);
                        printf("(%2d/%2d)\n", cardinalidade(l[id_local].pessoas_no_lugar)+1, l[id_local].lotacao_maxima);
                    }
                    
                    id_local_destino = rand() % 8;
                    
                    /*CALCULAR TEMPO DE DESLOCAMENTO ATÉ NOVO LOCAL...*/
                    
                    tdl = tempo_deslocamento(p, l, id_pessoa, id_local, id_local_destino);
                    
                    /*******CRIAR EVENTO DE CHEGADA********/
                        
                    tempo = m->tempo_atual + tdl/15;
                        
                    if ((evento_aux_cheg = cria_evento(id_pessoa, id_local_destino, tempo, 1)) == NULL)
                        return 0;
                        
                    adiciona_ordem_lef(LEF, evento_aux_cheg);
                }
            
            break;
            
            case (3) : /*DISSEMINACAO*/

                int a, j = 0;
                for (a = 0; a < m->n_pessoas; a++)
                {   /*passa por todas as pessoas e verifica quem esta no local...*/
                
                    if(pertence(l[id_local].pessoas_no_lugar, a) && a != id_pessoa)
                    {
                	        pessoa_escuta[j] = a;
                	        j++;
                    }
                }
                
                int b, z = 0;
                for (b = 0; b < m->n_rumores; b++)
                {   /*passa por todos os rumores e verifica quais estao no cj_rumor...*/

                    if (pertence(((dados_evento *)evento->dados)->cj_rumor, b))
                	{
                	    rumores_disseminacao[z] = b;
                	    z++;
                	}
                }
                
                if (j > 0) /*se houver alguem pra escutar...*/
                {
                    printf("%6d:RUMOR Pessoa %4d Local %2d ", m->tempo_atual, id_pessoa, id_local);
                    
                    b = 0;
                    a = 0;

                    while (a < j)
                    {
                        if ((b < z) && ((rand () % 101) < p[pessoa_escuta[a]].extroversao))
                        {
                            if (insere_conjunto(p[pessoa_escuta[a]].crc, rumores_disseminacao[b]))
                            {
                                printf("(P%d:R%d) ", pessoa_escuta[a], rumores_disseminacao[b]);
                                b++;

                                    for (i = a; i <= j-1; i++)
					                    pessoa_escuta[i] = pessoa_escuta[i+1];

                                    j--;
                                
                                a = -1;
                            }
                        }
                    a++;
                    }
                    printf("\n");
                }

                destroi_conjunto(((dados_evento *)evento->dados)->cj_rumor);
                
            break;
            
            case (4) : /*FIM DO MUNDO*/
            
                printf("%6d: FIM DO MUNDO \n", m->tempo_fim_do_mundo);
                return 0;
            
            break;
            
        }
    
    free(evento->dados);
    free(evento);
    i++;
    }

    return 0;
}


int main()
{
    evento_t *evento;
    
    lef_t *LEF;
    if ((LEF = cria_lef()) == NULL)
		return 0;
	
    mundo *m;
	if ((m = (mundo*)malloc(sizeof(mundo))) == NULL)
        return 0;
        
    m->tempo_atual = 0;
    m->n_locais = 8;
	m->n_pessoas = 100;
	m->n_rumores = 30;
	m->tamanho_mundo = 20000;
	m->tempo_fim_do_mundo = 34944;
    
    pessoa p[m->n_pessoas];
	local l[m->n_locais];
	
	if ((m->rumores = cria_conjunto(m->n_rumores)) == NULL)
		return 0;
	
	for (int i = 0; i < m->n_rumores; i++)
	{
	    if ( (insere_conjunto(m->rumores, i)) == 0 )
            return 0;
	}
	
    int id_pessoa; /*identificacao da pessoa*/
	for (id_pessoa = 0; id_pessoa < m->n_pessoas; id_pessoa++)
	{
        srand(time(NULL) * id_pessoa);

	    p[id_pessoa].extroversao = (rand() % 86) + 15;
	    p[id_pessoa].paciencia = rand() % 101;
	    p[id_pessoa].idade = (rand() % 83) + 18;
	        
	    if ((p[id_pessoa].crc = cria_subconjunto(m->rumores, ((rand() % 5) + 1))) == NULL)
	        return 0;
	}
	
	int id_local; /*identificacao do local*/
	for (id_local = 0; id_local < m->n_locais; id_local++)
	{
        srand(time(NULL) * id_local);

        if ((l[id_local].localizacao = (coordenadas*)malloc(sizeof(coordenadas))) == NULL)
            return 0;
        
        l[id_local].localizacao->x = (rand() % m->tamanho_mundo);
	    l[id_local].localizacao->y = (rand() % m->tamanho_mundo);
	    
	    l[id_local].lotacao_maxima = (rand() % 26) +5;
	    
	    if ((l[id_local].fila = cria_fila()) == NULL)
            return 0;
            
        if ((l[id_local].pessoas_no_lugar = cria_conjunto(l[id_local].lotacao_maxima)) == NULL)
	        return 0;
	}
	
/********************************************************************/

    int tempo;
	for (id_pessoa = 0; id_pessoa < m->n_pessoas; id_pessoa++)
    {
	    srand(time(NULL) * id_pessoa);
        
        id_local = rand() % 8;
        tempo = rand() % 96*7;
        
        if ((evento = cria_evento(id_pessoa, id_local, tempo, 1)) == NULL)
            return 0;
            
        adiciona_ordem_lef(LEF, evento);
    }
    
    organizar_eventos(m, LEF, p, l);
    
    /*Finalizar mundo...*/
    
    finalizar_mundo(m, LEF, p, l);
    
/********************************************************************/

	return 0;
}

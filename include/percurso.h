#include "../include/populacao.h"
#include "../include/mapa.h"

#ifndef _percurso_h_
#define _percurso_h_

#define qtd_geracoes 100000

typedef struct {
    Grafo *G;
    Populacao *P;
}Percurso;


int encontra_menor_divisor(int num);

Percurso *cria_percurso();
void apaga_percurso(Percurso *Per);

int pesquisa_valor(int *v, int tam, int valor);
void printa_vetor(int *v, int tam);
void percorre(Individuo *I, int **matriz, int qtd_genes, int ini);
void calcula_eficiencia(Individuo *I);
void calcula_mutacao(Individuo *I);
double calcula_eficiencia_media(Populacao *P);
double calcula_taxa_cruzamento(Populacao *P);
void treina(Percurso *Per);
void salva_melhor_individuo(Populacao *P);
void recupera_melhor_individuo(Populacao *P);
void realiza_mutacoes(Populacao *P, Individuo *I, double chance_de_acerto);
void cruza(Populacao *P, Individuo *I1, Individuo *I2, double chance_de_acerto);

#endif
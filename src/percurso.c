#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "../include/populacao.h"
#include "../include/mapa.h"
#include "../include/percurso.h"

int encontra_menor_divisor(int num){
    if(num == 1){
        return 1;
    }
    for(int i = 2; i < num/2 ; i ++){
        if(num % i == 0){
            return i;
        }
    }
    return num;
}

Percurso *cria_percurso(){
    Percurso *Per = (Percurso*) malloc(sizeof(Percurso));
    Per->G = cria_grafo();
    preenche_grafo(Per->G);
    Per->P = cria_população();
    return Per;
}

void apaga_percurso(Percurso *Per){
    if(Per == NULL){
        return;
    }
    apaga_grafo(Per->G);
    apaga_populacao(Per->P);
    free(Per);
}

int pesquisa_valor(int *v, int tam, int valor){
    for(int i = 0; i < tam; i ++){
        if(v[i] == valor){
            return i;
        }
    }
    return -1;
}

void printa_vetor(int *v, int tam){
    for(int i = 0; i < tam; i ++){
        printf("%d", v[i]);
        if(i < tam - 1){
            printf(", ");
        }
    }
    printf("\n");
}

void percorre(Individuo *I, int **matriz, int qtd_genes, int ini){

    int aux[qtd_genes];
    int indice = pesquisa_valor(I->genes, qtd_genes, ini);
    int i = 0;

    for(int j = indice; j < qtd_genes; j ++){
        aux[i] = I->genes[j];
        i ++;
    }
    for(int j = 0; j < indice; j ++){
        aux[i] = I->genes[j];
        i ++;
    }

    for(int i = 0; i < (qtd_genes - 1); i ++){
        I->distancia += matriz[aux[i]][aux[i + 1]];
        I->qtd_vertices ++;
    }
    I->distancia += matriz[aux[qtd_genes - 1]][aux[0]];
    I->qtd_vertices ++;
}

void calcula_eficiencia(Individuo *I){
    I->eficiencia = ((I->distancia)+(I->qtd_vertices * 100.0)) / (I->qtd_vertices * 100.0);
}
void calcula_mutacao(Individuo *I){
    I->taxa_de_mutacao = 6*(100 - (100.0 * pow(2.718, -0.07 * I->eficiencia)));
}
double calcula_eficiencia_media(Populacao *P){
    double aux = 0;
    for(int i = 0; i < tam_populacao; i ++){
        aux += P->I[i].eficiencia;
    }
    return aux / tam_populacao;
}
double calcula_taxa_cruzamento(Populacao *P){
    return 4*(100 - (100.0 * pow(2.718, -0.07 * P->eficiencia_media)));
}

void treina(Percurso *Per){
    for(int i = 0; i < tam_populacao; i ++){
        Per->P->I[i].distancia = 0;
        Per->P->I[i].qtd_vertices = 0;
        Per->P->I[i].taxa_de_mutacao = 0;
        Per->P->I[i].eficiencia = 10000;
        if(i > 0){
            realiza_mutacoes(Per->P, &Per->P->I[i], rand() % 10000 / 100.0);
            if(i < tam_populacao - 1){
                cruza(Per->P, &Per->P->I[i], &Per->P->I[i + 1], rand() % 10000 / 100.0);
            }
        }
        percorre(&Per->P->I[i], Per->G->Ma->M, Per->P->total_vertices, 0);
        calcula_eficiencia(&Per->P->I[i]);
        calcula_mutacao(&Per->P->I[i]);
    }
    Per->P->eficiencia_media = calcula_eficiencia_media(Per->P);
    Per->P->taxa_de_cruzamento =  calcula_taxa_cruzamento(Per->P);
    Per->P->qtd_geracao ++;
    salva_melhor_individuo(Per->P);
    recupera_individuo(Per->P);
}

void salva_melhor_individuo(Populacao *P){
    double aux = P->I[0].eficiencia;
    int indice = 0;
    for(int i = 1; i < tam_populacao; i ++){
        if(aux > P->I[i].eficiencia){
            aux = P->I[i].eficiencia;
            indice = i;
        }
    }
    if(P->I[0].eficiencia == P->I[indice].eficiencia){
        P->estaguinamento ++;
        return;
    }else{
        P->estaguinamento = 0;
    }

    FILE *F = fopen("melhor_individuo.txt", "w");
    fprintf(F, "%d", P->I[indice].nome);
    for(int i = 0; i < P->total_vertices; i ++){
        fprintf(F, " %d", P->I[indice].genes[i]);
    }
    fclose(F);
}

void recupera_individuo(Populacao *P){

    FILE *F = fopen("melhor_individuo.txt", "r");

    int nome;
    int genes[P->total_vertices];

    fscanf(F, "%d", &nome);
    for(int i = 0; i < P->total_vertices; i ++){
        fscanf(F, "%d", &genes[i]);
    }

    fclose(F);

    P->I[0].nome = nome;
    P->I[tam_populacao - 1].nome = rand() % 1000000;
    for(int i = 0; i < P->total_vertices; i ++){
        P->I[0].genes[i] = genes[i];
        P->I[tam_populacao - 1].genes[i] = genes[i];
    }
}

void realiza_mutacoes(Populacao *P, Individuo *I, double chance_de_acerto){
    if(chance_de_acerto < I->taxa_de_mutacao){
        return;
    }

    for(int i = 0; i < rand() % 10 + 1; i ++){
        int ind1 = rand() % P->total_vertices;
        int ind2 = rand() % P->total_vertices;
        
        int aux = I->genes[ind1];
        I->genes[ind1] = I->genes[ind2];
        I->genes[ind2] = aux;
    }
}

void cruza(Populacao *P, Individuo *I1, Individuo *I2, double chance_de_acerto){
    if(chance_de_acerto < P->taxa_de_cruzamento){
        return;
    }
    int genes1[2 * P->total_vertices];
    int genes2[2 * P->total_vertices];
    int limite = P->total_vertices;
    int j = 0;
    for(int i = 0; i < 2 * limite; i ++){
        genes1[i] = I1->genes[j];
        genes2[i] = I2->genes[j];
        j ++;
        if(j == limite)
            j = 0;
    }
    reseta_vetor(I1->genes, limite);
    reseta_vetor(I2->genes, limite);
    int divisor = P->fator_cruzamento;
    for(int i = divisor; i < 2 * divisor; i ++){
        I1->genes[i] = genes2[i];
        I2->genes[i] = genes1[i];
    }
    j = 2 * divisor;
    for(int i = j; i < 5 * divisor + (limite % 3); i ++){
        if(pesquisa_valor(I1->genes, P->total_vertices, genes1[i]) == -1){
            I1->genes[j] = genes1[i];
            j ++;
        }
        if(j == limite)
            j = 0;
    }
    j = 2 * divisor;
    for(int i = j; i < 5 * divisor + (limite % 3); i ++){
        if(pesquisa_valor(I2->genes, P->total_vertices, genes2[i]) == -1){
            I2->genes[j] = genes2[i];
            j ++;
        }
        if(j == limite)
            j = 0;
    }
}

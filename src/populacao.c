#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "../include/populacao.h"
#include "../include/mapa.h"
#include "../include/percurso.h"

int verifica_se_esta_no_vetor(int *v, int tam, int valor){
    for(int i = 0; i < tam; i ++){
        if(v[i] == valor){
            return 1;
        }
    }
    return 0;
}

void reseta_vetor(int *v, int tam){
    for(int i = 0; i < tam; i ++){
        v[i] = -1;
    }
}

void preenche_vetor_sem_repeticao(int *v, int tam, int intervalo){
    reseta_vetor(v, tam);
    for(int i = 0; i < tam; i ++){
        while(1){
            int novo_valor = rand() % intervalo;
            if(!verifica_se_esta_no_vetor(v, tam, novo_valor)){
                v[i] = novo_valor;
                break;
            }
        }
    }
}

int calcula_fator_cruzamento(int qtd_genes){
    return (2 *(qtd_genes/3)  - 1) - ((qtd_genes/3) - 1);
}

Populacao *cria_população(){
    Populacao *P = (Populacao*) malloc(sizeof(Populacao));
    P->eficiencia_media = 0;
    P->total_vertices = contar_linhas("entrada.txt");
    P->qtd_geracao = 0;
    P->taxa_de_cruzamento = 0;
    P->estaguinamento = 0;
    P->fator_cruzamento = calcula_fator_cruzamento(P->total_vertices);
    P->melhor_eficiencia_anterior = 100000;
    P->I = (Individuo*) malloc(tam_populacao * sizeof(Individuo));
    for(int i = 0; i < tam_populacao; i ++){
        P->I[i].nome = rand () % 1000000;
        P->I[i].eficiencia = 0;
        P->I[i].qtd_vertices = 0;
        P->I[i].taxa_de_mutacao = 0;
        P->I[i].distancia = 0;
        P->I[i].genes = (int *) malloc(P->total_vertices * sizeof(int));
        preenche_vetor_sem_repeticao(P->I[i].genes, P->total_vertices, P->total_vertices);
    }
    return P;
}

void printa_individuo(Individuo *I, int qtd_genes){
    printf("\t----------------------------------------------------------------\n");
    printf("\tNome : %d\n", I->nome);
    printf("\tGenes : [ ");
    for(int i = 0; i < qtd_genes; i ++){
        printf("%d", I->genes[i]);
        if(i < qtd_genes - 1){
            printf(", ");
        }
    }
    printf(" ]\n");
    printf("\tDistância: %d\n", I->distancia);
    printf("\tTaxa de mutação : %.2lf\n", I->taxa_de_mutacao);
    printf("\tEficiência : %.4lf\n", I->eficiencia);
    printf("\tQtd cidades : %d\n", I->qtd_vertices);
    printf("\t----------------------------------------------------------------\n");
}

void printa_populacao(Populacao *P){
    if(P == NULL){
        return;
    }
    printf("______________________________________________________________________________________\n");
    printf("Geração : %d\n", P->qtd_geracao);
    printf("Qtd cidades : %d\n", P->total_vertices);
    printf("EficiÊncia média : %.4lf\n", P->eficiencia_media);
    printf("Taxa de cruzamento : %.2lf\n", P->taxa_de_cruzamento);
    printf("Individuos : \n");
    for(int i = 0; i < tam_populacao; i ++){
        printa_individuo(&P->I[i], P->total_vertices);
    }
    printf("______________________________________________________________________________________\n");
}

void apaga_populacao(Populacao *P){
    for(int i = 0; i < tam_populacao; i ++){
        free(P->I[i].genes);
    }
    free(P->I);
    free(P);
}


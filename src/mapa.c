#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <graphviz/cgraph.h>
#include "../include/populacao.h"
#include "../include/mapa.h"
#include "../include/percurso.h"

int contar_linhas(const char *nome_do_arquivo) {
    FILE *arquivo;
    int contador = 0;
    char linha[4048]; 

    arquivo = fopen(nome_do_arquivo, "r");

    if (arquivo == NULL) {
        fprintf(stderr, "Não foi possível abrir o arquivo '%s'\n", nome_do_arquivo);
        return -1; 
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        int numero;

        if (sscanf(linha, "%d", &numero) == 1) {
            contador++;
        }
    }

    fclose(arquivo);

    return contador;
}

int erro_na_matriz(){
    int qtd_valores = contar_linhas("entrada.txt") * contar_linhas("entrada.txt");

    FILE *F = fopen("entrada.txt", "r");
    if(F == NULL){
        return 1;
    }

    int qtd_lidos = 0;
    int v;
    while(fscanf(F, "%d", &v) == 1){
        qtd_lidos ++;
    }
    fclose(F);
    if(qtd_lidos == qtd_valores)
        return 0;
    else
        return 1;
}

int verifica_erro_vertices(){
    int num_cidades = 0;
    char cidade[64];
    FILE *F = fopen("vertices.txt", "r");
    if(F == NULL){
        return 1;
    }
    while(!feof(F)){
        fgets(cidade, 64, F);
        num_cidades ++;
    }
    fclose(F);
    if(contar_linhas("entrada.txt") > num_cidades){
        return 1;
    }
    return 0;
}

int **cria_matriz(int L, int C){
    int **matriz;

    matriz = (int **)malloc(L * sizeof(int *));
    
    for (int i = 0; i < L; i++) {
        matriz[i] = (int *)malloc(C * sizeof(int));
    }

    return matriz;
}

void apaga_matriz(int **matriz, int L){
    for (int i = 0; i < L; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

Grafo *cria_grafo(){
    Grafo *G = (Grafo*) malloc(sizeof(Grafo));
    G->A = NULL;
    G->Ma = NULL;
    G->V = NULL;
    G->qtd_arestas = 0;
    G->qtd_vertices = 0;
    return G;
}

void preenche_vertices(Grafo *G){

    G->V = (Vertice*) malloc(G->qtd_vertices * sizeof(Vertice));
    
    FILE *F = fopen("vertices.txt", "r");
    for(int i = 0; i < G->qtd_vertices; i ++){
        fgets(G->V[i].nome, 32, F);
        G->V[i].nome[strcspn(G->V[i].nome, "\n")] = 0;
    }
    fclose(F);
}

void preenche_matriz(Grafo *G){

    G->Ma = (Matriz_adjacencia *) malloc(sizeof(Matriz_adjacencia));
    G->Ma->M = cria_matriz(G->qtd_vertices, G->qtd_vertices);

    FILE *F = fopen("entrada.txt", "r");
    for(int i = 0; i < G->qtd_vertices; i ++){
        for(int j = 0; j < G->qtd_vertices; j ++){
            fscanf(F, "%d", &G->Ma->M[i][j]);
        }
    }
    fclose(F);
}

void insere_aresta(Grafo *G, Aresta *A){
    if(G->qtd_arestas == 0){
        G->A = (Aresta*) malloc(sizeof(Aresta));
        G->qtd_arestas ++;
        strcpy(G->A[0].v_ini, A->v_ini);
        strcpy(G->A[0].v_fim, A->v_fim);
        G->A[0].distancia = A->distancia;
        return;
    }
    G->qtd_arestas ++;
    G->A = (Aresta*) realloc(G->A, (G->qtd_arestas) * sizeof(Aresta));
    strcpy(G->A[G->qtd_arestas - 1].v_ini, A->v_ini);
    strcpy(G->A[G->qtd_arestas - 1].v_fim, A->v_fim);
    G->A[G->qtd_arestas - 1].distancia = A->distancia;
}

void preenche_arestas(Grafo *G){

    Aresta *A = (Aresta*) malloc(sizeof(Aresta));
    
    for(int i = 0; i < G->qtd_vertices; i ++){
        for(int j = 0; j < G->qtd_vertices; j ++){

            strcpy(A->v_ini, G->V[i].nome);
            strcpy(A->v_fim, G->V[j].nome);
            A->distancia = G->Ma->M[i][j];
            
            insere_aresta(G, A);
        }
    }
    free(A);
}

void preenche_grafo(Grafo *G){

    G->qtd_vertices = contar_linhas("entrada.txt");
    G->qtd_arestas = 0;
    G->Ma = NULL;
    G->V = NULL;
    G->A = NULL;

    preenche_vertices(G);
    preenche_matriz(G);
    preenche_arestas(G);
    return;
}

void apaga_grafo(Grafo *G){
    if(G != NULL){
        if(G->Ma != NULL){
            apaga_matriz(G->Ma->M, G->qtd_vertices);
            free(G->Ma);
        }
        if(G->A != NULL){
            free(G->A);
        }
        if(G->V != NULL){
            free(G->V);
        }
        free(G);
    }

    return;
}

void printa_vertices(Grafo *G){
    printf("-------------------------------------------\n");
    printf("Vertices :\n\n");
    for(int i = 0; i < G->qtd_vertices; i ++){
        printf("\t%d - %s\n", i, G->V[i].nome);
    }
    printf("-------------------------------------------\n");
}

void printa_matriz(int **matriz, int L, int C){
    printf("\t");
    for(int i = 0; i < C; i ++){
        printf("%d\t", i);
    }
    printf("\n_________________________________________________________________________________________\n");
    for(int i = 0; i < L; i ++){
        printf("%d |\t", i);
        for(int j = 0; j < C; j ++){
            printf("%d\t", matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printa_arestas(Grafo *G){
    printf("-------------------------------------------\n");
    printf("Arestas : \n\n");
    for(int i = 0; i < G->qtd_arestas; i ++){
        printf("\t%s --(%d)--> %s\n", G->A[i].v_ini, G->A[i].distancia, G->A[i].v_fim);
    }
    printf("-------------------------------------------\n");
}

void printa_grafo(Grafo *G){
    if(G != NULL){
        if(G->Ma != NULL){
            printf("-------------------------------------------\n");
            printf("Matriz de adjacencia : \n\n");
            printa_matriz(G->Ma->M, G->qtd_vertices, G->qtd_vertices);
            printf("-------------------------------------------\n");
        }
        if(G->V != NULL){
            printa_vertices(G);
        }
        if(G->A != NULL){
            printa_arestas(G);
        }
    }
    return;
}

void printa_percurso(Individuo *I, Grafo *G){
    for(int i = 0; i < I->qtd_vertices - 1; i ++){
        printf("\t%s --(%d)--> %s\n", G->V[I->genes[i]].nome, G->Ma->M[I->genes[i]][I->genes[i+1]] ,G->V[I->genes[i + 1]].nome);
    }
    printf("\t%s --(%d)--> %s\n", G->V[I->genes[I->qtd_vertices - 1]].nome, G->Ma->M[I->genes[I->qtd_vertices - 1]][I->genes[0]], G->V[I->genes[0]].nome);
}

void criar_grafo_percurso(char *nome_arquivo, Individuo *I, Grafo *G) {
    Grafo *aux = cria_grafo();
    Aresta *A = (Aresta*) malloc(sizeof(Aresta));
    for(int i = 0; i < I->qtd_vertices; i ++){
        for(long unsigned int j = 0; j < strlen(G->V[i].nome); j ++){
            if(G->V[i].nome[j] == ' ' || G->V[i].nome[j] == '-'){
                G->V[i].nome[j] = '_';
            }
        }
    }
    for(int i = 0; i < I->qtd_vertices - 1; i ++){
        strcpy(A->v_ini, G->V[I->genes[i]].nome);
        strcpy(A->v_fim, G->V[I->genes[i+1]].nome);
        A->distancia = G->Ma->M[I->genes[i]][I->genes[i+1]];
        insere_aresta(aux, A);
        aux->qtd_vertices ++;
    }
    aux->qtd_vertices ++;
    strcpy(A->v_ini, G->V[I->genes[I->qtd_vertices - 1]].nome);
    strcpy(A->v_fim, G->V[I->genes[0]].nome);
    A->distancia = G->Ma->M[I->genes[I->qtd_vertices - 1]][I->genes[0]];
    insere_aresta(aux, A);
    free(A);

    gerar_grafo_PNG(aux, nome_arquivo);
    apaga_grafo(aux);
}

void gerar_grafo_PNG(Grafo *grafo, char *nomeArquivo) {
    FILE *arquivoDOT;
    arquivoDOT = fopen("grafo.dot", "w");

    if (arquivoDOT == NULL) {
        printf("Erro ao abrir o arquivo DOT.\n");
        return;
    }

    fprintf(arquivoDOT, "digraph G {\n");

    for (int i = 0; i < grafo->qtd_arestas; i++) {
        fprintf(arquivoDOT, "  %s -> %s [label=\"%d\"];\n", grafo->A[i].v_ini, grafo->A[i].v_fim, grafo->A[i].distancia);
    }

    fprintf(arquivoDOT, "}\n");

    fclose(arquivoDOT);

    int distancia = 0;
    for(int i = 0; i < grafo->qtd_arestas; i ++){
        distancia += grafo->A[i].distancia;
    }

    char *arq = (char*) malloc(256 * sizeof(char));
    arq[0] = '\0';
    char aux[40];

    strcat(arq, "image/qtdV-");
    sprintf(aux, "%d", grafo->qtd_vertices);
    strcat(arq, aux);
    strcat(arq, "_dist-");
    sprintf(aux, "%d", distancia);
    strcat(arq, aux);
    strcat(arq, "_");
    strcat(arq, nomeArquivo);

    char comando[100];
    sprintf(comando, "dot -Tpng grafo.dot -o %s", arq);
    system(comando);

    free(arq);
}
#ifndef _mapa_h_
#define _mapa_h_

typedef struct{
    char nome[32];
    int qtd_in;
    int qtd_out;
}Vertice;

typedef struct{
    char v_ini[32];
    char v_fim[32];
    int distancia;
}Aresta;

typedef struct {
    int **M;
}Matriz_adjacencia;

typedef struct {
    Vertice *V;
    Aresta *A;
    Matriz_adjacencia *Ma;
    int qtd_vertices;
    int qtd_arestas;
}Grafo;

int contar_linhas(const char *nomeArquivo);
int erro_na_matriz();
int verifica_erro_vertices();

int **cria_matriz(int L, int C);
void apaga_matriz(int **matriz, int L);

Grafo *cria_grafo();
void preenche_vertices(Grafo *G);
void preenche_matriz(Grafo *G);
void insere_aresta(Grafo *G, Aresta *A);
void preenche_arestas(Grafo *G);
void preenche_grafo(Grafo *G);
void apaga_grafo(Grafo *G);

void printa_vertices(Grafo *G);
void printa_matriz(int **matriz, int L, int C);
void printa_arestas(Grafo *G);
void printa_grafo(Grafo *G);
void printa_percurso(Individuo *I, Grafo *G);

void criar_grafo_percurso(char *nome_arquivo, Individuo *I, Grafo *G);
void gerar_grafo_PNG(Grafo *grafo, char *nomeArquivo);

#endif
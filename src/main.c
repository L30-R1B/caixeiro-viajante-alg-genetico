#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include "../include/populacao.h"
#include "../include/mapa.h"
#include "../include/percurso.h"

int main(){
    srand(time(NULL));
    struct timeval inicio, fim;
    long double comecou, acabou;

    if(erro_na_matriz()){
        printf("MATRIZ NÃO ESTÁ NA FORMA ESPECIFICADA ! ! !\n");
        return 1;
    }else if(verifica_erro_vertices()){
        printf("O ARQUIVO \"vertices.txt\" NÃO É VÁLIDO ! ! !\n");
        return 1;
    }

    gettimeofday(&inicio, NULL);
    Percurso *Per = cria_percurso();
    if(Per->P->total_vertices <= 32)
        gerar_grafo_PNG(Per->G, "Grafo_original.png");
    while(Per->P->qtd_geracao < (Per->G->qtd_vertices * qtd_geracoes)){
        treina(Per);
        if(Per->P->qtd_geracao % (qtd_geracoes/(qtd_geracoes/100000)) == 0){
            printf("Melhor individuo até a geração %d : \n", Per->P->qtd_geracao);
            printa_individuo(&Per->P->I[0], Per->P->total_vertices);
        }
    }
    printf("\n__________________________________________________\n");
    printf("MELHOR INDIVIDUO DE TODAS AS %d GERAÇÕES : \n", Per->P->qtd_geracao);
    printa_individuo(&Per->P->I[0], Per->P->total_vertices);
    printf("__________________________________________________\n");
    printa_percurso(&Per->P->I[0], Per->G);
    printf("__________________________________________________\n");
    criar_grafo_percurso("Melhor_caminho.png", &Per->P->I[0], Per->G);
    gettimeofday(&fim, NULL);

    comecou = inicio.tv_sec + inicio.tv_usec / 1000000.0L;
    acabou = fim.tv_sec + fim.tv_usec / 1000000.0L;

    printf("-----\nTempo de execução : %Lf s\n-----\n", acabou - comecou);

    apaga_percurso(Per);

    system("rm melhor_individuo.txt");
    system("rm grafo.dot");

    return 0;
}

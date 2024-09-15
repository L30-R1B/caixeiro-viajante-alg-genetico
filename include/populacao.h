#ifndef _populacao_h_
#define _populacao_h_

#define tam_populacao 16

typedef struct {
    int nome;
    int *genes;
    int qtd_vertices;
    int distancia;
    double taxa_de_mutacao;
    double eficiencia;
}Individuo;

typedef struct {
    Individuo *I;
    int total_vertices;
    int qtd_geracao;
    double melhor_eficiencia_anterior;
    double eficiencia_media;
    double taxa_de_cruzamento;
    int estaguinamento;
    int fator_cruzamento;
}Populacao;

int verifica_se_esta_no_vetor(int *v, int tam, int valor);
void reseta_vetor(int *v, int tam);
void preenche_vetor_sem_repeticao(int *v, int tam, int intervalo);
int calcula_fator_cruzamento(int qtd_genes);

Populacao *cria_população();
void printa_individuo(Individuo *I, int qtd_genes);
void printa_populacao(Populacao *P);
void apaga_populacao(Populacao *P);

#endif
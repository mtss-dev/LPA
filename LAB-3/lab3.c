#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "math.h"

//Aluno: Matheus Silva dos Santos
//Matricula: 22052573

//lista encadeada para os vizinhos
typedef struct lista_vizinhos{
    int id;
    struct lista_vizinhos *prox;
}lista_vizinhos_t;

//tipo de um nó na rede
typedef struct no {
    int id;
    double x;
    double y;
    lista_vizinhos_t* vizinhos;
} no_t;

//cria um nó da rede
no_t *criar_no(int id, double x, double y) {
    no_t *no = malloc(sizeof(no_t));
    if (no != NULL) {
        no->id = id;
        no->x = x;
        no->y = y;
        no->vizinhos = NULL;
    }
    return no;
}

// Adiciona um inteiro (id de um vizinho) ao início de uma lista encadeada (similar ao TP1/TP2)
// lista_vizinhos_t será uma lista de inteiros, portanto não há necessidade de criar um tipo
// “vizinho_t”.
bool lista_vizinhos_adicionar(int vizinho, lista_vizinhos_t **lista){
    lista_vizinhos_t *novo_vizinho = malloc(sizeof(lista_vizinhos_t));

    if (novo_vizinho == NULL) return false;

    novo_vizinho->id= vizinho;
    novo_vizinho->prox = *lista;
    *lista = novo_vizinho;

    return true;
}

// Imprime os números (ids) da lista se tiver vizinhos (similar ao TP1, mas no formato especificado)
void lista_vizinhos_imprimir(lista_vizinhos_t *lista){
    lista_vizinhos_t *aux = lista;

    while (aux != NULL){
        printf("%d ", aux->id);
        aux = aux->prox;
    }
}

//definição do tipo grafo
typedef no_t* grafo_t;

//Aloca memória para um vetor de nós.
//grafo_t pode ser definido como: typedef no_t* grafo_t;
grafo_t grafo_criar(int tam){
    grafo_t grafo = malloc(tam * sizeof(no_t));
    return grafo;
}

// Para cada nó (nó “i”) do vetor 
// de nós do grafo, compará-lo com todos os outros nós da rede (nó “j”,
// sendo j != i) e, caso a distância entre i e j seja menor que o raio de comunicação, adicione i à
// lista de vizinhos de j.
// Cálculo de distâncias Euclidianas:
// sqrt(pow(grafo[i].x - grafo[j].x, 2) + pow(grafo[i].y - grafo[j].y, 2))
void grafo_atualizar_vizinhos(int tam, double raio_comunicacao, grafo_t grafo){
    for(int i = 0; i < tam; i++){
        for(int j = 0; j < tam; j++){
            if(j != i){
                double distancia = sqrt(pow(grafo[i].x - grafo[j].x, 2) + pow(grafo[i].y - grafo[j].y, 2));
                if(distancia < raio_comunicacao){
                    lista_vizinhos_adicionar(grafo[i].id, &grafo[j].vizinhos);
                }
            }
        }
    }
}

// Para cada posição do vetor, executa a função lista_vizinhos_imprimir.
void grafo_imprimir(int tam, grafo_t grafo){
    if(grafo == NULL) return;

    for(int i = 0; i < tam; i++){
        printf("Nó %d: ", grafo[i].id);
        lista_vizinhos_imprimir(grafo[i].vizinhos);
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("Uso: ./lab3 <arquivo_entrada>\n");
        return 1;
    }

    FILE *arquivo;
    arquivo = fopen( argv[1] ,"r");

    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    int tam;
    double raio_comunicacao;

    fscanf(arquivo,"%d\t%lf\n",&tam,&raio_comunicacao);

    grafo_t grafo = grafo_criar(tam);

    for(int i = 0; i < tam; i++){
        int id;
        double x,y;
        fscanf(arquivo,"%d\t%lf\t%lf\n",&id,&x,&y);
        no_t* no = criar_no(id,x,y);
        grafo[i] = *no;
    }

    grafo_atualizar_vizinhos(tam,raio_comunicacao,grafo);

    grafo_imprimir(tam,grafo);

    fclose(arquivo);

    return 0;
}

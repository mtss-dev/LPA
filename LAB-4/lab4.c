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
    bool pacote_enviado;
} no_t;

//cria um nó da rede
no_t *criar_no(int id, double x, double y) {
    no_t *no = malloc(sizeof(no_t));
    if (no != NULL) {
        no->id = id;
        no->x = x;
        no->y = y;
        no->vizinhos = NULL;
        no->pacote_enviado = false;
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

typedef struct evento{
    int id;
    int tipo;
    double tempo; 
}evento_t;

evento_t *criaEvento(double tempo, int id){
    evento_t *evento = malloc(sizeof(evento_t));
    evento->tempo = tempo;
    evento->id = id;
    evento->tipo = 1;
    return evento;
}

typedef struct lista_eventos{
    evento_t *evento;
    struct lista_eventos *proximo;
}lista_eventos_t;

bool lista_eventos_adicionar_ordenado(evento_t *evento, lista_eventos_t **lista){
    lista_eventos_t *item_novo = malloc(sizeof(lista_eventos_t));

    //Verifica se foi possivel alocar memoria
    if (item_novo == NULL) return false;

    item_novo->evento = evento;

    //Verifica se a lista esta vazia
    if (*lista == NULL){
        //Se estiver vazia, adiciona no inicio
        item_novo->proximo = NULL;
        *lista = item_novo;
    }else{ //Se nao estiver vazia, adiciona ordenado
        lista_eventos_t *item = *lista;
        lista_eventos_t *item_anterior = NULL;

        //Percorre a lista ate encontrar um evento com tempo maior que o evento a ser adicionado
        while (item != NULL && item->evento->tempo < evento->tempo){
            item_anterior = item;
            item = item->proximo;
        }

        //Se o item anterior for nulo, significa que o evento a ser adicionado tem o menor tempo
        if (item_anterior == NULL){
            item_novo->proximo = *lista;
            *lista = item_novo;
        }else{ //Se nao, adiciona entre o item anterior e o item atual
            item_anterior->proximo = item_novo;
            item_novo->proximo = item;
        }
    }

    return true;
}

// Enquanto lista de eventos não for nulo
// É selecionado o primeiro evento e removido da lista de eventos
// Se o nó do evento atual ainda não mandou o pacote
// Para cada vizinho do nó atual
// É criado um evento de recebimento do pacote para o vizinho atual
// Em seguida, o evento é adiciona à lista de eventos
// Por fim, o pacote_enviado do nó atual é setado para true.
void simulacao_iniciar(lista_eventos_t **lista, grafo_t grafo){
    while(*lista != NULL){ //enquanto tiver eventos na lista
        lista_eventos_t *evento_atual = *lista; //pega o primeiro evento da lista
        *lista = evento_atual->proximo; //remove o evento da lista

        printf("[%3.6f] Nó %d recebeu pacote.\n",evento_atual->evento->tempo, evento_atual->evento->id);

        if(!grafo[evento_atual->evento->id].pacote_enviado){
            lista_vizinhos_t *vizinho_atual = grafo[evento_atual->evento->id].vizinhos; //pegando o vizinho do evento atual
            while(vizinho_atual != NULL){
                printf("\t--> Repassando pacote para o nó %d ...\n", vizinho_atual->id);

                //Se este vizinho nao enviou o pacote ainda, cria um evento para ele
                if(!grafo[evento_atual->evento->id].pacote_enviado){
                    evento_t *evento = criaEvento(evento_atual->evento->tempo + (0.1 + (vizinho_atual->id * 0.01)), vizinho_atual->id);
                    lista_eventos_adicionar_ordenado(evento, lista);
                }
                
                vizinho_atual = vizinho_atual->prox;
            }
            grafo[evento_atual->evento->id].pacote_enviado = true;
        }
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

    //grafo_imprimir(tam,grafo);

    evento_t* evento_inicial = criaEvento(1,0);
    lista_eventos_t *lista_eventos = NULL;
    lista_eventos_adicionar_ordenado(evento_inicial, &lista_eventos);

    simulacao_iniciar(&lista_eventos, grafo);

    fclose(arquivo);

    return 0;
}

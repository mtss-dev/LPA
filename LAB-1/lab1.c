#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

//Aluno: Matheus Silva dos Santos
//Matricula: 22052573

typedef struct evento{
    double tempo;
    int alvo;
    int tipo;
}evento_t;

evento_t *criaEvento(double tempo, int alvo, int tipo){
    evento_t *evento = malloc(sizeof(evento_t));
    evento->tempo = tempo;
    evento->alvo = alvo;
    evento->tipo = tipo;
    return evento;
}

typedef struct lista_eventos{
    evento_t *evento;
    struct lista_eventos *proximo;
}lista_eventos_t;


bool lista_eventos_adicionar_inicio(evento_t *evento, lista_eventos_t **lista){
    lista_eventos_t *item_novo = malloc(sizeof(lista_eventos_t));

    if (item_novo == NULL) return false;

    item_novo->evento = evento;

    item_novo->proximo = *lista;

    *lista = item_novo;

    return true;
}

bool lista_eventos_adicionar_fim(evento_t *evento, lista_eventos_t **lista){
    lista_eventos_t *item_novo = malloc(sizeof(lista_eventos_t));

    if (item_novo == NULL) return false;

    item_novo->evento = evento;

    item_novo->proximo = NULL;

    if (*lista == NULL){
        *lista = item_novo;
    }else{
        lista_eventos_t *item = *lista;
        while (item->proximo != NULL){
            item = item->proximo;
        }
        item->proximo = item_novo;
    }

    return true;
}

void lista_eventos_listar(lista_eventos_t *lista){
    lista_eventos_t *item = lista;

    while (item != NULL){
        printf("%3.6f\t%d\t%d\n", item->evento->tempo, item->evento->alvo, item->evento->tipo);
        item = item->proximo;
    }
}

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


int main(int argc, char *argv[])
{

    lista_eventos_t *lista = NULL;
    char* nome_arquivo = argv[1];
    FILE *arquivo = fopen(nome_arquivo, "r");

    double tempo;
    int alvo;
    int tipo;

    //Questão 1: Leitura dos eventos adicionando no inicio da lista
    // while (fscanf(arquivo, "%lf %d %d", &tempo, &alvo, &tipo) != EOF)
    // {
    //     evento_t *evento = criaEvento(tempo, alvo, tipo);
    //     lista_eventos_adicionar_inicio(evento, &lista);
    // }

    //Questão 2: Leitura dos eventos adicionando no fim da lista
    // while (fscanf(arquivo, "%lf %d %d", &tempo, &alvo, &tipo) != EOF)
    // {
    //     evento_t *evento = criaEvento(tempo, alvo, tipo);
    //     lista_eventos_adicionar_fim(evento, &lista);
    // }

    //Questão 3: Leitura dos eventos adicionando ordenado na lista
    while (fscanf(arquivo, "%lf %d %d", &tempo, &alvo, &tipo) != EOF)
    {
        evento_t *evento = criaEvento(tempo, alvo, tipo);
        lista_eventos_adicionar_ordenado(evento, &lista);
    }

    lista_eventos_listar(lista);

    fclose(arquivo);

    return 0;
}

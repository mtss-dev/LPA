#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"

//Aluno: Matheus Silva dos Santos
//Matricula: 22052573

typedef struct pessoa{
    char nome[51];
    long long int cpf;
    int idade;
}pessoa_t;

// Cria uma pessoa
pessoa_t *criaPessoa(char nome[51], long long int cpf, int idade){
    pessoa_t *pessoa = malloc(sizeof(pessoa_t));
    strcpy(pessoa->nome, nome);
    pessoa->cpf = cpf;
    pessoa->idade = idade;
    return pessoa;
}

typedef struct lista_pessoas{
    pessoa_t *pessoa;
    struct lista_pessoas *proximo;
}lista_pessoas_t;

//Adiciona uma pessoa no inicio da lista encadeada
bool lista_pessoas_adicionar(pessoa_t *pessoa, lista_pessoas_t **lista){
    lista_pessoas_t *nova_pessoa = malloc(sizeof(lista_pessoas_t));

    if (nova_pessoa == NULL) return false;

    nova_pessoa->pessoa = pessoa;

    nova_pessoa->proximo = *lista;

    *lista = nova_pessoa;

    return true;
}

//Imprime as pessoas na lista encadeada
void lista_pessoas_listar(lista_pessoas_t *lista){
    lista_pessoas_t* lista_pessoas = lista;

    while(lista_pessoas != NULL){
        pessoa_t* pessoa = lista_pessoas->pessoa;
        printf("- %s\t%lld\t%d\n",pessoa->nome,pessoa->cpf,pessoa->idade);
        lista_pessoas = lista_pessoas->proximo;
    }
}

typedef lista_pessoas_t** tabela_hash_t;

// Variavel global para o tamanho da tabela hash
int  tabela_hash_tam = 0;

// Cria uma tabela hash
tabela_hash_t tabela_hash_pessoas_criar() {
    tabela_hash_t tabela = (tabela_hash_t)malloc(sizeof(lista_pessoas_t*) * tabela_hash_tam);

    if (tabela == NULL) {
        return NULL;
    }

    for (int i = 0; i < tabela_hash_tam; i++) {
        tabela[i] = NULL;
    }

    return tabela;
}

// Função hash para pessoas (CPF)
int tabela_hash_pessoas_funcao(pessoa_t *pessoa){
    return pessoa->cpf % tabela_hash_tam;
}

// Adiciona uma pessoa na tabela hash
bool tabela_hash_pessoas_adicionar(pessoa_t *pessoa, tabela_hash_t tabela_hash){

    if(pessoa == NULL || tabela_hash == NULL){
        printf("Não foi possível adicionar a pessoa!\n");
        return false;
    }

    int posicao = tabela_hash_pessoas_funcao(pessoa);

    //quando nao tem nenhuma pessoa nesta posição (lista nao existe)
    if(tabela_hash[posicao] == NULL){
        lista_pessoas_t* lista_pessoas = NULL;
        lista_pessoas_adicionar(pessoa, &lista_pessoas);
        tabela_hash[posicao] = lista_pessoas;
    }else{ //quando ja tem uma pessoa nesta posição (lista existe)
        lista_pessoas_t* lista_pessoas = tabela_hash[posicao];
        lista_pessoas_adicionar(pessoa,&lista_pessoas);
        tabela_hash[posicao] = lista_pessoas;
    }

    return true;
}

// Lista todas as pessoas da tabela hash
void tabela_hash_pessoas_listar(tabela_hash_t tabela_hash){
    if(tabela_hash != NULL){
        for(int i = 0; i < tabela_hash_tam; i++){
            printf("POSIÇÃO %d DA TABELA HASH:\n",i);
            if(tabela_hash[i] != NULL){
                lista_pessoas_listar(tabela_hash[i]);
            }else{
                printf("- Nenhuma pessoa nesta posição!\n");
            }
        }
    }
}

int main(int argc, char *argv[])
{
     if (argc < 3) {
        printf("Uso: %s <tamanho_tabela> <arquivo_entrada>\n", argv[0]);
        return 1;
    }
    
    tabela_hash_tam = atoi(argv[1]);

    tabela_hash_t tabela_hash = tabela_hash_pessoas_criar(tabela_hash_tam);

    FILE* arq_in = fopen(argv[2], "r");

    pessoa_t* p = NULL;

    char nome[51];
    long long int cpf;
    int idade;

    while(fscanf(arq_in, "%50[^\t]\t%lld\t%d\n", nome, &cpf, &idade) != EOF){
        p = criaPessoa(nome,cpf,idade);
        tabela_hash_pessoas_adicionar(p,tabela_hash);
    }

    tabela_hash_pessoas_listar(tabela_hash);

    return 0;
}

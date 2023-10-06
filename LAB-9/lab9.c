#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"

//Aluno: Matheus Silva dos Santos
//Matricula: 22052573

// OBJETIVO: Exercitar a manipulação de arquivos/dados binários através da decodificação/divisão de um arquivo de vídeo MPEG. 

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Uso: %s <arquivo.mpg> <tamanho_parte>\n", argv[0]);
        return 1;
    }

    // Abre o arquivo mpeg de entrada
    FILE *mpg_file = fopen(argv[1], "rb");
    if (mpg_file == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    int i = 1;
    char nome_arquivo[20];

    // Abre o primeiro arquivo de saída (video_parte_1.mpg)
    sprintf(nome_arquivo, "video_parte_%d.mpg", i);
    FILE *mpg_file_parte = fopen(nome_arquivo, "wb");
    if (mpg_file_parte == NULL) {
        printf("Erro ao criar o arquivo de saída.\n");
        fclose(mpg_file);
        return 1;
    }

    unsigned int tamanho_parte = atoi(argv[2]) * 1024 * 1024;
    unsigned int tamanho_atual = 0;
    unsigned char buffer[4];

    while (fread(buffer, sizeof(unsigned char), 4, mpg_file) == 4) {
        // Se não for código de Sequence Header e não for o final do arquivo
        if (memcmp(buffer, "\x00\x00\x01\xB3", 4) != 0 && !feof(mpg_file)) {
            // Salva os 4 bytes lidos no arquivo de saída
            fwrite(buffer, sizeof(unsigned char), 4, mpg_file_parte);
            tamanho_atual += 4;
        }

        // Se o tamanho atual mais 4 bytes for maior que o tamanho máximo da parte
        if (tamanho_atual + 4 > tamanho_parte) {
            fclose(mpg_file_parte);
            
            // Abre um novo arquivo de saída (atualizando o contador i)
            sprintf(nome_arquivo, "video_parte_%d.mpg", ++i);
            mpg_file_parte = fopen(nome_arquivo, "wb");
            if (mpg_file_parte == NULL) {
                printf("Erro ao criar o arquivo de saída.\n");
                fclose(mpg_file);
                return 1;
            }
            // Escreve os 4 bytes no novo arquivo de saída
            fwrite(buffer, sizeof(unsigned char), 4, mpg_file_parte);
            // Atualiza o tamanho do arquivo atual para o tamanho do buffer recém-salvo (incrementando 4 bytes)
            tamanho_atual = 4;
        }

        // Se for o final do arquivo de entrada
        if (feof(mpg_file)) {
            fclose(mpg_file_parte);
            break;
        }
    }

    fclose(mpg_file);


    return 0;
}
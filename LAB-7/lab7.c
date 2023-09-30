#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "arpa/inet.h"

//Aluno: Matheus Silva dos Santos  
//Matrícula: 22052573

typedef struct png_chunk{
    int tamanho;
    char tipo[4];
} __attribute__ ((packed))png_chunk_t;

typedef struct png_chunk_IHDR{
    int largura;
    int altura;
    char bit_depth;
    char color_type;
    char compression_method;
    char filter_method;
    char interlace_method;

} __attribute__ ((packed))png_chunk_IHDR_t;


int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("Uso: %s <nome_arquivo>\n", argv[0]);
        return 1;
    }

    char* nome_arquivo = argv[1];

    FILE *png_file = fopen(nome_arquivo,"rb");

    png_chunk_t* png_chunk = malloc(sizeof(png_chunk_t));
    png_chunk_IHDR_t* png_chunk_IHDR = malloc(sizeof(png_chunk_IHDR_t));

    fseek(png_file, 8, SEEK_SET);

    while(feof(png_file) == 0){
        fread(png_chunk, sizeof(png_chunk_t), 1, png_file);
        printf(" --> Tamanho: %d\n", ntohl(png_chunk->tamanho));
        printf(" --> Tipo: %s\n", png_chunk->tipo);

        if(strncmp(png_chunk->tipo,"IHDR",4) == 0){
            fread(png_chunk_IHDR, sizeof(png_chunk_IHDR_t), 1, png_file);
            printf(" --> Largura: %d\n", ntohl(png_chunk_IHDR->largura));
            printf(" --> Altura: %d\n", ntohl(png_chunk_IHDR->altura));

            fseek(png_file, 4, SEEK_CUR);
        }else if(strncmp(png_chunk->tipo,"IEND",4) == 0){
            printf(" --> Fim do arquivo\n");
        
            break;
        }else{
            fseek(png_file, ntohl(png_chunk->tamanho) + 4, SEEK_CUR);
        }
    }

    fclose(png_file);
    free(png_chunk);
    free(png_chunk_IHDR);

    return 0;
}

#include "stdio.h"
#include "stdlib.h"

//Nome: Matheus Silva dos Santos
//Matrícula: 22052573

//Objetivo: Exercitar a manipulação de arquivos/dados binários através da leitura de um arquivo ZIP

struct zip_file_hdr {
int signature;
short version;
short flags;
short compression;
short mod_time;
short mod_date;
int crc;
int compressed_size;
int uncompressed_size;
short name_length;
short extra_field_length;
} __attribute__ ((packed));

int main(int argc, char *argv[])
{
    FILE *zip_file = fopen(argv[1], "rb");
    struct zip_file_hdr *file_hdr = malloc(sizeof(struct zip_file_hdr));
    int file_count = 1;

    while(!feof(zip_file)){
        fread(file_hdr, sizeof(struct zip_file_hdr), 1, zip_file);

        int signature = file_hdr->signature;

        if(signature == 0x04034b50){
            printf("Arquivo %d ..\n", file_count);
            file_count++;
            char* file_name = malloc(file_hdr->name_length + 1);
            fread(file_name, file_hdr->name_length, 1, zip_file);
            file_name[file_hdr->name_length] = '\0';
            printf("  --> Nome do Arquivo: %s\n", file_name);
            printf("  --> Tamanho Compactado: %d\n", file_hdr->compressed_size);
            printf("  --> Tamanho Descompactado: %d\n", file_hdr->uncompressed_size);
            free(file_name);
            fseek(zip_file, file_hdr->extra_field_length +
            file_hdr->compressed_size, SEEK_CUR);
        }else{
            break;
        }
    }

    return 0;
}

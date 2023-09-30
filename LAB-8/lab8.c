#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"

//Aluno: Matheus Silva dos Santos
//Matricula: 22052573

// OBJETIVO: Exercitar a manipulação de arquivos/dados binários através da decodificação de um arquivo de vídeo MPEG.

//função para verificar se o stream é do tipo slice (0x01 até 0xAF)
bool is_slice(unsigned char byte){
    if(byte >= 0x01 && byte <= 0xAF){
        return true;
    }
    return false;
}

//função para verificar se o stream é do tipo packet video (0xC0 até 0xDF)
bool is_packet_video(unsigned char byte){
    if(byte >= 0xC0 && byte <= 0xDF){
        return true;
    }
    return false;
}

//função para verificar se o stream é do tipo packet audio (0xE0 até 0xEF)
bool is_packet_audio(unsigned char byte){
    if(byte >= 0xE0 && byte <= 0xEF){
        return true;
    }
    return false;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Uso: %s <file.mpg>\n", argv[0]);
        return 1;
    }

    FILE *mpg_file = fopen(argv[1], "rb");

    //variaveis para os bytes
    unsigned char byte1 = 0;
    unsigned char byte2 = 0;
    unsigned char byte3 = 0;
    unsigned char byte4 = 0;

    //variaveis para os dados do cabecalho do stream sequence
    unsigned int largura = 0;
    unsigned int altura = 0;
    unsigned int frame_rate = 0;

    //variavel para o tipo de figura do stream picture
    unsigned char tipo = 0;

    //leitura do arquivo mpeg de três em três bytes
    while(!feof(mpg_file)){
        fread(&byte1, sizeof(unsigned char), 1, mpg_file);
        fread(&byte2, sizeof(unsigned char), 1, mpg_file);
        fread(&byte3, sizeof(unsigned char), 1, mpg_file);
        
        //verificar se os três bytes lidos são iguais ao start code
        //se não forem igual ao start code prefix (memcmp), é retornado dois bytes (fseek)
        //e prosseguido para a próxima iteração (lendo novamente os três próximos bytes do arquivo mpeg)
        if(memcmp(&byte1, "\x00", 1) == 0 && memcmp(&byte2, "\x00", 1) == 0 && memcmp(&byte3, "\x01", 1) == 0){
            fread(&byte4, sizeof(unsigned char), 1, mpg_file);

            printf("--> Código: ");

            //verificar o tipo de stream
            if(memcmp(&byte4, "\xB3", 1) == 0){
                printf("%02hhx -- Sequence Header -- ", byte4);
                fread(&byte1, sizeof(unsigned char), 1, mpg_file);
                fread(&byte2, sizeof(unsigned char), 1, mpg_file);
                fread(&byte3, sizeof(unsigned char), 1, mpg_file);
                fread(&byte4, sizeof(unsigned char), 1, mpg_file);

                //leitura dos dados do cabecalho do stream sequence
                largura = byte1 * 16 + (byte2 >> 4);
                altura = ( byte2 & 0x0F) * 256 + byte3;
                frame_rate = byte4 & 0x0f;

                printf("Width =  %d, ", largura);
                printf("Height = %d -- ", altura);
                
                if(frame_rate == 1){
                    printf("Frame Rate = 23.976fps\n");
                }
                else if(frame_rate == 2){
                    printf("Frame Rate = 24.000fps\n");
                }
                else if(frame_rate == 3){
                    printf("Frame Rate = 25.000fps\n");
                }
                else if(frame_rate == 4){
                    printf("Frame Rate = 29.970fps\n");
                }
                else if(frame_rate == 5){
                    printf("Frame Rate = 30.000fps\n");
                }
                else if(frame_rate == 6){
                    printf("Frame Rate = 50.000fps\n");
                }
                else if(frame_rate == 7){
                    printf("Frame Rate = 59.940fps\n");
                }
                else if(frame_rate == 8){
                    printf("Frame Rate = 60.000fps\n");
                }
            }
            else if(memcmp(&byte4, "\x00", 1) == 0){
                printf("%02hhx -- Picture -- ", byte4);
                fread(&byte1, sizeof(unsigned char), 1, mpg_file);
                fread(&byte2, sizeof(unsigned char), 1, mpg_file);
                fread(&byte3, sizeof(unsigned char), 1, mpg_file);
                fread(&byte4, sizeof(unsigned char), 1, mpg_file);

                //pegar o tipo de figura --> (fazer três “shifts à direita” e depois “& 0x07”)
                tipo = (byte2 >> 3) & 0x07;

                if(tipo == 1){
                    printf("Tipo = I\n");
                }
                else if(tipo == 2){
                    printf("Tipo = P\n");
                }
                else if(tipo == 3){
                    printf("Tipo = B\n");
                }
            }else if(memcmp(&byte4, "\xBA", 1) == 0){
                printf("%02hhx -- Pack\n", byte4);
            }else if(memcmp(&byte4, "\xBB", 1) == 0){
                printf("%02hhx -- System\n", byte4);
            }else if(memcmp(&byte4, "\xB8", 1) == 0){
                printf("%02hhx -- Group of Pictures\n", byte4);
            }else if(is_slice(byte4)){
                printf("%02hhx -- Slice\n", byte4);
            }else if(is_packet_video(byte4)){
                printf("%02hhx -- Packet Video\n", byte4);
            }else if(is_packet_audio(byte4)){
                printf("%02hhx -- Packet Audio\n", byte4);
            }
            else{
                printf("%02hhx -- Tipo de stream não implementado\n", byte4);
            }
        }else{ 
            fseek(mpg_file, -2, SEEK_CUR);
        }
    }

    fclose(mpg_file);

    return 0;
}

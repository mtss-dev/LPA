#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include <netinet/in.h>

//Aluno: Matheus Silva dos Santos
//Matricula: 22052573

//OBJETIVO: Exercitar a manipulação de arquivos/dados binários através da decodificação do TCP/IP

// Dados que devem ser impressos:

// Ethernet: endereco MAC de origem e destino
// IP: versão, tamanho do cabeçalho, tamanho total do pacote, endereço IP de origem e destino
// TCP: porta de origem e destino, tamanho do cabeçalho
// HTTP: tamanho dos dados e os dados propriamente ditos

// Definição do tipo de dados para o cabeçalho HTTP
typedef struct {
 uint8_t data[512]; // Dados do HTTP
} http_hdr_t;

// Definição do tipo de dados para o cabeçalho Ethernet
typedef struct {
 uint8_t daddr[6]; // Endereco MAC de destino
 uint8_t saddr[6]; // Endereco MAC de origem (source)
 uint16_t protocol; // Protocolo da próxima camada (IP!)
} ethernet_hdr_t;

// Definição do tipo de dados para o cabeçalho IP
typedef struct {
 uint8_t hdr_len:4, // Tamanho do Cabeçalho,
 version:4; // Versão do IP
 uint8_t tos; // Tipo de serviço
 uint16_t tot_len; // Tamanho total do IP
 uint16_t id; // Id do pacote
 uint16_t frag_off; // Fragmentado?
 uint8_t ttl; // Tempo de vida
 uint8_t protocol; // Protocolo próxima camada (TCP!)
 uint16_t check; // Checksum
 uint8_t saddr[4]; // Endereço IP de origem
 uint8_t daddr[4]; // Endereço IP de destino
} ip_hdr_t;

// Definição do tipo de dados para o cabeçalho TCP
typedef struct {
 uint16_t sport; // Porta TCP de origem
 uint16_t dport; // Porta TCP de destino
 uint32_t seq; // Sequência
 uint32_t ack_seq; // Acknowledgement
 uint8_t reservado:4, // Não usado
 hdr_len:4; // Tamanho do cabecalho
 uint8_t flags; // Flags do TCP
 uint16_t window; // Tamanho da janela
 uint16_t check; // Checksum
 uint16_t urg_ptr; // Urgente
} tcp_hdr_t;

void print_mac_source(uint8_t *saddr){
    printf("  --> MAC de Origem: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", saddr[0], saddr[1], saddr[2], saddr[3], saddr[4], saddr[5]);
}

void print_mac_destiny(uint8_t *daddr){
    printf("  --> MAC de Destino: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", daddr[0], daddr[1], daddr[2], daddr[3], daddr[4], daddr[5]);
}

void print_ip_source(uint8_t *saddr){
    printf("  --> Endereço IP de Origem: %d.%d.%d.%d\n", saddr[0], saddr[1], saddr[2], saddr[3]);
}

void print_ip_destiny(uint8_t *daddr){
    printf("  --> Endereço IP de Destino: %d.%d.%d.%d\n", daddr[0], daddr[1], daddr[2], daddr[3]);
}

int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "rb");

    if(fp == NULL){
        printf("Erro ao abrir o arquivo %s\n", argv[1]);
        return 1;
    }

    ethernet_hdr_t ethernet_hdr;
    ip_hdr_t ip_hdr;
    tcp_hdr_t tcp_hdr;
    http_hdr_t http_hdr;

    printf("Lendo Ethernet ..\n");
    fread(&ethernet_hdr, sizeof(ethernet_hdr_t), 1, fp);

    print_mac_source(ethernet_hdr.saddr);
    print_mac_destiny(ethernet_hdr.daddr);

    printf("Lendo IP ..\n");
    fread(&ip_hdr, sizeof(ip_hdr_t), 1, fp);

    printf("  --> Versão do IP: %d\n", ip_hdr.version);
    printf("  --> Tamanho do Cabeçalho: %d bytes\n", ip_hdr.hdr_len * 4);
    // Abaixo é usado a função ntohs para converter o tamanho total do pacote para a “ordem de byte” usado pela máquina que está executando o programa
    printf("  --> Tamanho do Pacote: %d bytes\n", ntohs(ip_hdr.tot_len)); 
    print_ip_source(ip_hdr.saddr);
    print_ip_destiny(ip_hdr.daddr);

    // Pulando as Opções do IP, que possuem tamanho variável com fseek

    fseek(fp, ip_hdr.hdr_len * 4 - sizeof(ip_hdr_t), SEEK_CUR); 

    printf("Lendo TCP ..\n");
    fread(&tcp_hdr, sizeof(tcp_hdr_t), 1, fp);

    //Nas Porta de Origem e Porta de Destino do TCP, como eles ocupam mais de um byte, é usado a função ntohs para convertê-los para a “ordem de byte” usado pela máquina que está executando o programa
    printf("  --> Porta de Origem: %d\n", ntohs(tcp_hdr.sport));
    printf("  --> Porta de Destino: %d\n", ntohs(tcp_hdr.dport));
    //No Tamanho do cabeçalho do TCP, este tamanho é em múltiplos de 32 bits (4 bytes) e, portanto, para mostrar este valor em bytes, é multiplicado por 4.
    printf("  --> Tamanho do Cabeçalho: %d bytes\n", tcp_hdr.hdr_len * 4);

    //Pulando as Opções do TCP, que possuem tamanho variável com fseek

    fseek(fp, tcp_hdr.hdr_len * 4 - sizeof(tcp_hdr_t), SEEK_CUR);

    printf("Lendo Dados (HTTP) ..\n");
    fread(&http_hdr, sizeof(http_hdr_t), 1, fp);

    //O tamanho dos dados é calculado subtraindo o tamanho do cabeçalho IP e do cabeçalho TCP do tamanho total do pacote
    int tam_dados = ntohs(ip_hdr.tot_len) - (ip_hdr.hdr_len*4) - (tcp_hdr.hdr_len*4);
    
    printf("  --> Tamanho dos Dados: %d bytes\n", tam_dados);
    printf("  --> Dados: \n");
    printf("%s", http_hdr.data);

    fclose(fp);
    
    return 0;
}

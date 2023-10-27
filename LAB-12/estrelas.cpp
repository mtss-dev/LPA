#include <stdio.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

//Aluno: Matheus Silva dos Santos
//Matrícula: 22052573

//Objetivo: implementar um programa que abra a imagem e conte a quantidade de estrelas nela. Além disso, seu programa deverá pegar a imagem original e pintar nela os quadrados resultantes do flood fill. Ou seja, será a imagem original com um quadrado encapsulando todas as estrelas encontradas. 

using namespace cv;

typedef struct {
    Point p1, p2;
} bounding_box;

// img  - Imagem
// x, y - Posição da imagem onde a pintura irá começar
// whiteThreshold - Escala de cinza em que qualquer valor maior é considerado "branco"
// bbox - Retângulo que contém os pixels pintados
void floodFill(Mat img, int x, int y, int whiteThreshold, bounding_box *bbox) {
    // Se estiver fora dos limites da imagem, ignora
    if (x < 0 || x >= img.cols || y < 0 || y >= img.rows) return;

    // Acessa o valor do pixel
    uchar pixel = img.at<uchar>(y,x);

    // Se o pixel atual não é considerado "branco", ignora
    if (pixel < whiteThreshold) return;

    // Pinta o pixel para preto
    img.at<uchar>(y,x) = 0;

    // Atualiza o retângulo
    if (x < bbox->p1.x) bbox->p1.x = x;
    if (x > bbox->p2.x) bbox->p2.x = x;
    if (y < bbox->p1.y) bbox->p1.y = y;
    if (y > bbox->p2.y) bbox->p2.y = y;

    // Executa recursivamente para cima, baixo, esquerda e direita
    floodFill(img, x, y+1, whiteThreshold, bbox);
    floodFill(img, x, y-1, whiteThreshold, bbox);
    floodFill(img, x-1, y, whiteThreshold, bbox);
    floodFill(img, x+1, y, whiteThreshold, bbox);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Sintaxe: %s <nome-arquivo-png> <valor do whiteTreshold\n", argv[0]);
        return 1;
    }

    // Abertura da imagem
    Mat img = imread(argv[1], IMREAD_GRAYSCALE);
    int whiteTreshold = atoi(argv[2]);

    //Vetor para guardar as estrelas encontradas
    bounding_box stars[20000];
    int cont = 0;

    // Para cada pixel em uma linha/coluna da imagem
    for(int y=0; y<img.rows; y++) {
        for (int x=0; x<img.cols; x++) {
            uchar pixel = img.at<uchar>(y,x);
            // Se o pixel é branco (maior que o whiteTreshold)
            if(pixel > whiteTreshold ){
                // Um bounding box é criado 
                bounding_box bbox = {
                    .p1 = Point(img.cols, img.rows),
                    .p2 = Point(0, 0)
                };
                // O flood fill é executado
                floodFill(img, x, y, whiteTreshold, &bbox);
                // O bounding box é salvo no vetor de estrelas
                stars[cont] = bbox;
                cont++;
            }
        }
    }

    // Salva a imagem resultante do flood fill (floofill_stars_X.png) onde X é o valor do limiar
    imwrite("floodfill_stars_" + std::to_string(whiteTreshold) + ".png", img);

    printf("Utilizando whiteTreshold = %d\n", whiteTreshold);
    printf("Total de Estrelas = %d\n", cont);

    //Abertura da imagem original (colorida)
    Mat imgOriginal = imread(argv[1], IMREAD_COLOR);

    // Para cada estrela encontrada
    for(int i=0; i<cont; i++){
        // Desenha um retângulo na imagem original
        rectangle(imgOriginal, stars[i].p1, stars[i].p2, Scalar(0, 255, 0), 1);
    }

    // Salva a imagem ORIGINAL com o retângulo resultante do flood fill (limiarX.png) onde X é o valor do limiar
    imwrite("limiar" + std::to_string(whiteTreshold) + ".png", imgOriginal);
    return 0;
}
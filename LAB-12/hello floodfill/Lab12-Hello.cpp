#include <stdio.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

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
    if (argc < 2) {
        printf("Sintaxe: %s <nome-arquivo-png>\n", argv[0]);
        return 1;
    }

    // Abre a imagem, passada pela linha de comando, em modo escala-de-cinza
    Mat img = imread(argv[1], IMREAD_GRAYSCALE);

    printf("Arquivo: %s\n", argv[1]);
    printf("  --> Colunas: %d\n", img.cols);
    printf("  --> Linhas:  %d\n", img.rows);
    printf("  --> Canais:  %d\n", img.channels());

    printf("  --> Pixels:");
    for(int y=0; y<img.rows; y++) {
        for (int x=0; x<img.cols; x++) {
            uchar pixel = img.at<uchar>(y,x);
            printf("%3d ", pixel);
        }
        printf("\n             ");
    }
    printf("\n");

    img.at<uchar>(7,9) = 0;  // Pixel preto
    img.at<uchar>(7,10) = 0; // Pixel preto
    imwrite("output-hello-pixel.png", img);

    // Retângulo resultante do flood fill
    bounding_box bbox = {
        .p1 = Point(img.cols, img.rows),
        .p2 = Point(0, 0)
    };
    // Executa o flood fill
    floodFill(img, 8, 5, 200, &bbox);

    // Imprime o resultado do flood fill
    printf("  --> Pixels:");
    for(int y=0; y<img.rows; y++) {
        for (int x=0; x<img.cols; x++) {
            uchar pixel = img.at<uchar>(y,x);
            printf("%3d ", pixel);
        }
        printf("\n             ");
    }
    printf("\n");

    // Salva a imagem com o círculo "pintado"
    imwrite("output-hello-floodfill.png", img);
    printf("  --> Flood fill: %d, %d -> %d, %d\n", bbox.p1.x, bbox.p1.y, bbox.p2.x, bbox.p2.y);

    // Salva a imagem ORIGINAL com o retângulo resultante do flood fill
    Mat imgOriginal = imread(argv[1], IMREAD_COLOR);
    rectangle(imgOriginal, bbox.p1, bbox.p2, Scalar(0, 255, 0), 1);
    imwrite("output-hello-rect.png", imgOriginal);
}
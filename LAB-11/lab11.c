#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>

//Aluno: Matheus Silva dos Santos
//Matricula: 22052573

//Objetivo: Aprender e exercitar programação com geometria computacional usando o OpenGL.

// Valores passados por linha de comando
int largura, altura, qtdSegmentos;

// Estrutura para armazenar um ponto
typedef struct {
    float x;
    float y;
} Ponto;

// Gera um ponto aleatório dentro dos limites da janela
Ponto generateRandomPoint(int largura, int altura) {
    Ponto p;
    p.x = rand() % largura;
    p.y = rand() % altura;
    return p;
}
//Funções do Bryce Boe para verificar se há interseção entre dois segmentos
int ccw(Ponto A, Ponto B, Ponto C) {
    return (C.y - A.y) * (B.x - A.x) > (B.y - A.y) * (C.x - A.x);
}

int intersect(Ponto A, Ponto B, Ponto C, Ponto D) {
    return ccw(A, C, D) != ccw(B, C, D) && ccw(A, B, C) != ccw(A, B, D);
}

// Verifica se o segmento AB intersecta algum dos segmentos armazenados no vetor "segmentos"
int isIntersecting(Ponto A, Ponto B, Ponto segmentos[], int count) {
    for (int i = 0; i < count; ++i) {
        Ponto C = segmentos[i * 2];
        Ponto D = segmentos[i * 2 + 1];
        if (intersect(A, B, C, D)) {
            return 1; // há interseção
        }
    }
    return 0; // não há interseção
}

// Desenha os segmentos aleatórios
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);

    glBegin(GL_LINES);

    Ponto segmentos[2 * qtdSegmentos]; // vetor para armazenar os segmentos(inicio e fim dos segmentos)

    // Gera os segmentos aleatórios (sem interseção)
    for (int i = 0; i < qtdSegmentos; ++i) {
        Ponto A, B;
        do {
            A = generateRandomPoint(largura, altura);
            B = generateRandomPoint(largura, altura);
        } while (isIntersecting(A, B, segmentos, i)); // verifica se há interseção

        segmentos[i * 2] = A;
        segmentos[i * 2 + 1] = B;
    }

    // Desenha os segmentos que foram gerados aleatoriamente sem interseção
    for (int i = 0; i < qtdSegmentos; ++i) {
        Ponto A = segmentos[i * 2];
        Ponto B = segmentos[i * 2 + 1];
        glVertex2f(A.x, A.y);
        glVertex2f(B.x, B.y);
    }

    glEnd();
    glFlush();
}

void main(int argc, char *argv[])
{
    srand(time(NULL));

    if(argc != 4){
        printf("Uso: %s <largura> <altura> <qtdSegmentos>\n", argv[0]);
        return;
    }

    largura = atoi(argv[1]);
    altura = atoi(argv[2]);
    qtdSegmentos = atoi(argv[3]);

    glutInit(&argc, argv);
    glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize(largura, altura);
    glutCreateWindow ("Segmentos Aleatorios");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, largura, 0.0, altura, -1.0, 1.0);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glutDisplayFunc(display);
    glutMainLoop();
}
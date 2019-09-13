#include <stdlib.h>
#include<stdio.h>
#include <windows.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "objLoader.h"

#define OBJ "monkey_head2.obj"
//#define OBJ "MekaGyakushuGoji.obj"
#define LARGURA 512
#define ALTURA 512

GLfloat obsX, obsY, obsZ;
GLfloat angle, fAspect = LARGURA/ALTURA;

float tempo;

objLoader* modelo;

static void PosicionaObservador(void);

// Função callback de redesenho da janela de visualização
void Desenha(void){

    // Limpa a janela de vizualização com a cor preta
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Define a cor de desenho: branco
    //         R     G     B
    glColor3f(1.0, 1.0, 1.0);

	// Percorre todas as faces
    for(int f = 0; f < modelo->faceCount; ++f){

        obj_face* o = modelo->faceList[f];

        glBegin(GL_LINE_LOOP);
            // Percorre todos os vértices da face
            glVertex3f(modelo->vertexList[o->vertex_index[0]]->e[0],
                       modelo->vertexList[o->vertex_index[0]]->e[1],
                       modelo->vertexList[o->vertex_index[0]]->e[2]);

            glVertex3f(modelo->vertexList[o->vertex_index[1]]->e[0],
                       modelo->vertexList[o->vertex_index[1]]->e[1],
                       modelo->vertexList[o->vertex_index[1]]->e[2]);

            glVertex3f(modelo->vertexList[o->vertex_index[2]]->e[0],
                       modelo->vertexList[o->vertex_index[2]]->e[1],
                       modelo->vertexList[o->vertex_index[2]]->e[2]);

        glEnd();

    }

    /*float raio = 10.0f;
    tempo += 0.01;
    obsX = sin(tempo) * raio;
    obsZ = cos(tempo) * raio;
    if(obsX >= 180 || obsZ >= 180)
        tempo = obsX = obsZ = 0;*/

    //printf("obsX: %f, obsY: %f, obsZ: %f\n", obsX, obsY, obsZ);

    PosicionaObservador();

    // Executa os comandos OpenGL
    //glFlush();
	glutSwapBuffers();
	glutPostRedisplay();

}

// Função usada para especificar a posição do observador virtual
void PosicionaObservador(void){

    // Especifica sistema de coordenadas do modelo
    glMatrixMode(GL_MODELVIEW);
    // Inicializa sistema de coordenadas do modelo
    glLoadIdentity();

    gluLookAt(obsX,obsY,obsZ, 0,0,0, 0,1,0);
    //gluLookAt(obsX,obsY,obsZ, 0,6,0, 0,1,0);

}

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisulizacao(void){

    // Especifica sistema de coordenadas de projeção
    glMatrixMode(GL_PROJECTION);
    // Inicializa sistema de coordenadas de projeção
    glLoadIdentity();

    // Especifica a projeção perspectiva (ângulo, aspecto, zMin, zMax)
    gluPerspective(angle, fAspect, 2, 100);

    PosicionaObservador();

}

// Função callback chamada para gerenciar eventos de teclas
void Teclado(unsigned char key, int x, int y){

    if(key == 27)
        exit(0);

}

// Função responsável por inicializar parâmetros e variáveis
void Inicializa(void){

    // Define a cor de fundo da janela de exibição como preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Inicializa a variável que especifica o ângulo de projeção perspectiva
    angle = 20;
    //angle = 80;

    fAspect = LARGURA/ALTURA;

    // Inicializa as variáveis usadas para alterar a posição do observador virtual
    obsX = 0;
    //obsX = -8;
    obsY = 0;
    //obsY = 7;
    obsZ = 8;
    //obsZ = 7;

}

// Programa principal
int main(int argc, char** argv){

    // Carrega Objeto
	modelo = new objLoader();
	char* obj = (char *) malloc(sizeof(char) * (strlen(OBJ) + 1));
	strcpy(obj, OBJ);
	modelo->load(obj);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(LARGURA, ALTURA);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("OpenGL");
    Inicializa();
    EspecificaParametrosVisulizacao();
    glutDisplayFunc(Desenha);
    glutKeyboardFunc(Teclado);
    glutMainLoop();

    return 0;

}

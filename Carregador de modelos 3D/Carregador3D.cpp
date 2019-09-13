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

// Fun��o callback de redesenho da janela de visualiza��o
void Desenha(void){

    // Limpa a janela de vizualiza��o com a cor preta
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Define a cor de desenho: branco
    //         R     G     B
    glColor3f(1.0, 1.0, 1.0);

	// Percorre todas as faces
    for(int f = 0; f < modelo->faceCount; ++f){

        obj_face* o = modelo->faceList[f];

        glBegin(GL_LINE_LOOP);
            // Percorre todos os v�rtices da face
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

// Fun��o usada para especificar a posi��o do observador virtual
void PosicionaObservador(void){

    // Especifica sistema de coordenadas do modelo
    glMatrixMode(GL_MODELVIEW);
    // Inicializa sistema de coordenadas do modelo
    glLoadIdentity();

    gluLookAt(obsX,obsY,obsZ, 0,0,0, 0,1,0);
    //gluLookAt(obsX,obsY,obsZ, 0,6,0, 0,1,0);

}

// Fun��o usada para especificar o volume de visualiza��o
void EspecificaParametrosVisulizacao(void){

    // Especifica sistema de coordenadas de proje��o
    glMatrixMode(GL_PROJECTION);
    // Inicializa sistema de coordenadas de proje��o
    glLoadIdentity();

    // Especifica a proje��o perspectiva (�ngulo, aspecto, zMin, zMax)
    gluPerspective(angle, fAspect, 2, 100);

    PosicionaObservador();

}

// Fun��o callback chamada para gerenciar eventos de teclas
void Teclado(unsigned char key, int x, int y){

    if(key == 27)
        exit(0);

}

// Fun��o respons�vel por inicializar par�metros e vari�veis
void Inicializa(void){

    // Define a cor de fundo da janela de exibi��o como preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Inicializa a vari�vel que especifica o �ngulo de proje��o perspectiva
    angle = 20;
    //angle = 80;

    fAspect = LARGURA/ALTURA;

    // Inicializa as vari�veis usadas para alterar a posi��o do observador virtual
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

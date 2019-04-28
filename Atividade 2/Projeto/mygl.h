#ifndef _MYGL_H_
#define _MYGL_H_

#include "definitions.h"
#include <math.h> // Função sqrt()
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "objLoader.h"

#define ABS(x) ((x) < 0 ? -(x) : (x)) // Macro utilizada para calcular módulo
#define OBJ "monkey_head2.obj"

//*****************************************************************************
// Defina aqui as suas funções gráficas
//*****************************************************************************

GLfloat obsX = 0;
GLfloat obsY = 0;
GLfloat obsZ = 100;

float tempo;

objLoader* modelo;

/*******************************************************************************
* Matriz model: Esp. Obj. --> Esp. Univ.
********************************************************************************/

glm::mat4 matrizModel(1, 0, 0, 0,
            	      0, 1, 0, 0,
            	      0, 0, 1, 0,
                      0, 0, 0, 1);

/*******************************************************************************
* Parâmetros da câmera
********************************************************************************/

glm::vec3 posicaoCamera(-2, 0, 100);  // Posicão da câmera no universo.
//glm::vec3 posicaoCamera(0, 0, 8);  // Posicão da câmera no universo.
glm::vec3 lookAtCamera(-12, 2, 0);  // Ponto para onde a câmera está olhando.
//glm::vec3 lookAtCamera(0, 0, 0);  // Ponto para onde a câmera está olhando.
glm::vec3 upCamera(0, 1, 0);  // 'up' da câmera no espaço do universo.

/*******************************************************************************
* Cálculo do sistema ortonormal gerado a partir dos parâmetros da câmera
********************************************************************************/

glm::vec3 apontaCamera = lookAtCamera - posicaoCamera;

glm::vec3 zCamera = -(apontaCamera) / glm::length(apontaCamera);
//glm::vec3 zCamera =  - glm::normalize(apontaCamera);
glm::vec3 xCamera = glm::cross(upCamera, zCamera) / glm::length(glm::cross(upCamera, zCamera));
//glm::vec3 xCamera = glm::normalize(glm::cross(upCamera, zCamera));
glm::vec3 yCamera = glm::cross(zCamera, xCamera);

/*******************************************************************************
* Construção da matriz view: Esp. Univ. --> Esp. Cam.
********************************************************************************/

glm::mat4 bTransposta(xCamera[0], xCamera[1], xCamera[2], 0,
      		          yCamera[0], yCamera[1], yCamera[2], 0,
      		          zCamera[0], zCamera[1], zCamera[2], 0,
      		          0, 0, 0, 1);

glm::mat4 translacao(1, 0, 0, -posicaoCamera[0],
     		         0, 1, 0, -posicaoCamera[1],
     		         0, 0, 1, -posicaoCamera[2],
     		         0, 0, 0, 1);

glm::mat4 matrizView = bTransposta * translacao;

/*******************************************************************************
* Construção da matriz ModelView: Esp. Obj. --> Esp. Cam.
********************************************************************************/

glm::mat4 matrizModelView = matrizView * matrizModel;

/*******************************************************************************
* Construção da matriz de Projeção: Esp. Cam. --> Esp. Recorte
********************************************************************************/
int d = 100; // Distância do centro de projeção para o viewplane

glm::mat4 matrizProjection(1, 0, 0, 0,
                 	       0, 1, 0, 0,
                 	       0, 0, 1, d,
                 	       0, 0, -(1/d), 0);


/*******************************************************************************
* Construção da matriz ModelViewProjection: Esp. Obj. --> Esp. Recorte
********************************************************************************/

glm::mat4 matrizModelViewProj = matrizProjection * matrizModelView;

/*******************************************************************************
* Conversão de coordenadas do espaço canônico para o espaço de tela.
********************************************************************************/

glm::mat4 espelhamentoYCanonico(1, 0, 0, 0,
                                0, -1, 0, 0,
                                0, 0, 1, 0,
                                0, 0, 0, 1);

glm::mat4 translacaoCanonico(1, 0, 0, (IMAGE_WIDTH - 1)/2,
                             0, 1, 0, (IMAGE_HEIGHT - 1)/2,
                             0, 0, 1, 0,
                             0, 0, 0, 1);

glm::mat4 escalaTela(IMAGE_WIDTH/2, 0, 0, 0,
      		         0, IMAGE_HEIGHT/2, 0, 0,
      		         0, 0, 1, 0,
      		         0, 0, 0, 1);

glm::mat4 matrizViewport = escalaTela * translacaoCanonico * espelhamentoYCanonico;

typedef struct{

    unsigned char R, G, B, A;

} tRGBA;

typedef struct{

    int x, y;
    tRGBA cor;

} tPixel;

int PutPixel(tPixel* pixel){

    // Posição em memória de cada componente do pixel
    int componente;

    if(pixel->x < 0 || pixel->y < 0 ||
       pixel->x > IMAGE_WIDTH || pixel->y > IMAGE_HEIGHT){

       /* Tentativa inválida de acessar uma posição que não
        consta no sistema de coordenadas da janela de exibição*/

       return 1;

    }

    // Posição do primeiro byte de cada pixel
    componente = 4*(pixel->x + pixel->y*IMAGE_WIDTH);

    FBptr[componente] = pixel->cor.R; // Componente R do pixel
    FBptr[++componente] = pixel->cor.G; // Componente G do pixel
    FBptr[++componente] = pixel->cor.B; // Componente B do pixel
    FBptr[++componente] = pixel->cor.A; // Componente A do pixel

    return 0;

} // Fim da função PutPixel()

tPixel InterpolaCor(float p, tPixel* pixel1, tPixel* pixel2){

    tPixel pixel;

    pixel.cor.R = p*(pixel1->cor.R) + (1 - p)*(pixel2->cor.R);
    pixel.cor.G = p*(pixel1->cor.G) + (1 - p)*(pixel2->cor.G);
    pixel.cor.B = p*(pixel1->cor.B) + (1 - p)*(pixel2->cor.B);
    pixel.cor.A = p*(pixel1->cor.A) + (1 - p)*(pixel2->cor.A);

    return pixel;

}

float Distancia(tPixel* pixel1, tPixel* pixel2){

    return sqrt((pixel2->x - pixel1->x)*(pixel2->x - pixel1->x) + (pixel2->y - pixel1->y)*(pixel2->y - pixel1->y));

}

void DrawLine(tPixel* pixel1, tPixel* pixel2){

    int dx = pixel2->x - pixel1->x;
    int dy = pixel2->y - pixel1->y;
    int d, incr_e, incr_ne; // Respectivamente, variável de decisão, pixel E (East), pixel NE (Northeast)
    int auxX, auxY; // Guardará a posição atual do próximo pixel a ser aceso

    float dParcial;
    float dTotal = Distancia(pixel1, pixel2);

    tPixel pixel = *pixel1;

    PutPixel(&pixel);

    // Primeiro octante, 0 < m < 1
    if(dx >= 0 && dy >= 0 && ABS(dx) >= ABS(dy)){

        d = 2*dy - dx;
        incr_e = 2*dy;
        incr_ne = 2*(dy - dx);

        while(pixel.x != pixel2->x || pixel.y != pixel2->y){

            if(d < 0){
                d += incr_e;
                pixel.x++;

            } else{
                d += incr_ne;
                pixel.x++;
                pixel.y++;
            }

            auxX = pixel.x; // Guarda a posição x do pixel que será aceso depois da interpolação de cor
            auxY = pixel.y; // Guarda a posição y do pixel que será aceso depois da interpolação de cor
            dParcial = Distancia(&pixel, pixel2);
            pixel = InterpolaCor(dParcial/dTotal, pixel1, pixel2);
            pixel.x = auxX;
            pixel.y = auxY;

            PutPixel(&pixel);

        }

        PutPixel(pixel2);
        return;
    }

    // Segundo octante, m > 1
    if(dx >= 0 && dy >= 0 && ABS(dy) >= ABS(dx)){

        d = dy - 2*dx;
        incr_e = -2*dx;
        incr_ne = 2*(dy - dx);

        while(pixel.x != pixel2->x || pixel.y != pixel2->y){

            if(d < 0){
                d += incr_ne;
                pixel.x++;
                pixel.y++;

            } else{
                d += incr_e;
                pixel.y++;
            }

            auxX = pixel.x; // Guarda a posição x do pixel que será aceso depois da interpolação de cor
            auxY = pixel.y; // Guarda a posição y do pixel que será aceso depois da interpolação de cor
            dParcial = Distancia(&pixel, pixel2);
            pixel = InterpolaCor(dParcial/dTotal, pixel1, pixel2);
            pixel.x = auxX;
            pixel.y = auxY;

            PutPixel(&pixel);

        }

        PutPixel(pixel2);
        return;

    }

    // Terceiro octante, m < -1
    if(dx <= 0 && dy >= 0 && ABS(dy) >= ABS(dx)){

        d = dy + 2*dx;
        incr_e = 2*dx;
        incr_ne = 2*(dy + dx);

        while(pixel.x != pixel2->x || pixel.y != pixel2->y){

            if(d < 0){
                d += incr_ne;
                pixel.x--;
                pixel.y++;

            } else{
                d += incr_e;
                pixel.y++;
            }

            auxX = pixel.x; // Guarda a posição x do pixel que será aceso depois da interpolação de cor
            auxY = pixel.y; // Guarda a posição y do pixel que será aceso depois da interpolação de cor
            dParcial = Distancia(&pixel, pixel2);
            pixel = InterpolaCor(dParcial/dTotal, pixel1, pixel2);
            pixel.x = auxX;
            pixel.y = auxY;

            PutPixel(&pixel);

        }

        PutPixel(pixel2);
        return;

    }

    // Quarto octante, -1 < m < 0
    if(dx <= 0 && dy >= 0 && ABS(dx) >= ABS(dy)){

        d = 2*dy + dx;
        incr_e = 2*dy;
        incr_ne = 2*(dy + dx);

        while(pixel.x != pixel2->x || pixel.y != pixel2->y){

            if(d < 0){
                d += incr_e;
                pixel.x--;

            } else{
                d += incr_ne;
                pixel.x--;
                pixel.y++;
            }

            auxX = pixel.x; // Guarda a posição x do pixel que será aceso depois da interpolação de cor
            auxY = pixel.y; // Guarda a posição y do pixel que será aceso depois da interpolação de cor
            dParcial = Distancia(&pixel, pixel2);
            pixel = InterpolaCor(dParcial/dTotal, pixel1, pixel2);
            pixel.x = auxX;
            pixel.y = auxY;

            PutPixel(&pixel);

        }

        PutPixel(pixel2);
        return;

    }

    // Quinto octante, 0 < m < 1
    if(dx <= 0 && dy <= 0 && ABS(dx) >= ABS(dy)){

        d = -2*dy + dx;
        incr_e = -2*dy;
        incr_ne = 2*(-dy + dx);

        while(pixel.x != pixel2->x || pixel.y != pixel2->y){

            if(d < 0){
                d += incr_e;
                pixel.x--;

            } else{
                d += incr_ne;
                pixel.x--;
                pixel.y--;
            }

            auxX = pixel.x; // Guarda a posição x do pixel que será aceso depois da interpolação de cor
            auxY = pixel.y; // Guarda a posição y do pixel que será aceso depois da interpolação de cor
            dParcial = Distancia(&pixel, pixel2);
            pixel = InterpolaCor(dParcial/dTotal, pixel1, pixel2);
            pixel.x = auxX;
            pixel.y = auxY;

            PutPixel(&pixel);

        }

        PutPixel(pixel2);
        return;

    }

    // Sexto octante, m > 1
    if(dx <= 0 && dy <= 0 && ABS(dy) >= ABS(dx)){

        d = -dy + 2*dx;
        incr_e = 2*dx;
        incr_ne = 2*(-dy + dx);

        while(pixel.x != pixel2->x || pixel.y != pixel2->y){

            if(d < 0){
                d += incr_ne;
                pixel.x--;
                pixel.y--;

            } else{
                d += incr_e;
                pixel.y--;
            }

            auxX = pixel.x; // Guarda a posição x do pixel que será aceso depois da interpolação de cor
            auxY = pixel.y; // Guarda a posição y do pixel que será aceso depois da interpolação de cor
            dParcial = Distancia(&pixel, pixel2);
            pixel = InterpolaCor(dParcial/dTotal, pixel1, pixel2);
            pixel.x = auxX;
            pixel.y = auxY;

            PutPixel(&pixel);

        }

        PutPixel(pixel2);
        return;

    }

    // Sétimo octante, m < -1
    if(dx >= 0 && dy <= 0 && ABS(dy) >= ABS(dx)){

        d = -dy - 2*dx;
        incr_e = -2*dx;
        incr_ne = 2*(-dy - dx);

        while(pixel.x != pixel2->x || pixel.y != pixel2->y){

            if(d < 0){
                d += incr_ne;
                pixel.x++;
                pixel.y--;

            } else{
                d += incr_e;
                pixel.y--;
            }

            auxX = pixel.x; // Guarda a posição x do pixel que será aceso depois da interpolação de cor
            auxY = pixel.y; // Guarda a posição y do pixel que será aceso depois da interpolação de cor
            dParcial = Distancia(&pixel, pixel2);
            pixel = InterpolaCor(dParcial/dTotal, pixel1, pixel2);
            pixel.x = auxX;
            pixel.y = auxY;

            PutPixel(&pixel);

        }

        PutPixel(pixel2);
        return;

    }

    // Oitavo octante, -1 < m < 0
    if(dx >= 0 && dy <= 0 && (ABS(dx) >= ABS(dy))){

        d = -2*dy - dx;
        incr_e = -2*dy;
        incr_ne = 2*(-dy - dx);

        while(pixel.x != pixel2->x || pixel.y != pixel2->y){

            if(d < 0){
                d += incr_e;
                pixel.x++;

            } else{
                d += incr_ne;
                pixel.x++;
                pixel.y--;
            }

            auxX = pixel.x; // Guarda a posição x do pixel que será aceso depois da interpolação de cor
            auxY = pixel.y; // Guarda a posição y do pixel que será aceso depois da interpolação de cor
            dParcial = Distancia(&pixel, pixel2);
            pixel = InterpolaCor(dParcial/dTotal, pixel1, pixel2);
            pixel.x = auxX;
            pixel.y = auxY;

            PutPixel(&pixel);

        }

        PutPixel(pixel2);
        return;

    }

} // Fim da função DrawLine()

void DrawTriangle(tPixel* pixel1, tPixel* pixel2, tPixel* pixel3){

    DrawLine(pixel1, pixel2);
    DrawLine(pixel2, pixel3);
    DrawLine(pixel3, pixel1);

}

void PreencheTriangulo(tPixel* pixel1, tPixel* pixel2, tPixel* pixel3){

    float w1, w2, w3, numerador, denominador, cor;

    tPixel pixel;

    for(int i = 0; i < IMAGE_WIDTH; i++){

        for(int j = 0; j < IMAGE_HEIGHT; j++){

           numerador = (pixel2->y - pixel3->y)*(i - pixel3->x) + (pixel3->x - pixel2->x)*(j - pixel3->y);
           denominador = (pixel2->y - pixel3->y)*(pixel1->x - pixel3->x) + (pixel3->x - pixel2->x)*(pixel1->y - pixel3->y);
           if(numerador == 0){
                w1 = 0;
           } else{
                w1 = numerador / denominador;
           }

           numerador = (pixel3->y - pixel1->y)*(i - pixel3->x) + (pixel1->x - pixel3->x)*(j - pixel3->y);
           if(numerador == 0){
                w2 = 0;
           } else{
                w2 = numerador / denominador;
           }

           w3 = 1 - w1 - w2;

           if(w1 >= 0 && w2 >= 0 && (w1 + w2) <= 1){

                pixel.x = i;
                pixel.y = j;
                cor = (w1*pixel1->cor.R + w2*pixel2->cor.R + w3*pixel3->cor.R) + 0.5;
                pixel.cor.R = (char) cor;
                cor = (w1*pixel1->cor.G + w2*pixel2->cor.G + w3*pixel3->cor.G) + 0.5;
                pixel.cor.G = (char) cor;
                cor = (w1*pixel1->cor.B + w2*pixel2->cor.B + w3*pixel3->cor.B) + 0.5;
                pixel.cor.B = (char) cor;
                cor = (w1*pixel1->cor.A + w2*pixel2->cor.A + w3*pixel3->cor.A) + 0.5;
                pixel.cor.A = (char) cor;

                PutPixel(&pixel);

           }
        }
    }

} // Fim da função PreencheTriangulo()

void defineCamera(glm::vec3 posCam){

    posicaoCamera = posCam;  // Posicão da câmera no universo.

    apontaCamera = lookAtCamera - posicaoCamera;

    zCamera = -(apontaCamera) / glm::length(apontaCamera);
    //glm::vec3 zCamera =  - glm::normalize(apontaCamera);
    xCamera = glm::cross(upCamera, zCamera) / glm::length(glm::cross(upCamera, zCamera));
    //glm::vec3 xCamera = glm::normalize(glm::cross(upCamera, zCamera));
    yCamera = glm::cross(zCamera, xCamera);

    bTransposta = glm::mat4(xCamera[0], xCamera[1], xCamera[2], 0,
                            yCamera[0], yCamera[1], yCamera[2], 0,
                            zCamera[0], zCamera[1], zCamera[2], 0,
                            0, 0, 0, 1);
    translacao = glm::mat4(1, 0, 0, -posicaoCamera[0],
                           0, 1, 0, -posicaoCamera[1],
                           0, 0, 1, -posicaoCamera[2],
                           0, 0, 0, 1);

    matrizView = bTransposta * translacao;

    matrizModelView = matrizView * matrizModel;

    matrizModelViewProj = matrizProjection * matrizModelView;

}

/*******************************************************************************
* Desenho da geometria na "tela"
********************************************************************************/

void DesenhaModelo(){

    /*float raio = 10.0f;
    tempo += 0.02;
    obsX = sin(tempo) * raio;
    obsZ = cos(tempo) * raio;
    if(obsX >= 180 || obsZ >= 180)
        tempo = obsX = obsZ = 0;

    glm::vec3 posCam(obsX, obsY, obsZ);
    defineCamera(posCam);*/

    glm::mat4 matrizModelViewProjViewPort = matrizViewport * matrizModelViewProj;

	// Carrega triângulos vértice a vértice
	for(int i = 0; i < modelo->faceCount; i++){
		obj_face* o = modelo->faceList[i];

		glm::vec4 v1, v2, v3;

		v1[0] = modelo->vertexList[o->vertex_index[0]]->e[0];
		v1[1] = modelo->vertexList[o->vertex_index[0]]->e[1];
		v1[2] = modelo->vertexList[o->vertex_index[0]]->e[2];
		v1[3] = (1 - (v1[2]/d)); //coordenada homogênea;

		v2[0] = modelo->vertexList[o->vertex_index[1]]->e[0];
		v2[1] = modelo->vertexList[o->vertex_index[1]]->e[1];
		v2[2] = modelo->vertexList[o->vertex_index[1]]->e[2];
		v2[3] = (1 - (v2[2]/d)); //coordenada homogênea;

		v3[0] = modelo->vertexList[o->vertex_index[2]]->e[0];
		v3[1] = modelo->vertexList[o->vertex_index[2]]->e[1];
		v3[2] = modelo->vertexList[o->vertex_index[2]]->e[2];
		v3[3] = (1 - (v3[2]/d)); //coordenada homogênea;


		v1 = matrizModelViewProjViewPort * v1;
		v2 = matrizModelViewProjViewPort * v2;
		v3 = matrizModelViewProjViewPort * v3;

		v1 = v1 / (1 - (v1[2]/d));
		v2 = v2 / (1 - (v2[2]/d));
		v3 = v3 / (1 - (v3[2]/d));

		tPixel pixel1 = {(int) v1[0], (int) v1[1], {255, 255, 255, 255}}; // Pixel branco
		tPixel pixel2 = {(int) v2[0], (int) v2[1], {255, 255, 255, 255}}; // Pixel branco
		tPixel pixel3 = {(int) v3[0], (int) v3[1], {255, 255, 255, 255}}; // Pixel branco

		DrawTriangle(&pixel1, &pixel2, &pixel3);
	}

}

#endif // _MYGL_H_

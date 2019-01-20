#ifndef _MYGL_H_
#define _MYGL_H_

#include "definitions.h"

#include <math.h>

//*****************************************************************************
// Defina aqui as suas funções gráficas
//*****************************************************************************

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

}

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
    int cAng = 0; // Coeficiente angular

    int auxX, auxY;

    float dParcial;
    float dTotal = Distancia(pixel1, pixel2);

    if(dx < 0){

        DrawLine(pixel2, pixel1);
        return;
    }

    if(dy < 0)
        cAng = -1;
    else
        cAng = 1;

    int d;//, incr_e, incr_nee;

    tPixel pixel = *pixel1;

    PutPixel(&pixel);

    // Escolha dos octantes
    if(dx >= cAng*dy){ // m <= 1

        if(dy < 0){ // Caso y2 < y1
            d = 2*dy + dx;
            while(pixel.x < pixel2->x){

                if(d < 0){ // Pixel E (East) escolhido
                    d += 2*(dy + dx);
                    pixel.x++;
                    pixel.y--;
                } else{ // Pixel NE (Northeast) escolhido
                    d += 2*dy;
                    pixel.x++; // Varia apenas no eixo x
                }

                auxX = pixel.x;
                auxY = pixel.y;
                dParcial = Distancia(&pixel, pixel2);
                pixel = InterpolaCor(dParcial/dTotal, pixel1, pixel2);
                pixel.x = auxX;
                pixel.y = auxY;

                PutPixel(&pixel);
            }

        } else{ // Caso y1 < y2
            d = 2*dy - dx;
            while(pixel.x < pixel2->x){

                if(d < 0){ // Pixel E (East) escolhido
                    d += 2*dy;
                    pixel.x++; // Varia apenas no eixo x
                } else{ // Pixel NE (Northeast) escolhido
                    d += 2*(dy - dx);
                    pixel.x++;
                    pixel.y++;
                }

                auxX = pixel.x;
                auxY = pixel.y;
                dParcial = Distancia(&pixel, pixel2);
                pixel = InterpolaCor(dParcial/dTotal, pixel1, pixel2);
                pixel.x = auxX;
                pixel.y = auxY;

                PutPixel(&pixel);
            }
        }

    } else{ // |m|>1

        if(dy < 0){ // Caso y2<y1
            d = dy + 2*dx;
            while(pixel.y > pixel2->y){

                if(d < 0){
                    d += 2*dx;
                    pixel.y--; // Varia apenas no eixo y
                } else{
                    d += 2*(dy + dx);
                    pixel.x++;
                    pixel.y--;
                }

                auxX = pixel.x;
                auxY = pixel.y;
                dParcial = Distancia(&pixel, pixel2);
                pixel = InterpolaCor(dParcial/dTotal, pixel1, pixel2);
                pixel.x = auxX;
                pixel.y = auxY;

                PutPixel(&pixel);
            }

        } else{ // Caso y1 < y2
            d = dy - 2*dx;
            while(pixel.y < pixel2->y){

                if(d < 0){
                    d += 2*(dy - dx);
                    pixel.x++;
                    pixel.y++;
                } else{
                    d += -2*dx;
                    pixel.y++; // Varia apenas no eixo y
                }

                auxX = pixel.x;
                auxY = pixel.y;
                dParcial = Distancia(&pixel, pixel2);
                pixel = InterpolaCor(dParcial/dTotal, pixel1, pixel2);
                pixel.x = auxX;
                pixel.y = auxY;

                PutPixel(&pixel);
            }
        }
    }

    PutPixel(pixel2);

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

}


#endif // _MYGL_H_


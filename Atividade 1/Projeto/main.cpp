#include "main.h"

//-----------------------------------------------------------------------------
void MyGlDraw(void)
{
	//*************************************************************************
	// Chame aqui as funções do mygl.h
	//*************************************************************************

    // Teste de PutPixel()
    /*
    tPixel pixel;

    // Canto superior esquerdo
    for(int i = 0; i < IMAGE_WIDTH/2; i = i + 7){

        for(int j = 0; j < IMAGE_HEIGHT/2; j = j + 7){

            pixel = {i, j, {255, 0, 0, 255}}; // Pixel vermelho
            PutPixel(&pixel);

        }
    }

    // Canto superior direito
    for(int i = 256; i < IMAGE_WIDTH; i = i + 7){

        for(int j = 0; j < IMAGE_HEIGHT/2; j = j + 7){

            pixel = {i, j, {0, 255, 0, 255}}; // Pixel verde
            PutPixel(&pixel);

        }
    }

    // Canto inferior esquerdo
    for(int i = 0; i < IMAGE_WIDTH/2; i = i + 7){

        for(int j = 256; j < IMAGE_HEIGHT; j = j + 7){

            pixel = {i, j, {0, 0, 255, 255}}; // Pixel azul
            PutPixel(&pixel);

        }
    }

    // Canto inferior direito
    for(int i = 256; i < IMAGE_WIDTH; i = i + 7){

        for(int j = 256; j < IMAGE_HEIGHT; j = j + 7){

            pixel = {i, j, {255, 255, 255, 0}};
            PutPixel(&pixel);

        }
    }*/

    // Teste de DrawLine()
    /*
    // Primeiro octante
    tPixel pixel1 = {256, 256, {0, 255, 0, 255}}; // Pixel verde
    tPixel pixel2 = {512, 256, {0, 255, 0, 255}}; // Pixel verde
    tPixel pixel3 = {512, 384, {0, 255, 0, 255}}; // Pixel verde
    tPixel pixel4 = {512, 512, {0, 255, 0, 255}}; // Pixel verde

    DrawLine(&pixel1, &pixel2); // Ângulo de 0 grau
    DrawLine(&pixel1, &pixel3); // Ângulo de 30 graus
    DrawLine(&pixel1, &pixel4); // Ângulo de 45 graus

    // Segundo octante
    pixel1 = {256, 256, {255, 0, 0, 255}}; // Pixel vermelho
    tPixel pixel5 = {384, 512, {255, 0, 0, 255}}; // Pixel vermelho
    tPixel pixel6 = {256, 512, {255, 0, 0, 255}}; // Pixel vermelho

    DrawLine(&pixel1, &pixel5); // Ângulo de 60 graus
    DrawLine(&pixel1, &pixel6); // Ângulo de 90 graus

    // Terceiro octante
    pixel1 = {256, 256, {0, 0, 255, 255}}; // Pixel azul
    tPixel pixel7 = {128, 512, {0, 0, 255, 255}}; // Pixel azul
    tPixel pixel8 = {0, 512, {255, 255, 255, 0}}; // Pixel branco

    DrawLine(&pixel1, &pixel7); // Ângulo de 120 graus
    pixel1 = {256, 256, {255, 255, 255, 0}}; // Pixel branco
    DrawLine(&pixel1, &pixel8); // Ângulo de 135 graus

    // Quarto octante
    pixel1 = {256, 256, {255, 255, 255, 0}}; // Pixel branco
    tPixel pixel9 = {0, 384, {255, 255, 255, 0}}; // Pixel branco
    tPixel pixel10 = {0, 256, {255, 255, 255, 0}}; // Pixel branco

    DrawLine(&pixel1, &pixel9); // Ângulo de 150 graus
    DrawLine(&pixel1, &pixel10); // Ângulo de 180 graus

    // Quinto octante
    pixel1 = {256, 256, {0, 255, 0, 255}}; // Pixel verde
    tPixel pixel11 = {0, 128, {0, 255, 0, 255}}; // Pixel verde
    tPixel pixel12 = {0, 0, {0, 255, 0, 255}}; // Pixel verde

    DrawLine(&pixel1, &pixel11); // Ângulo de 210 graus
    DrawLine(&pixel1, &pixel12); // Ângulo de 225 graus

    // Sexto octante
    pixel1 = {256, 256, {255, 0, 0, 255}}; // Pixel vermelho
    tPixel pixel13 = {128, 0, {255, 0, 0, 255}}; // Pixel vermelho
    tPixel pixel14 = {256, 0, {255, 0, 0, 255}}; // Pixel vermelho

    DrawLine(&pixel1, &pixel13); // Ângulo de 240 graus
    DrawLine(&pixel1, &pixel14); // Ângulo de 270 graus

    // Sétino octante
    pixel1 = {256, 256, {0, 0, 255, 255}}; // Pixel azul
    tPixel pixel15 = {384, 0, {0, 0, 255, 255}}; // Pixel azul

    DrawLine(&pixel1, &pixel15); // Ângulo de 300 graus

    // Oitavo octante
    pixel1 = {256, 256, {255, 255, 255, 0}}; // Pixel branco
    tPixel pixel16 = {512, 0, {255, 255, 255, 0}}; // Pixel branco
    tPixel pixel17 = {512, 128, {255, 255, 255, 0}}; // Pixel branco

    DrawLine(&pixel1, &pixel16); // Ângulo de 315 graus
    DrawLine(&pixel1, &pixel17); // Ângulo de 330 graus*/

    // Teste InterpolaCor()
    /*
    // De vermelho para verde
    tPixel pixel1 = {128, 128, {255, 0, 0, 255}}; // Pixel vermelho
    tPixel pixel2 = {384, 128, {0, 255, 0, 255}}; // Pixel verde

    DrawLine(&pixel1, &pixel2);

    // De verde para azul
    pixel1 = {128, 256, {0, 255, 0, 255}}; // Pixel verde
    pixel2 = {384, 256, {0, 0, 255, 255}}; // Pixel azul

    DrawLine(&pixel1, &pixel2);

    // De azul para vermelho
    pixel1 = {128, 384, {0, 0, 255, 255}}; // Pixel azul
    pixel2 = {384, 384, {255, 0, 0, 255}}; // Pixel vermelho

    DrawLine(&pixel1, &pixel2);*/

    // Teste DrawTriangle()

    // Primeiro vértice
    tPixel pixel1 = {128, 384, {0, 255, 0, 255}}; // Pixel verde
    // Segundo vértice
    tPixel pixel2 = {256, 128, {0, 0, 255, 255}}; // Pixel azul
    // Terceiro vértice
    tPixel pixel3 = {384, 384, {255, 0, 0, 255}}; // Pixel vermelho

    DrawTriangle(&pixel1, &pixel2, &pixel3);
    PreencheTriangulo(&pixel1, &pixel2, &pixel3);

    // Teste e Falha de segmentação
    /*
    tPixel pixel1 = {384, 384, {0, 255, 0, 255}}; // Pixel verde
    tPixel pixel2 = {768, 128, {0, 0, 255, 255}}; // Pixel azul
    tPixel pixel3 = {768, 384, {255, 0, 0, 255}}; // Pixel vermelho

    DrawTriangle(&pixel1, &pixel2, &pixel3);*/

    // Teste Bresenham puro
    /*
    tPixel pixel1 = {256, 256, {0, 255, 0, 255}}; // Pixel verde
    tPixel pixel2 = {512, 256, {0, 255, 0, 255}}; // Pixel verde
    tPixel pixel3 = {512, 384, {0, 255, 0, 255}}; // Pixel verde
    tPixel pixel4 = {512, 512, {0, 255, 0, 255}}; // Pixel verde

    DrawLine(&pixel1, &pixel2); // Ângulo de 0 grau
    DrawLine(&pixel1, &pixel3); // Ângulo de 30 graus
    DrawLine(&pixel1, &pixel4); // Ângulo de 45 graus*/

}

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	// Inicializações.
	InitOpenGL(&argc, argv);
	InitCallBacks();
	InitDataStructures();

	// Ajusta a função que chama as funções do mygl.h
	DrawFunc = MyGlDraw;

	// Framebuffer scan loop.
	glutMainLoop();

	return 0;
}


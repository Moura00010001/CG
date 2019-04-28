# Pipeline gráfico

## Atividade

Esta foi a segunda atividade realizada para a disciplina de Introdução à Computação Gráfica, proposta no intuito de familiarizar os alunos com a estrutura e o funcionamento do pipeline gráfico. O trabalho utilizou um carregador de malhas 3D, em formato OBJ, feito e disponibilizado pelo professor. O objetivo é implementar um pipeline completo, capaz de transformar vértices descritos no espaço do objeto em primitivas rasterizadas no espaço de tela, ou seja, o pipeline gráfico é uma sequência de transformações geométricas entre sistema de coordenadas.

## Implementação

Foram utilizados vetores e matrizes da biblioteca GLM para representar as transformações entre cada espaço do pipeline gráfico.

### Espaço do Objeto para o espaço do Universo

#### Trecho de código:
~~~c++
glm::mat4 matrizModel(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
~~~	

A matrizModel foi carregada com a identidade, o que significa que cada vértice do objeto descrito em seu espaço não sofrerá nenhuma alteração quando passar para o espaço do universo.

### Espaço do Universo para o espaço da câmera

Antes de passar para esse próximo passo do pipeline, precisamos definir os parâmetros da câmera e construir um sistema ortonormal à partir desses parâmetros.

#### Trecho de código:
~~~c++
// Parâmetros
glm::vec3 posicaoCamera(..., ..., ...);  // Posicão da câmera no universo.
glm::vec3 lookAtCamera(..., ..., ...);  // Ponto para onde a câmera está olhando.
glm::vec3 upCamera(0, 1, 0);  // 'up' da câmera no espaço do universo.

// Sistema ortonormal
glm::vec3 apontaCamera = lookAtCamera - posicaoCamera;

glm::vec3 zCamera = -(apontaCamera) / glm::length(apontaCamera);
//glm::vec3 zCamera =  - glm::normalize(apontaCamera);
glm::vec3 xCamera = glm::cross(upCamera, zCamera) / glm::length(glm::cross(upCamera, zCamera));
//glm::vec3 xCamera = glm::normalize(glm::cross(upCamera, zCamera));
glm::vec3 yCamera = glm::cross(zCamera, xCamera);
~~~	

A matriz que leva os vértices para o próximo espaço é carregada com um rotação e uma translação para alinhar o sistema de coordenadas do Universo com o da Câmera.

#### Trecho de código:
~~~c++
glm::mat4 bTransposta(xCamera[0], xCamera[1], xCamera[2], 0,
      	          yCamera[0], yCamera[1], yCamera[2], 0,
      	          zCamera[0], zCamera[1], zCamera[2], 0,
      	          0, 0, 0, 1);

glm::mat4 translacao(1, 0, 0, -posicaoCamera[0],
     	         0, 1, 0, -posicaoCamera[1],
     	         0, 0, 1, -posicaoCamera[2],
     	         0, 0, 0, 1);

glm::mat4 matrizView = bTransposta * translacao;
~~~	

### Espaço da Câmera para o espaço de Recorte

Este espaço é importante porque ele faz uma primeira triagem do que deve ser descartado (por exemplo o que está atrás do view plane) ou do que deve continuar no pipeline gráfico. Devido a simplicidade do pipeline a ser desenvolvido, este não lida com os tais descartes o que significa que se houver alguma geometria atrás da câmera, ela também será rasterizada, espelhada em todos os eixos coordenados, o que pode resultar numa geometria deformada e que não faz sentido, assim que for projetada na Tela. 

#### Trecho de código:
~~~c++
int d = ...; // Distância entre o centro de projeção e o viewplane

glm::mat4 matrizProjection(1, 0, 0, 0,
                 	   0, 1, 0, 0,
                 	   0, 0, 1, d,
                 	   0, 0, -(1/d), 0);
~~~	

### Espaço de recorte para o espaço Canônico

Este é o último passo antes dos vértices caírem no espaço da Tela. Depois de todas as transformações anteriores, muito provavelmente a coordenada homogênea (quarta coordenada utilizada, entre outras coisas, para composição de matrizes) é direferente de 1, o que é necessário para que possamos descartá-la e termos nossos vértices no sistema euclidiano. Supondo que X, Y e Z sejam as coordenadas dos vértices, nesse momento a coordenada homogênea é caracterizada pelo seguinte trecho de código:

#### Trecho de código:
~~~c++
glm::vec3 vetorDeVertices;

vetorDeVertices[0] = X;
vetorDeVertices[1] = Y;
vetorDeVertices[2] = Z;
vetorDeVertices[3] = 1 - (vetorDeVertices[2]/d); //coordenada homogênea;
~~~	

Sem grandes problemas, é só dividir todas as coordenadas, inclusive ela mesma pela última coordenada dos nossos vetores de vértices (coordenada homogênea). Esse passo pode ser deixado para o final do pipeline.

### Espaço canônico para o espaço da Tela

A matriz que coloca os vértices no espaço da Tela é carregada com um espelhamento em Y do espaço Canônico, visto que o eixo Y da  tela cresce no sentido contrário, uma translação, pois o sistema de coordenadas da Tela só trabalha com valores positivos, e finalmente uma escala que leva em consideração sua largura e altura. 

#### Trecho de código:
~~~c++
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
~~~	

__Obs:__ Todas as matrizes apresentadas anteriormente podem ser combinadas por meio de uma multiplicação, o que acarreta num maior desempenho durante a transformação de cada vértice do espaço do Objeto para o espaço da Tela. Porém, deve-se lembrar que no final disso tudo, a coordenada homogênea tem que ser descartada, o que é feito dividindo-se todas as coordenadas do vértice por ela. 

#### Trecho de código:
~~~c++
glm::mat4 matrizModelViewProjViewPort = matrizViewport * matrizModelViewProj;
~~~	

## Testes

Ocorreu um grave problema, que, por falta de tempo, não foi investigado e solucionado. Como pode ser visto nas imagens a seguir, é como se a câmera do pipeline desenvolvido aqui estivesse "enferrujada", presa numa posição fixa, o que compromete grande parte da visualização do modelo.

____Parâmetros de teste utilizados em OpenGL:__ Posição da Câmera: (0,0,8); Look At: (0,0,0); UP da Câmera: (0,1,0); Fovy: 20; Aspect: 1; NearPlane: 2; FarPlane: 100.

__Parâmetros de teste utilizados em My OpenGL:__ posicaoCamera(0,0,4); lookAtCamera(0,0,0); upCamera(0,1,0); d = 2.

OpenGL | Pipeline desenvolvido
------------ | ------------
![](https://github.com/Moura00010001/CG/blob/master/Atividade%202/Printscreens/Pipeline%20OpenGL.png) | ![](https://github.com/Moura00010001/CG/blob/master/Atividade%202/Printscreens/Pipeline%20Projeto%20-%201.png)

__Outros parâmetros de teste utilizados em My OpenGL:__ 
<p>1: posicaoCamera(-2,0,100); lookAtCamera(-12,2,0); upCamera(0,1,0); d = 100.</p>
<p>2: posicaoCamera(-2,0,10); lookAtCamera(-12,2,0); upCamera(0,1,0); d = 100.</p>

1 | 2
------------ | ------------
![](https://github.com/Moura00010001/CG/blob/master/Atividade%202/Printscreens/Pipeline%20Projeto%20-%203.png) | ![](https://github.com/Moura00010001/CG/blob/master/Atividade%202/Printscreens/Pipeline%20Projeto%20-%202.png)

## Resultados e possíveis melhoras

Houve uma pequena dificuldade até se conseguir utilizar o carregador de malhas disponibilizado pelo professor. Também, muito infelizmente, como citado na seção anterior, um problema inesperado foi encontrado e não foi resolvido. Tentou-se algumas soluções como trocar as operações de pós-multiplicação por pré-multiplicação, linhas por colunas mas estas modificações apenas geravam outros problemas. No final das contas, não conseguiu-se identificar a natureza do problema e, por falta de tempo, o pipeline foi enviado assim mesmo. 

## Referências

Aulas e Slides do professor Christian Azambuja Pagot.

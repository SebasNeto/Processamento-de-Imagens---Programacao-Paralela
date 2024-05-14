

/*
    algoritmo:
        1.carregar imagem de entrada
        2.definir funções e variáveis - em Halide isso é feito com a sintaxe Func e Var
        3.definir o escopo da função e manipulação de variáveis
        4.paralelismo
        5.realizar operação - em Halide isso é feito com a sintaxe realize()
        6.salvar resultado
*/

/*
    pontos importantes:
        1. coordenadas de cada pixel na imagem:
        x(dimensão horizontal - colunas) 
        y(dimensão vertical - linhas)
        c(canal de cor - RED(vermelho - c = 1); GREEN(verde - c = 2); BLUE(azul - c =3))

        2. função clamp - garante que os valores de cada pixel esteja dentro de um intervalo especifico.
        sintaxe (... + 1, 2, 255) - sendo 0 a 255 o limite especifico.

        3. Aplicar agendamentos - parallel(); vetorize()

        4. Aplicar função (realize) e salvar imagem alterada.
*/

#include "Halide.h"
#include "halide_image_io.h"
#include <stdio.h>
#include <chrono>

using namespace Halide;
using namespace Halide::Tools;
using namespace std::chrono;

int main(int argc, char **argv) {

    int valorBrilho;

    printf("Digite o valor do brilho. Negativo para diminuir o brilho. Positivo para aumentar o brilho ->>>");
    scanf("%d", &valorBrilho);
    
    Buffer<uint8_t> input = load_image("/mnt/c/Users/Cliente/Downloads/Halide/input_images/Teatro Amazonas.jpg");
    if (input.defined()) {
        printf("Imagem carregada\n");
    } else {
        printf("Erro ao carregar a imagem.\n");
        return 1; 
    }

    Func brilho("brilho");
    Var x("x"), y("y"), c("c");

    
    printf("Aplicando função de brilho \n");
    //int valorBrilho = 50; 
    //brilho(x, y, c) = clamp(cast<int>(input(x, y, c)) + valorBrilho, 0, 255);
    brilho(x, y, c) = cast<uint8_t>(clamp(cast<int>(input(x, y, c)) + valorBrilho, 0, 255));

    printf("Aplicando paralelização \n");
    brilho.parallel(y);
    printf("Aplicando vetorização \n");
    brilho.vectorize(x, 16);


    auto inicio = high_resolution_clock::now();
    Buffer<uint8_t> output = brilho.realize({input.width(), input.height(), input.channels()});
    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<microseconds>(fim - inicio);

    save_image(output, "/mnt/c/Users/Cliente/Downloads/Halide/output_images/Teatro_Amazonas_v1.jpg");

    printf("Código compilado com sucesso. Imagem Salva \n");

    printf("Tempo de execução em Halide: %ld microsegundos\n", duracao.count());
    return 0;
}

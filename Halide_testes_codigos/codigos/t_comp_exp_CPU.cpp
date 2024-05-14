#include "Halide.h"
#include "halide_image_io.h"
#include <stdio.h>
#include <chrono>

using namespace Halide;
using namespace Halide::Tools;
using namespace std::chrono;

int main(int argc, char **argv) {

    //int z1 = atoi(argv[2]);
    //int z2 = atoi(argv[3]);

    int z1 = 100;
    int z2 = 200;

    Buffer<uint8_t> input = load_image("/mnt/c/Users/Cliente/Downloads/Halide/input_images/Teatro Amazonas.jpg");
    if (input.defined()) {
        printf("Imagem carregada\n");
    } else {
        printf("Erro ao carregar a imagem.\n");
        return 1; 
    }

    Var x("x"), y("y"), c("c");
    Func compressa_expansao("compressa_expansao");
    Expr valor = input(x, y, c);
    Expr transformada = select(valor <= z1, cast<uint8_t>(valor / 2),
                              valor > z1 && valor < z2, cast<uint8_t>((2 * valor) - 127),
                              cast<uint8_t>((valor / 2) + 128)); //sintaxe select(condição1, expressão1, condição2, expressão2,...,expressão_default) //semelhante a if else 

    compressa_expansao(x, y, c) = transformada;

    printf("Aplicando Paralelização e vetorização \n");
    compressa_expansao.parallel(y).vectorize(x, 16); //processa 16 pixels na direção x de cada vez

    /*
        compressa_expansao.parallel(y) computação executada em paralelo ao longo do eixo y (diferentes linhas da imagem)
        compressa_expansao.vectorize(x, 16) operações ao longo do eixo x (dentro de uma linha) é vetorizada com um vetor de 16 elementos sendo processado simultaneamente.
    */

    // Realizar a função e salvar a imagem de saída
    Buffer<uint8_t> output = compressa_expansao.realize({input.width(), input.height(), input.channels()});

    //auto start = high_resolution_clock::now();
    
    save_image(output, "/mnt/c/Users/Cliente/Downloads/Halide/output_images/t_Teatro_Amazonas_v1.jpg");
    
    //auto stop = high_resolution_clock::now();
    //auto duration = duration_cast<milliseconds>(stop - start);
    
    printf("Código compilado com sucesso. Imagem Salva \n");
    //printf("Duração: %lld ms\n", duration.count());

    return 0;
}

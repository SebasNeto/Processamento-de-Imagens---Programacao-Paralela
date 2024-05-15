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

    int za = 100; 
    int zb = 200;
    int z1 = 100;
    int zn = 200;

    float a = static_cast<float>(zn - z1) /  (zb - za);
    float b = z1 - a * za;


    Buffer<uint8_t> input = load_image("/mnt/c/Users/Cliente/Downloads/Halide/input_images/t_fusca_teste.png");
    if (input.defined()) {
        printf("Imagem carregada\n");
    } else {
        printf("Erro ao carregar a imagem.\n");
        return 1; 
    }

    Var x("x"), y("y"), c("c");
    Func expansaoLinear("expansaoLinear");
    Expr valor = input(x, y, c);
    Expr transformada = select(valor <= za, cast<uint8_t>(z1),
                               valor >= zb, cast<uint8_t>(zn),
                               cast<uint8_t>(a * valor + b));


    expansaoLinear(x, y, c) = transformada;

    //printf("Aplicando Paralelização e vetorização \n");
    expansaoLinear.parallel(y).vectorize(x, 16); //processa 16 pixels na direção x de cada vez

    Buffer<uint8_t> output = expansaoLinear.realize({input.width(), input.height(), input.channels()});

    //auto start = high_resolution_clock::now();
    
    save_image(output, "/mnt/c/Users/Cliente/Downloads/Halide/output_images/t_fusca_teste_02.png");
    
    //auto stop = high_resolution_clock::now();
    //auto duration = duration_cast<milliseconds>(stop - start);
    
    printf("Código compilado com sucesso. Imagem Salva \n");
    //printf("Duração: %lld ms\n", duration.count());

    return 0;
}

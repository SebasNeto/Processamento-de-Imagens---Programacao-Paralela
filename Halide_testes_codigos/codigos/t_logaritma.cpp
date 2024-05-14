#include "Halide.h"
#include "halide_image_io.h"
#include <stdio.h>
#include <chrono>

using namespace Halide;
using namespace Halide::Tools;
using namespace std::chrono;

int main(int argc, char **argv) {


    Buffer<uint8_t> input = load_image("/mnt/c/Users/Cliente/Downloads/Halide/input_images/t_fusca_teste.png");
    if (input.defined()) {
        printf("Imagem carregada\n");
    } else {
        printf("Erro ao carregar a imagem.\n");
        return 1; 
    }

    Var x("x"), y("y"), c("c");
    Func logaritmaTransformada("logaritmaTransformada");
    
    Expr valor = cast<float>(input(x, y, c));
    float coef = 255.0f / log(1.0f + 255.0f);
    Expr resultado = coef * log(1.0f + valor);
    
    logaritmaTransformada(x, y, c) = cast<uint8_t>(clamp(resultado, 0.0f, 255.0f)); //imagem de 8 bits (0 a 255)

    //printf("Aplicando Paralelização e vetorização \n");
    expansaoLinear.parallel(y).vectorize(x, 16); //processa 16 pixels na direção x de cada vez

    Buffer<uint8_t> output = logaritmaTransformada.realize({input.width(), input.height(), input.channels()});

    //auto start = high_resolution_clock::now();
    
    save_image(output, "/mnt/c/Users/Cliente/Downloads/Halide/output_images/t_fusca_teste_02.png");
    
    //auto stop = high_resolution_clock::now();
    //auto duration = duration_cast<milliseconds>(stop - start);
    
    printf("Código compilado com sucesso. Imagem Salva \n");
    //printf("Duração: %lld ms\n", duration.count());

    return 0;
}

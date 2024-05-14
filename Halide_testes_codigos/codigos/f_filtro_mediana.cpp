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

    int tamJanela = 3;
    int margem = tamJanela / 2;

    Func clamped = BoundaryConditions::repeat_edge(input);

    Func janelas("janelas");
    RDom r(-margem, tamJanela, -margem, tamJanela);
    janelas(x, y, r.x, r.y) = clamped(x + r.x, y + r.y);

    Func mediana("mediana");
    Expr n = tamJanela * tamJanela;
    Expr median_val = janelas(x, y, r.x, r.y)[n/2];
    mediana(x, y) = median_val;

    // Aplicando paralelização e vetorização
    mediana.parallel(y).vectorize(x, 16);

    Buffer<uint8_t> output = mediana.realize({input.width(), input.height(), input.channels()});


    //auto start = high_resolution_clock::now();
    
    save_image(output, "/mnt/c/Users/Cliente/Downloads/Halide/output_images/t_fusca_teste_02.png");
    
    //auto stop = high_resolution_clock::now();
    //auto duration = duration_cast<milliseconds>(stop - start);
    
    printf("Código compilado com sucesso. Imagem Salva \n");
    //printf("Duração: %lld ms\n", duration.count());

    return 0;
}

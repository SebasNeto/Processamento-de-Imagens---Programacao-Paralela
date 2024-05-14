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
    Expr margem = tamJanela / 2;

    Func clamped = BoundaryConditions::repeat_edge(input);

    // Definir a região de vizinhança.
    RDom r(-margem, tamJanela, -margem, tamJanela);

    // Calcular a média dos pixels na janela para cada canal.
    Func media("media");
    media(x, y, c) = sum(clamped(x + r.x, y + r.y, c)) / (tamJanela * tamJanela);

    // Aplicar a média calculada para suavizar a imagem.
    Func suavizacao("suavizacao");
    suavizacao(x, y, c) = cast<uint8_t>(media(x, y, c));


    suavizacao.parallel(y).vectorize(x, 16);

    Buffer<uint8_t> output = suavizacao.realize({input.width(), input.height(), input.channels()});


    //auto start = high_resolution_clock::now();
    
    save_image(output, "/mnt/c/Users/Cliente/Downloads/Halide/output_images/t_fusca_teste_02.png");
    
    //auto stop = high_resolution_clock::now();
    //auto duration = duration_cast<milliseconds>(stop - start);
    
    printf("Código compilado com sucesso. Imagem Salva \n");
    //printf("Duração: %lld ms\n", duration.count());

    return 0;
}

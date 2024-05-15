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
    Func transformada_dente_de_serra("transformada_dente_de_serra");
    Expr valor = cast<float>(input(x, y, c));
    Expr resultado = select(
        valor < 63, cast<uint8_t>(255 * valor / 62),
        valor < 127, cast<uint8_t>(255 * (valor - 63) / 63),
        valor < 191, cast<uint8_t>(255 * (valor - 127) / 63),
        cast<uint8_t>(255 * (valor - 191) / 64)
    );

    transformada_dente_de_serra(x, y, c) = cast<uint8_t>(resultado);
    transformada_dente_de_serra.parallel(y).vectorize(x, 16);


    auto inicio = high_resolution_clock::now();
    Buffer<uint8_t> output = transformada_dente_de_serra.realize({input.width(), input.height(), input.channels()});
    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<microseconds>(fim - inicio);

    
    //auto start = high_resolution_clock::now();
    
    save_image(output, "/mnt/c/Users/Cliente/Downloads/Halide/output_images/t_fusca_teste_02.png");
    
    //auto stop = high_resolution_clock::now();
    //auto duration = duration_cast<milliseconds>(stop - start);
    
    printf("Código compilado com sucesso. Imagem Salva \n");
    printf("Tempo de execução em Halide: %ld microsegundos\n", duracao.count());
    //printf("Duração: %lld ms\n", duration.count());

    return 0;
}

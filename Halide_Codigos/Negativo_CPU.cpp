#include "Halide.h"
#include "halide_image_io.h"
#include <stdio.h>
#include <chrono>

using namespace Halide;
using namespace Halide::Tools;
using namespace std::chrono;

int main(int argc, char **argv) {

    
    Buffer<uint8_t> input = load_image("/mnt/c/Users/Cliente/OneDrive/Documentos/PIBIC/Programacao_Paralela_V2/codigos/input imagens/por_do_sol.jpeg");
    if (input.defined()) {
        printf("Imagem carregada\n");
    } else {
        printf("Erro ao carregar a imagem.\n");
        return 1; 
    }

    Func negativo("Negativo");
    Var x("x"), y("y"), c("c");

    printf("Aplicando função efeito negativo \n");
    negativo(x,y,c) = 255 - input(x,y,c);

    printf("Aplicando paralelização \n");
    negativo.parallel(y); 
    printf("Aplicando vetorização \n");
    negativo.vectorize(x, 16);

    //realiza operação e salva o resultado
    //auto inicio = high_resolution_clock::now();
    Buffer<uint8_t> output = negativo.realize({input.width(), input.height(), input.channels()});
    //auto fim = high_resolution_clock::now();
    //auto duracao = duration_cast<microseconds>(fim - inicio);

    save_image(output, "/mnt/c/Users/Cliente/OneDrive/Documentos/PIBIC/Programacao_Paralela_V2/codigos/output imagens/por_do_sol_v1.jpeg");

    printf("ok \n");

    //printf("Tempo de execução em Halide: %ld microsegundos\n", duracao.count());
    return 0;
}

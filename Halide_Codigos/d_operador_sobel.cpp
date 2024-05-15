#include "Halide.h"
#include "halide_image_io.h"
using namespace Halide;

int main(int argc, char **argv) {


    Buffer<uint8_t> input = load_image("/mnt/c/Users/Cliente/Downloads/Halide/input_images/t_fusca_teste.png");
    if (input.defined()) {
        printf("Imagem carregada\n");
    } else {
        printf("Erro ao carregar a imagem.\n");
        return 1; 
    }

    Var x("x"), y("y"), c("c");

    // Definir os kernels de Sobel para Gx e Gy.
    int sobel_x[3][3] = {{-1, 0, 1},
                         {-2, 0, 2},
                         {-1, 0, 1}};
    int sobel_y[3][3] = {{-1, -2, -1},
                         { 0,  0,  0},
                         { 1,  2,  1}};

    Func clamped = BoundaryConditions::repeat_edge(input);
    Func sobel_gx("sobel_gx"), sobel_gy("sobel_gy"), magnitude("magnitude");
    RDom r(-1, 3, -1, 3);

    // Convolve com Gx
    sobel_gx(x, y, c) = sum(clamped(x + r.x, y + r.y, c) * sobel_x[r.x + 1][r.y + 1]);

    // Convolve com Gy
    sobel_gy(x, y, c) = sum(clamped(x + r.x, y + r.y, c) * sobel_y[r.x + 1][r.y + 1]);

    // Calcular a magnitude do gradiente (sqrt(gx^2 + gy^2)).
    magnitude(x, y, c) = cast<uint8_t>(sqrt(sobel_gx(x, y, c) * sobel_gx(x, y, c) +
                                            sobel_gy(x, y, c) * sobel_gy(x, y, c)));

    // Aplicar paralelismo e vetorização.
    sobel_gx.compute_root().parallel(y).vectorize(x, 16);
    sobel_gy.compute_root().parallel(y).vectorize(x, 16);
    magnitude.compute_root().parallel(y).vectorize(x, 16);

    // Realizar a função e salvar a imagem de saída.
    Buffer<uint8_t> output = magnitude.realize(input.width(), input.height(), input.channels());
    save_image(output, "/mnt/c/Users/Cliente/Downloads/Halide/output_images/t_fusca_teste_02.png");

    printf("Código compilado com sucesso. Imagem Salva \n");

    return 0;
}

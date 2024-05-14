#include "Halide.h"
#include "halide_image_io.h"

using namespace Halide;

int main() {
    Var x("x"), y("y"), canal("canal"); 

    Buffer<uint8_t> input = load_image("/mnt/c/Users/Cliente/Downloads/Halide/input_images/t_fusca_teste.png");

    int raio = 1;
    int tamanho = 2 * raio + 1;
    Func clamped = BoundaryConditions::repeat_edge(entrada);

    Func media("media"); 
    Expr soma = 0.0f;
    for (int dy = -raio; dy <= raio; ++dy) {
        for (int dx = -raio; dx <= raio; ++dx) {
            soma += clamped(x + dx, y + dy, canal);
        }
    }

    media(x, y, canal) = cast<uint8_t>(soma / (tamanho * tamanho));
    media.parallel(y).vectorize(x, 16);
    Halide::Tools::save_image(media.realize(entrada.width(), entrada.height(), entrada.channels()), 
    
    save_image(output, "/mnt/c/Users/Cliente/Downloads/Halide/output_images/t_fusca_teste_02.png");

    return 0;
}

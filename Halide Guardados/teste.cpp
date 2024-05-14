#include "cabecalhosHalide.h"

int main(int argc, char** argv) {

    //int z1 = atoi(argv[2]);
    //int z2 = atoi(argv[3]);

    int za = 100;
    int zb = 200;
    int z1 = 100;
    int zn = 200;

    float a = static_cast<float>(zn - z1) / (zb - za);
    float b = z1 - a * za;


    Buffer<uint8_t> input = load_image("C:\\Users\\Cliente\\Downloads\\Halide\\input_images\\pixels.jpg");
    if (input.defined()) {
        printf("Imagem carregada\n");
    }
    else {
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

    auto inicio = high_resolution_clock::now();
    Buffer<uint8_t> output = expansaoLinear.realize({ input.width(), input.height(), input.channels() });
    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<microseconds>(fim - inicio);

    save_image(output, "C:\\Users\\Cliente\\Downloads\\Halide\\output_images\\pixels_C.jpg");

    printf("Código compilado com sucesso. Imagem Salva \n");
    printf("Duração: %lld ms\n", duracao.count());

    return 0;
}


//transformada do logaritmo com pré compilação

#include <vector>
#include <string>
#include <filesystem>
#include <numeric>
#include <chrono>
#include <iostream>
#include "Halide.h"
#include "halide_image_io.h"

using namespace Halide;
using namespace Halide::Tools;
using namespace std::chrono;
namespace fs = std::filesystem;

float processImage(const std::string& inputPath, const std::string& outputPath) {
    auto inicio_carregamento = high_resolution_clock::now();
    Buffer<uint8_t> input = load_image(inputPath);
    auto fim_carregamento = high_resolution_clock::now();

    if (!input.defined()) {
        std::cerr << "Erro ao carregar a imagem: " << inputPath << std::endl;
        return 0.0f;
    }

    Var x("x"), y("y"), c("c");
    Func logaritmaTransformada("logaritmaTransformada");

    Expr valor = cast<float>(input(x, y, c));
    float coef = 255.0f / log(1.0f + 255.0f);
    Expr resultado = coef * log(1.0f + valor);

    logaritmaTransformada(x, y, c) = cast<uint8_t>(clamp(resultado, 0.0f, 255.0f)); 
    logaritmaTransformada.parallel(y).vectorize(x, 16); 

    auto inicio = high_resolution_clock::now();
    Buffer<uint8_t> output = logaritmaTransformada.realize({ input.width(), input.height(), input.channels() });
    auto fim = high_resolution_clock::now();

    save_image(output, outputPath);

    auto duracao_processamento = duration_cast<milliseconds>(fim - inicio).count();
    std::string fileName = fs::path(inputPath).filename().string();
    std::cout << "Duracao do processamento de " << fileName << ": " << duracao_processamento << " ms" << std::endl;
    
    return static_cast<float>(duracao_processamento);
}

int main(int argc, char** argv) {


    std::string inputDir = "/mnt/c/Users/Cliente/Downloads/base_dados/Imagens_Selecionadas";
    std::string outputDir = "/mnt/c/Users/Cliente/Downloads/base_dados/teste";

    std::vector<float> mediasGerais;

    for (int exec = 0; exec < 1; exec++) {
        std::vector<float> medias;

        for (const auto& entry : fs::directory_iterator(inputDir)) {
            std::string inputPath = entry.path().string();
            std::string outputPath = outputDir + "\\" + entry.path().filename().string();
            float duracao = processImage(inputPath, outputPath);
            medias.push_back(static_cast<float>(duracao));
            std::string fileName = fs::path(inputPath).filename().string();
            std::cout << "Duracao da execucao para " << fileName << ": " << duracao << " ms" << std::endl;
        }


        float mediaExecucao = std::accumulate(medias.begin(), medias.end(), 0.0f) / medias.size();
        mediasGerais.push_back(mediaExecucao);
        std::cout << "Media de tempo para a execucao " << exec + 1 << ": " << mediaExecucao << " ms" << std::endl;
    }

    float mediaFinal = std::accumulate(mediasGerais.begin(), mediasGerais.end(), 0.0f) / mediasGerais.size();
    std::cout << "Media geral das medias de tempo apos 1 execucoes: " << mediaFinal << " ms" << std::endl;

    return 0;
}

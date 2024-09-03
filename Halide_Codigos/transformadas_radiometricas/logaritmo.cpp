//transformada do logaritmo com pré compilação

#include <vector>
#include <string>
#include <filesystem>
#include <numeric>
#include <chrono>

#include "Halide.h"
#include "halide_image_io.h"
#include <stdio.h>
#include <chrono>

using namespace Halide;
using namespace Halide::Tools;
using namespace std::chrono;
namespace fs = std::filesystem;

namespace fs = std::filesystem;

float processImage(const std::string& inputPath, const std::string& outputPath) {
    auto inicio_carregamento = std::chrono::high_resolution_clock::now();
    Buffer<uint8_t> input = load_image(inputPath);
    auto fim_carregamento = std::chrono::high_resolution_clock::now();

    if (!input.defined()) {
        printf("Erro ao carregar a imagem: %s\n", inputPath.c_str());
        return 0.0f;
    }

    Var x("x"), y("y"), c("c");
    Func logaritmaTransformada("logaritmaTransformada");

    Expr valor = cast<float>(input(x, y, c));
    float coef = 255.0f / log(1.0f + 255.0f);
    Expr resultado = coef * log(1.0f + valor);

    logaritmaTransformada(x, y, c) = cast<uint8_t>(clamp(resultado, 0.0f, 255.0f)); // imagem de 8 bits (0 a 255)
    logaritmaTransformada.parallel(y).vectorize(x, 16); // processa 16 pixels na direção x de cada vez

    auto inicio = std::chrono::high_resolution_clock::now();
    Buffer<uint8_t> output = logaritmaTransformada.realize({ input.width(), input.height(), input.channels() });
    auto fim = std::chrono::high_resolution_clock::now();

    save_image(output, outputPath);

    auto duracao_processamento = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count();
    std::string fileName = fs::path(inputPath).filename().string();
    printf("Duracao do processamento de %s: %ld ms\n", fileName.c_str(), duracao_processamento);
    return static_cast<float>(duracao_processamento);
}

int main(int argc, char** argv) {
    std::string inputDir = "/mnt/c/Users/Cliente/Downloads/base_dados/Imagens_Selecionadas";
    std::string outputDir = "/mnt/c/Users/Cliente/Downloads/base_dados/Saida_Python_Logaritmo";

    std::vector<float> mediasGerais;

    // Pré-aquecimento
    for (int i = 0; i < 1; i++) {
        for (const auto& entry : fs::directory_iterator(inputDir)) {
            std::string inputPath = entry.path().string();
            std::string outputPath = outputDir + "\\" + entry.path().filename().string();
            processImage(inputPath, outputPath);
        }
    }

    for (int exec = 0; exec < 1; exec++) {
        std::vector<float> medias;

        for (const auto& entry : fs::directory_iterator(inputDir)) {
            std::string inputPath = entry.path().string();
            std::string outputPath = outputDir + "\\" + entry.path().filename().string();
            float duracao = processImage(inputPath, outputPath);
            medias.push_back(static_cast<float>(duracao));
            std::string fileName = fs::path(inputPath).filename().string();
            printf("Duracao da execucao para %s: %f ms\n", fileName.c_str(), duracao);
        }

        if (!medias.empty()) {
            medias.erase(medias.begin()); // Remove a primeira medição
        }

        float mediaExecucao = std::accumulate(medias.begin(), medias.end(), 0.0f) / medias.size();
        mediasGerais.push_back(mediaExecucao);
        printf("Media de tempo para a execucao %d: %f ms\n", exec + 1, mediaExecucao);
    }

    float mediaFinal = std::accumulate(mediasGerais.begin(), mediasGerais.end(), 0.0f) / mediasGerais.size();
    printf("Media geral das medias de tempo apos 1 execucoes: %f ms\n", mediaFinal);

    return 0;
}
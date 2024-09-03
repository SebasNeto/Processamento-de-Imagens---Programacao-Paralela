//Filtro da media com pré compilação

#include "cabecalhosHalide.h"
#include <vector>
#include <string>
#include <filesystem>
#include <numeric>
#include <chrono>

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
    Func mediaFiltro("mediaFiltro");

    // Utilizar BoundaryConditions para tratar bordas
    Func inputBound = BoundaryConditions::repeat_edge(input);

    // Definindo a janela do filtro da média (3x3)
    RDom janela(-1, 3, -1, 3);

    // Aplicando o filtro da média
    Expr soma = sum(cast<float>(inputBound(x + janela.x, y + janela.y, c)));
    Expr media = soma / 9.0f;

    mediaFiltro(x, y, c) = cast<uint8_t>(clamp(media, 0.0f, 255.0f));
    mediaFiltro.parallel(y).vectorize(x, 16);

    auto inicio = std::chrono::high_resolution_clock::now();
    Buffer<uint8_t> output = mediaFiltro.realize({ input.width(), input.height(), input.channels() });
    auto fim = std::chrono::high_resolution_clock::now();

    save_image(output, outputPath);

    auto duracao_processamento = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count();
    std::string fileName = fs::path(inputPath).filename().string();
    printf("Duracao do processamento de %s: %lld ms\n", fileName.c_str(), duracao_processamento);
    return static_cast<float>(duracao_processamento);
}

int main(int argc, char** argv) {
    std::string inputDir = "C:\\Users\\Cliente\\Downloads\\base_dados\\Imagens_Selecionadas";
    std::string outputDir = "C:\\Users\\Cliente\\Downloads\\base_dados\\Saida_Halide_Filtro_Media";

    std::vector<float> mediasGerais;

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

        // if (!medias.empty()) {
        //     medias.erase(medias.begin()); // Remove a primeira medição
        // }

        float mediaExecucao = std::accumulate(medias.begin(), medias.end(), 0.0f) / medias.size();
        mediasGerais.push_back(mediaExecucao);
        printf("Media de tempo para a execucao %d: %f ms\n", exec + 1, mediaExecucao);
    }

    float mediaFinal = std::accumulate(mediasGerais.begin(), mediasGerais.end(), 0.0f) / mediasGerais.size();
    printf("Media geral das medias de tempo apos 1 execucoes: %f ms\n", mediaFinal);

    return 0;
}
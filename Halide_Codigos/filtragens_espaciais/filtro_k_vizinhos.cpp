//Filtro dos k-vizinhos mais próximos com pré compilação

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

Expr knnFilter(Expr a, Expr b, Expr c, Expr d, Expr e, Expr f, Expr g, Expr h, Expr i, int k) {
    // Ordenando os nove valores manualmente para encontrar os k menores valores
    Expr m1 = min(a, b); a = max(a, b); b = m1;
    m1 = min(c, d); c = max(c, d); d = m1;
    m1 = min(e, f); e = max(e, f); f = m1;
    m1 = min(g, h); g = max(g, h); h = m1;
    a = min(a, c); c = max(a, c);
    e = min(e, g); g = max(e, g);
    b = min(b, d); d = max(b, d);
    f = min(f, h); h = max(f, h);
    a = min(a, e); e = max(a, e);
    b = min(b, f); f = max(b, f);
    c = min(c, g); g = max(c, g);
    d = min(d, h); h = max(d, h);
    c = min(c, e); e = max(c, e);
    d = min(d, f); f = max(d, f);
    g = min(g, h); h = max(g, h);
    c = min(c, d); d = max(c, d);
    e = min(e, f); f = max(e, f);
    c = min(c, g); g = max(c, g);
    e = min(e, g); g = max(e, g);
    f = min(f, g); g = max(f, g);

    // Selecionando os k menores valores e calculando a média
    std::vector<Expr> menores = { a, b, c, d, e };
    Expr soma = menores[0];
    for (int j = 1; j < k; j++) {
        soma += menores[j];
    }
    return soma / k;
}

float processImage(const std::string& inputPath, const std::string& outputPath) {
    auto inicio_carregamento = std::chrono::high_resolution_clock::now();
    Buffer<uint8_t> input = load_image(inputPath);
    auto fim_carregamento = std::chrono::high_resolution_clock::now();

    if (!input.defined()) {
        printf("Erro ao carregar a imagem: %s\n", inputPath.c_str());
        return 0.0f;
    }

    Var x("x"), y("y"), c("c");
    Func knnFiltro("knnFiltro");

    // Utilizar BoundaryConditions para tratar bordas
    Func inputBound = BoundaryConditions::repeat_edge(input);

    // Aplicando o filtro dos k-vizinhos mais próximos (k=5)
    Expr knn = knnFilter(
        inputBound(x - 1, y - 1, c), inputBound(x, y - 1, c), inputBound(x + 1, y - 1, c),
        inputBound(x - 1, y, c), inputBound(x, y, c), inputBound(x + 1, y, c),
        inputBound(x - 1, y + 1, c), inputBound(x, y + 1, c), inputBound(x + 1, y + 1, c),
        5
    );

    knnFiltro(x, y, c) = cast<uint8_t>(knn);
    knnFiltro.parallel(y).vectorize(x, 16);

    auto inicio = std::chrono::high_resolution_clock::now();
    Buffer<uint8_t> output = knnFiltro.realize({ input.width(), input.height(), input.channels() });
    auto fim = std::chrono::high_resolution_clock::now();

    save_image(output, outputPath);

    auto duracao_processamento = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count();
    std::string fileName = fs::path(inputPath).filename().string();
    printf("Duracao do processamento de %s: %ld ms\n", fileName.c_str(), duracao_processamento);
    return static_cast<float>(duracao_processamento);
}

int main(int argc, char** argv) {

    std::string inputDir = "/mnt/c/Users/Cliente/Downloads/base_dados/Imagens_Selecionadas";
    std::string outputDir = "/mnt/c/Users/Cliente/Downloads/base_dados/Saida_Halide_Filtro_KNN";
    
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
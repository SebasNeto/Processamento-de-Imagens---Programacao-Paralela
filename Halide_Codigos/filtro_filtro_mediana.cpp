//Filtro da mediana com pré compilação

#include "cabecalhosHalide.h"
#include <vector>
#include <string>
#include <filesystem>
#include <numeric>
#include <chrono>

namespace fs = std::filesystem;

Expr retornaMediana(Expr a, Expr b, Expr c, Expr d, Expr e, Expr f, Expr g, Expr h, Expr i) {
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
    return e;
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
    Func medianaFiltro("medianaFiltro");

    // Utilizar BoundaryConditions para tratar bordas
    Func inputBound = BoundaryConditions::repeat_edge(input);

    // Definindo a janela do filtro da mediana (3x3)
    RDom janela(-1, 3, -1, 3);

    // Aplicando o filtro da mediana
    Expr mediana = retornaMediana(
        inputBound(x - 1, y - 1, c), inputBound(x, y - 1, c), inputBound(x + 1, y - 1, c),
        inputBound(x - 1, y, c), inputBound(x, y, c), inputBound(x + 1, y, c),
        inputBound(x - 1, y + 1, c), inputBound(x, y + 1, c), inputBound(x + 1, y + 1, c)
    );

    medianaFiltro(x, y, c) = cast<uint8_t>(mediana);
    medianaFiltro.parallel(y).vectorize(x, 16);

    auto inicio = std::chrono::high_resolution_clock::now();
    Buffer<uint8_t> output = medianaFiltro.realize({ input.width(), input.height(), input.channels() });
    auto fim = std::chrono::high_resolution_clock::now();

    save_image(output, outputPath);

    auto duracao_processamento = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count();
    std::string fileName = fs::path(inputPath).filename().string();
    printf("Duracao do processamento de %s: %lld ms\n", fileName.c_str(), duracao_processamento);
    return static_cast<float>(duracao_processamento);
}

int main(int argc, char** argv) {
    std::string inputDir = "C:\\Users\\Cliente\\Downloads\\base_dados\\Imagens_Selecionadas";
    std::string outputDir = "C:\\Users\\Cliente\\Downloads\\base_dados\\Saida_Halide_Filtro_Mediana";

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
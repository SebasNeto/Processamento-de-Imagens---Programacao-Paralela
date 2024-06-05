/*

    O CÓDIGO ABAIXO ESTÁ SENDO ARMAZENADO PARA FINS DE BACKUP - 20/05/2024.
    TRANSFORMADA UTILIZADA PARA TESTE.
*/

//Transformada Expansão de Constraste Linear 

#include "cabecalhosHalide.h"
#include <vector>
#include <string>
#include <filesystem>
#include <numeric>
#include <chrono>

namespace fs = std::filesystem;

float processImage(const std::string& inputPath, const std::string& outputPath) {

    Buffer<uint8_t> input = load_image(inputPath);

    if (!input.defined()) {
        printf("Erro ao carregar a imagem: %s\n", inputPath.c_str());
        return 0.0f;
    }

    int za = 100;
    int zb = 200;
    int z1 = 50;
    int zn = 200;
    float a = static_cast<float>(zn - z1) / (zb - za);
    float b = z1 - a * za;

    Var x("x"), y("y"), c("c");
    Func expansaoLinear("expansaoLinear");
    Expr valor = input(x, y, c);
    Expr transformada = select(valor <= za, cast<uint8_t>(z1),
        valor >= zb, cast<uint8_t>(zn),
        cast<uint8_t>(a * valor + b));

    expansaoLinear(x, y, c) = transformada;
    expansaoLinear.parallel(y).vectorize(x, 16);

    auto inicio = std::chrono::high_resolution_clock::now();
    Buffer<uint8_t> output = expansaoLinear.realize({ input.width(), input.height(), input.channels() });
    auto fim = std::chrono::high_resolution_clock::now();


    save_image(output, outputPath);
    //printf("Imagem Salva: %s\n", outputPath.c_str());


    auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count();
    printf("Duracao do processamento de %s: %lld ms\n", inputPath.c_str(), duracao);
    return static_cast<float>(duracao);
}

int main(int argc, char** argv) {
    std::string inputDir = "C:\\Users\\Cliente\\Downloads\\base_dados\\Imagens_Selecionadas";
    std::string outputDir = "C:\\Users\\Cliente\\Downloads\\base_dados\\Saida_Halide";

    std::vector<float> mediasGerais;

    for (int exec = 0; exec < 1; exec++) {
        printf("############### Iniciando execucao %d ######################\n", exec + 1);
        std::vector<float> medias;

        for (const auto& entry : fs::directory_iterator(inputDir)) {
            std::string inputPath = entry.path().string();
            std::string outputPath = outputDir + "\\" + entry.path().filename().string();
            float duracao = processImage(inputPath, outputPath);
            medias.push_back(static_cast<float>(duracao));
            printf("Duracao da execucao para %s: %f ms\n", inputPath.c_str(), duracao);
        }

        float mediaExecucao = std::accumulate(medias.begin(), medias.end(), 0.0f) / medias.size();
        mediasGerais.push_back(mediaExecucao);
        printf("Media de tempo para a execucao %d: %f ms\n", exec + 1, mediaExecucao);
    }

    float mediaFinal = std::accumulate(mediasGerais.begin(), mediasGerais.end(), 0.0f) / mediasGerais.size();
    printf("Media geral das medias de tempo apos 1 execucoes: %f ms\n", mediaFinal);

    return 0;
}

//Transformada do Logaritmo

#include "cabecalhosHalide.h"
#include <vector>
#include <string>
#include <filesystem>
#include <numeric>
#include <chrono>

namespace fs = std::filesystem;

float processImage(const std::string& inputPath, const std::string& outputPath) {

    Buffer<uint8_t> input = load_image(inputPath);

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
    // printf("Imagem Salva: %s\n", outputPath.c_str());

    auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count();
    std::string fileName = fs::path(inputPath).filename().string();
    printf("Duracao do processamento de %s: %lld ms\n", fileName.c_str(), duracao);
    return static_cast<float>(duracao);
}

int main(int argc, char** argv) {
    std::string inputDir = "C:\\Users\\Cliente\\Downloads\\base_dados\\Imagens_Selecionadas";
    std::string outputDir = "C:\\Users\\Cliente\\Downloads\\base_dados\\Saida_Halide_Logaritmo";

    std::vector<float> mediasGerais;

    for (int exec = 0; exec < 1; exec++) {
        printf("############### Iniciando execucao %d ######################\n", exec + 1);
        std::vector<float> medias;

        for (const auto& entry : fs::directory_iterator(inputDir)) {
            std::string inputPath = entry.path().string();
            std::string outputPath = outputDir + "\\" + entry.path().filename().string();
            float duracao = processImage(inputPath, outputPath);
            medias.push_back(static_cast<float>(duracao));
            std::string fileName = entry.path().filename().string();
            printf("Duracao da execucao para %s: %f ms\n", fileName.c_str(), duracao);
        }

        float mediaExecucao = std::accumulate(medias.begin(), medias.end(), 0.0f) / medias.size();
        mediasGerais.push_back(mediaExecucao);
        printf("Media de tempo para a execucao %d: %f ms\n", exec + 1, mediaExecucao);
    }

    float mediaFinal = std::accumulate(mediasGerais.begin(), mediasGerais.end(), 0.0f) / mediasGerais.size();
    printf("Media geral das medias de tempo apos 1 execucoes: %f ms\n", mediaFinal);

    return 0;
}

//transformada dente de serra

#include "cabecalhosHalide.h"
#include <vector>
#include <string>
#include <filesystem>
#include <numeric>
#include <chrono>

namespace fs = std::filesystem;

float processImage(const std::string& inputPath, const std::string& outputPath) {

    Buffer<uint8_t> input = load_image(inputPath);

    if (!input.defined()) {
        printf("Erro ao carregar a imagem: %s\n", inputPath.c_str());
        return 0.0f;
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

    auto inicio = std::chrono::high_resolution_clock::now();
    Buffer<uint8_t> output = transformada_dente_de_serra.realize({ input.width(), input.height(), input.channels() });
    auto fim = std::chrono::high_resolution_clock::now();

    save_image(output, outputPath);
    // printf("Imagem Salva: %s\n", outputPath.c_str());

    auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count();
    std::string fileName = fs::path(inputPath).filename().string();
    printf("Duracao do processamento de %s: %lld ms\n", fileName.c_str(), duracao);
    return static_cast<float>(duracao);
}

int main(int argc, char** argv) {
    std::string inputDir = "C:\\Users\\Cliente\\Downloads\\base_dados\\Imagens_Selecionadas";
    std::string outputDir = "C:\\Users\\Cliente\\Downloads\\base_dados\\Saida_Halide_Dente_de_Serra";

    std::vector<float> mediasGerais;

    for (int exec = 0; exec < 1; exec++) {
        printf("############### Iniciando execucao %d ######################\n", exec + 1);
        std::vector<float> medias;

        for (const auto& entry : fs::directory_iterator(inputDir)) {
            std::string inputPath = entry.path().string();
            std::string outputPath = outputDir + "\\" + entry.path().filename().string();
            float duracao = processImage(inputPath, outputPath);
            medias.push_back(static_cast<float>(duracao));
            std::string fileName = entry.path().filename().string();
            printf("Duracao da execucao para %s: %f ms\n", fileName.c_str(), duracao);
        }

        float mediaExecucao = std::accumulate(medias.begin(), medias.end(), 0.0f) / medias.size();
        mediasGerais.push_back(mediaExecucao);
        printf("Media de tempo para a execucao %d: %f ms\n", exec + 1, mediaExecucao);
    }

    float mediaFinal = std::accumulate(mediasGerais.begin(), mediasGerais.end(), 0.0f) / mediasGerais.size();
    printf("Media geral das medias de tempo apos 1 execucoes: %f ms\n", mediaFinal);

    return 0;
}

//transformada expansão linear com pré compilação

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

    int za = 100;
    int zb = 200;
    int z1 = 50;
    int zn = 200;
    float a = static_cast<float>(zn - z1) / (zb - za);
    float b = z1 - a * za;

    Var x("x"), y("y"), c("c");
    Func expansaoLinear("expansaoLinear");
    Expr valor = input(x, y, c);
    Expr transformada = select(valor <= za, cast<uint8_t>(z1),
        valor >= zb, cast<uint8_t>(zn),
        cast<uint8_t>(a * valor + b));

    expansaoLinear(x, y, c) = transformada;
    expansaoLinear.parallel(y).vectorize(x, 16);

    auto inicio = std::chrono::high_resolution_clock::now();
    Buffer<uint8_t> output = expansaoLinear.realize({ input.width(), input.height(), input.channels() });
    auto fim = std::chrono::high_resolution_clock::now();

    save_image(output, outputPath);

    //auto duracao_carregamento = std::chrono::duration_cast<std::chrono::milliseconds>(fim_carregamento - inicio_carregamento).count();
    auto duracao_processamento = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count();
    //auto duracao_total = duracao_carregamento + duracao_processamento;

    std::string fileName = fs::path(inputPath).filename().string();
    //printf("Duracao do carregamento de %s: %lld ms\n", fileName.c_str(), duracao_carregamento);
    printf("Duracao do processamento de %s: %lld ms\n", fileName.c_str(), duracao_processamento);
    //printf("Duracao total de %s: %lld ms\n", fileName.c_str(), duracao_total);
    return static_cast<float>(duracao_processamento);
}

int main(int argc, char** argv) {
    std::string inputDir = "C:\\Users\\Cliente\\Downloads\\base_dados\\Imagens_Selecionadas";
    std::string outputDir = "C:\\Users\\Cliente\\Downloads\\base_dados\\Saida_Halide_Expansao_Contraste";

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

//transformada do logaritmo com pré compilação

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
    printf("Duracao do processamento de %s: %lld ms\n", fileName.c_str(), duracao_processamento);
    return static_cast<float>(duracao_processamento);
}

int main(int argc, char** argv) {
    std::string inputDir = "C:\\Users\\Cliente\\Downloads\\base_dados\\Imagens_Selecionadas";
    std::string outputDir = "C:\\Users\\Cliente\\Downloads\\base_dados\\Saida_Halide_Logaritmo";

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

//transformada dente de serra com pré compilação

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

    auto inicio = std::chrono::high_resolution_clock::now();
    Buffer<uint8_t> output = transformada_dente_de_serra.realize({ input.width(), input.height(), input.channels() });
    auto fim = std::chrono::high_resolution_clock::now();

    save_image(output, outputPath);

    auto duracao_processamento = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count();
    std::string fileName = fs::path(inputPath).filename().string();
    printf("Duracao do processamento de %s: %lld ms\n", fileName.c_str(), duracao_processamento);
    return static_cast<float>(duracao_processamento);
}

int main(int argc, char** argv) {
    std::string inputDir = "C:\\Users\\Cliente\\Downloads\\base_dados\\Imagens_Selecionadas";
    std::string outputDir = "C:\\Users\\Cliente\\Downloads\\base_dados\\Saida_Halide_Dente_de_Serra";

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
        printf("############### Iniciando execucao %d ######################\n", exec + 1);
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

//Filtro dos k-vizinhos mais próximos com pré compilação

#include "cabecalhosHalide.h"
#include <vector>
#include <string>
#include <filesystem>
#include <numeric>
#include <chrono>

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
    printf("Duracao do processamento de %s: %lld ms\n", fileName.c_str(), duracao_processamento);
    return static_cast<float>(duracao_processamento);
}

int main(int argc, char** argv) {
    std::string inputDir = "C:\\Users\\Cliente\\Downloads\\base_dados\\Imagens_Selecionadas";
    std::string outputDir = "C:\\Users\\Cliente\\Downloads\\base_dados\\Saida_Halide_Filtro_KNN";

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

//detecção de bordas sobel
#include "cabecalhosHalide.h"
#include <vector>
#include <string>
#include <filesystem>
#include <numeric>
#include <chrono>

namespace fs = std::filesystem;

float processSobelImage(const std::string& inputPath, const std::string& outputPath) {
    auto inicio_carregamento = std::chrono::high_resolution_clock::now();
    Buffer<uint8_t> input = load_image(inputPath);
    auto fim_carregamento = std::chrono::high_resolution_clock::now();

    if (!input.defined()) {
        printf("Erro ao carregar a imagem: %s\n", inputPath.c_str());
        return 0.0f;
    }

    Var x("x"), y("y"), c("c");

    // Definir os kernels de Sobel para Gx e Gy como Func.
    Func sobel_x("sobel_x"), sobel_y("sobel_y");
    sobel_x(x, y) = select(
        x == -1 && y == -1, -1, x == 0 && y == -1, 0, x == 1 && y == -1, 1,
        x == -1 && y == 0, -2, x == 0 && y == 0, 0, x == 1 && y == 0, 2,
        x == -1 && y == 1, -1, x == 0 && y == 1, 0, x == 1 && y == 1, 1,
        0);
    sobel_y(x, y) = select(
        x == -1 && y == -1, -1, x == 0 && y == -1, -2, x == 1 && y == -1, -1,
        x == -1 && y == 0, 0, x == 0 && y == 0, 0, x == 1 && y == 0, 0,
        x == -1 && y == 1, 1, x == 0 && y == 1, 2, x == 1 && y == 1, 1,
        0);

    Func clamped = BoundaryConditions::repeat_edge(input);
    Func sobel_gx("sobel_gx"), sobel_gy("sobel_gy"), magnitude("magnitude");
    RDom r(-1, 3, -1, 3);

    // Convolve com Gx
    sobel_gx(x, y, c) = sum(clamped(x + r.x, y + r.y, c) * sobel_x(r.x, r.y));

    // Convolve com Gy
    sobel_gy(x, y, c) = sum(clamped(x + r.x, y + r.y, c) * sobel_y(r.x, r.y));

    // Calcular a magnitude do gradiente (sqrt(gx^2 + gy^2)).
    magnitude(x, y, c) = cast<uint8_t>(sqrt(sobel_gx(x, y, c) * sobel_gx(x, y, c) +
        sobel_gy(x, y, c) * sobel_gy(x, y, c)));

    // Aplicar paralelismo e vetorização.
    sobel_gx.compute_root().parallel(y).vectorize(x, 16);
    sobel_gy.compute_root().parallel(y).vectorize(x, 16);
    magnitude.compute_root().parallel(y).vectorize(x, 16);

    auto inicio = std::chrono::high_resolution_clock::now();
    Buffer<uint8_t> output = magnitude.realize({ input.width(), input.height(), input.channels() });
    auto fim = std::chrono::high_resolution_clock::now();

    save_image(output, outputPath);

    auto duracao_processamento = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count();
    std::string fileName = fs::path(inputPath).filename().string();
    printf("Duracao do processamento de %s: %lld ms\n", fileName.c_str(), duracao_processamento);
    return static_cast<float>(duracao_processamento);
}

int main(int argc, char** argv) {
    std::string inputDir = "C:\\Users\\Cliente\\Downloads\\base_dados\\Imagens_Selecionadas";
    std::string outputDir = "C:\\Users\\Cliente\\Downloads\\base_dados\\Saida_Halide_Sobel";

    std::vector<float> mediasGerais;

    // Pré-aquecimento
    for (int i = 0; i < 1; i++) {
        for (const auto& entry : fs::directory_iterator(inputDir)) {
            std::string inputPath = entry.path().string();
            std::string outputPath = outputDir + "\\" + entry.path().filename().string();
            processSobelImage(inputPath, outputPath);
        }
    }

    for (int exec = 0; exec < 1; exec++) {
        std::vector<float> medias;

        for (const auto& entry : fs::directory_iterator(inputDir)) {
            std::string inputPath = entry.path().string();
            std::string outputPath = outputDir + "\\" + entry.path().filename().string();
            float duracao = processSobelImage(inputPath, outputPath);
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

//detecção de bordas roberts
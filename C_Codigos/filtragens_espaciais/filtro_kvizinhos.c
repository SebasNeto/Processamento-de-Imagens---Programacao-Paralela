#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <dirent.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <numeric>

//g++ dente_de_serra.c -o testeSerra `pkg-config --cflags --libs opencv4`

using namespace cv;

void filtro_k_vizinhos_proximos(Mat imagem, Mat& imagemFiltrada, int tamJanela, int k) {
    int altura = imagem.rows;
    int largura = imagem.cols;
    int canais = imagem.channels();

    imagemFiltrada = imagem.clone();
    int margem = tamJanela / 2;

    for (int i = margem; i < altura - margem; i++) {
        for (int j = margem; j < largura - margem; j++) {
            for (int canal = 0; canal < canais; canal++) {
                std::vector<int> vizinhos;
                
                for (int x = -margem; x <= margem; x++) {
                    for (int y = -margem; y <= margem; y++) {
                        int pixel_vizinho = imagem.at<Vec3b>(i + x, j + y)[canal];
                        vizinhos.push_back(pixel_vizinho);
                    }
                }
                
                int pixel_central = imagem.at<Vec3b>(i, j)[canal];
                std::vector<int> distancias;
                for (int vizinho : vizinhos) {
                    distancias.push_back(abs(vizinho - pixel_central));
                }

                std::vector<size_t> indices_vizinhos_proximos(distancias.size());
                iota(indices_vizinhos_proximos.begin(), indices_vizinhos_proximos.end(), 0);

                std::partial_sort(indices_vizinhos_proximos.begin(), indices_vizinhos_proximos.begin() + k, indices_vizinhos_proximos.end(), 
                    [&distancias](size_t a, size_t b) { return distancias[a] < distancias[b]; });

                int soma_vizinhos = 0;
                for (int n = 0; n < k; n++) {
                    soma_vizinhos += vizinhos[indices_vizinhos_proximos[n]];
                }

                imagemFiltrada.at<Vec3b>(i, j)[canal] = soma_vizinhos / k;
            }
        }
    }
}

void processarDiretorio(const char* input_dir, const char* output_dir, std::vector<double>& tempos_execucao, int tamJanela, int k) {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(input_dir)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".png") || strstr(ent->d_name, ".jpg") || strstr(ent->d_name, ".jpeg") || strstr(ent->d_name, ".bmp")) {
                char caminho_imagem[1024];
                snprintf(caminho_imagem, sizeof(caminho_imagem), "%s/%s", input_dir, ent->d_name);

                Mat imagem = imread(caminho_imagem, IMREAD_COLOR);
                if (!imagem.empty()) {
                    Mat imagemFiltrada;
                    clock_t start_time = clock();
                    filtro_k_vizinhos_proximos(imagem, imagemFiltrada, tamJanela, k);
                    clock_t end_time = clock();
                    double elapsed_time = (double)(end_time - start_time) * 1000.0 / CLOCKS_PER_SEC;
                    tempos_execucao.push_back(elapsed_time);

                    char caminho_imagem_filtrada[1024];
                    snprintf(caminho_imagem_filtrada, sizeof(caminho_imagem_filtrada), "%s/%s", output_dir, ent->d_name);
                    imwrite(caminho_imagem_filtrada, imagemFiltrada);

                    printf("Tempo de processamento de %s: %.4f ms\n", ent->d_name, elapsed_time);
                } else {
                    printf("Erro ao carregar a imagem: %s\n", caminho_imagem);
                }
            }
        }
        closedir(dir);
    } else {
        perror("Erro ao abrir diretório");
    }
}

void multiplasExecucoes(const char* input_dir, const char* output_dir, int tamJanela, int k, int execucoes) {
    std::vector<double> tempos_todas_execucoes;
    for (int execucao = 0; execucao < execucoes; execucao++) {
        printf("Iniciando execução %d\n", execucao + 1);
        std::vector<double> tempos_execucao;
        processarDiretorio(input_dir, output_dir, tempos_execucao, tamJanela, k);
        tempos_todas_execucoes.insert(tempos_todas_execucoes.end(), tempos_execucao.begin(), tempos_execucao.end());

        if (!tempos_execucao.empty()) {
            double media_execucao = std::accumulate(tempos_execucao.begin(), tempos_execucao.end(), 0.0) / tempos_execucao.size();
            printf("Média de tempo para a execução %d: %.4f ms\n", execucao + 1, media_execucao);
        } else {
            printf("Nenhuma imagem processada nesta execução.\n");
        }
    }

    if (!tempos_todas_execucoes.empty()) {
        double media_geral = std::accumulate(tempos_todas_execucoes.begin(), tempos_todas_execucoes.end(), 0.0) / tempos_todas_execucoes.size();
        printf("Média geral das médias de tempo após %d execuções: %.4f ms\n", execucoes, media_geral);
    } else {
        printf("Nenhuma imagem foi processada em nenhuma execução.\n");
    }
}

int main() {
    const char* input_dir = "/mnt/c/Users/Cliente/Downloads/base_dados/Imagens_Selecionadas";
    const char* output_dir = "/mnt/c/Users/Cliente/Downloads/base_dados/Saida_C_Filtro_KNN";

    multiplasExecucoes(input_dir, output_dir, 3, 3, 1);

    return 0;
}

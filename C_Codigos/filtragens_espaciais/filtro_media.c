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

using namespace cv;

void filtroMedia(Mat imagem, Mat& imagemFiltrada, int tam_janela, int T) {
    int altura = imagem.rows;
    int largura = imagem.cols;
    int canais = imagem.channels();
    
    imagemFiltrada = imagem.clone();
    int margem = tam_janela / 2;

    for (int i = margem; i < altura - margem; i++) {
        for (int j = margem; j < largura - margem; j++) {
            Vec3f media(0, 0, 0);

            for (int x = -margem; x <= margem; x++) {
                for (int y = -margem; y <= margem; y++) {
                    Vec3b pixel = imagem.at<Vec3b>(i + x, j + y);
                    media[0] += pixel[0];
                    media[1] += pixel[1];
                    media[2] += pixel[2];
                }
            }

            int area = tam_janela * tam_janela;
            media[0] /= area;
            media[1] /= area;
            media[2] /= area;

            Vec3b pixel_central = imagem.at<Vec3b>(i, j);
            Vec3f diferencaAbs(fabs(media[0] - pixel_central[0]), fabs(media[1] - pixel_central[1]), fabs(media[2] - pixel_central[2]));

            if (diferencaAbs[0] > T) imagemFiltrada.at<Vec3b>(i, j)[0] = (uchar)media[0];
            if (diferencaAbs[1] > T) imagemFiltrada.at<Vec3b>(i, j)[1] = (uchar)media[1];
            if (diferencaAbs[2] > T) imagemFiltrada.at<Vec3b>(i, j)[2] = (uchar)media[2];
        }
    }
}

void processarDiretorio(const char* input_dir, const char* output_dir, std::vector<double>& tempos_execucao, int tam_janela, int T) {
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
                    filtroMedia(imagem, imagemFiltrada, tam_janela, T);
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

void multiplasExecucoes(const char* input_dir, const char* output_dir, int tam_janela, int T, int execucoes) {
    std::vector<double> tempos_todas_execucoes;
    for (int execucao = 0; execucao < execucoes; execucao++) {
        printf("Iniciando execução %d\n", execucao + 1);
        std::vector<double> tempos_execucao;
        processarDiretorio(input_dir, output_dir, tempos_execucao, tam_janela, T);
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
    const char* output_dir = "/mnt/c/Users/Cliente/Downloads/base_dados/Saida_C_Filtro_Media";

    multiplasExecucoes(input_dir, output_dir, 3, 60, 1);

    return 0;
}

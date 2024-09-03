#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <numeric>

using namespace cv;

void transformada_dente_de_serra(Mat imagem, Mat& imagem_transformada) {
    int altura = imagem.rows;
    int largura = imagem.cols;
    
    imagem_transformada = Mat::zeros(altura, largura, CV_32F);

    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            int pixel = imagem.at<uchar>(i, j);
            
            if (pixel < 63) {
                imagem_transformada.at<float>(i, j) = 255.0 * pixel / 62.0;
            } else if (pixel >= 63 && pixel < 127) {
                imagem_transformada.at<float>(i, j) = 255.0 * (pixel - 63) / 63.0;
            } else if (pixel >= 127 && pixel < 191) {
                imagem_transformada.at<float>(i, j) = 255.0 * (pixel - 127) / 63.0;
            } else {
                imagem_transformada.at<float>(i, j) = 255.0 * (pixel - 191) / 64.0;
            }
        }
    }

    imagem_transformada.convertTo(imagem_transformada, CV_8U, 1.0, 0.0);
}

void processarDiretorio(const char* input_dir, const char* output_dir, std::vector<double>& tempos_execucao) {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(input_dir)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".png") || strstr(ent->d_name, ".jpg") || strstr(ent->d_name, ".jpeg") || strstr(ent->d_name, ".bmp")) {
                char caminho_imagem[1024];
                snprintf(caminho_imagem, sizeof(caminho_imagem), "%s/%s", input_dir, ent->d_name);

                Mat imagem = imread(caminho_imagem, IMREAD_GRAYSCALE);
                if (!imagem.empty()) {
                    Mat imagem_transformada;
                    clock_t start_time = clock();
                    transformada_dente_de_serra(imagem, imagem_transformada);
                    clock_t end_time = clock();
                    double elapsed_time = (double)(end_time - start_time) * 1000.0 / CLOCKS_PER_SEC;
                    tempos_execucao.push_back(elapsed_time);

                    char caminho_imagem_transformada[1024];
                    snprintf(caminho_imagem_transformada, sizeof(caminho_imagem_transformada), "%s/%s", output_dir, ent->d_name);
                    imwrite(caminho_imagem_transformada, imagem_transformada);

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

void multiplasExecucoes(const char* input_dir, const char* output_dir, int execucoes) {
    std::vector<double> tempos_todas_execucoes;
    for (int execucao = 0; execucao < execucoes; execucao++) {
        printf("Iniciando execução %d\n", execucao + 1);
        std::vector<double> tempos_execucao;
        processarDiretorio(input_dir, output_dir, tempos_execucao);
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
    const char* output_dir = "/mnt/c/Users/Cliente/Downloads/base_dados/Saida_C_Dente_de_Serra";

    multiplasExecucoes(input_dir, output_dir, 1);

    return 0;
}

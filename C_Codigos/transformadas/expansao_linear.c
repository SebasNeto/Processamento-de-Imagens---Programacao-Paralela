
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <numeric>

using namespace cv;

void expansaoLinear(Mat imagem, Mat& imagem_expandida, int za, int zb, int z1, int zn) {
    int altura = imagem.rows;
    int largura = imagem.cols;
    
    imagem_expandida = Mat::zeros(altura, largura, imagem.type());

    float a = (float)(zn - z1) / (zb - za);
    int b = z1 - a * za;

    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            int pixel = imagem.at<uchar>(i, j);
            if (pixel <= za) {
                imagem_expandida.at<uchar>(i, j) = z1;
            } else if (pixel >= zb) {
                imagem_expandida.at<uchar>(i, j) = zn;
            } else {
                imagem_expandida.at<uchar>(i, j) = saturate_cast<uchar>(a * pixel + b);
            }
        }
    }
}

void processarDiretorio(const char* input_dir, const char* output_dir, std::vector<double>& tempos_execucao, int za, int zb, int z1, int zn) {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(input_dir)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".png") || strstr(ent->d_name, ".jpg") || strstr(ent->d_name, ".jpeg") || strstr(ent->d_name, ".bmp")) {
                char caminho_imagem[1024];
                snprintf(caminho_imagem, sizeof(caminho_imagem), "%s/%s", input_dir, ent->d_name);

                Mat imagem = imread(caminho_imagem, IMREAD_GRAYSCALE);
                if (!imagem.empty()) {
                    Mat imagem_expandida;
                    clock_t start_time = clock();
                    expansaoLinear(imagem, imagem_expandida, za, zb, z1, zn);
                    clock_t end_time = clock();
                    double elapsed_time = (double)(end_time - start_time) * 1000.0 / CLOCKS_PER_SEC;
                    tempos_execucao.push_back(elapsed_time);

                    char caminho_imagem_expandida[1024];
                    snprintf(caminho_imagem_expandida, sizeof(caminho_imagem_expandida), "%s/%s", output_dir, ent->d_name);
                    imwrite(caminho_imagem_expandida, imagem_expandida);

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

void multiplasExecucoes(const char* input_dir, const char* output_dir, int za, int zb, int z1, int zn, int execucoes) {
    std::vector<double> tempos_todas_execucoes;
    for (int execucao = 0; execucao < execucoes; execucao++) {
        printf("Iniciando execução %d\n", execucao + 1);
        std::vector<double> tempos_execucao;
        processarDiretorio(input_dir, output_dir, tempos_execucao, za, zb, z1, zn);
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
    int za = 100;
    int zb = 200;
    int z1 = 30;
    int zn = 150;

    const char* input_dir = "/mnt/c/Users/Cliente/Downloads/base_dados/Imagens_Selecionadas";
    const char* output_dir = "/mnt/c/Users/Cliente/Downloads/base_dados/Saida_C_Exp_Comp";

    multiplasExecucoes(input_dir, output_dir, za, zb, z1, zn, 1);

    return 0;
}

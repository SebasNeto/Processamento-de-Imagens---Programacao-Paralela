// #include <opencv2/opencv.hpp>
// #include <iostream>
// #include <vector>
// #include <string>
// #include <dirent.h>
// #include <sys/time.h>
// #include <algorithm>
// #include <numeric>

// cv::Mat filtroDaMediana(cv::Mat imagem, int tamJanela = 3) {
//     int altura = imagem.rows;
//     int largura = imagem.cols;
//     int canais = imagem.channels();
//     cv::Mat imagemFiltrada = imagem.clone();
//     int margem = tamJanela / 2;

//     for (int i = margem; i < altura - margem; i++) {
//         for (int j = margem; j < largura - margem; j++) {
//             for (int canal = 0; canal < canais; canal++) {
//                 std::vector<uchar> valores;
//                 for (int mi = -margem; mi <= margem; mi++) {
//                     for (int mj = -margem; mj <= margem; mj++) {
//                         valores.push_back(imagem.at<cv::Vec3b>(i + mi, j + mj)[canal]);
//                     }
//                 }
//                 std::nth_element(valores.begin(), valores.begin() + valores.size() / 2, valores.end());
//                 uchar mediana = valores[valores.size() / 2];
//                 imagemFiltrada.at<cv::Vec3b>(i, j)[canal] = mediana;
//             }
//         }
//     }

//     return imagemFiltrada;
// }

// std::vector<double> processarDiretorio(const std::string& input_dir, const std::string& output_dir, int tamJanela = 3) {
//     std::vector<double> tempos_execucao;
//     DIR* dir;
//     struct dirent* entry;

//     if ((dir = opendir(input_dir.c_str())) != NULL) {
//         while ((entry = readdir(dir)) != NULL) {
//             std::string filename = entry->d_name;
//             if (filename.find(".png") != std::string::npos || filename.find(".jpg") != std::string::npos ||
//                 filename.find(".jpeg") != std::string::npos || filename.find(".bmp") != std::string::npos) {
                
//                 std::string caminho_imagem = input_dir + "/" + filename;
//                 cv::Mat imagem = cv::imread(caminho_imagem, cv::IMREAD_COLOR);
//                 if (!imagem.empty()) {
//                     struct timeval start, end;
//                     gettimeofday(&start, NULL);

//                     cv::Mat imagemFiltrada = filtroDaMediana(imagem, tamJanela);

//                     gettimeofday(&end, NULL);
//                     double elapsed_time = ((end.tv_sec - start.tv_sec) * 1000.0) + ((end.tv_usec - start.tv_usec) / 1000.0);
//                     tempos_execucao.push_back(elapsed_time);

//                     std::string caminho_imagem_filtrada = output_dir + "/" + filename;
//                     cv::imwrite(caminho_imagem_filtrada, imagemFiltrada);

//                     std::cout << "Tempo de processamento de " << filename << ": " << elapsed_time << " ms" << std::endl;
//                 } else {
//                     std::cerr << "Erro ao carregar a imagem: " << caminho_imagem << std::endl;
//                 }
//             }
//         }
//         closedir(dir);
//     } else {
//         perror("Erro ao abrir o diretório de entrada");
//     }

//     return tempos_execucao;
// }

// void multiplasExecucoes(const std::string& input_dir, const std::string& output_dir, int tamJanela = 3, int execucoes = 1) {
//     std::vector<double> tempos_todas_execucoes;

//     for (int execucao = 0; execucao < execucoes; execucao++) {
//         std::cout << "Iniciando execução " << execucao + 1 << std::endl;
//         std::vector<double> tempos_execucao = processarDiretorio(input_dir, output_dir, tamJanela);
//         tempos_todas_execucoes.insert(tempos_todas_execucoes.end(), tempos_execucao.begin(), tempos_execucao.end());

//         if (!tempos_execucao.empty()) {
//             double soma_tempos = std::accumulate(tempos_execucao.begin(), tempos_execucao.end(), 0.0);
//             double media_execucao = soma_tempos / tempos_execucao.size();
//             std::cout << "Média de tempo para a execução " << execucao + 1 << ": " << media_execucao << " ms" << std::endl;
//         } else {
//             std::cout << "Nenhuma imagem processada nesta execução." << std::endl;
//         }
//     }

//     if (!tempos_todas_execucoes.empty()) {
//         double soma_tempos_geral = std::accumulate(tempos_todas_execucoes.begin(), tempos_todas_execucoes.end(), 0.0);
//         double media_geral = soma_tempos_geral / tempos_todas_execucoes.size();
//         std::cout << "Média geral das médias de tempo após " << execucoes << " execuções: " << media_geral << " ms" << std::endl;
//     } else {
//         std::cout << "Nenhuma imagem foi processada em nenhuma execução." << std::endl;
//     }
// }

// int main() {
//     std::string input_dir = "/mnt/c/Users/Cliente/Downloads/base_dados/Imagens_Selecionadas";
//     std::string output_dir = "/mnt/c/Users/Cliente/Downloads/base_dados/Saida_C_Filtro_Mediana";
//     int tamJanela = 3;
//     int execucoes = 1;

//     multiplasExecucoes(input_dir, output_dir, tamJanela, execucoes);

//     return 0;
// }

////////////////////////////////////////q

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <algorithm>
#include <numeric>

using namespace cv;

void filtro_da_mediana(Mat imagem, Mat& imagemFiltrada, int tamJanela) {
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
                        int pixel = imagem.at<Vec3b>(i + x, j + y)[canal];
                        vizinhos.push_back(pixel);
                    }
                }

                std::nth_element(vizinhos.begin(), vizinhos.begin() + vizinhos.size() / 2, vizinhos.end());
                int mediana = vizinhos[vizinhos.size() / 2];

                imagemFiltrada.at<Vec3b>(i, j)[canal] = mediana;
            }
        }
    }
}

void processarDiretorio(const char* input_dir, const char* output_dir, std::vector<double>& tempos_execucao, int tamJanela) {
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
                    filtro_da_mediana(imagem, imagemFiltrada, tamJanela);
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

void multiplasExecucoes(const char* input_dir, const char* output_dir, int tamJanela, int execucoes) {
    std::vector<double> tempos_todas_execucoes;
    for (int execucao = 0; execucao < execucoes; execucao++) {
        printf("Iniciando execução %d\n", execucao + 1);
        std::vector<double> tempos_execucao;
        processarDiretorio(input_dir, output_dir, tempos_execucao, tamJanela);
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
    const char* output_dir = "/mnt/c/Users/Cliente/Downloads/base_dados/Saida_C_Filtro_Mediana";

    multiplasExecucoes(input_dir, output_dir, 3, 1);

    return 0;
}

// #include <opencv2/opencv.hpp>
// #include <iostream>
// #include <vector>
// #include <string>
// #include <dirent.h>
// #include <sys/time.h>
// #include <cmath>

// cv::Mat filtroRoberts(const cv::Mat& imagem) {
//     int altura = imagem.rows;
//     int largura = imagem.cols;
//     int canais = imagem.channels();

//     cv::Mat imagemFiltrada = cv::Mat::zeros(imagem.size(), CV_32F);

//     cv::Mat Gx = (cv::Mat_<float>(2, 2) << 1, 0, 0, -1);
//     cv::Mat Gy = (cv::Mat_<float>(2, 2) << 0, 1, -1, 0);

//     for (int canal = 0; canal < canais; canal++) {
//         for (int i = 0; i < altura - 1; i++) {
//             for (int j = 0; j < largura - 1; j++) {
//                 cv::Mat regiao;
//                 if (canais == 1) {
//                     regiao = imagem(cv::Rect(j, i, 2, 2));
//                 } else {
//                     regiao = imagem(cv::Rect(j, i, 2, 2)).clone();
//                     regiao = regiao.reshape(1);
//                 }

//                 float gx = 0.0f, gy = 0.0f;
//                 for (int x = 0; x < 2; x++) {
//                     for (int y = 0; y < 2; y++) {
//                         gx += Gx.at<float>(x, y) * regiao.at<uchar>(x, y);
//                         gy += Gy.at<float>(x, y) * regiao.at<uchar>(x, y);
//                     }
//                 }

//                 float gradiente = std::sqrt(gx * gx + gy * gy);

//                 if (canais == 1) {
//                     imagemFiltrada.at<float>(i, j) = gradiente;
//                 } else {
//                     imagemFiltrada.at<cv::Vec3f>(i, j)[canal] = gradiente;
//                 }
//             }
//         }
//     }

//     double minVal, maxVal;
//     cv::minMaxLoc(imagemFiltrada, &minVal, &maxVal);
//     imagemFiltrada.convertTo(imagemFiltrada, CV_8U, 255.0 / maxVal);

//     return imagemFiltrada;
// }

// std::vector<double> processarDiretorio(const std::string& input_dir, const std::string& output_dir) {
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

//                     cv::Mat imagemFiltrada = filtroRoberts(imagem);

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

// void multiplasExecucoes(const std::string& input_dir, const std::string& output_dir, int execucoes = 1) {
//     std::vector<double> tempos_todas_execucoes;

//     for (int execucao = 0; execucao < execucoes; execucao++) {
//         std::cout << "Iniciando execução " << execucao + 1 << std::endl;
//         std::vector<double> tempos_execucao = processarDiretorio(input_dir, output_dir);
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
//     std::string output_dir = "/mnt/c/Users/Cliente/Downloads/base_dados/Saida_C_Roberts";
//     int execucoes = 1;

//     multiplasExecucoes(input_dir, output_dir, execucoes);

//     return 0;
// }

/////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <algorithm>
#include <numeric>

using namespace cv;

void filtro_roberts(Mat imagem, Mat& imagemFiltrada) {
    int altura = imagem.rows;
    int largura = imagem.cols;
    int canais = imagem.channels();

    imagemFiltrada = Mat::zeros(altura, largura, imagem.type());

    float Gx[2][2] = {{1, 0}, {0, -1}};
    float Gy[2][2] = {{0, 1}, {-1, 0}};

    for (int canal = 0; canal < canais; canal++) {
        for (int i = 0; i < altura - 1; i++) {
            for (int j = 0; j < largura - 1; j++) {
                float gx = 0.0, gy = 0.0;

                for (int x = 0; x < 2; x++) {
                    for (int y = 0; y < 2; y++) {
                        if (canais == 1) {
                            gx += Gx[x][y] * imagem.at<uchar>(i + x, j + y);
                            gy += Gy[x][y] * imagem.at<uchar>(i + x, j + y);
                        } else {
                            gx += Gx[x][y] * imagem.at<Vec3b>(i + x, j + y)[canal];
                            gy += Gy[x][y] * imagem.at<Vec3b>(i + x, j + y)[canal];
                        }
                    }
                }

                float gradiente = sqrt(gx * gx + gy * gy);

                if (canais == 1) {
                    imagemFiltrada.at<uchar>(i, j) = saturate_cast<uchar>(gradiente);
                } else {
                    imagemFiltrada.at<Vec3b>(i, j)[canal] = saturate_cast<uchar>(gradiente);
                }
            }
        }
    }

    // Normalização para o intervalo [0, 255]
    normalize(imagemFiltrada, imagemFiltrada, 0, 255, NORM_MINMAX, -1, Mat());
}

void processarDiretorio(const char* input_dir, const char* output_dir, std::vector<double>& tempos_execucao) {
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
                    filtro_roberts(imagem, imagemFiltrada);
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
    const char* output_dir = "/mnt/c/Users/Cliente/Downloads/base_dados/Saida_C_Roberts";


    multiplasExecucoes(input_dir, output_dir, 1);

    return 0;
}

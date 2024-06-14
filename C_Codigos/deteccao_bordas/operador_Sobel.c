#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <cmath>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;

void filtroSobel(const Mat& imagem, Mat& imagemFiltrada) {
    int altura = imagem.rows;
    int largura = imagem.cols;
    int canais = imagem.channels();

    imagemFiltrada = Mat::zeros(imagem.size(), CV_32F);

    float Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    float Gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    for (int canal = 0; canal < canais; ++canal) {
        for (int i = 0; i < altura - 2; ++i) {
            for (int j = 0; j < largura - 2; ++j) {
                float gx = 0.0, gy = 0.0;
                for (int x = 0; x < 3; ++x) {
                    for (int y = 0; y < 3; ++y) {
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
                    imagemFiltrada.at<float>(i + 1, j + 1) = gradiente;
                } else {
                    imagemFiltrada.at<Vec3f>(i + 1, j + 1)[canal] = gradiente;
                }
            }
        }
    }

    double minVal, maxVal;
    minMaxLoc(imagemFiltrada, &minVal, &maxVal);
    imagemFiltrada.convertTo(imagemFiltrada, CV_8U, 255.0 / maxVal);
}

vector<double> processarDiretorio(const string& input_dir, const string& output_dir) {
    vector<double> tempos_execucao;

    for (const auto& entry : fs::directory_iterator(input_dir)) {
        if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg" || 
            entry.path().extension() == ".jpeg" || entry.path().extension() == ".bmp") {

            Mat imagem = imread(entry.path().string(), IMREAD_COLOR);
            if (!imagem.empty()) {
                Mat imagemFiltrada;
                auto start_time = chrono::high_resolution_clock::now();
                filtroSobel(imagem, imagemFiltrada);
                auto end_time = chrono::high_resolution_clock::now();
                chrono::duration<double, milli> elapsed_time = end_time - start_time;
                tempos_execucao.push_back(elapsed_time.count());

                string caminho_imagem_filtrada = output_dir + "/" + entry.path().filename().string();
                imwrite(caminho_imagem_filtrada, imagemFiltrada);
                cout << "Tempo de processamento de " << entry.path().filename().string() << ": " << elapsed_time.count() << " ms" << endl;
            } else {
                cout << "Erro ao carregar a imagem: " << entry.path().string() << endl;
            }
        }
    }

    return tempos_execucao;
}

void multiplasExecucoes(const string& input_dir, const string& output_dir, int execucoes = 1) {
    vector<double> tempos_todas_execucoes;
    for (int execucao = 0; execucao < execucoes; ++execucao) {
        cout << "Iniciando execução " << execucao + 1 << endl;
        vector<double> tempos_execucao = processarDiretorio(input_dir, output_dir);
        tempos_todas_execucoes.insert(tempos_todas_execucoes.end(), tempos_execucao.begin(), tempos_execucao.end());
        if (!tempos_execucao.empty()) {
            double media_execucao = accumulate(tempos_execucao.begin(), tempos_execucao.end(), 0.0) / tempos_execucao.size();
            cout << "Média de tempo para a execução " << execucao + 1 << ": " << media_execucao << " ms" << endl;
        } else {
            cout << "Nenhuma imagem processada nesta execução." << endl;
        }
    }

    if (!tempos_todas_execucoes.empty()) {
        double media_geral = accumulate(tempos_todas_execucoes.begin(), tempos_todas_execucoes.end(), 0.0) / tempos_todas_execucoes.size();
        cout << "Média geral das médias de tempo após " << execucoes << " execuções: " << media_geral << " ms" << endl;
    } else {
        cout << "Nenhuma imagem foi processada em nenhuma execução." << endl;
    }
}

int main() {
    string input_dir = "C:/Users/Cliente/Downloads/base_dados/Imagens_Selecionadas";
    string output_dir = "C:/Users/Cliente/Downloads/base_dados/Saida_Python_Sobel";

    // Chama a função que executa o processamento múltiplas vezes
    multiplasExecucoes(input_dir, output_dir);

    return 0;
}

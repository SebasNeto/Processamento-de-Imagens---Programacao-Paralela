#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;

void filtroDaMediana(const Mat& imagem, Mat& imagemFiltrada, int tamJanela = 3) {
    int altura = imagem.rows;
    int largura = imagem.cols;
    int canais = imagem.channels();
    int margem = tamJanela / 2;

    imagemFiltrada = imagem.clone();

    for (int i = margem; i < altura - margem; ++i) {
        for (int j = margem; j < largura - margem; ++j) {
            for (int canal = 0; canal < canais; ++canal) {
                vector<uchar> regiao;
                for (int x = -margem; x <= margem; ++x) {
                    for (int y = -margem; y <= margem; ++y) {
                        regiao.push_back(imagem.at<Vec3b>(i + x, j + y)[canal]);
                    }
                }
                nth_element(regiao.begin(), regiao.begin() + regiao.size() / 2, regiao.end());
                uchar mediana = regiao[regiao.size() / 2];
                imagemFiltrada.at<Vec3b>(i, j)[canal] = mediana;
            }
        }
    }
}

vector<double> processarDiretorio(const string& input_dir, const string& output_dir, int tamJanela = 3) {
    vector<double> tempos_execucao;

    for (const auto& entry : fs::directory_iterator(input_dir)) {
        if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg" || 
            entry.path().extension() == ".jpeg" || entry.path().extension() == ".bmp") {

            Mat imagem = imread(entry.path().string(), IMREAD_COLOR);
            if (!imagem.empty()) {
                Mat imagemFiltrada;
                auto start_time = chrono::high_resolution_clock::now();
                filtroDaMediana(imagem, imagemFiltrada, tamJanela);
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

void multiplasExecucoes(const string& input_dir, const string& output_dir, int tamJanela = 3, int execucoes = 1) {
    vector<double> tempos_todas_execucoes;
    for (int execucao = 0; execucao < execucoes; ++execucao) {
        cout << "Iniciando execução " << execucao + 1 << endl;
        vector<double> tempos_execucao = processarDiretorio(input_dir, output_dir, tamJanela);
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
    string output_dir = "C:/Users/Cliente/Downloads/base_dados/Saida_Python_Filtro_Mediana";

    // Chama a função que executa o processamento múltiplas vezes
    multiplasExecucoes(input_dir, output_dir);

    return 0;
}

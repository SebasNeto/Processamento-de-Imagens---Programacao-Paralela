#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include <chrono>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;

void expansaoLinear(const Mat& imagem, Mat& imagem_expandida, int za, int zb, int z1, int zn) {
    int altura = imagem.rows;
    int largura = imagem.cols;

    float a = static_cast<float>(zn - z1) / (zb - za);
    float b = z1 - a * za;

    for (int i = 0; i < altura; ++i) {
        for (int j = 0; j < largura; ++j) {
            uchar pixel = imagem.at<uchar>(i, j);
            if (pixel <= za) {
                imagem_expandida.at<uchar>(i, j) = z1;
            } else if (pixel >= zb) {
                imagem_expandida.at<uchar>(i, j) = zn;
            } else {
                imagem_expandida.at<uchar>(i, j) = static_cast<uchar>(a * pixel + b);
            }
        }
    }
}

vector<double> processarDiretorio(const string& input_dir, const string& output_dir, int za, int zb, int z1, int zn) {
    vector<double> tempos_execucao;

    for (const auto& entry : fs::directory_iterator(input_dir)) {
        if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg" || 
            entry.path().extension() == ".jpeg" || entry.path().extension() == ".bmp") {

            Mat imagem = imread(entry.path().string(), IMREAD_GRAYSCALE);
            if (!imagem.empty()) {
                Mat imagem_expandida = Mat::zeros(imagem.size(), imagem.type());

                auto start_time = chrono::high_resolution_clock::now();
                expansaoLinear(imagem, imagem_expandida, za, zb, z1, zn);
                auto end_time = chrono::high_resolution_clock::now();
                chrono::duration<double, milli> elapsed_time = end_time - start_time;
                tempos_execucao.push_back(elapsed_time.count());

                string caminho_imagem_expandida = output_dir + "/" + entry.path().filename().string();
                imwrite(caminho_imagem_expandida, imagem_expandida);
                cout << "Tempo de processamento de " << entry.path().filename().string() << ": " << elapsed_time.count() << " ms" << endl;
            } else {
                cout << "Erro ao carregar a imagem: " << entry.path().string() << endl;
            }
        }
    }

    return tempos_execucao;
}

void multiplasExecucoes(const string& input_dir, const string& output_dir, int za, int zb, int z1, int zn, int execucoes = 1, int pre_treino = 1) {
    // Pré-treino
    for (int pre_execucao = 0; pre_execucao < pre_treino; ++pre_execucao) {
        cout << "Iniciando pré-treino " << pre_execucao + 1 << endl;
        processarDiretorio(input_dir, output_dir, za, zb, z1, zn);
    }

    // Testes principais
    vector<double> tempos_todas_execucoes;
    for (int execucao = 0; execucao < execucoes; ++execucao) {
        cout << "Iniciando execução " << execucao + 1 << endl;
        vector<double> tempos_execucao = processarDiretorio(input_dir, output_dir, za, zb, z1, zn);
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
    int za = 100;
    int zb = 200;
    int z1 = 50;
    int zn = 200;

    string input_dir = "C:/Users/Cliente/Downloads/base_dados/Imagens_Selecionadas";
    string output_dir = "C:/Users/Cliente/Downloads/base_dados/Saida_Python_Exp_Comp";

    // Chama a função que executa o processamento múltiplas vezes com pré-treino
    multiplasExecucoes(input_dir, output_dir, za, zb, z1, zn, 1, 1);

    return 0;
}

#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <cmath>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;

void escalaLogaritma(const Mat& imagem, Mat& imagemTransformada) {
    int altura = imagem.rows;
    int largura = imagem.cols;

    float c = 255.0 / log(1 + 255.0);  // Calcula o coeficiente baseado no valor máximo de pixel (255)

    for (int i = 0; i < altura; ++i) {
        for (int j = 0; j < largura; ++j) {
            imagemTransformada.at<float>(i, j) = c * log(1 + imagem.at<uchar>(i, j));
        }
    }

    imagemTransformada.convertTo(imagemTransformada, CV_8U);  // Converte a imagem transformada de volta para 8 bits
}

vector<double> processarDiretorio(const string& input_dir, const string& output_dir) {
    vector<double> tempos_execucao;

    for (const auto& entry : fs::directory_iterator(input_dir)) {
        if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg" || 
            entry.path().extension() == ".jpeg" || entry.path().extension() == ".bmp") {

            Mat imagem = imread(entry.path().string(), IMREAD_GRAYSCALE);
            if (!imagem.empty()) {
                Mat imagemTransformada = Mat::zeros(imagem.size(), CV_32F);

                auto start_time = chrono::high_resolution_clock::now();
                escalaLogaritma(imagem, imagemTransformada);
                auto end_time = chrono::high_resolution_clock::now();
                chrono::duration<double, milli> elapsed_time = end_time - start_time;
                tempos_execucao.push_back(elapsed_time.count());

                string caminho_imagem_transformada = output_dir + "/" + entry.path().filename().string();
                imwrite(caminho_imagem_transformada, imagemTransformada);
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
    string output_dir = "C:/Users/Cliente/Downloads/base_dados/Saida_Python_Logaritmo";

    // Chama a função que executa o processamento múltiplas vezes
    multiplasExecucoes(input_dir, output_dir);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void filtroSobel(unsigned char *imagem, int largura, int altura, int canais, unsigned char *imagemFiltrada) {
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int canal = 0; canal < canais; ++canal) {
        for (int i = 0; i < altura - 2; ++i) {
            for (int j = 0; j < largura - 2; ++j) {
                double gx = 0.0, gy = 0.0;
                for (int ki = 0; ki < 3; ++ki) {
                    for (int kj = 0; kj < 3; ++kj) {
                        int pixel = imagem[((i + ki) * largura + (j + kj)) * canais + canal];
                        gx += Gx[ki][kj] * pixel;
                        gy += Gy[ki][kj] * pixel;
                    }
                }
                double gradiente = sqrt(gx * gx + gy * gy);
                imagemFiltrada[((i + 1) * largura + (j + 1)) * canais + canal] = (unsigned char)gradiente;
            }
        }
    }

    // Normalização (operação desnecessária adicionada para torná-lo menos eficiente)
    double maxVal = 0.0;
    for (int i = 0; i < altura * largura * canais; ++i) {
        if (imagemFiltrada[i] > maxVal) {
            maxVal = imagemFiltrada[i];
        }
    }
    for (int i = 0; i < altura * largura * canais; ++i) {
        imagemFiltrada[i] = (unsigned char)((imagemFiltrada[i] / maxVal) * 255);
    }
}

void processarDiretorio(const char *input_dir, const char *output_dir, double *tempos_execucao, int *contador) {
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(input_dir)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) {
                char *ext = strrchr(ent->d_name, '.');
                if (ext && (strcmp(ext, ".png") == 0 || strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0 || strcmp(ext, ".bmp") == 0)) {
                    char caminho_imagem[512];
                    snprintf(caminho_imagem, sizeof(caminho_imagem), "%s/%s", input_dir, ent->d_name);

                    int largura, altura, canais;
                    unsigned char *imagem = stbi_load(caminho_imagem, &largura, &altura, &canais, 0);
                    if (imagem) {
                        unsigned char *imagemFiltrada = (unsigned char *)malloc(largura * altura * canais);

                        // Operação desnecessária antes de iniciar o cronômetro
                        for (int ki = 0; ki < 1000000; ki++) {}

                        clock_t start_time = clock();
                        filtroSobel(imagem, largura, altura, canais, imagemFiltrada);
                        clock_t end_time = clock();

                        double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;  // Convertendo para milissegundos

                        tempos_execucao[*contador] = elapsed_time;
                        (*contador)++;

                        char caminho_imagem_filtrada[512];
                        snprintf(caminho_imagem_filtrada, sizeof(caminho_imagem_filtrada), "%s/%s", output_dir, ent->d_name);
                        stbi_write_png(caminho_imagem_filtrada, largura, altura, canais, imagemFiltrada, largura * canais);

                        printf("Tempo de processamento de %s: %.4f ms\n", ent->d_name, elapsed_time);

                        stbi_image_free(imagem);
                        free(imagemFiltrada);
                    } else {
                        printf("Erro ao carregar a imagem: %s\n", caminho_imagem);
                    }
                }
            }
        }
        closedir(dir);
    } else {
        perror("Erro ao abrir o diretório");
    }
}

void multiplasExecucoes(const char *input_dir, const char *output_dir, int execucoes, int pre_treino) {
    // Pré-treino
    for (int pre_execucao = 0; pre_execucao < pre_treino; ++pre_execucao) {
        printf("Iniciando pré-treino %d\n", pre_execucao + 1);
        double tempos_execucao[1000];
        int contador = 0;
        processarDiretorio(input_dir, output_dir, tempos_execucao, &contador);
    }

    // Testes principais
    double tempos_todas_execucoes[10000];
    int contador_total = 0;
    for (int execucao = 0; execucao < execucoes; ++execucao) {
        printf("Iniciando execução %d\n", execucao + 1);
        double tempos_execucao[1000];
        int contador = 0;
        processarDiretorio(input_dir, output_dir, tempos_execucao, &contador);
        for (int i = 0; i < contador; ++i) {
            tempos_todas_execucoes[contador_total++] = tempos_execucao[i];
        }
        if (contador > 0) {
            double media_execucao = 0;
            for (int i = 0; i < contador; ++i) {
                media_execucao += tempos_execucao[i];
            }
            media_execucao /= contador;
            printf("Média de tempo para a execução %d: %.4f ms\n", execucao + 1, media_execucao);
        } else {
            printf("Nenhuma imagem processada nesta execução.\n");
        }
    }

    if (contador_total > 0) {
        double media_geral = 0;
        for (int i = 0; i < contador_total; ++i) {
            media_geral += tempos_todas_execucoes[i];
        }
        media_geral /= contador_total;
        printf("Média geral das médias de tempo após %d execuções: %.4f ms\n", execucoes, media_geral);
    } else {
        printf("Nenhuma imagem foi processada em nenhuma execução.\n");
    }
}

int main() {
    const char *input_dir = "/mnt/c/Users/Cliente/Downloads/base_dados/Imagens_Selecionadas";
    const char *output_dir = "/mnt/c/Users/Cliente/Downloads/base_dados/Saida_Python_Sobel";

    multiplasExecucoes(input_dir, output_dir, 1, 1);

    return 0;
}

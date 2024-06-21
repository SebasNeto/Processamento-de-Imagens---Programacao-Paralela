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

void filtroKVizinhosProximos(unsigned char *imagem, int largura, int altura, int canais, int tamJanela, int k, unsigned char *imagemFiltrada) {
    int margem = tamJanela / 2;

    for (int i = 0; i < altura; ++i) {
        for (int j = 0; j < largura; ++j) {
            for (int canal = 0; canal < canais; ++canal) {
                if (i >= margem && i < altura - margem && j >= margem && j < largura - margem) {
                    int pixelIndex = (i * largura + j) * canais + canal;
                    unsigned char pixelCentral = imagem[pixelIndex];
                    int distancias[tamJanela * tamJanela];
                    int valoresVizinhos[tamJanela * tamJanela];
                    int count = 0;

                    for (int ki = -margem; ki <= margem; ++ki) {
                        for (int kj = -margem; kj <= margem; ++kj) {
                            int vizinhoIndex = ((i + ki) * largura + (j + kj)) * canais + canal;
                            unsigned char vizinho = imagem[vizinhoIndex];
                            distancias[count] = abs(vizinho - pixelCentral);
                            valoresVizinhos[count] = vizinho;
                            count++;
                        }
                    }

                    // Ordena as distâncias (bubble sort para simplificar)
                    for (int m = 0; m < tamJanela * tamJanela - 1; ++m) {
                        for (int n = 0; n < tamJanela * tamJanela - m - 1; ++n) {
                            if (distancias[n] > distancias[n + 1]) {
                                int tempDist = distancias[n];
                                distancias[n] = distancias[n + 1];
                                distancias[n + 1] = tempDist;

                                int tempValor = valoresVizinhos[n];
                                valoresVizinhos[n] = valoresVizinhos[n + 1];
                                valoresVizinhos[n + 1] = tempValor;
                            }
                        }
                    }

                    // Calcula a média dos k vizinhos mais próximos
                    int soma = 0;
                    for (int m = 0; m < k; ++m) {
                        soma += valoresVizinhos[m];
                    }
                    imagemFiltrada[pixelIndex] = soma / k;
                } else {
                    // Mantém o pixel original nas bordas
                    imagemFiltrada[(i * largura + j) * canais + canal] = imagem[(i * largura + j) * canais + canal];
                }
            }
        }
    }
}

void processarDiretorio(const char *input_dir, const char *output_dir, int tamJanela, int k, double *tempos_execucao, int *contador) {
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
                        filtroKVizinhosProximos(imagem, largura, altura, canais, tamJanela, k, imagemFiltrada);
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

void multiplasExecucoes(const char *input_dir, const char *output_dir, int tamJanela, int k, int execucoes, int pre_treino) {
    // Pré-treino
    for (int pre_execucao = 0; pre_execucao < pre_treino; ++pre_execucao) {
        printf("Iniciando pré-treino %d\n", pre_execucao + 1);
        double tempos_execucao[1000];
        int contador = 0;
        processarDiretorio(input_dir, output_dir, tamJanela, k, tempos_execucao, &contador);
    }

    // Testes principais
    double tempos_todas_execucoes[10000];
    int contador_total = 0;
    for (int execucao = 0; execucao < execucoes; ++execucao) {
        printf("Iniciando execução %d\n", execucao + 1);
        double tempos_execucao[1000];
        int contador = 0;
        processarDiretorio(input_dir, output_dir, tamJanela, k, tempos_execucao, &contador);
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
    int tamJanela = 3;
    int k = 3;

    const char *input_dir = "/mnt/c/Users/Cliente/Downloads/base_dados/Imagens_Selecionadas";
    const char *output_dir = "/mnt/c/Users/Cliente/Downloads/base_dados/Saida_Python_Filtro_KNN";

    multiplasExecucoes(input_dir, output_dir, tamJanela, k, 1, 1);

    return 0;
}

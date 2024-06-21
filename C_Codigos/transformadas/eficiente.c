#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Função de expansão linear otimizada
void expansaoLinear(unsigned char *imagem, int largura, int altura, int za, int zb, int z1, int zn, unsigned char *imagem_expandida) {
    double a = (double)(zn - z1) / (zb - za);
    double b = z1 - a * za;

    for (int i = 0; i < altura; ++i) {
        for (int j = 0; j < largura; ++j) {
            int pixel = imagem[i * largura + j];
            if (pixel <= za) {
                imagem_expandida[i * largura + j] = z1;
            } else if (pixel >= zb) {
                imagem_expandida[i * largura + j] = zn;
            } else {
                imagem_expandida[i * largura + j] = (unsigned char)(a * pixel + b);
            }
        }
    }
}

// Processar diretório de forma otimizada
void processarDiretorio(const char *input_dir, const char *output_dir, int za, int zb, int z1, int zn, double *tempos_execucao, int *contador) {
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
                    unsigned char *imagem = stbi_load(caminho_imagem, &largura, &altura, &canais, 1);
                    if (imagem) {
                        unsigned char *imagem_expandida = (unsigned char *)malloc(largura * altura);

                        clock_t start_time = clock();
                        expansaoLinear(imagem, largura, altura, za, zb, z1, zn, imagem_expandida);
                        clock_t end_time = clock();

                        double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;  // Convertendo para milissegundos

                        tempos_execucao[*contador] = elapsed_time;
                        (*contador)++;

                        char caminho_imagem_expandida[512];
                        snprintf(caminho_imagem_expandida, sizeof(caminho_imagem_expandida), "%s/%s", output_dir, ent->d_name);
                        stbi_write_png(caminho_imagem_expandida, largura, altura, 1, imagem_expandida, largura);

                        printf("Tempo de processamento de %s: %.4f ms\n", ent->d_name, elapsed_time);

                        stbi_image_free(imagem);
                        free(imagem_expandida);
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

// Função principal que realiza múltiplas execuções de forma otimizada
void multiplasExecucoes(const char *input_dir, const char *output_dir, int za, int zb, int z1, int zn, int execucoes, int pre_treino) {
    // Pré-treino
    for (int pre_execucao = 0; pre_execucao < pre_treino; ++pre_execucao) {
        printf("Iniciando pré-treino %d\n", pre_execucao + 1);
        double tempos_execucao[1000];
        int contador = 0;
        processarDiretorio(input_dir, output_dir, za, zb, z1, zn, tempos_execucao, &contador);
    }

    // Testes principais
    double tempos_todas_execucoes[10000];
    int contador_total = 0;
    for (int execucao = 0; execucao < execucoes; ++execucao) {
        printf("Iniciando execução %d\n", execucao + 1);
        double tempos_execucao[1000];
        int contador = 0;
        processarDiretorio(input_dir, output_dir, za, zb, z1, zn, tempos_execucao, &contador);
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
    int za = 100;
    int zb = 200;
    int z1 = 50;
    int zn = 200;

    const char *input_dir = "/mnt/c/Users/Cliente/Downloads/base_dados/Imagens_Selecionadas";
    const char *output_dir = "/mnt/c/Users/Cliente/Downloads/base_dados/Saida_Python_Exp_Comp";

    multiplasExecucoes(input_dir, output_dir, za, zb, z1, zn, 1, 1);

    return 0;
}

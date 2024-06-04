import cv2
import numpy as np
import os
import time

def filtroMedia(imagem, tam_janela=3, T=60):
    altura, largura, _ = imagem.shape
    imagemFiltrada = np.copy(imagem)
    margem = tam_janela // 2
    for i in range(margem, altura - margem):
        for j in range(margem, largura - margem):
            regiao = imagem[i - margem:i + margem + 1, j - margem:j + margem + 1]
            
            mediaB = np.mean(regiao[:, :, 0])
            mediaG = np.mean(regiao[:, :, 1])
            mediaR = np.mean(regiao[:, :, 2])

            diferencaAbsolutaB = abs(mediaB - imagem[i, j, 0])
            diferencaAbsolutaG = abs(mediaG - imagem[i, j, 1])
            diferencaAbsolutaR = abs(mediaR - imagem[i, j, 2])

            if diferencaAbsolutaB > T:
                imagemFiltrada[i, j, 0] = mediaB
            if diferencaAbsolutaG > T:
                imagemFiltrada[i, j, 1] = mediaG
            if diferencaAbsolutaR > T:
                imagemFiltrada[i, j, 2] = mediaR

    return imagemFiltrada

def processarDiretorio(input_dir, output_dir, tam_janela=3, T=60):
    tempos_execucao = []
    for filename in os.listdir(input_dir):
        if filename.lower().endswith(('.png', '.jpg', '.jpeg', '.bmp')):
            caminho_imagem = os.path.join(input_dir, filename)
            imagem = cv2.imread(caminho_imagem, cv2.IMREAD_COLOR)
            if imagem is not None:
                start_time = time.time()
                imagemFiltrada = filtroMedia(imagem, tam_janela, T)
                elapsed_time = time.time() - start_time
                tempos_execucao.append(elapsed_time * 1000)  # Convertendo para milissegundos

                caminho_imagem_filtrada = os.path.join(output_dir, filename)
                cv2.imwrite(caminho_imagem_filtrada, imagemFiltrada)
                print(f"Tempo de processamento de {filename}: {elapsed_time * 1000:.4f} ms")
            else:
                print(f"Erro ao carregar a imagem: {caminho_imagem}")
    return tempos_execucao

def multiplasExecucoes(input_dir, output_dir, tam_janela=3, T=60, execucoes=1):
    tempos_todas_execucoes = []
    for execucao in range(execucoes):
        print(f"Iniciando execução {execucao + 1}")
        tempos_execucao = processarDiretorio(input_dir, output_dir, tam_janela, T)
        tempos_todas_execucoes.extend(tempos_execucao)
        if tempos_execucao:
            media_execucao = sum(tempos_execucao) / len(tempos_execucao)
            print(f"Média de tempo para a execução {execucao + 1}: {media_execucao:.4f} ms")
        else:
            print("Nenhuma imagem processada nesta execução.")

    if tempos_todas_execucoes:
        media_geral = sum(tempos_todas_execucoes) / len(tempos_todas_execucoes)
        print(f"Média geral das médias de tempo após {execucoes} execuções: {media_geral:.4f} ms")
    else:
        print("Nenhuma imagem foi processada em nenhuma execução.")

input_dir = '/mnt/c/Users/Cliente/Downloads/base_dados/Imagens_Selecionadas'
output_dir = '/mnt/c/Users/Cliente/Downloads/base_dados/Saida_Python_Filtro_Media'

# Chama a função que executa o processamento múltiplas vezes
multiplasExecucoes(input_dir, output_dir, tam_janela=3, T=60, execucoes=1)

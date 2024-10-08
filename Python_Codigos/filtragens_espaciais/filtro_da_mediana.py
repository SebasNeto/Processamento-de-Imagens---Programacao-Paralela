import cv2
import numpy as np
import os
import time

def filtro_da_mediana(imagem, tamJanela=3):
    if len(imagem.shape) == 2:
        altura, largura = imagem.shape
        canais = 1
        imagem = imagem[:, :, np.newaxis]
    else:
        altura, largura, canais = imagem.shape

    imagemFiltrada = np.copy(imagem)
    margem = tamJanela // 2

    for i in range(margem, altura - margem):
        for j in range(margem, largura - margem):
            for canal in range(canais):
                regiao = imagem[i - margem:i + margem + 1, j - margem:j + margem + 1, canal]
                mediana = np.median(regiao)
                imagemFiltrada[i, j, canal] = mediana

    if canais == 1:
        imagemFiltrada = imagemFiltrada[:, :, 0]

    return imagemFiltrada

def processarDiretorio(input_dir, output_dir, tamJanela=3):
    tempos_execucao = []
    for filename in os.listdir(input_dir):
        if filename.lower().endswith(('.png', '.jpg', '.jpeg', '.bmp')):
            caminho_imagem = os.path.join(input_dir, filename)
            imagem = cv2.imread(caminho_imagem, cv2.IMREAD_COLOR)
            if imagem is not None:
                start_time = time.time()
                imagemFiltrada = filtro_da_mediana(imagem, tamJanela)
                elapsed_time = time.time() - start_time
                tempos_execucao.append(elapsed_time * 1000)

                caminho_imagem_filtrada = os.path.join(output_dir, filename)
                cv2.imwrite(caminho_imagem_filtrada, imagemFiltrada)
                print(f"Tempo de processamento de {filename}: {elapsed_time * 1000:.4f} ms")
            else:
                print(f"Erro ao carregar a imagem: {caminho_imagem}")
    return tempos_execucao

def multiplasExecucoes(input_dir, output_dir, tamJanela=3, execucoes=1):
    tempos_todas_execucoes = []
    for execucao in range(execucoes):
        print(f"Iniciando execução {execucao + 1}")
        tempos_execucao = processarDiretorio(input_dir, output_dir, tamJanela)
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
output_dir = '/mnt/c/Users/Cliente/Downloads/base_dados/teste'

# Chama a função que executa o processamento múltiplas vezes
multiplasExecucoes(input_dir, output_dir, tamJanela=3, execucoes=1)

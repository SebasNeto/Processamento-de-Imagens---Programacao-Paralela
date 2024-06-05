import cv2
import numpy as np
import os
import time

def filtro_sobel(imagem):
    if len(imagem.shape) == 2:
        altura, largura = imagem.shape
        canais = 1
    else:
        altura, largura, canais = imagem.shape

    imagemFiltrada = np.zeros_like(imagem, dtype=np.float32)
    
    Gx = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])
    Gy = np.array([[-1, -2, -1], [0, 0, 0], [1, 2, 1]])

    for canal in range(canais):
        for i in range(altura - 2):
            for j in range(largura - 2):
                if canais == 1:
                    regiao = imagem[i:i + 3, j:j + 3]
                else:
                    regiao = imagem[i:i + 3, j:j + 3, canal]

                gx = np.sum(Gx * regiao)
                gy = np.sum(Gy * regiao)

                gradiente = np.sqrt(gx**2 + gy**2)

                if canais == 1:
                    imagemFiltrada[i+1, j+1] = gradiente
                else:
                    imagemFiltrada[i+1, j+1, canal] = gradiente

    imagemFiltrada = (imagemFiltrada / imagemFiltrada.max()) * 255
    return imagemFiltrada.astype(np.uint8)

def processarDiretorio(input_dir, output_dir):
    tempos_execucao = []
    for filename in os.listdir(input_dir):
        if filename.lower().endswith(('.png', '.jpg', '.jpeg', '.bmp')):
            caminho_imagem = os.path.join(input_dir, filename)
            imagem = cv2.imread(caminho_imagem, cv2.IMREAD_COLOR)
            if imagem is not None:
                start_time = time.time()
                imagemFiltrada = filtro_sobel(imagem)
                elapsed_time = time.time() - start_time
                tempos_execucao.append(elapsed_time * 1000)

                caminho_imagem_filtrada = os.path.join(output_dir, filename)
                cv2.imwrite(caminho_imagem_filtrada, imagemFiltrada)
                print(f"Tempo de processamento de {filename}: {elapsed_time * 1000:.4f} ms")
            else:
                print(f"Erro ao carregar a imagem: {caminho_imagem}")
    return tempos_execucao

def multiplasExecucoes(input_dir, output_dir, execucoes=1):
    tempos_todas_execucoes = []
    for execucao in range(execucoes):
        print(f"Iniciando execução {execucao + 1}")
        tempos_execucao = processarDiretorio(input_dir, output_dir)
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
output_dir = '/mnt/c/Users/Cliente/Downloads/base_dados/Saida_Python_Sobel'

# Chama a função que executa o processamento múltiplas vezes
multiplasExecucoes(input_dir, output_dir, execucoes=1)

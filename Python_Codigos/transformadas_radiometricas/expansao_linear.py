import cv2
import numpy as np
import os
import time

def expansaoLinear(imagem, za, zb, z1, zn):
    altura, largura = imagem.shape
    imagem_expandida = np.zeros_like(imagem)

    a = (zn - z1) / (zb - za)
    b = z1 - a * za

    for i in range(altura):
        for j in range(largura):
            pixel = imagem[i, j]
            if pixel <= za:
                imagem_expandida[i, j] = z1
            elif pixel >= zb:
                imagem_expandida[i, j] = zn
            else:
                imagem_expandida[i, j] = int(a * pixel + b)
    return imagem_expandida

def processarDiretorio(input_dir, output_dir, za, zb, z1, zn):
    tempos_execucao = []  
    for filename in os.listdir(input_dir):
        if filename.lower().endswith(('.png', '.jpg', '.jpeg', '.bmp')):
            caminho_imagem = os.path.join(input_dir, filename)
            imagem = cv2.imread(caminho_imagem, cv2.IMREAD_GRAYSCALE)
            if imagem is not None:
                start_time = time.time()
                imagem_expandida = expansaoLinear(imagem, za, zb, z1, zn)
                elapsed_time = time.time() - start_time
                tempos_execucao.append(elapsed_time * 1000)  # Convertendo para milissegundos

                caminho_imagem_expandida = os.path.join(output_dir, filename)
                cv2.imwrite(caminho_imagem_expandida, imagem_expandida)
                print(f"Tempo de processamento de {filename}: {elapsed_time * 1000:.4f} ms")
            else:
                print(f"Erro ao carregar a imagem: {caminho_imagem}")
    return tempos_execucao

def multiplasExecucoes(input_dir, output_dir, za, zb, z1, zn, execucoes=1):

    # Testes principais
    tempos_todas_execucoes = []
    for execucao in range(execucoes):
        print(f"Iniciando execução {execucao + 1}")
        tempos_execucao = processarDiretorio(input_dir, output_dir, za, zb, z1, zn)
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

za = 100
zb = 200
z1 = 30
zn = 150

input_dir = '/mnt/c/Users/Cliente/Downloads/base_dados/Imagens_Selecionadas'
output_dir = '/mnt/c/Users/Cliente/Downloads/base_dados/Saida_Python_Exp_Comp'

# Chama a função que executa o processamento múltiplas vezes com pré-treino
multiplasExecucoes(input_dir, output_dir, za, zb, z1, zn, execucoes=1)
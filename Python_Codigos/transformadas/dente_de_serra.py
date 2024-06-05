import cv2
import numpy as np
import os
import time

def transformada_dente_de_serra(imagem):
    altura, largura = imagem.shape
    imagem_transformada = np.zeros_like(imagem, dtype=np.float32)
    
    for i in range(altura):
        for j in range(largura):
            pixel = imagem[i][j]
            
            if pixel < 63:
                imagem_transformada[i][j] = int(255 * pixel / 62)
            
            elif 63 <= pixel < 127:
                imagem_transformada[i][j] = int(255 * (pixel - 63) / 63)
            
            elif 127 <= pixel < 191:
                imagem_transformada[i][j] = int(255 * (pixel - 127) / 63)
            
            else:
                imagem_transformada[i][j] = int(255 * (pixel - 191) / 64)

    return np.clip(imagem_transformada, 0, 255).astype(np.uint8)

def processarDiretorio(input_dir, output_dir):
    tempos_execucao = []  
    for filename in os.listdir(input_dir):
        if filename.lower().endswith(('.png', '.jpg', '.jpeg', '.bmp')):
            caminho_imagem = os.path.join(input_dir, filename)
            imagem = cv2.imread(caminho_imagem, cv2.IMREAD_GRAYSCALE)
            if imagem is not None:
                start_time = time.time()
                imagem_transformada = transformada_dente_de_serra(imagem)
                elapsed_time = time.time() - start_time
                tempos_execucao.append(elapsed_time * 1000)  # Convertendo para milissegundos

                caminho_imagem_transformada = os.path.join(output_dir, filename)
                cv2.imwrite(caminho_imagem_transformada, imagem_transformada)
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
output_dir = '/mnt/c/Users/Cliente/Downloads/base_dados/Saida_Python_Dente_de_Serra'

# Chama a função que executa o processamento múltiplas vezes
multiplasExecucoes(input_dir, output_dir)

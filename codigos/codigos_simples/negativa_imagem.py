import cv2
import numpy as np
import time

def negativo(imagem):
    # cria uma cópia da imagem 
    imagem_negativa = imagem.copy()

    # percorre os pixels da imagem
    for i in range(imagem.shape[0]):
        for j in range(imagem.shape[1]):
            # obtem o valor do pixel
            valor_pixel = imagem[i, j]

            # calcula o valor negativo do pixel
            valor_negativo = 255 - valor_pixel

            # atribui o novo valor ao pixel
            imagem_negativa[i, j] = valor_negativo

    return imagem_negativa


imagem = cv2.imread('/mnt/c/Users/Cliente/OneDrive/Documentos/PIBIC/processamentoImagens_Codigos/codigos/codigos_simples/Teatro Amazonas.jpg')


if imagem is None:
    print('Erro ao carregar a imagem.')
    exit()

inicio = time.perf_counter()
nova_imagem = negativo(imagem)
fim = time.perf_counter()


nova_imagem = np.array(nova_imagem, dtype=np.uint8)

caminho_salvar_imagem = '/mnt/c/Users/Cliente/OneDrive/Documentos/PIBIC/processamentoImagens_Codigos/codigos/codigos_simples/Imagem_teatro_alterada_Negativa.jpg'
cv2.imwrite(caminho_salvar_imagem, nova_imagem)
duracao = (fim - inicio) * 1_000_000 #microsegundos

print(f"Tempo de execução em Python: {duracao:.2f} microsegundos")
#print(f"Imagem com brilho alterado salva em: {caminho_salvar_imagem}")

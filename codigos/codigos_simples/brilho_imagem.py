import cv2
import numpy as np
import time

def alterar_brilho(imagem, valor):
    
    altura, largura, canais = imagem.shape
    
    #matriz com mesmo tamanho que armazenará a imagem alterada 
    matriz_brilho = [[[0] * canais for _ in range(largura)] for _ in range(altura)]

    # Percorre cada pixel da imagem e altera o brilho
    for i in range(altura):
        for j in range(largura):
            for k in range(canais):
                novo_valor = int(imagem[i][j][k]) + valor

                if novo_valor < 0:
                    novo_valor = 0
                elif novo_valor > 255:
                    novo_valor = 255

                matriz_brilho[i][j][k] = novo_valor

    return matriz_brilho

# Carrega a imagem
imagem = cv2.imread('/mnt/c/Users/Cliente/OneDrive/Documentos/PIBIC/processamentoImagens_Codigos/codigos/codigos_simples/Teatro Amazonas.jpg')


if imagem is None:
    print('Erro ao carregar a imagem.')
    exit()

valor_brilho = int(input('Digite o valor de brilho (negativo para escurecer, positivo para clarear): '))

inicio = time.perf_counter()
nova_imagem = alterar_brilho(imagem, valor_brilho)
fim = time.perf_counter()


nova_imagem = np.array(nova_imagem, dtype=np.uint8)

caminho_salvar_imagem = '/mnt/c/Users/Cliente/OneDrive/Documentos/PIBIC/processamentoImagens_Codigos/codigos/codigos_simples/Imagem_teatro_alterada.jpg'
cv2.imwrite(caminho_salvar_imagem, nova_imagem)
duracao = (fim - inicio) * 1_000_000 #microsegundos

print(f"Tempo de execução em Python: {duracao:.2f} microsegundos")
#print(f"Imagem com brilho alterado salva em: {caminho_salvar_imagem}")



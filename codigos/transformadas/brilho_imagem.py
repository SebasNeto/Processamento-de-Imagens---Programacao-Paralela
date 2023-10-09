import cv2
import numpy as np

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
imagem = cv2.imread('senhorIncrivel.jpeg')


if imagem is None:
    print('Erro ao carregar a imagem.')
    exit()

valor_brilho = int(input('Digite o valor de brilho (negativo para escurecer, positivo para clarear): '))

nova_imagem = alterar_brilho(imagem, valor_brilho)

#Converte a matriz em um array de dado correto 
nova_imagem = np.array(nova_imagem, dtype=np.uint8)


cv2.imshow('Imagem Original', imagem)
cv2.imshow('Imagem com Brilho Alterado', nova_imagem)
cv2.waitKey(0)
cv2.destroyAllWindows()


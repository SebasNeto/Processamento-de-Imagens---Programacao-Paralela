import cv2
import numpy as np

def transformarNegativa(imagem, valor):
    imagem_negativa = [] #armazena a imagem negativa 
    for linha in imagem:
        linha_negativa = [] 
        for pixel in linha:
            pixel_negativo = valor - pixel
            linha_negativa.append(pixel_negativo)
        imagem_negativa.append(linha_negativa)
    return imagem_negativa


caminho_imagem = 'senhorIncrivel.jpeg'  

imagem_original = cv2.imread(caminho_imagem, cv2.IMREAD_COLOR) #padrão RGB

valorBrilho = int(input("Digite o valor (negativo para escurecer): "))


imagem_negativa = transformarNegativa(imagem_original, valorBrilho)

# Converter a matriz para o tipo de dado correto
imagem_negativa = [[pixel.astype('uint8') for pixel in linha] for linha in imagem_negativa]

# Converter a matriz para um array numpy
imagem_negativa = np.array(imagem_negativa)


cv2.imshow('Imagem Original', imagem_original)
cv2.imshow('Imagem Negativa', imagem_negativa)


cv2.waitKey(0)
cv2.destroyAllWindows()

import cv2
import numpy as np
import math

def escalaLogaritma(imagem, c):
    altura, largura = len(imagem), len(imagem[0])
    
    imagemTransformada = [[0 for _ in range(largura)] for _ in range(altura)]
    
    for i in range(altura):
        for j in range(largura):
            imagemTransformada[i][j] = int (c * math.log(1+imagem[i][j]) )
            
    return imagemTransformada
            
            
pico = int(input("Digite o valor do pico: "))

            
caminho_imagem = 'imagens\\fusca2.png'
imagem = cv2.imread(caminho_imagem, cv2.IMREAD_GRAYSCALE)


imagemTransformada = escalaLogaritma(imagem, pico)


imagemTransformada = np.array(imagemTransformada, dtype=np.uint8)


caminho_imagem_alterada = 'C:\\Users\\Cliente\\OneDrive\\Documentos\\PIBIC\\processamentoImagens_Codigos\\teste\\imagem_expandidaC.jpg'
cv2.imwrite(caminho_imagem_alterada, imagemTransformada)

print("Imagem expandida salva com sucesso")
import cv2
import numpy as np

def compressao_expansao(imagem, z1, z2):
    altura, largura = len(imagem), len(imagem[0])
    
    imagemTransformada = [[0 for _ in range(largura)] for _ in range(altura)]
    
    for i in range(altura):
        for j in range(largura):
            z = imagem[i][j]
            
            if z <= z1:
                imagemTransformada[i][j] = int(z/2)
            elif z1 < z < z2:
                imagemTransformada[i][j] = (2 * z ) - 127
            
            elif z >= z2:
                imagemTransformada[i][j] = int((z/2) + 128)
                
    return imagemTransformada
            
z1 = int(input("Digite o valor de z1 dentro do intervalo: "))
z2 = int(input("Digite o valor de z2 dentro do intervalo: "))

            
caminho_imagem = 'imagens\mamaDensa_mamalipo.jpg'
imagem = cv2.imread(caminho_imagem, cv2.IMREAD_GRAYSCALE)


imagemTransformada = compressao_expansao(imagem, z1, z2)
imagemTransformada = np.array(imagemTransformada, dtype=np.uint8)


caminho_imagem_alterada = 'C:\\Users\\Cliente\\OneDrive\\Documentos\\PIBIC\\processamentoImagens_Codigos\\teste\\imagem_expandidaC.jpg'
cv2.imwrite(caminho_imagem_alterada, imagemTransformada)

print("Imagem expandida salva com sucesso")
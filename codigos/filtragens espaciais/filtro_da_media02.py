import cv2
import numpy as np

def filtro_media(img, tam_janela=3):
    altura, largura, _ = img.shape

    imagemFiltrada = np.zeros_like(img)

   #margem baseada no tamanho da janela
    margem = tam_janela // 2 #evitar acesso a pixels fora da jenela

    for i in range(margem, altura - margem):
        for j in range(margem, largura - margem):
            # extraindo vizinhos
            regiao = img[i - margem:i + margem + 1, j - margem:j + margem + 1]

            media_b = np.mean(regiao[:, :, 0])
            media_g = np.mean(regiao[:, :, 1])
            media_r = np.mean(regiao[:, :, 2])


            imagemFiltrada[i, j] = [media_b, media_g, media_r]

    return imagemFiltrada

# Teste o filtro da média
img = cv2.imread('sua_imagem.jpg')
imagemFiltrada = filtro_media(img)
cv2.imwrite('sua_imagem_filtrada.jpg', imagemFiltrada)

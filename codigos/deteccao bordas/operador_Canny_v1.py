import cv2
import numpy as np

def aplicar_filtro_gaussiano(imagem):
    kernel_gaussiano = np.array([[1, 2, 1], [2, 4, 2], [1, 2, 1]]) / 16
    altura, largura = imagem.shape
    imagem_suavizada = np.zeros_like(imagem, dtype=np.float32)

    for i in range(1, altura - 1):
        for j in range(1, largura - 1):
            regiao = imagem[i - 1:i + 2, j - 1:j + 2]
            imagem_suavizada[i, j] = np.sum(kernel_gaussiano * regiao)

    return imagem_suavizada

def calcular_gradiente_e_direcao(imagem):
    Gx = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])
    Gy = np.array([[1, 2, 1], [0, 0, 0], [-1, -2, -1]])
    
    altura, largura = imagem.shape
    gradiente = np.zeros_like(imagem, dtype=np.float32)
    direcao = np.zeros_like(imagem, dtype=np.float32)

    for i in range(1, altura - 1):
        for j in range(1, largura - 1):
            regiao = imagem[i - 1:i + 2, j - 1:j + 2]

            gx = np.sum(Gx * regiao)
            gy = np.sum(Gy * regiao)

            gradiente[i, j] = np.sqrt(gx**2 + gy**2)
            direcao[i, j] = np.arctan2(gy, gx)

    return gradiente, direcao

def supressao_nao_maximos(gradiente, direcao):
    altura, largura = gradiente.shape
    resultado = np.zeros_like(gradiente, dtype=np.float32)


    for i in range(1, altura - 1):
        for j in range(1, largura - 1):
            angulo = direcao[i, j] * (180 / np.pi)
            angulo = (angulo + 180) % 360

            q = 255
            r = 255

            if (0 <= angulo < 22.5) or (157.5 <= angulo <= 180) or (337.5 <= angulo <= 360):
                q = gradiente[i, j + 1]
                r = gradiente[i, j - 1]
            elif 22.5 <= angulo < 67.5:
                q = gradiente[i + 1, j - 1]
                r = gradiente[i - 1, j + 1]
            elif 67.5 <= angulo < 112.5:
                q = gradiente[i + 1, j]
                r = gradiente[i - 1, j]
            elif 112.5 <= angulo < 157.5:
                q = gradiente[i - 1, j - 1]
                r = gradiente[i + 1, j + 1]
            if gradiente[i, j] >= q and gradiente[i, j] >= r:
                resultado[i, j] = gradiente[i, j]
            else:
                resultado[i, j] = 0

    return resultado

def histerese(img, baixo, alto):
    altura, largura = img.shape
    resultado = np.zeros_like(img, dtype=np.uint8)

    for i in range(altura):
        for j in range(largura):
            if img[i, j] >= alto:
                resultado[i, j] = 255
            elif img[i, j] < baixo:
                resultado[i, j] = 0
            elif (baixo <= img[i, j] < alto):
                if np.any(img[i-1:i+2, j-1:j+2] >= alto):
                    resultado[i, j] = 255
                else:
                    resultado[i, j] = 0

    return resultado

def filtro_canny(imagem, limiar_baixo=50, limiar_alto=150):
    imagem_cinza = cv2.cvtColor(imagem, cv2.COLOR_BGR2GRAY) if len(imagem.shape) == 3 else imagem
    imagem_suavizada = aplicar_filtro_gaussiano(imagem_cinza)
    gradiente, direcao = calcular_gradiente_e_direcao(imagem_suavizada)
    imagem_suprimida = supressao_nao_maximos(gradiente, direcao)
    imagem_histerese = histerese(imagem_suprimida, limiar_baixo, limiar_alto)
    return imagem_histerese

caminho_imagem = '/mnt/c/Users/Cliente/OneDrive/Documentos/PIBIC/processamentoImagens_Codigos/codigos/deteccao bordas/input_teste_imagens/formatos.png'

imagem = cv2.imread(caminho_imagem)
imagemFiltrada = filtro_canny(imagem)

caminho_imagem_alterada = '/mnt/c/Users/Cliente/OneDrive/Documentos/PIBIC/processamentoImagens_Codigos/codigos/deteccao bordas/output_teste_imagens/formatos_Canny_PrimeiroTeste.png'
cv2.imwrite(caminho_imagem_alterada, imagemFiltrada)

print("Imagem alterada salva com sucesso")

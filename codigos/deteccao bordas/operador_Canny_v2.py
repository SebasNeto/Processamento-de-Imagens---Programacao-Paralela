import cv2
import numpy as np

def filtro_canny(imagem, limiar_baixo=50, limiar_alto=150):

    imagem_suavizada = cv2.GaussianBlur(imagem, (5, 5), 0)

    imagem_canny = cv2.Canny(imagem_suavizada, limiar_baixo, limiar_alto)

    return imagem_canny

caminho_imagem = '/mnt/c/Users/Cliente/OneDrive/Documentos/PIBIC/processamentoImagens_Codigos/codigos/deteccao bordas/input_teste_imagens/formatos_colorido.png'

imagem = cv2.imread(caminho_imagem)
imagemFiltrada = filtro_canny(imagem)

caminho_imagem_alterada = '/mnt/c/Users/Cliente/OneDrive/Documentos/PIBIC/processamentoImagens_Codigos/codigos/deteccao bordas/output_teste_imagens/formatos_Colorido_Canny_v2_SegundoTeste.png'
cv2.imwrite(caminho_imagem_alterada, imagemFiltrada)

print("Imagem alterada salva com sucesso")

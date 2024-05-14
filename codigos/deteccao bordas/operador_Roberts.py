import cv2
import numpy as np

def filtro_roberts(imagem):
    if len(imagem.shape) == 2:
        altura, largura = imagem.shape
        canais = 1
    else:
        altura, largura, canais = imagem.shape

    imagemFiltrada = np.zeros_like(imagem, dtype=np.float32)
    
    Gx = np.array([[1, 0], [0, -1]])
    Gy = np.array([[0, 1], [-1, 0]])

    for canal in range(canais):
        for i in range(altura - 1):
            for j in range(largura - 1):
                if canais == 1:
                    regiao = imagem[i:i + 2, j:j + 2]
                else:
                    regiao = imagem[i:i + 2, j:j + 2, canal]

                gx = np.sum(Gx * regiao)
                gy = np.sum(Gy * regiao)

                gradiente = np.sqrt(gx**2 + gy**2)

                if canais == 1:
                    imagemFiltrada[i, j] = gradiente
                else:
                    imagemFiltrada[i, j, canal] = gradiente

    imagemFiltrada = (imagemFiltrada / imagemFiltrada.max()) * 255
    return imagemFiltrada.astype(np.uint8)

caminho_imagem = '/mnt/c/Users/Cliente/OneDrive/Documentos/PIBIC/processamentoImagens_Codigos/codigos/deteccao bordas/input_teste_imagens/formatos_colorido.png'

imagem = cv2.imread(caminho_imagem)
imagemFiltrada = filtro_roberts(imagem)


caminho_imagem_alterada = '/mnt/c/Users/Cliente/OneDrive/Documentos/PIBIC/processamentoImagens_Codigos/codigos/deteccao bordas/output_teste_imagens/formatos_SegundoTeste.png'
cv2.imwrite(caminho_imagem_alterada, imagemFiltrada)

print("Imagem alterada salva com sucesso")
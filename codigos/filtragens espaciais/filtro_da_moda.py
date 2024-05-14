import cv2
import numpy as np

def filtroDaModa(imagem, tam_janela=3):
    # Verificar o número de canais da imagem
    if len(imagem.shape) == 3:
        altura, largura, canais = imagem.shape
    else:
        altura, largura = imagem.shape
        canais = 1
        imagem = imagem[:, :, np.newaxis]  # Adicionar uma dimensão extra para manter a consistência

    imagemFiltrada = np.copy(imagem)

    margem = tam_janela // 2

    for i in range(margem, altura - margem):
        for j in range(margem, largura - margem):
            for canal in range(canais):
                regiao = imagem[i - margem:i + margem + 1, j - margem:j + margem + 1, canal]
                lista_pixels_vizinhos = regiao.flatten()
                moda = max(set(lista_pixels_vizinhos), key=list(lista_pixels_vizinhos).count)
                imagemFiltrada[i, j, canal] = moda

    if canais == 1:
        imagemFiltrada = imagemFiltrada[:, :, 0]  # Remover a dimensão extra

    return imagemFiltrada



caminho_imagem = 'codigos\\input_testes_imagens\\input imagens filtragens espaciais\\teste.png'
imagem = cv2.imread(caminho_imagem, cv2.IMREAD_COLOR)

imagemFiltrada = filtroDaModa(imagem)

imagemFiltrada = np.array(imagemFiltrada, dtype=np.uint8)

caminho_imagem_alterada = 'C:\\Users\\Cliente\\OneDrive\\Documentos\\PIBIC\\processamentoImagens_Codigos\\codigos\\output_testes_imagens\\output filtragens espaciais\\teste02_filtro_da_moda.png'
cv2.imwrite(caminho_imagem_alterada, imagemFiltrada)

print("Imagem alterada salva com sucesso")

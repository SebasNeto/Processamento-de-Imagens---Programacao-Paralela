import cv2
import numpy as np

def filtro_da_media(imagem, tam_janela=3, T=60):
    
    altura, largura, _ = imagem.shape

    imagemFiltrada = np.copy(imagem)

    margem = tam_janela // 2

    for i in range(margem, altura - margem):
        for j in range(margem, largura - margem):
            
            #extraindo regioes/vizinhos
            regiao = imagem[i - margem:i + margem + 1, j - margem:j + margem + 1]
            
            mediaB = np.mean(regiao[:, :, 0])
            mediaG = np.mean(regiao[:, :, 1])
            mediaR = np.mean(regiao[:, :, 2])

            
            diferencaAbsolutaB = abs(mediaB - imagem[i, j, 0])
            diferencaAbsolutaG = abs(mediaG - imagem[i, j, 1])
            diferencaAbsolutaR = abs(mediaR - imagem[i, j, 2])

            if diferencaAbsolutaB > T:
                imagemFiltrada[i, j, 0] = mediaB
            if diferencaAbsolutaG > T:
                imagemFiltrada[i, j, 1] = mediaG
            if diferencaAbsolutaR > T:
                imagemFiltrada[i, j, 2] = mediaR

    return imagemFiltrada



caminho_imagem = 'codigos\\input_testes_imagens\\input imagens filtragens espaciais\\lenna02.png'
imagem = cv2.imread(caminho_imagem, cv2.IMREAD_COLOR)

imagemFiltrada = filtro_da_media(imagem, T=60)

imagemFiltrada = np.array(imagemFiltrada, dtype=np.uint8)

caminho_imagem_alterada = 'C:\\Users\\Cliente\\OneDrive\\Documentos\\PIBIC\\processamentoImagens_Codigos\\codigos\\output_testes_imagens\\output filtragens espaciais\\lenna02.png'
cv2.imwrite(caminho_imagem_alterada, imagemFiltrada)

print("Imagem alterada salva com sucesso")

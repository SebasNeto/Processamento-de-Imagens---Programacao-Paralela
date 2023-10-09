import cv2
import numpy as np

def filtro_da_media_V2(imagem, tamJanela=3, T=60):
    
    altura, largura, _ = imagem.shape

    imagemFiltrada = np.copy(imagem)

    margem = tamJanela // 2

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

def filtro_da_media(imagem, tamJanela=3, T=60):
    
    if len(imagem.shape) == 2:
        altura, largura = imagem.shape
        canais = 1
        imagem = imagem[:, :, np.newaxis] 
    else:
        altura, largura, canais = imagem.shape
    
    imagemFiltrada = np.copy(imagem)
    margem = tamJanela // 2

    for i in range(margem, altura - margem):
        for j in range(margem, largura - margem):
            for canal in range(canais):
                
                regiao = imagem[i - margem:i + margem + 1, j - margem:j + margem + 1, canal]
                
                media = np.mean(regiao) #calculando a media
                
                diferencaAbsoluta = abs(media - imagem[i, j, canal]) # calculo entre a media e valor do pixel para cada canal
                
                if diferencaAbsoluta > T:
                    imagemFiltrada[i, j, canal] = media

    if canais == 1:
        imagemFiltrada = imagemFiltrada[:, :, 0]

    return imagemFiltrada


def filtro_da_mediana(imagem, tamJanela = 3):
    
    #escala cinza
    if len(imagem.shape) == 2:
        altura, largura = imagem.shape
        canais = 1
        imagem = imagem[:, :, np.newaxis] 
    else:
        #RGB
        altura, largura, canais = imagem.shape

    imagemFiltrada = np.copy(imagem)

    margem = tamJanela // 2

    for i in range(margem, altura - margem):
        for j in range(margem, largura - margem):
            for canal in range(canais):   
                
            #extraindo regioes/vizinhos
                regiao = imagem[i - margem:i + margem + 1, j - margem:j + margem + 1, canal]
                mediana = np.median(regiao)
                #mediana = np.median(regiao[:,:,canal])
                imagemFiltrada[i,j,canal] = mediana   
    if canais == 1:
        imagemFiltrada = imagemFiltrada[:, :, 0]  #imagem original ==  cinza original -> 2D

    return imagemFiltrada

def filtro_da_moda(imagem, tamJanela=3):
    
    if len(imagem.shape) == 2:
        altura, largura = imagem.shape
        canais = 1
        imagem = imagem[:, :, np.newaxis] #Transforma a imagem cinza em 3D - estou fazendo isso para não tratar a condição separadamente e deixar o código robusto
    else:
        #RGB
        altura, largura, canais = imagem.shape

    imagemFiltrada = np.copy(imagem)

    margem = tamJanela // 2

    for i in range(margem, altura - margem):
        for j in range(margem, largura - margem):
            for canal in range(canais):
                regiao = imagem[i - margem:i + margem + 1, j - margem:j + margem + 1, canal]
                lista_pixels_vizinhos = regiao.flatten() # se a regiao for 3x3 flatten() converte o array em 9 elementos
                moda = max(set(lista_pixels_vizinhos), key=list(lista_pixels_vizinhos).count)
                imagemFiltrada[i, j, canal] = moda

    if canais == 1:
        imagemFiltrada = imagemFiltrada[:, :, 0]  #retornando a imagem 3D para 2D -> relacionado a essa linha: imagem = imagem[:, :, np.newaxis]

    return imagemFiltrada

import cv2
import numpy as np

def filtro_k_vizinhos_proximos(imagem, tamJanela=3, k=3):
    
    if len(imagem.shape) == 2:
        altura, largura = imagem.shape
        canais = 1
        imagem = imagem[:, :, np.newaxis]  # Transforma em 3D para padronização
    else:
        altura, largura, canais = imagem.shape

    imagemFiltrada = np.copy(imagem)
    margem = tamJanela // 2

    for i in range(margem, altura - margem):
        for j in range(margem, largura - margem):
            for canal in range(canais):
                regiao = imagem[i - margem:i + margem + 1, j - margem:j + margem + 1, canal]
                pixel_central = imagem[i, j, canal]
                
                # Calcula as distâncias entre o pixel central e todos os outros pixels na janela
                distancias = np.abs(regiao - pixel_central)
                
                # Pega os índices dos k vizinhos mais próximos na região
                indices_vizinhos_proximos = np.argsort(distancias, axis=None)[:k] #contem os indices dos k vizinhos em relação ao pixel central
                valores_vizinhos_proximos = regiao.flatten()[indices_vizinhos_proximos]
                
                # Calcula a média dos k vizinhos mais próximos e substitui o pixel central
                imagemFiltrada[i, j, canal] = np.mean(valores_vizinhos_proximos)

    if canais == 1:
        imagemFiltrada = imagemFiltrada[:, :, 0]

    return imagemFiltrada


def main():
    print("Escolha o filtro: ")
    
    print("1. Filtro da Média")
    print("2. Filtro da Mediana")
    print("3. Filtro da Moda")
    print("4. Filtro dos k vizinhos mais próximos")
    
    escolha = input("Digite o número do filtro: ")

    caminho_imagem = 'codigos\\input_testes_imagens\\input imagens filtragens espaciais\\teste.png'
    imagem = cv2.imread(caminho_imagem, cv2.IMREAD_COLOR)

    if escolha == "1":
        imagemFiltrada = filtro_da_media(imagem, T=60)
        caminho_imagem_alterada = 'C:\\Users\\Cliente\\OneDrive\\Documentos\\PIBIC\\processamentoImagens_Codigos\\codigos\\output_testes_imagens\\output filtragens espaciais\\teste_filtrada_media.png'
    
    elif escolha == 2:
        imagemFiltrada = filtro_da_mediana(imagem)
        caminho_imagem_alterada = ''
    
    elif escolha == "3":
        imagemFiltrada = filtro_da_moda(imagem)
        caminho_imagem_alterada = ''
        
    elif escolha == 4:
        imagemFiltrada = filtro_k_vizinhos_proximos(imagem)
        caminho_imagem_alterada = ''
        
    else:
        print("Opção inválida!")
        return

    imagemFiltrada = np.array(imagemFiltrada, dtype=np.uint8)
    
    cv2.imwrite(caminho_imagem_alterada, imagemFiltrada)
    
    #print(f"Imagem alterada salva em {caminho_imagem_alterada}")
    
    print("Imagem salva com sucesso")

# Chamando a função main
if __name__ == "__main__":
    main()

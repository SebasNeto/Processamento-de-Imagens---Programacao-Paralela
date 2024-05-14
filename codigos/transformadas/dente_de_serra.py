import cv2
import numpy as np
import matplotlib.pyplot as plt
import time

def transformada_dente_de_serra(imagem):
    altura, largura = len(imagem), len(imagem[0])
    imagem_transformada = [[0 for _ in range(largura)] for _ in range(altura)]
    
    for i in range(altura):
        for j in range(largura):
            pixel = imagem[i][j]
            
            if pixel < 63:
                imagem_transformada[i][j] = int(255 * pixel / 62)
            
            elif 63 <= pixel < 127:
                imagem_transformada[i][j] = int(255 * (pixel - 63) / 63)
            
            elif 127 <= pixel < 191:
                imagem_transformada[i][j] = int(255 * (pixel - 127) / 63)
            
            else:
                imagem_transformada[i][j] = int(255 * (pixel - 191) / 64)

    return imagem_transformada


caminho_imagem = '/mnt/c/Users/Cliente/OneDrive/Documentos/PIBIC/processamentoImagens_Codigos/codigos/transformadas/input_imagens/fusca2.png'
imagem = cv2.imread(caminho_imagem, cv2.IMREAD_GRAYSCALE)

inicio =  time.time()
imagem_transformada = transformada_dente_de_serra(imagem)
fim =  time.time()

#salvar com OpenCV
#imagem_transformada_array = [[imagem_transformada[i][j] for j in range(len(imagem_transformada[0]))] for i in range(len(imagem_transformada))]
#imagem_transformada_array = (cv2.UMat.get(imagem_transformada_array)).astype('uint8')

imagem_transformada = np.array(imagem_transformada, dtype=np.uint8)

# def mapeamento_dente_de_serra(valor):
#     if valor < 63:
#         return int(255 * valor / 62)
#     elif 63 <= valor < 127:
#         return int(255 * (valor - 63) / 63)
#     elif 127 <= valor < 191:
#         return int(255 * (valor - 127) / 63)
#     else:
#         return int(255 * (valor - 191) / 64)

# # Gerando os valores de entrada e saída para o gráfico
# valores_entrada = list(range(256))
# valores_saida = [mapeamento_dente_de_serra(valor) for valor in valores_entrada]

# # Plotando o gráfico
# plt.figure(figsize=(8, 6))
# plt.plot(valores_entrada, valores_saida, '-r', label='Transformada Dente de Serra')
# plt.title('Gráfico da Transformada Dente de Serra')
# plt.xlabel('Nível de Cinza Original')
# plt.ylabel('Nível de Cinza Transformado')
# plt.grid(True)
# plt.legend()
# plt.show()

# Salva a imagem transformada
caminho_imagem_transformada = '/mnt/c/Users/Cliente/OneDrive/Documentos/PIBIC/processamentoImagens_Codigos/codigos/transformadas/output_imagens/fusca2_Super.png'
cv2.imwrite(caminho_imagem_transformada, imagem_transformada)
tempo_execucao = (fim - inicio) * 1e6

print("Transformação dente de serra aplicada e imagem salva com sucesso!")
print(f"Tempo de execução: {tempo_execucao} microsegundos")


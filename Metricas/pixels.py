from PIL import Image
import os

# Caminho para a pasta onde as imagens estão localizadas
pasta_imagens = "/mnt/c/Users/Cliente/OneDrive/Documentos/PIBIC/PROCESSAMENTO_DE_IMAGENS_PROGAMACAO_PARALELA_PIBIC/base_dados/Imagens_Selecionadas"

# Lista para armazenar informações das imagens
imagens = []

# Percorre todos os arquivos na pasta
for nome_arquivo in os.listdir(pasta_imagens):
    if nome_arquivo.endswith(".jpg") or nome_arquivo.endswith(".png"):
        caminho_arquivo = os.path.join(pasta_imagens, nome_arquivo)
        with Image.open(caminho_arquivo) as img:
            largura, altura = img.size
            pixels = largura * altura
            imagens.append((nome_arquivo, largura, altura, pixels))

# Ordena as imagens pela quantidade de pixels
imagens_ordenadas = sorted(imagens, key=lambda x: x[3])

# Exibe a lista ordenada
print(f'{"Nome":<40} {"Largura":<10} {"Altura":<10} {"Pixels":<10}')
for nome, largura, altura, pixels in imagens_ordenadas:
    print(f'{nome:<40} {largura:<10} {altura:<10} {pixels:<10}')


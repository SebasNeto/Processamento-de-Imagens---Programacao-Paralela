import cv2
import numpy as np


#expansao de constraste linear
def expansao_constraste_linear(img, za, zb, z1, zn):
    altura, largura, bandas = img.shape
    imagem_expandida = np.zeros((altura, largura, bandas), dtype=np.uint8)

    for i in range(altura):
        for j in range(largura):
            if img.item(i,j,0) <= za:
                B = z1
            else:
                if img.item(i,j,0) >= zb:
                    B = zn
                else:
                    B = int((zn - z1) / (zb - za) * (img.item(i,j,0) - za) + z1)
            if img.item(i, j, 1) <= za:
                G = z1
            else:
                if img.item(i,j,1) >= zb:
                    G = zn
                else:
                    G = int((zn - z1) / (zb - za) * (img.item(i,j,1) - za) + z1)
            if img.item(i,j,2) <= za:
                R = z1
            else:
                if img.item(i,j,2) >= zb:
                    R = zn
                else:
                    R = int((zn - z1) / (zb - za) * (img.item(i,j,2) - za) + z1)

            imagem_expandida.itemset((i,j,0),B)
            imagem_expandida.itemset((i,j,1),G)
            imagem_expandida.itemset((i,j,2),R)
    return imagem_expandida


#Compressão e expansão
def compressao_expansao(img, za, zb):
    altura, largura, bandas = img.shape
    imagem_compressao_expansao = np.zeros((altura, largura, bandas), dtype=np.uint8)

    for i in range(altura):
        for j in range(largura):
            if img.item(i,j,0) <= za:
                B = img.item(i,j,0)//2
            else:
                if img.item(i,j,0) > zb:
                    B = img.item(i,j,0) // 2 + 128
                else:
                    B = img.item(i,j,0) * 2 - 127
            if img.item(i,j,1) <= za:
                G = img.item(i,j,1)//2
            else:
                if img.item(i,j,1) > zb:
                    G = img.item(i,j,1) // 2 + 128
                else:
                    G = img.item(i,j,1) * 2 - 127
            if img.item(i,j,2) <= za:
                R = img.item(i,j,2)//2
            else:
                if img.item(i,j,2) > zb:
                    R = img.item(i,j,2) // 2 + 128
                else:
                    R = img.item(i,j,2) * 2 - 127
            if B<0:
                B = 0
            elif B>255:
                B = 255
            if G<0:
                G = 0
            elif G>255:
                G = 255
            if R<0:
                R = 0
            elif R>255:
                R = 255
            imagem_compressao_expansao.itemset((i,j,0),B)
            imagem_compressao_expansao.itemset((i,j,1),G)
            imagem_compressao_expansao.itemset((i,j,2),R)            
    return imagem_compressao_expansao   

def escala_log_contraste(img):
    altura, largura, bandas = img.shape
    imagem_escala_log_contraste = np.zeros((altura, largura, bandas), dtype=np.uint8)

    max_B = img[..., 0].max()
    max_G = img[..., 1].max()
    max_R = img[..., 2].max()

    cB = 255 // np.log(1 + max_B)
    cG = 255 // np.log(1 + max_G)
    cR = 255 // np.log(1 + max_R)

    for i in range(altura):
        for j in range(largura):
            valor_pixel_B = img.item(i, j, 0)
            valor_pixel_G = img.item(i, j, 1)
            valor_pixel_R = img.item(i, j, 2)
            novo_pixel_B =  cB * np.log(valor_pixel_B + 1)
            novo_pixel_G =  cG * np.log(valor_pixel_G + 1)
            novo_pixel_R =  cR * np.log(valor_pixel_R + 1)
            imagem_escala_log_contraste.itemset((i,j,0),novo_pixel_B)
            imagem_escala_log_contraste.itemset((i,j,1),novo_pixel_G)
            imagem_escala_log_contraste.itemset((i,j,2),novo_pixel_R)
    return imagem_escala_log_contraste

#za = 100
#zb = 200
z1 = 0
zn = 255

def main():
    option = int(input("\n1-expansao_constraste_linear\n2-compressao_expansao\n3-escala_log_contraste\n"))
    
    image_name = input("imagem.formato: ")
    input_image = cv2.imread(image_name)

    if input_image is None:
        print("Imagem não encontrada.")
        return

    if option == 2:
        za = int(input("Za = "))
        zb = int(input("Zb = "))
        output_image = compressao_expansao(input_image, za, zb)
    elif option == 3:
        output_image = escala_log_contraste(input_image)
    elif option ==1:
        za = int(input("Za = "))
        zb = int(input("Zb = "))
        output_image = expansao_constraste_linear(input_image, za, zb, z1, zn)
    else:
        print("Opção inválida.")
        return
    output_image_name = image_name.replace('.', f'_{option}_transformada.')
    cv2.imwrite(output_image_name, output_image)

if __name__ == "__main__":
    main()
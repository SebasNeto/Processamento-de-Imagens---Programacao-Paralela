def expansaoLinear(imagem, za, zb, z1, zn):
    altura, largura = len(imagem), len(imagem[0])

    imagem_expandida = [[0 for _ in range(largura)] for _ in range(altura)]
    
    #FORMULA UTILIZADA NO PASSO 3
    a = (zn - z1) / (zb - za)
    b = z1 - a * za

    for i in range(altura): #uma imagem possui altura 
        for j in range(largura): #uma imagem possui largura
            #SE FOSSE UMA IMAGEM COM CORES SERIAM 3 VARIAVEIS: ALTURA, LARGURA E CANAIS
            
            if imagem[i][j] <= za: #PASSO 1
                imagem_expandida[i][j] = z1
                
            elif imagem[i][j] >= zb: #PASSO 2
                imagem_expandida[i][j] = zn
                
            elif za < imagem[i][j] < zb: #PASSO 3
                imagem_expandida[i][j] = int(a * imagem[i][j] + b)
        
    return imagem_expandida #RETORNANDO A IMAGEM COM AS ALTERAÇÕES


caminho_imagem = 'imagens\\fusca2.png'
imagem = cv2.imread(caminho_imagem, cv2.IMREAD_GRAYSCALE)

za = 100
zb = 200
z1 = 50
zn = 100
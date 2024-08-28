import numpy as np

print("Transformadas Radiométricas: \n")

valor_python_transformada_expansao_linear = 272.8832
valor_python_transformada_logaritmo = 421.8791
valor_python_transformada_serra = 260.5805

valor_halide_transformada_expansao_linear = 185.4667
valor_halide_transformada_logaritmo = 211.5000
valor_halide_transformada_serra = 189.3667

ganho_transformada_expansao = valor_python_transformada_expansao_linear / valor_halide_transformada_expansao_linear
ganho_transformada_logaritmo = valor_python_transformada_logaritmo / valor_halide_transformada_logaritmo
ganho_transformada_serra = valor_python_transformada_serra / valor_halide_transformada_serra

print("ganho de desempenho trasnformada expansao linear (ms) em Halide: {:.4f} \n".format(ganho_transformada_expansao))
print("ganho de desempenho trasnformada logaritmo (ms) em Halide: {:.4f} \n".format(ganho_transformada_logaritmo))
print("ganho de desempenho trasnformada dente de serra (ms) em Halide: {:.4f} \n".format(ganho_transformada_serra))

print(" Filtragens Espaciais")

valor_python_flitragem_mediana = 98179.7097
valor_python_flitragem_media = 5138.1128
valor_python_flitragem_kvizinhos = 225874.5325

valor_halide_flitragem_mediana = 663.8667
valor_halide_flitragem_media = 442.1333
valor_halide_flitragem_kvizinhos = 1084.3000

ganho_flitragem_mediana= valor_python_flitragem_mediana / valor_halide_flitragem_mediana
ganho_flitragem_media = valor_python_flitragem_media / valor_halide_flitragem_media
ganho_flitragem_kvizinhos = valor_python_flitragem_kvizinhos / valor_halide_flitragem_kvizinhos

print("ganho de desempenho filtragem mediana (ms) em Halide: {:.4f} \n".format(ganho_flitragem_mediana))
print("ganho de desempenho filtragem media (ms) em Halide: {:.4f} \n".format(ganho_flitragem_media))
print("ganho de desempenho filtragem k vizinhos (ms) em Halide: {:.4f} \n".format(ganho_flitragem_kvizinhos))

print(" Detecção de Bordas")

valor_python_sobel = 5574.5200
valor_python_roberts = 4750.9302


valor_halide_sobel = 260.1667
valor_halide_roberts = 1006.5000


ganho_sobel= valor_python_sobel / valor_halide_sobel
ganho_roberts = valor_python_roberts / valor_halide_roberts


print("ganho de desempenho Sobel (ms) em Halide: {:.4f} \n".format(ganho_sobel))
print("ganho de desempenho Roberts (ms) em Halide: {:.4f} \n".format(ganho_roberts))

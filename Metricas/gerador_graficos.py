import matplotlib.pyplot as plt

# Dados dos tempos de execução e tamanhos de imagem
total_pixels = [
    1830550, 1852994, 1918540, 1920000, 1935966,  # Grupo I
    2039976, 2280256, 2818590, 3586836, 3902475,  # Grupo II
    4385934, 5901628, 6900300, 7237869, 8819306,  # Grupo III
    9336990, 10186836, 11194932, 11855844, 12192768,  # Grupo IV
    12780288, 15871387, 16084992, 18889344, 19961856,  # Grupo V
    22250970, 24000000, 25555284, 31216590, 33720352  # Grupo VI
]

halide_times = [
    339.0000, 311.0000, 295.0000, 325.0000, 319.0000,  # Grupo I
    296.0000, 302.0000, 354.0000, 308.0000, 256.0000,  # Grupo II
    348.0000, 251.0000, 257.0000, 261.0000, 282.0000,  # Grupo III
    257.0000, 258.0000, 279.0000, 260.0000, 262.0000,  # Grupo IV
    272.0000, 269.0000, 265.0000, 297.0000, 270.0000,  # Grupo V
    282.0000, 295.0000, 300.0000, 295.0000, 296.0000   # Grupo VI
]

python_times = [
    7445.4596 , 7793.6611 ,11314.2622 , 8870.6107 , 13369.8478 ,  # Grupo I
    10722.0287 , 9980.2921 , 10383.7144 , 16932.3351 , 20748.3628 ,  # Grupo II
    24908.2062, 42765.8238, 34485.6250, 19169.5409, 32175.8008,  # Grupo III
    49719.0969, 72016.3686, 65860.1713, 38848.0940, 64922.0078,  # Grupo IV
    40929.5595, 47252.0275, 74262.8911, 103217.0148, 125244.2317,  # Grupo V
    64523.3657, 67699.3787, 73130.7538, 95603.4956, 184184.8819  # Grupo VI
]

# Gráfico total pixels x tempo em milissegundos para Halide
plt.figure(figsize=(10, 5))
plt.plot(total_pixels, halide_times, 'o-', label='Halide', color='blue')
plt.xlabel('Total de Pixels')
plt.ylabel('Tempo (ms)')
plt.title('Total de Pixels vs Tempo de Execução (Halide)')
plt.yscale('log')
plt.legend()
plt.grid(True, which="both", ls="--")
plt.show()

# Gráfico total pixels x tempo em milissegundos para Python
plt.figure(figsize=(10, 5))
plt.plot(total_pixels, python_times, 'o-', label='Python', color='red')
plt.xlabel('Total de Pixels')
plt.ylabel('Tempo (ms)')
plt.title('Total de Pixels vs Tempo de Execução (Python)')
#plt.yscale('log')
plt.legend()
plt.grid(True, which="both", ls="--")
plt.show()

# Gráfico de comparação entre Halide e Python
plt.figure(figsize=(10, 5))
plt.plot(total_pixels, halide_times, 'o-', label='Halide', color='blue')
plt.plot(total_pixels, python_times, 'o-', label='Python', color='red')
plt.xlabel('Total de Pixels')
plt.ylabel('Tempo (ms)')
plt.title('Comparação Total de Pixels vs Tempo de Execução (Halide vs Python)')
plt.yscale('log')
plt.legend()
plt.grid(True, which="both", ls="--")
plt.show()

# import matplotlib.pyplot as plt

# # Dados dos tempos de execução e tamanhos de imagem
# total_pixels = [
#     1830550, 1852994, 1918540, 1920000, 1935966,  # Grupo I
#     2039976, 2280256, 2818590, 3586836, 3902475,  # Grupo II
#     4385934, 5901628, 6900300, 7237869, 8819306,  # Grupo III
#     9336990, 10186836, 11194932, 11855844, 12192768,  # Grupo IV
#     12780288, 15871387, 16084992, 18889344, 19961856,  # Grupo V
#     22250970, 24000000, 25555284, 31216590, 33720352  # Grupo VI
# ]

# halide_times = [
#     339.0000, 311.0000, 295.0000, 325.0000, 319.0000,  # Grupo I
#     296.0000, 302.0000, 354.0000, 308.0000, 256.0000,  # Grupo II
#     348.0000, 251.0000, 257.0000, 261.0000, 282.0000,  # Grupo III
#     257.0000, 258.0000, 279.0000, 260.0000, 262.0000,  # Grupo IV
#     272.0000, 269.0000, 265.0000, 297.0000, 270.0000,  # Grupo V
#     282.0000, 295.0000, 300.0000, 295.0000, 296.0000   # Grupo VI
# ]

# python_times = [
#     10223.2027, 9421.1404, 16259.6214, 10445.9932, 11338.0768,  # Grupo I
#     7247.1359, 6756.0272, 7255.6148, 15721.6446, 19280.4341,  # Grupo II
#     24908.2062, 42765.8238, 34485.6250, 19169.5409, 32175.8008,  # Grupo III
#     49719.0969, 72016.3686, 65860.1713, 38848.0940, 64922.0078,  # Grupo IV
#     40929.5595, 47252.0275, 74262.8911, 103217.0148, 125244.2317,  # Grupo V
#     64523.3657, 67699.3787, 73130.7538, 95603.4956, 184184.8819  # Grupo VI
# ]

# # Gráfico total pixels x tempo em milissegundos para Halide
# plt.figure(figsize=(10, 5))
# plt.plot(total_pixels, halide_times, 'o-', label='Halide', color='blue')
# plt.xlabel('Total de Pixels')
# plt.ylabel('Tempo (ms)')
# plt.title('Total de Pixels vs Tempo de Execução (Halide)')
# plt.legend()
# plt.grid(True)
# plt.show()

# # Gráfico total pixels x tempo em milissegundos para Python
# plt.figure(figsize=(10, 5))
# plt.plot(total_pixels, python_times, 'o-', label='Python', color='red')
# plt.xlabel('Total de Pixels')
# plt.ylabel('Tempo (ms)')
# plt.title('Total de Pixels vs Tempo de Execução (Python)')
# plt.legend()
# plt.grid(True)
# plt.show()

# # Gráfico de comparação entre Halide e Python
# plt.figure(figsize=(10, 5))
# plt.plot(total_pixels, halide_times, 'o-', label='Halide', color='blue')
# plt.plot(total_pixels, python_times, 'o-', label='Python', color='red')
# plt.xlabel('Total de Pixels')
# plt.ylabel('Tempo (ms)')
# plt.title('Comparação Total de Pixels vs Tempo de Execução (Halide vs Python)')
# plt.legend()
# plt.grid(True)
# plt.show()

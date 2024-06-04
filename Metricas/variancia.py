import numpy as np

# Dados fornecidos 
tempos_execucao = [
    141207.1157, 162064.4724, 184849.5958, 172449.5475, 188662.1773,
    181624.5449, 190776.5410, 199876.7972, 213743.5622, 219104.5542,
    188186.9497, 166494.4801, 201141.2179, 192801.9273, 186668.8123,
    914530.8723, 968041.2948, 1018601.7921, 1117678.1747, 1148956.8503,
    809409.1678, 803760.8793, 937911.6552, 897168.6590, 896057.9474,
    912060.0393, 997359.1485, 1127019.4790, 1121780.9086, 1151978.2617
]


# Calculando a média
media = np.mean(tempos_execucao)

# Calculando a variância
variancia = np.var(tempos_execucao)

# Calculando o desvio padrão
desvio_padrao = np.std(tempos_execucao)


# Convertendo para microssegundos (1 ms = 1000 microssegundos)
media_us = media * 1000
variancia_us = variancia * (1000**2)
desvio_padrao_us = desvio_padrao * 1000

# Tempos de execução
tempo_python = 587065.5808  # ms
tempo_halide = 1449.7333 # ms

# Calcular o ganho de tempo absoluto
ganho_ms = tempo_python - tempo_halide

# Calcular o ganho percentual para Halide
ganho_percentual_halide_ms = (ganho_ms / tempo_python) * 100
ganho_percentual_python_ms = 100 - ganho_percentual_halide_ms

# Convertendo tempos para microssegundos
tempo_python_us = tempo_python * 1000
tempo_halide_us = tempo_halide * 1000
ganho_us = ganho_ms * 1000

# Calcular o ganho percentual em microssegundos
ganho_percentual_halide_us = (ganho_us / tempo_python_us) * 100
ganho_percentual_python_us = 100 - ganho_percentual_halide_us

print ("############################# Valores em Milissegundos #############################")
print(f"Média: {media:.4f} ms")
print(f"Variância: {variancia:.4f} ms²")
print(f"Desvio Padrão: {desvio_padrao:.4f} ms")

print(f"Tempo de execução em Python: {tempo_python:.4f} ms")
print(f"Tempo de execução em Halide: {tempo_halide:.4f} ms")
print(f"Ganho em tempo absoluto: {ganho_ms:.4f} ms")
print(f"Ganho percentual para Halide: {ganho_percentual_halide_ms:.2f} %")
print(f"Ganho percentual para Python: {ganho_percentual_python_ms:.2f} %")

print ("############################# Valores em Microssegundos #############################")
print(f"Média: {media_us:.4f} µs")
print(f"Variância: {variancia_us:.4f} µs²")
print(f"Desvio Padrão: {desvio_padrao_us:.4f} µs")

print(f"Tempo de execução em Python: {tempo_python_us:.4f} µs")
print(f"Tempo de execução em Halide: {tempo_halide_us:.4f} µs")
print(f"Ganho em tempo absoluto: {ganho_us:.4f} µs")
print(f"Ganho percentual para Halide: {ganho_percentual_halide_us:.2f} %")
print(f"Ganho percentual para Python: {ganho_percentual_python_us:.2f} %")


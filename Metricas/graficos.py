import matplotlib.pyplot as plt
import numpy as np

# Dados para as máquinas do ICOMP
tecnicas = ["Contraste Linear", "Logaritmo", "Dente de Serra", "Média", "Mediana", "K Vizinhos Mais Próximos", "Sobel", "Roberts"]
halide_python_icomp = [52.2667, 57.9667, 54.6000, 108.4333, 167.2000, 256.9000, 260.1667, 234.8667]
python_icomp = [10540.8364, 7649.6159, 23112.5866, 89158.4204, 197033.2473, 175001.3283, 163969.2557, 161012.9039]
halide_c_icomp = [52.2667, 57.9667, 54.6000, 108.4333, 167.2000, 256.9000, 260.1667, 234.8667]
c_icomp = [108.4604, 154.5248, 121.7629, 1175.0984, 20485.0092, 42856.6879, 1367.5888, 1048.4216]

# Dados para a máquina do desenvolvedor
halide_python_dev = [185.4667, 211.5000, 189.3667, 442.1333, 663.8667, 1084.3000, 1230.2667, 1006.5000]
python_dev = [54202.6059, 40204.0658, 119273.2424, 450812.6250, 1105394.0205, 918534.6280, 786337.9029, 816129.8633]
halide_c_dev = [185.4667, 211.5000, 189.3667, 442.1333, 663.8667, 1084.3000, 1230.2667, 1006.5000]
c_dev = [272.8832, 421.8791, 260.5805, 5138.1128, 98179.7097, 225874.5325, 5574.5200, 4750.9302]

x = np.arange(len(tecnicas))

# Gráfico para máquinas do ICOMP
fig, ax = plt.subplots(figsize=(10, 6))
ax.bar(x - 0.3, python_icomp, 0.3, label='Python', color='blue')
ax.bar(x, halide_python_icomp, 0.3, label='Halide', color='green')
ax.bar(x + 0.3, c_icomp, 0.3, label='C', color='red')

ax.set_yscale('log')
ax.set_ylabel('Tempo (ms) (Escala Logarítmica)')
ax.set_title('Comparação de Desempenho (Máquinas ICOMP)')
ax.set_xticks(x)
ax.set_xticklabels(tecnicas, rotation=45, ha="right")
ax.legend()

plt.tight_layout()
plt.show()

# Gráfico para a máquina do desenvolvedor
fig, ax = plt.subplots(figsize=(10, 6))
ax.bar(x - 0.3, python_dev, 0.3, label='Python', color='blue')
ax.bar(x, halide_python_dev, 0.3, label='Halide', color='green')
ax.bar(x + 0.3, c_dev, 0.3, label='C', color='red')

ax.set_yscale('log')
ax.set_ylabel('Tempo (ms) (Escala Logarítmica)')
ax.set_title('Comparação de Desempenho (Máquina do Desenvolvedor)')
ax.set_xticks(x)
ax.set_xticklabels(tecnicas, rotation=45, ha="right")
ax.legend()

plt.tight_layout()
plt.show()

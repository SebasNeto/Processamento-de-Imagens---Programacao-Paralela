import numpy as np

# Tempos de execução em Halide (ms)
halide_times = [
    69.0680, 69.6220, 68.1350, 69.6950, 68.7620,
    69.1580, 74.2980, 78.6320, 77.2260, 80.0440,
    79.0780, 93.5040, 107.6300, 102.0260, 123.3050,
    133.9780, 129.1480, 133.9720, 130.4590, 138.2500,
    151.1680, 153.1410, 156.9710, 163.8330, 169.2120,
    166.2110, 177.1970, 179.0330, 207.2700, 232.8600
]

# Cálculos para Halide
halide_mean = np.mean(halide_times)
halide_variance = np.var(halide_times)
halide_std_dev = np.std(halide_times)

print("Média Halide (ms): {:.4f}".format(halide_mean))
print("Variância Halide (ms²): {:.4f}".format(halide_variance))
print("Desvio Padrão Halide (ms): {:.4f}".format(halide_std_dev))
print()

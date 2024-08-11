import numpy as np

# Tempos de execução em Halide (ms)

halide_times = [
    125939.7056, 127179.1759, 133390.5654, 134674.7520, 140021.3809,
    147205.7126, 171535.2125, 202124.3982, 267170.7997, 278919.5936,
    325068.2647, 519221.1130, 501824.8320, 540378.3917, 641447.1951,
    685480.1652, 742478.7047, 821178.1018, 837201.2448, 867041.0287,
    949000.2558, 1177673.0487, 1179455.6384, 1382163.6508, 1452320.7908,
    1601902.5502, 1759412.0934, 1887976.7296, 2390746.8312, 2493763.9706
]

# Cálculos para Halide
halide_mean = np.mean(halide_times)
halide_variance = np.var(halide_times)
halide_std_dev = np.std(halide_times)

print("Média Halide (ms): {:.4f}".format(halide_mean))
print("Variância Halide (ms²): {:.4f}".format(halide_variance))
print("Desvio Padrão Halide (ms): {:.4f}".format(halide_std_dev))
print()

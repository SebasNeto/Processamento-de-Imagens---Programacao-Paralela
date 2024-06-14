import numpy as np

# Tempos de execução em Halide (ms)
halide_times = [
    339.0000, 311.0000, 295.0000, 325.0000, 319.0000,
    296.0000, 302.0000, 354.0000, 308.0000, 256.0000,
    348.0000, 251.0000, 257.0000, 261.0000, 282.0000,
    257.0000, 258.0000, 279.0000, 260.0000, 262.0000,
    272.0000, 269.0000, 265.0000, 297.0000, 270.0000,
    282.0000, 295.0000, 300.0000, 295.0000, 296.0000
]

# Tempos de execução em Python (ms)
python_times = [
    7445.4596 , 7793.6611 ,11314.2622 , 8870.6107 , 13369.8478 ,  # Grupo I
    10722.0287 , 9980.2921 , 10383.7144 , 16932.3351 , 20748.3628 ,  # Grupo II
    24908.2062, 42765.8238, 34485.6250, 19169.5409, 32175.8008,  # Grupo III
    49719.0969, 72016.3686, 65860.1713, 38848.0940, 64922.0078,  # Grupo IV
    40929.5595, 47252.0275, 74262.8911, 103217.0148, 125244.2317,  # Grupo V
    64523.3657, 67699.3787, 73130.7538, 95603.4956, 184184.8819  # Grupo VI
]

# Cálculos para Halide
halide_mean = np.mean(halide_times)
halide_variance = np.var(halide_times)
halide_std_dev = np.std(halide_times)

# Cálculos para Python
python_mean = np.mean(python_times)
python_variance = np.var(python_times)
python_std_dev = np.std(python_times)

# Ganho percentual
gain_percentage = python_mean/halide_mean

print("Média Halide (ms): {:.4f}".format(halide_mean))
print("Variância Halide (ms²): {:.4f}".format(halide_variance))
print("Desvio Padrão Halide (ms): {:.4f}".format(halide_std_dev))
print()
print("Média Python (ms): {:.4f}".format(python_mean))
print("Variância Python (ms²): {:.4f}".format(python_variance))
print("Desvio Padrão Python (ms): {:.4f}".format(python_std_dev))
print()
print("Ganho Percentual (%): {:.4f}".format(gain_percentage))

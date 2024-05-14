# Novos tempos de execução em milissegundos
tempos_execucao_ms = [
    970.4912, 591.6586, 341.4867, 1179.5490, 537.6740,
    747.8318, 519.5112, 1034.7826, 900.8639, 718.5659,
    1125.5007, 1077.7912, 1701.0553, 474.7002, 450.0661,
    972.7678, 880.4367, 1667.2206, 142.1659, 121.3920,
    1076.5898, 936.8005, 769.5425, 1124.0819, 545.8446,
    1134.8765, 387.7795, 330.4071, 1174.3476, 1474.0572
]

# Conversão de milissegundos para microsegundos (1 ms = 1000 µs)
tempos_execucao = [ms * 1000 for ms in tempos_execucao_ms]

# Cálculo da média de tempo de execução
media_tempo_execucao = sum(tempos_execucao) / len(tempos_execucao)
print(f"Tempo de execução médio: {media_tempo_execucao:.4f} microsegundos")

# Soma de todos os tempos de execução
soma_tempo_execucao = sum(tempos_execucao)
print(f"A soma total em: {soma_tempo_execucao:.4f} microsegundos")
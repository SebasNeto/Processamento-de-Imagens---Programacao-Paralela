# Tempos de execução em milissegundos
tempos_execucao = [
    513.000000, 432.000000, 475.000000, 491.000000, 786.000000,
    512.000000, 434.000000, 538.000000, 477.000000, 511.000000,
    443.000000, 498.000000, 496.000000, 4079.000000, 509.000000,
    518.000000, 450.000000, 520.000000, 688.000000, 492.000000,
    586.000000, 408.000000, 453.000000, 468.000000, 463.000000,
    512.000000, 485.000000, 461.000000, 506.000000, 472.000000
]

# Cálculo da média de tempo de execução em milissegundos
media_tempo_execucao = sum(tempos_execucao) / len(tempos_execucao)
print(f"Tempo de execução médio: {media_tempo_execucao:.2f} milissegundos")

# Soma de todos os tempos de execução em milissegundos
soma_tempo_execucao = sum(tempos_execucao)
print(f"A soma total em: {soma_tempo_execucao:.2f} milissegundos")

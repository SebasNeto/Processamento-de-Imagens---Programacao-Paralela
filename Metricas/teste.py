import pandas as pd

# Atualizar o caminho do arquivo CSV para o caminho correto no WSL
data = pd.read_csv('/mnt/c/PerfLogs/dados_cpu.csv', encoding='ISO-8859-1')

# Exibir as primeiras linhas do DataFrame
print(data.head())
print(data.columns)

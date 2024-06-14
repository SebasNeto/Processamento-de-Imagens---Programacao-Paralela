import pandas as pd
import matplotlib.pyplot as plt

# Atualizar o caminho do arquivo CSV para o caminho correto no WSL
data = pd.read_csv('/mnt/c/PerfLogs/dados_cpu_comp_exp_python.csv', encoding='ISO-8859-1')

# Renomear a coluna de tempo para simplificar
data = data.rename(columns={'(PDH-CSV 4.0) (Hora Padrão de Caracas)(240)': 'Time'})

# Converter a coluna de tempo para datetime
data['Time'] = pd.to_datetime(data['Time'], format='%m/%d/%Y %H:%M:%S.%f')

# Filtrar colunas relevantes relacionadas à utilização da CPU
columns_to_plot = [
    '\\\\SEBASTIAO\\Informações do Processador(_Total)\\% Tempo do Processador',
    '\\\\SEBASTIAO\\Informações do Processador(_Total)\\% Tempo do Usuário',
    '\\\\SEBASTIAO\\Informações do Processador(_Total)\\% Tempo Privilegiado',
    '\\\\SEBASTIAO\\Informações do Processador(_Total)\\% de Utilitário do Processador',
    '\\\\SEBASTIAO\\Informações do Processador(_Total)\\% de Utilitário Privilegiado',
    '\\\\SEBASTIAO\\Informações do Processador(_Total)\\% de Desempenho do Processador'
]

# Verificar os valores das colunas selecionadas (opcional)
for col in columns_to_plot:
    print(f"Valores da coluna {col}:")
    print(data[col].head())

# Plotar os dados de utilização da CPU
plt.figure(figsize=(15, 7))

for col in columns_to_plot:
    plt.plot(data['Time'], data[col], label=col.split('\\')[-1])

plt.xlabel('Tempo')
plt.ylabel('Utilização da CPU (%)')
plt.title('Utilização da CPU durante a execução do programa')
plt.legend(loc='upper left')
plt.grid(True)
plt.tight_layout()

# Salvar o gráfico em um arquivo PNG
output_path = '/mnt/c/Users/Cliente/OneDrive/Documentos/PIBIC/PROCESSAMENTO_DE_IMAGENS_PROGAMACAO_PARALELA_PIBIC/Metricas/utilizacao_cpu_t_exp_comp_Python.png'
plt.savefig(output_path)

# Exibir o gráfico (opcional)
plt.show()



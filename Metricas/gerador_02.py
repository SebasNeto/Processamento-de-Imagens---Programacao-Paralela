import matplotlib.pyplot as plt

# Dados dos tamanhos de imagem e tempos de execução para os grupos I, II e III
group_pixels = {
    'Grupo I': [1830550, 1852994, 1918540, 1920000, 1935966],
    #'Grupo II': [2039976, 2280256, 2818590, 3586836, 3902475],
    'Grupo III': [4385934, 5901628, 6900300, 7237869, 8819306]
}

group_python_times = {
    'Grupo I': [7445.4596 , 7793.6611 ,11314.2622 , 8870.6107 , 13369.8478],
    #'Grupo II': [10722.0287 , 9980.2921 , 10383.7144 , 16932.3351 , 20748.3628],
    'Grupo III': [24908.2062, 42765.8238, 34485.6250, 19169.5409, 32175.8008]
}

# Plotando os gráficos para os três grupos
for group in group_pixels:
    plt.figure(figsize=(10, 5))
    plt.plot(group_pixels[group], group_python_times[group], 'o-', label=f'Tempos {group}', color='red')
    plt.xlabel('Total de Pixels')
    plt.ylabel('Tempo (ms)')
    plt.title(f'Total de Pixels vs Tempo de Execução (Python) - {group}')
    plt.legend()
    plt.grid(True)
    plt.show()

# Plotando os gráficos para os três grupos com escala logarítmica
for group in group_pixels:
    plt.figure(figsize=(10, 5))
    plt.plot(group_pixels[group], group_python_times[group], 'o-', label=f'Tempos {group}', color='red')
    plt.xlabel('Total de Pixels')
    plt.ylabel('Tempo (ms)')
    plt.yscale('log')
    plt.title(f'Total de Pixels vs Tempo de Execução (Python) - {group} (Escala Logarítmica)')
    plt.legend()
    plt.grid(True, which="both", ls="--")
    plt.show()

# Plotando o gráfico comparativo para os três grupos
plt.figure(figsize=(12, 6))
for group in group_pixels:
    plt.plot(group_pixels[group], group_python_times[group], 'o-', label=f'{group}')
plt.xlabel('Total de Pixels')
plt.ylabel('Tempo (ms)')
plt.title('Comparação Total de Pixels vs Tempo de Execução (Python) - Grupos I e III')
plt.legend()
plt.grid(True)
plt.show()

# Plotando o gráfico comparativo para os três grupos com escala logarítmica
plt.figure(figsize=(12, 6))
for group in group_pixels:
    plt.plot(group_pixels[group], group_python_times[group], 'o-', label=f'{group}')
plt.xlabel('Total de Pixels')
plt.ylabel('Tempo (ms)')
plt.yscale('log')
plt.title('Comparação Total de Pixels vs Tempo de Execução (Python) - Grupos I e III (Escala Logarítmica)')
plt.legend()
plt.grid(True, which="both", ls="--")
plt.show()

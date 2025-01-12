import matplotlib.pyplot as plt
import numpy as np

# Dados
data = {
    'Tamanho Entrada': [2, 10],
    'FCFS': [1.00924, 2.50493],
    'SJF': [2.50452, 5.50493],
    'RR': [1.00102, 3.50271],
    'P': [1.00209, 3.5026],
}

colors = {
    'FCFS': '#a6cee3',  # azul claro
    'SJF': '#b2df8a',   # verde claro
    'RR': '#ff9999',    # vermelho claro
    'P': '#cab2d6'      # roxo claro
}

# Configuração do gráfico
plt.figure(figsize=(12, 8))

# Aplicando deslocamento para distinguir os dados
offsets = {
    'FCFS': 0.01,
    'SJF': 0.02,
    'RR': -0.01,
    'P': -0.02,
}

# Plotar os dados com deslocamento
for scheduler, color in colors.items():
    adjusted_values = [val + offsets[scheduler] for val in data[scheduler]]
    plt.plot(data['Tamanho Entrada'], adjusted_values, label=scheduler, color=color, linewidth=2, marker='o', markersize=8, linestyle='--')

    # Adicionar os valores dos pontos no gráfico
    for i, (x, y) in enumerate(zip(data['Tamanho Entrada'], adjusted_values)):
        plt.text(x, y, f'{y:.4f}', color=color, ha='center', va='bottom', fontsize=10)

# Configurações do gráfico
plt.xlabel('Tamanho da Entrada', fontsize=12)
plt.ylabel('Tempo de Execução (s)', fontsize=12)
plt.title('Tamanho da Entrada vs Tempo de Execução (s)', fontsize=14)
plt.legend(title='Escalonador', fontsize=10)
plt.grid(visible=True, linestyle='--', alpha=0.6)
plt.ylim(0, 6)  # Ajustar a escala para começar antes do 2 e dar espaço para os valores

# Mostrar o gráfico
plt.tight_layout()
plt.show()

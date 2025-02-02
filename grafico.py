import matplotlib.pyplot as plt
import numpy as np

# Dados
tipos = ["FCFS", "SJF", "RR", "Prioridade"]

valores_sem_cache = [4.00465, 4.50446, 12.0158, 12.0153]
valores_com_cache = [4.00504, 4.50608, 8.01214, 8.01282]  # Cache LRU

# Posições no eixo X
x = np.arange(len(tipos))
largura = 0.3  # Largura das barras

# Cores
theme_colors = {
    'LRU': 'lightblue',
    'Sem Cache': 'lightcoral',
}

# Criando o gráfico com um tamanho maior
fig, ax = plt.subplots(figsize=(14, 8))  # Ajustando altura

# Criando barras
barras_sem_cache = ax.bar(x - largura/2, valores_sem_cache, largura, label='Sem Cache', color=theme_colors['Sem Cache'])
barras_com_cache = ax.bar(x + largura/2, valores_com_cache, largura, label='Com Cache (LRU)', color=theme_colors['LRU'])

# Ajustando os valores para evitar sobreposição
for barra in barras_sem_cache:
    ax.text(barra.get_x() + barra.get_width()/2, barra.get_height() + 0.05, f'{barra.get_height():.5f}', ha='center', va='bottom', fontsize=12)

for barra in barras_com_cache:
    ax.text(barra.get_x() + barra.get_width()/2, barra.get_height() + 0.05, f'{barra.get_height():.5f}', ha='center', va='bottom', fontsize=12)

# Configurações do gráfico
ax.set_xlabel('Algoritmos', fontsize=14)
ax.set_ylabel('Tempo de Execução', fontsize=14)
ax.set_title('Cenário 3: Comparação de Desempenho com e sem Cache (8 arquivos)', fontsize=16)
ax.set_xticks(x)
ax.set_xticklabels(tipos, rotation=0, fontsize=12)
ax.legend(fontsize=12)

# Ajustando limites do eixo Y para evitar corte
y_max = max(max(valores_sem_cache), max(valores_com_cache)) * 1.1  # Aumentando 10% do maior valor
ax.set_ylim(0, y_max)

# Melhorando layout para evitar cortes
plt.tight_layout()

# Exibir gráfico
plt.show()
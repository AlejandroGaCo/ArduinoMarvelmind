#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Apr  5 22:13:37 2025

@author: alegaco
"""

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# Leer archivo .xlsx y hoja
archivo = "datos.xlsx"
nombre_hoja = "Fig523"
df = pd.read_excel(archivo, sheet_name=nombre_hoja)

# Extraer columnas
x = df['x']
y = df['y']
xd = df['xd']
yd = df['yd']

# Colores personalizados
azul_oscuro = "#005A9C"
naranja_oscuro = "#D35400"

# Crear figura cuadrada exacta
fig, ax = plt.subplots(figsize=(6, 6))  # en pulgadas, cuadrado

# Trazar trayectorias
ax.plot(x, y, 'o-', color=azul_oscuro, linewidth=2.2, label='Trayectoria real')
ax.plot(xd, yd, color=naranja_oscuro, linewidth=2.2, linestyle='--', label='Trayectoria deseada')

# Etiquetas de ejes
ax.set_xlabel('Posición en eje X [m]', fontsize=12)
ax.set_ylabel('Posición en eje Y [m]', fontsize=12)

# Ejes de 0 a 5 sin márgenes
ax.set_xlim(1, 5)
ax.set_ylim(0, 4)

# Ticks para el eje X de 2 a 6, con espaciado de 0.5
ticks_x = np.arange(1, 5.01, 0.5)  # Desde 2 hasta 6 con paso de 0.5
ax.set_xticks(ticks_x)

# Ticks para el eje Y
ticks_y = np.arange(0, 4.01, 0.5)  # Desde 0 hasta 5 con paso de 0.5
ax.set_yticks(ticks_y)

# Negrita en los números
ax.tick_params(axis='both', labelsize=10)
for label in ax.get_xticklabels() + ax.get_yticklabels():
    label.set_fontweight('bold')

# Cuadrícula y proporción 1:1
ax.grid(True)
ax.set_aspect('equal', adjustable='box')

# Leyenda dentro del gráfico
ax.legend(loc='best', fontsize=10)

# Sin título
ax.set_title("")

# Layout sin márgenes
plt.subplots_adjust(left=0.12, right=0.97, top=0.97, bottom=0.12)

# Guardar imagen con alta resolución
nombre_archivo = f"{nombre_hoja}.png"
plt.savefig(nombre_archivo, dpi=600)
print(f"Gráfica guardada como '{nombre_archivo}' en alta resolución.")

# Mostrar
plt.show()

#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Apr  5 23:37:33 2025

@author: alegaco
"""

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.animation import FuncAnimation
import matplotlib

matplotlib.rcParams['animation.ffmpeg_path'] = '/opt/homebrew/bin/ffmpeg'  # Sustituye con la ruta completa


# Leer archivo .xlsx y hoja
archivo = "Datos.xlsx"
nombre_hoja = "Fig516"
df = pd.read_excel(archivo, sheet_name=nombre_hoja)

# Convertir 'tout' a tipo numérico (float)
df['tout'] = pd.to_numeric(df['tout'], errors='coerce')  # Convierte a numérico, forzando errores a NaN

# Extraer columnas
x = df['x']
y = df['y']
xd = df['xd']
yd = df['yd']
tout = df['tout']  # Columna adicional

# Colores personalizados
azul_oscuro = "#005A9C"
naranja_oscuro = "#D35400"

# Encontrar los índices para el segundo 
# Para 23 de 3.5 a 13.5
# Para 18 de 3.5 a 10
# Para 17 de 0 a 10
# Para 16 de 0 a 20
inicio = tout[tout >= 0].index[0]
fin = tout[tout <= 20].index[-1]

# Subconjunto de datos que va desde el segundo 3.5 hasta el segundo 15
x_sub = x[inicio:fin+1]
y_sub = y[inicio:fin+1]
xd_sub = xd[inicio:fin+1]
yd_sub = yd[inicio:fin+1]
tout_sub = tout[inicio:fin+1]


# Crear figura cuadrada exacta
fig, ax = plt.subplots(figsize=(6, 6))  # en pulgadas, cuadrado

# Trazar trayectorias
linea_real, = ax.plot([], [], 'o-', color=azul_oscuro, linewidth=2.2, label='Trayectoria real')
linea_deseada, = ax.plot([], [], color=naranja_oscuro, linewidth=2.2, linestyle='--', label='Trayectoria deseada')

# Etiquetas de ejes
ax.set_xlabel('Posición en eje X [m]', fontsize=12)
ax.set_ylabel('Posición en eje Y [m]', fontsize=12)

# Ejes de 0 a 5 sin márgenes
ax.set_xlim(1, 5)
ax.set_ylim(0, 4)

# Ticks para el eje X de 2 a 6, con espaciado de 0.5
ticks_x = np.arange(1, 5.01, 0.5)
ax.set_xticks(ticks_x)

# Ticks para el eje Y
ticks_y = np.arange(0, 4.01, 0.5)
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

# Función para inicializar la animación
def init():
    linea_real.set_data([], [])
    linea_deseada.set_data([], [])
    return linea_real, linea_deseada

# Función para actualizar la animación
def update(frame):
    # Actualizar las trayectorias
    linea_real.set_data(x_sub[:frame], y_sub[:frame])
    linea_deseada.set_data(xd_sub[:frame], yd_sub[:frame])
    return linea_real, linea_deseada

# Crear la animación
ani = FuncAnimation(fig, update, frames=len(tout_sub), init_func=init, blit=True, interval=100)

# Guardar la animación como un archivo .mp4 (puedes cambiar el formato si lo deseas)
nombre_archivo = f"{nombre_hoja}.mp4"
ani.save(nombre_archivo, writer='ffmpeg', dpi=600)

print(f"Animación guardada como '{nombre_archivo}'.")

# Mostrar la animación
plt.show()

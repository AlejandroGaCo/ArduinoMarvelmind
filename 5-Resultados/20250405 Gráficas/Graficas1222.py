#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Nueva gráfica para PWM vs wL y PWM vs wR
"""

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy.stats import linregress

# Leer archivo .xlsx y hoja
archivo = "Datos.xlsx"
nombre_hoja = "Fig512"  # Cambia esto al nombre correcto de la hoja
df = pd.read_excel(archivo, sheet_name=nombre_hoja)

# Extraer columnas
pwm = df['PWM']
wL = df['wL']
wR = df['wR']

# Colores personalizados
azul_oscuro = "#005A9C"
naranja_oscuro = "#D35400"

# Crear figura con dos subgráficas (una encima de la otra)
fig, (ax1, ax2) = plt.subplots(nrows=2, figsize=(10, 8), sharex=True)  # Compartir eje X (PWM)

# Gráfica para wL vs PWM
ax1.plot(pwm, wL, 'o-', color=azul_oscuro, label='Motor izquierdo')
ax1.set_ylabel('Velocidad Angular [rad/s] (wL)', fontsize=12)

# Ajuste de la recta para wL
slope_wL, intercept_wL, r_value_wL, p_value_wL, std_err_wL = linregress(pwm, wL)
# Cambiar color de la línea de ajuste (fit)
ax1.plot(pwm, slope_wL * pwm + intercept_wL, color='gray', linestyle='--', label=f'Recta wL (R²={r_value_wL**2:.2f})')

# Añadir la fórmula de la recta y R² para wL
ax1.text(200, max(wL) * 0.8, f"wL = {slope_wL:.4f} * PWM + {intercept_wL:.4f}\nR² = {r_value_wL**2:.2f}",
         fontsize=12, color='black', weight='bold', ha='center', va='top')

# Trazar leyenda en el primer gráfico
ax1.legend(loc='best', fontsize=10)
ax1.grid(True)

# Gráfica para wR vs PWM
ax2.plot(pwm, wR, 'o-', color=naranja_oscuro, label='Motor derecho')
ax2.set_xlabel('PWM', fontsize=12)
ax2.set_ylabel('Velocidad Angular [rad/s] (wR)', fontsize=12)

# Ajuste de la recta para wR
slope_wR, intercept_wR, r_value_wR, p_value_wR, std_err_wR = linregress(pwm, wR)
# Cambiar color de la línea de ajuste (fit)
ax2.plot(pwm, slope_wR * pwm + intercept_wR, color='gray', linestyle='--', label=f'Recta wR (R²={r_value_wR**2:.2f})')

# Añadir la fórmula de la recta y R² para wR
ax2.text(200, max(wR) * 0.8, f"wR = {slope_wR:.4f} * PWM + {intercept_wR:.4f}\nR² = {r_value_wR**2:.2f}",
         fontsize=12, color='black', weight='bold', ha='center', va='top')

# Trazar leyenda en el segundo gráfico
ax2.legend(loc='best', fontsize=10)
ax2.grid(True)

# Ajustar límites de los ejes
ax1.set_xlim(0, 265)  # Eje X va de 0 a 255 (PWM)
ax2.set_xlim(0, 265)  # Eje X va de 0 a 255 (PWM)

# Límites de los ejes Y para ambos gráficos
ax1.set_ylim(14, 20)
ax2.set_ylim(14, 20)

# Layout sin márgenes
plt.subplots_adjust(left=0.12, right=0.97, top=0.97, bottom=0.12)

# Guardar imagen con alta resolución
nombre_archivo = f"{nombre_hoja}_PWM_vs_velocidad.png"
plt.savefig(nombre_archivo, dpi=600, bbox_inches='tight')
print(f"Gráfica guardada como '{nombre_archivo}' en alta resolución.")

# Mostrar las gráficas
plt.show()


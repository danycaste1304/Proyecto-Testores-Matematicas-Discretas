# Proyecto-Testores-Matematicas-Discretas
Proyecto de Luis David Borja, Camila Granda y Daniela Castellanos

Este proyecto implementa los algoritmos YYC y BT para el cálculo de testores típicos a partir de matrices básicas, siguiendo los lineamientos de la materia Matemáticas Discretas (USFQ).  
Incluye generación de matrices booleanas y matrices básicas, ejecución detallada de ambos algoritmos, cálculo de densidades, operadores sintéticos (φ, θ, γ), análisis experimental y mediciones de tiempo.

---

## **Contenido del Proyecto**

El repositorio incluye:

- Código fuente completo en C++ para todos los ejercicios:
  - Generación de matrices booleanas.
  - Conversión a matriz básica.
  - Densidad.
  - Ejecución del algoritmo YYC.
  - Ejecución del algoritmo BT.
  - Construcción de matrices sintéticas usando φ, θ y γ.
  - Comparaciones experimentales (tablas similares a Tablas 5 y 6 del paper "Generating synthetic test matrices as a benchmark for the computational behavior of typical testor-finding algorithms").
- Código modularizado y comentado.
- Pruebas manuales del YYC y BT (en el reporte LaTeX).
- Archivos auxiliares, ejemplos y matrices base.
- Archivo `.gitignore` para excluir temporales.

---

## **Descripción General**

Los testores son subconjuntos mínimos de características que permiten distinguir completamente todos los objetos de una matriz básica.  
Los testores típicos son testores minimalmente representativos según la teoría de matrices de diferencias.

Este proyecto cumple con los siguientes objetivos:

1. Generar matrices booleanas al azar (hasta 100×10).
2. Convertirlas en matrices básicas y calcular su densidad.
3. Ejecutar manualmente y computacionalmente:
   - YYC: "Fast Incremental Algorithm for Finding Typical Testors" 
   - BT: Algoritmo por salto
4. Construir la matriz B a partir de los testores típicos de A.
5. Implementar operadores sintéticos φ, θ, γ.
6. Ejecutar experimentos de rendimiento y comparar tiempos YYC vs BT.

---

## **Tecnologías usadas**

- C++17
- `chrono` para medición de tiempos
- `vector` para estructuras matriciales
- `std::algorithm`, `stdint.h`
- Compilación recomendada con g++ o clang++

---

## **Instalación**

Sigue estos pasos para compilar y ejecutar el proyecto en cualquier sistema con C++:

### **Clonar el repositorio**
```bash
git clone https://github.com/<TU_USUARIO> Proyecto-Testores-Matematicas-Discretas.git
cd Proyecto-Testores-Matematicas-Discretas

### **Compilar**
```bash
g++ -std=c++17 -O2 main.cpp -o testores


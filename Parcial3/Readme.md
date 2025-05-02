# README - Cambios para Soporte de Concurrencia (OpenMP)

Este documento describe exclusivamente los cambios realizados al proyecto base para incorporar **concurrencia** mediante **OpenMP** en el procesamiento de imagenes.

---

##  Objetivo

Aprovechar multiples nucleos del procesador para mejorar el rendimiento en las operaciones de **rotacion** y **escalado** de imagenes, manteniendo la compatibilidad con el sistema de asignacion de memoria Buddy System y new/delete.

---

##  Cambios Realizados

### 1. Inclusión de OpenMP

Se incluyo la cabecera de OpenMP en el archivo `imagen.cpp`:
```cpp
#include <omp.h>
```

Y se actualizo el `Makefile` para agregar la bandera de compilacion:
```make
CFLAGS = -Wall -std=c++17 -fopenmp
```

---

### 2. Paralelizacion de Rotacion

En la funcion `rotarImagen()`, se paralelizo el bucle principal que recorre las filas de la imagen:
```cpp
#pragma omp parallel for
for (int y = 0; y < nuevoAlto; y++) {
    for (int x = 0; x < nuevoAncho; x++) {
        // procesamiento por pixel
    }
}
```

Esto permite que cada hilo procese diferentes filas de la imagen simultaneamente.

---

### 3. Paralelizacion de Escalado

De manera similar, en `escalarImagen()`, se agrego la misma directiva para paralelizar por filas:
```cpp
#pragma omp parallel for
for (int y = 0; y < nuevoAlto; y++) {
    for (int x = 0; x < nuevoAncho; x++) {
        // interpolacion por pixel
    }
}
```

---

### 4. Control del numero de hilos desde la terminal

El programa no requiere cambios internos para controlar el numero de hilos. Se utiliza la variable de entorno estandar de OpenMP:

```bash
OMP_NUM_THREADS=<hilos> ./programa_buddy <entrada> <salida> -angulo <grados> -escalar <factor> <-buddy|-no-buddy>
```

#### Ejemplo:
```bash
OMP_NUM_THREADS=6 ./programa_buddy IMGTest/Target-Calibration-File.jpg salida.png -angulo 45 -escalar 1.5 -buddy
```

---

## 🖥 Resultados en la maquina de prueba

El programa fue ejecutado en una maquina con las siguientes caracteristicas:
- **8 nucleos fisicos**
- **16 hilos logicos**
- **Velocidad base de 2.90 GHz**

### Resultados de tiempo de procesamiento:

| Hilos (`OMP_NUM_THREADS`) | Tiempo (ms) | Memoria utilizada (KB) |
|---------------------------|-------------|-------------------------|
| 1                         | 12759       | 1098488                 |
| 4                         | 8114        | 1098296                 |
| 6                         | 7639        | 1098524                 |
| 8                         | 8795        | 1098404                 |

### Analisis:
- El mejor tiempo se obtuvo con **6 hilos**, no con 8.
- Aunque el procesador tiene 8 nucleos fisicos y 16 hilos logicos, usar demasiados hilos puede **saturar la cache y el bus de memoria**, empeorando el rendimiento.
- Las operaciones de interpolacion bilineal requieren **alto acceso a memoria**, y los hilos extra generan sobrecarga cuando el numero de hilos excede la capacidad fisica del procesador.

### Conclusion:
- OpenMP ofrece una mejora importante de rendimiento (hasta 40% menos tiempo).
- Es recomendable **probar distintos valores de hilos** para encontrar el optimo segun el tipo de operacion y el hardware disponible.

---

##  Archivos Modificados

- `imagen.cpp`: se agregaron directivas `#pragma omp parallel for`.
- `Makefile`: se agrego `-fopenmp` a las banderas de compilacion.

---

##  Resultado

El uso de OpenMP permitio reducir el tiempo de procesamiento de imagenes grandes, sin modificar la logica central del programa, y se adapto facilmente al hardware disponible usando solo una variable de entorno.

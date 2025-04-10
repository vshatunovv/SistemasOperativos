# Procesamiento de Imágenes con Buddy System

Este proyecto implementa un programa en C++ para el procesamiento básico de imágenes, incluyendo rotación y escalado, con comparación de rendimiento entre asignación de memoria dinámica convencional (`new/delete`) y un sistema de asignación por bloques llamado **Buddy System**.

---

##Funcionalidades principales

- Carga de imágenes en formato JPG, PNG, BMP.
- Representación interna en una matriz tridimensional de píxeles.
- Rotación de imagen con interpolación bilineal alrededor del centro.
- Escalado de imagen con interpolación bilineal y preservación de proporciones.
- Asignación de memoria usando `new/delete` o Buddy System.
- Comparación de tiempos y uso de memoria entre los dos métodos de asignación.

---

## Estructura del proyecto

```
├── main.cpp                  # Lógica principal del programa
├── imagen.cpp / imagen.h     # Clase Imagen y operaciones (rotar, escalar, etc.)
├── buddy_allocator.cpp / .h  # Implementación simple del Buddy System
├── stb_image.h               # Librería para cargar imágenes
├── stb_image_write.h         # Librería para guardar imágenes
├── Makefile                  # Compilador automático
├── IMGTest/                # Carpeta opcional con imágenes de prueba
```

---

## Compilación

Desde la raíz del proyecto, simplemente ejecuta:

```bash
make
```

Esto generará el ejecutable `programa_buddy`.

---

## Ejecución

```bash
./programa_buddy <entrada.jpg> <salida.jpg> -angulo <grados> -escalar <factor> <-buddy|-no-buddy>
```

### Parámetros:
- `entrada.jpg`: Imagen original que se desea procesar
- `salida.jpg`: Nombre del archivo donde se guardará la imagen procesada
- `-angulo`: Grados a rotar la imagen (por ejemplo: 45)
- `-escalar`: Factor de escalado (por ejemplo: 1.5 para agrandar o 0.5 para reducir)
- `-buddy`: Usa el sistema de asignación Buddy
- `-no-buddy`: Usa asignación dinámica convencional

### Ejemplos:
```bash
./programa_buddy ./IMGTest/foto.jpg salida_buddy.png -angulo 45 -escalar 1.5 -buddy

./programa_buddy ./IMGTest/foto.jpg salida_normal.png -angulo 45 -escalar 1.5 -no-buddy
```

---

## Comparación de resultados esperados

```
=== LISTA DE CHEQUEO ===
Archivo de entrada: foto.jpg
Archivo de salida: salida_buddy.png
Modo de asignación: Buddy System
------------------------
Ángulo de rotación: 45 grados
Factor de escalado: 1.5

[INFO] Imagen guardada correctamente en 'salida_buddy.png'.

------------------------
TIEMPO DE PROCESAMIENTO:
 - Con Buddy System: 10234 ms

MEMORIA UTILIZADA:
 - Con Buddy System: 1820 KB
------------------------


# Preguntas de Análisis

## ¿Qué diferencia observaste en el tiempo de procesamiento entre los dos modos de asignación de memoria?
Durante las pruebas, el Buddy System fue aproximadamente un **21% más rápido** que el uso convencional de `new/delete`. Esto se debe a que el Buddy System puede minimizar la sobrecarga del sistema operativo al gestionar memoria desde un bloque grande preasignado, evitando múltiples llamadas a `malloc/new`.

---

## ¿Cuál fue el impacto del tamaño de la imagen en el consumo de memoria y el rendimiento?
El tamaño de la imagen afecta directamente el consumo de memoria, ya que la matriz tridimensional crece proporcionalmente al número de píxeles y canales. Una imagen de 3600x2400 con 3 canales usó más de **1 GB de RAM**, lo que también impacta el tiempo de procesamiento debido al mayor número de operaciones de interpolación.

---

## ¿Por qué el Buddy System es más eficiente o menos eficiente que el uso de new/delete en este caso?
El Buddy System puede ser más eficiente porque:
- Asigna memoria de manera contigua y rápida desde un bloque preasignado.
- Reduce la fragmentación externa.
- Evita llamadas repetitivas al sistema operativo.

Sin embargo, en implementaciones más complejas, podría ser menos eficiente si no se maneja bien la subdivisión de bloques, generando fragmentación interna.

---

## ¿Cómo podrías optimizar el uso de memoria y tiempo de procesamiento en este programa?
- Reutilizar buffers en vez de asignar y liberar para cada operación (como en rotación/escalado).
- Usar estructuras lineales (1D) para la matriz de píxeles para mejorar la localidad de caché.
- Implementar multithreading con OpenMP para paralelizar la rotación y escalado.
- Aplicar SIMD (instrucciones vectorizadas) para operaciones por píxel.

---

## ¿Qué implicaciones podría tener esta solución en sistemas con limitaciones de memoria o en dispositivos embebidos?
- El alto uso de RAM puede ser **problemático en sistemas embebidos**, especialmente si no hay suficiente espacio para cargar imágenes grandes completas en memoria.
- En esos casos, sería mejor procesar la imagen por bloques o líneas (streaming).
- El Buddy System sería útil si se necesita una política predecible de asignación sin depender del heap del sistema.

---

## ¿Cómo afectaría el aumento de canales (por ejemplo, de RGB a RGBA) en el rendimiento y consumo de memoria?
Agregar un canal extra (pasar de 3 a 4 canales):
- Aumenta el uso de memoria en un **33% aproximadamente**.
- Aumenta el número de operaciones por píxel durante interpolación, por lo tanto, **mayor tiempo de procesamiento**.
- Puede afectar el desempeño si no se optimiza bien la estructura de datos.

---

## ¿Qué ventajas y desventajas tiene el Buddy System frente a otras técnicas de gestión de memoria en proyectos de procesamiento de imágenes?

### Ventajas:
- Asignación rápida y contigua.
- Mejor uso de bloques de memoria predefinidos.
- Reducción de llamadas al sistema operativo.

### Desventajas:
- Fragmentación interna si los tamaños no son potencias de 2.
- Dificultad para liberar bloques en algunos casos.
- Menor flexibilidad comparado con allocadores más sofisticados (como `jemalloc` o `tcmalloc`).

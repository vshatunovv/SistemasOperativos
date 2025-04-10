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

# Sistema de Backup Seguro con Compresion y Encriptacion (C++ + OpenMP)

Este proyecto es un sistema de respaldo (backup) desarrollado en **C++** que permite:

* Escanear carpetas y subcarpetas.
* Comprimir archivos usando **GZIP**.
* Encriptar los archivos comprimidos con **AES-256 (OpenSSL)**.
* Restaurar archivos: desencriptar y descomprimir.
* Ejecutarse en paralelo utilizando **OpenMP** para alto rendimiento.

---

## Requisitos

Instala las dependencias necesarias en Ubuntu/WSL:

```bash
sudo apt update
sudo apt install build-essential libssl-dev zlib1g-dev
```

---

## Compilacion

Con el archivo `Makefile` incluido, puedes compilar con:

```bash
make
```

Esto genera el ejecutable `backup`.

Para limpiar los archivos compilados:

```bash
make clean
```

---

## Ejecucion del backup

```bash
./backup <ruta_a_carpeta>
```

Ejemplo:

```bash
./backup /home/vlad/documentos/proyecto
```

---

## Paso a paso del funcionamiento

1. **Exploracion**:

   * El programa explora la carpeta indicada de forma recursiva.
   * Lista todos los archivos encontrados.

2. **Compresion**:

   * Cada archivo se comprime en formato `.gz` usando `zlib`.
   * Los archivos comprimidos se guardan en `backup_out/`.

3. **Encriptacion (opcional)**:

   * Si el usuario lo desea, cada archivo `.gz` es encriptado con AES-256 (modo CBC).
   * La clave se deriva desde una contrasena ingresada.
   * Los archivos `.gz.enc` se guardan en `backup_enc/`.

4. **Restauracion (opcional)**:

   * Se desencriptan los archivos `.enc` usando la misma contrasena.
   * Los archivos resultantes `.gz` se almacenan en `restored_gz/`.
   * Luego se descomprimen a su forma original en `restored_final/`.

---

## Arquitectura del sistema

El sistema esta compuesto por modulos independientes:

* `main.cpp`: coordina todo el flujo del sistema.
* `gestor_archivos`: se encarga de escanear carpetas recursivamente.
* `compresor`: comprime archivos en paralelo usando `zlib`.
* `encriptador`: encripta archivos `.gz` usando `OpenSSL`.
* `restaurador`: desencripta `.enc` y descomprime `.gz`.
* `utils`: funciones auxiliares para derivar claves (AES-256).

Cada modulo es reutilizable y mantiene responsabilidades bien definidas.

---

## Paralelismo con OpenMP

Este sistema aplica paralelismo con `OpenMP` en las etapas de:

* Compresion:

```cpp
#pragma omp parallel for
for (int i = 0; i < archivos.size(); i++) {
    comprimir_archivo(archivos[i], carpeta);
}
```

* Encriptacion:

```cpp
#pragma omp parallel for
for (int i = 0; i < archivos_gz.size(); i++) {
    encriptar_archivo(...);
}
```

* Restauracion:

```cpp
#pragma omp parallel for
for (int i = 0; i < archivos_enc.size(); i++) {
    desencriptar_archivo(...);
}
```

Se aprovechan multiples nucleos del procesador, acelerando el proceso especialmente con grandes volumenes de archivos.

---

## Algoritmo de compresion y biblioteca utilizada

* Algoritmo: GZIP (deflate)
* Biblioteca: `zlib`
* Se usa lectura por bloques (`ifstream`) y escritura con `gzwrite()`

---

## Encriptacion y biblioteca utilizada

* Algoritmo: AES-256 en modo CBC
* Biblioteca: `OpenSSL`
* Se deriva clave e IV usando SHA-256 sobre la contrasena del usuario
* Se usa `EVP_EncryptInit_ex`, `EVP_EncryptUpdate`, `EVP_EncryptFinal_ex`

---

## Justificacion del lenguaje y decisiones de diseno

* **Lenguaje**: C++

  * Permite control de bajo nivel sobre entrada/salida de archivos.
  * Tiene buena integracion con librerias como zlib y OpenSSL.
  * Excelente soporte para paralelismo con OpenMP.

* **OpenMP**:

  * Facil de integrar en bucles paralelizables.
  * Requiere unicamente una directiva `#pragma omp` para escalar a multiples nucleos.

* **Diseño modular**:

  * Separar en modulos facilita mantenibilidad, pruebas e integracion.
  * Permite mejorar o cambiar componentes (ej. reemplazar zlib o cambiar algoritmo de encriptacion) sin afectar el resto del sistema.

---

## Carpetas generadas

| Carpeta           | Contenido                                 |
| ----------------- | ----------------------------------------- |
| `backup_out/`     | Archivos `.gz` comprimidos                |
| `backup_enc/`     | Archivos `.gz.enc` encriptados            |
| `restored_gz/`    | Archivos `.gz` desencriptados             |
| `restored_final/` | Archivos finales restaurados (.txt, etc.) |

---

## Acceso a los archivos restaurados

Los archivos originales restaurados se encuentran en la carpeta:

```
restored_final/
```

Puedes abrirlos con cualquier programa como si nunca hubieran sido comprimidos ni encriptados.

---

## Tecnologias usadas

* **C++17**
* **OpenMP** para paralelismo
* **zlib** para compresion
* **OpenSSL** para encriptacion AES-256
* **std::filesystem** para recorrer carpetas

---

## Seguridad

* La encriptacion se realiza con AES-256.
* La clave se deriva mediante SHA-256 desde la contrasena ingresada.
* Sin la contrasena correcta, los archivos `.enc` no pueden ser restaurados.

---

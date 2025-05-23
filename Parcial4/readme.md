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

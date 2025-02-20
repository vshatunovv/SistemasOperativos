#ifndef MANEJADOR_ARCHIVO_H
#define MANEJADOR_ARCHIVO_H

#include <iostream>
#include <vector>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <bitset>
#include <cstdint>

// Clase que administra la lectura, escritura y encriptacion de archivos.
// La implementacion permite procesar el archivo a nivel de bytes.
// Ademas, se incluye una funcion que determina si el archivo es de texto o binario.
class ManejadorArchivo
{
private:
    std::vector<uint8_t> datos;    // Almacena el contenido del archivo en bytes.
    std::vector<std::string> bits; // Almacena la representacion en bits de cada byte.

public:
    // Lee el contenido de un archivo y lo almacena en 'datos'.
    bool leerArchivo(const std::string &nombreArchivo);

    // Convierte el contenido de 'datos' en su representacion binaria.
    void convertirABits();

    // Escribe el contenido de 'datos' en un archivo de salida.
    void escribirArchivo(const std::string &nombreSalida);

    // Muestra en consola la representacion binaria del contenido.
    void mostrarBits() const;

    // Genera una matriz de bits donde cada fila representa un byte.
    std::vector<std::vector<int>> matrizBits();

    // Aplica encriptacion o desencriptacion mediante XOR con una clave fija.
    void encriptarDatos();

    // Determina si el archivo es de texto o binario.
    bool esArchivoTexto() const;
};

#endif

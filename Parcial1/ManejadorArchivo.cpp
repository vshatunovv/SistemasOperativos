#include "ManejadorArchivo.h"
#include <bitset>
#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include <iostream>
#include <vector>
#include <string>

// Funcion: leerArchivo
// Lee el archivo en modo binario y almacena su contenido en el vector 'datos'
bool ManejadorArchivo::leerArchivo(const std::string &nombreArchivo)
{
    int fd = open(nombreArchivo.c_str(), O_RDONLY);
    if (fd == -1)
    {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return false;
    }
    datos.clear();
    uint8_t buffer[1024];
    ssize_t bytesLeidos;
    while ((bytesLeidos = read(fd, buffer, sizeof(buffer))) > 0)
    {
        datos.insert(datos.end(), buffer, buffer + bytesLeidos);
    }
    close(fd);
    return true;
}

// Funcion: convertirABits
// Convierte cada byte de 'datos' a su representacion en bits y lo almacena en 'bits'
void ManejadorArchivo::convertirABits()
{
    bits.clear();
    for (uint8_t byte : datos)
    {
        bits.push_back(std::bitset<8>(byte).to_string());
    }
}

// Funcion: mostrarBits
// Muestra en pantalla la representacion en bits de cada byte almacenado
void ManejadorArchivo::mostrarBits() const
{
    std::cout << "Contenido en bits:\n[";
    for (const auto &bitString : bits)
    {
        std::cout << bitString << "]\n[";
    }
    std::cout << "]";
    std::cout << std::endl;
}

// Funcion: escribirArchivo
// Escribe el contenido del vector 'datos' en el archivo de salida
void ManejadorArchivo::escribirArchivo(const std::string &nombreSalida)
{
    int fd = open(nombreSalida.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        std::cerr << "Error: No se pudo abrir el archivo de salida " << nombreSalida << std::endl;
        return;
    }
    ssize_t bytesEscritos = write(fd, datos.data(), datos.size());
    if (bytesEscritos == -1)
    {
        std::cerr << "Error: No se pudo escribir en el archivo " << nombreSalida << std::endl;
    }
    else
    {
        std::cout << "Archivo guardado exitosamente: " << nombreSalida << std::endl;
    }
    close(fd);
}

// Funcion: matrizBits
// Convierte cada byte de 'datos' en una fila de 8 bits y retorna una matriz de bits
std::vector<std::vector<int>> ManejadorArchivo::matrizBits()
{
    std::vector<std::vector<int>> matriz;
    for (uint8_t byte : datos)
    {
        std::vector<int> filaBits(8);
        for (int i = 7; i >= 0; --i)
        {
            filaBits[7 - i] = (byte >> i) & 1;
        }
        matriz.push_back(filaBits);
    }
    return matriz;
}

// Funcion: encriptarDatos
// Encripta o desencripta el contenido de 'datos' aplicando la operacion XOR con una clave fija
// Debido a la simetria de XOR la misma funcion se utiliza para ambas operaciones
void ManejadorArchivo::encriptarDatos()
{
    uint8_t key = 0xAA;
    for (size_t i = 0; i < datos.size(); i++)
    {
        datos[i] ^= key;
    }
}

// Funcion: esArchivoTexto
// Determina si el archivo es de texto recorriendo cada byte del vector 'datos'
// Se considera que el archivo es de texto si todos los bytes son:
// tabulador (9) o salto de linea (10) o retorno de carro (13) o estan en el rango de 32 a 126 (caracteres imprimibles)
// Si se encuentra algun byte fuera de estos rangos se considera que el archivo es binario
bool ManejadorArchivo::esArchivoTexto() const
{
    for (uint8_t byte : datos)
    {
        if (!(byte == 9 || byte == 10 || byte == 13 || (byte >= 32 && byte <= 126)))
            return false;
    }
    return true;
}

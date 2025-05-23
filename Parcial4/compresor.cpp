#include "compresor.hpp"
#include <zlib.h>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <omp.h>

namespace fs = std::filesystem;

// Comprime un solo archivo y lo guarda en .gz
bool comprimir_archivo(const std::string &archivo_entrada, const std::string &carpeta_salida)
{
    std::ifstream entrada(archivo_entrada, std::ios_base::binary);
    if (!entrada)
    {
        std::cerr << "No se pudo abrir: " << archivo_entrada << "\n";
        return false;
    }

    fs::create_directories(carpeta_salida);
    std::string nombre_archivo = fs::path(archivo_entrada).filename().string();
    std::string salida_path = carpeta_salida + "/" + nombre_archivo + ".gz";

    gzFile salida = gzopen(salida_path.c_str(), "wb");
    if (!salida)
    {
        std::cerr << "No se pudo crear: " << salida_path << "\n";
        return false;
    }

    char buffer[4096];
    while (entrada.read(buffer, sizeof(buffer)))
    {
        gzwrite(salida, buffer, entrada.gcount());
    }
    if (entrada.gcount() > 0)
    {
        gzwrite(salida, buffer, entrada.gcount());
    }

    entrada.close();
    gzclose(salida);
    return true;
}

// Funcion principal que paraleliza la compresion de todos los archivos
void comprimir_archivos(const std::vector<std::string> &archivos, const std::string &carpeta_salida)
{
#pragma omp parallel for
    for (int i = 0; i < archivos.size(); i++)
    {
        const std::string &archivo = archivos[i];
        if (comprimir_archivo(archivo, carpeta_salida))
        {
#pragma omp critical
            std::cout << "[OK] " << archivo << " comprimido correctamente.\n";
        }
        else
        {
#pragma omp critical
            std::cout << "[ERROR] Fallo al comprimir " << archivo << "\n";
        }
    }
}

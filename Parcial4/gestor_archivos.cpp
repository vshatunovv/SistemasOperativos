#include "gestor_archivos.hpp"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

std::vector<std::string> obtener_archivos(const std::string &ruta)
{
    std::vector<std::string> archivos;
    try
    {
        for (const auto &entry : fs::recursive_directory_iterator(ruta))
        {
            if (entry.is_regular_file())
            {
                archivos.push_back(entry.path().string());
            }
        }
    }
    catch (const fs::filesystem_error &e)
    {
        std::cerr << "Error al explorar carpeta: " << e.what() << std::endl;
    }
    return archivos;
}

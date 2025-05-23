#pragma once
#include <string>
#include <vector>

// Funcion que devuelve una lista con rutas completas de todos los archivos dentro de una carpeta (recursivo)
std::vector<std::string> obtener_archivos(const std::string &ruta);

#pragma once
#include <string>
#include <vector>

// Encripta los archivos .gz generados y los guarda con extension .enc
void encriptar_archivos(const std::vector<std::string> &archivos_gz, const std::string &password, const std::string &carpeta_salida);

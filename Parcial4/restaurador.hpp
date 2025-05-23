#pragma once
#include <string>
#include <vector>

// Desencripta archivos .enc a .gz
void desencriptar_archivos(const std::vector<std::string> &archivos_enc, const std::string &password, const std::string &carpeta_salida);

// Descomprime archivos .gz a sus versiones originales
void descomprimir_archivos(const std::vector<std::string> &archivos_gz, const std::string &carpeta_salida);

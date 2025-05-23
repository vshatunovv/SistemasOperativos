#include "encriptador.hpp"
#include "utils.hpp"
#include <openssl/evp.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <omp.h>
#include <cstring> // Para memcpy

namespace fs = std::filesystem;

// Funcion que encripta un archivo individual usando AES-256-CBC
bool encriptar_archivo(const std::string &entrada_path, const std::string &salida_path, const std::string &password)
{
    // Abrir archivo de entrada (original comprimido .gz)
    std::ifstream entrada(entrada_path, std::ios::binary);
    // Crear archivo de salida (encriptado .gz.enc)
    std::ofstream salida(salida_path, std::ios::binary);
    if (!entrada || !salida)
    {
        std::cerr << "[ERROR] No se pudo abrir archivo para encriptar: " << entrada_path << "\n";
        return false;
    }

    // Generar clave y vector de inicializacion desde la clave del usuario
    unsigned char key[32], iv[16];
    derivar_clave_iv(password, key, iv);

    // Crear y configurar el contexto de encriptacion
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv);

    // Buffers para lectura y escritura
    const size_t buffer_size = 4096;
    unsigned char buffer_in[buffer_size];
    unsigned char buffer_out[buffer_size + EVP_MAX_BLOCK_LENGTH];
    int out_len;

    // Leer el archivo de entrada por bloques y encriptar cada bloque
    while (entrada.read(reinterpret_cast<char *>(buffer_in), buffer_size))
    {
        int in_len = entrada.gcount();
        EVP_EncryptUpdate(ctx, buffer_out, &out_len, buffer_in, in_len);
        salida.write(reinterpret_cast<char *>(buffer_out), out_len);
    }

    // Finalizar el proceso de encriptacion
    int final_len;
    EVP_EncryptFinal_ex(ctx, buffer_out, &final_len);
    salida.write(reinterpret_cast<char *>(buffer_out), final_len);

    // Liberar recursos
    EVP_CIPHER_CTX_free(ctx);
    entrada.close();
    salida.close();
    return true;
}

// Funcion que encripta todos los archivos .gz usando OpenMP
void encriptar_archivos(const std::vector<std::string> &archivos_gz, const std::string &password, const std::string &carpeta_salida)
{
    // Crear carpeta de salida si no existe
    fs::create_directories(carpeta_salida);

// Paralelizar la encriptacion de archivos
#pragma omp parallel for
    for (int i = 0; i < archivos_gz.size(); i++)
    {
        std::string entrada = archivos_gz[i];
        std::string nombre = fs::path(entrada).filename().string() + ".enc";
        std::string salida = carpeta_salida + "/" + nombre;

        if (encriptar_archivo(entrada, salida, password))
        {
#pragma omp critical
            std::cout << "[OK] Encriptado: " << entrada << " → " << salida << "\n";
        }
        else
        {
#pragma omp critical
            std::cerr << "[ERROR] Fallo en la encriptacion: " << entrada << "\n";
        }
    }
}

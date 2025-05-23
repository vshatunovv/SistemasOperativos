#include "restaurador.hpp"
#include "utils.hpp"
#include <openssl/evp.h>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <omp.h>
#include <zlib.h>
#include <cstring>

namespace fs = std::filesystem;

// Funcion que desencripta un archivo .enc y lo convierte en .gz
bool desencriptar_archivo(const std::string &entrada_path, const std::string &salida_path, const std::string &password)
{
    std::ifstream entrada(entrada_path, std::ios::binary);
    std::ofstream salida(salida_path, std::ios::binary);
    if (!entrada || !salida)
        return false;

    // Obtener clave e IV desde la clave ingresada por el usuario
    unsigned char key[32], iv[16];
    derivar_clave_iv(password, key, iv);

    // Inicializar contexto de desencriptacion
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv);

    // Buffers
    const size_t buffer_size = 4096;
    unsigned char buffer_in[buffer_size];
    unsigned char buffer_out[buffer_size + EVP_MAX_BLOCK_LENGTH];
    int out_len;

    // Leer bloques cifrados y desencriptarlos
    while (entrada.read(reinterpret_cast<char *>(buffer_in), buffer_size))
    {
        int in_len = entrada.gcount();
        EVP_DecryptUpdate(ctx, buffer_out, &out_len, buffer_in, in_len);
        salida.write(reinterpret_cast<char *>(buffer_out), out_len);
    }

    // Finalizar desencriptacion
    int final_len;
    EVP_DecryptFinal_ex(ctx, buffer_out, &final_len);
    salida.write(reinterpret_cast<char *>(buffer_out), final_len);

    EVP_CIPHER_CTX_free(ctx);
    entrada.close();
    salida.close();
    return true;
}

// Desencripta todos los archivos .enc usando OpenMP
void desencriptar_archivos(const std::vector<std::string> &archivos_enc, const std::string &password, const std::string &carpeta_salida)
{
    fs::create_directories(carpeta_salida);
#pragma omp parallel for
    for (int i = 0; i < archivos_enc.size(); i++)
    {
        const std::string &entrada = archivos_enc[i];
        std::string nombre = fs::path(entrada).filename().stem().string(); // quitar extension .enc
        std::string salida = carpeta_salida + "/" + nombre;
        if (desencriptar_archivo(entrada, salida, password))
        {
#pragma omp critical
            std::cout << "[OK] Desencriptado: " << entrada << " -> " << salida << "\n";
        }
        else
        {
#pragma omp critical
            std::cerr << "[ERROR] Fallo al desencriptar " << entrada << "\n";
        }
    }
}

// Funcion que descomprime un archivo .gz
bool descomprimir_archivo(const std::string &entrada_path, const std::string &salida_path)
{
    gzFile entrada = gzopen(entrada_path.c_str(), "rb");
    std::ofstream salida(salida_path, std::ios::binary);
    if (!entrada || !salida)
        return false;

    char buffer[4096];
    int leidos;
    while ((leidos = gzread(entrada, buffer, sizeof(buffer))) > 0)
    {
        salida.write(buffer, leidos);
    }

    gzclose(entrada);
    salida.close();
    return true;
}

// Descomprime todos los archivos .gz en paralelo
void descomprimir_archivos(const std::vector<std::string> &archivos_gz, const std::string &carpeta_salida)
{
    fs::create_directories(carpeta_salida);
#pragma omp parallel for
    for (int i = 0; i < archivos_gz.size(); i++)
    {
        const std::string &entrada = archivos_gz[i];
        std::string nombre = fs::path(entrada).filename().stem().string(); // quitar .gz
        std::string salida = carpeta_salida + "/" + nombre;
        if (descomprimir_archivo(entrada, salida))
        {
#pragma omp critical
            std::cout << "[OK] Descomprimido: " << entrada << " -> " << salida << "\n";
        }
        else
        {
#pragma omp critical
            std::cerr << "[ERROR] Fallo al descomprimir " << entrada << "\n";
        }
    }
}

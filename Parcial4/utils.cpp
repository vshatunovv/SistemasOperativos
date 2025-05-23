#include "utils.hpp"
#include <openssl/sha.h>
#include <cstring> // Para memcpy

// Funcion que convierte una contraseña en una clave (key) y vector de inicializacion (iv)
// Se usa SHA-256 para generar un hash a partir del texto ingresado por el usuario
// Los primeros 32 bytes del hash se usan como clave, y los ultimos 16 como IV
void derivar_clave_iv(const std::string &password, unsigned char *key, unsigned char *iv)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];

    // Calcular el hash SHA-256 de la contraseña
    SHA256(reinterpret_cast<const unsigned char *>(password.c_str()), password.size(), hash);

    // Copiar los primeros 32 bytes del hash como la clave AES-256
    memcpy(key, hash, 32);

    // Usar los ultimos 16 bytes del hash como el IV (vector de inicializacion)
    memcpy(iv, hash + 16, 16);
}

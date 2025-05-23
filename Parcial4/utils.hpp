#pragma once
#include <string>

// Deriva una clave e IV desde una contraseña usando SHA-256
void derivar_clave_iv(const std::string &password, unsigned char *key, unsigned char *iv);

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

// Inclusiones de los modulos del proyecto
#include "gestor_archivos.hpp"
#include "compresor.hpp"
#include "encriptador.hpp"
#include "restaurador.hpp"

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    // Verifica que se haya pasado una ruta por linea de comando
    if (argc != 2)
    {
        std::cerr << "Uso: " << argv[0] << " <ruta_de_carpeta_a_respaladar>\n";
        return 1;
    }

    // Obtener la ruta desde los argumentos del usuario
    std::string ruta = argv[1];

    // Buscar todos los archivos dentro de la carpeta y subcarpetas
    std::vector<std::string> archivos = obtener_archivos(ruta);

    // Validar si se encontraron archivos
    if (archivos.empty())
    {
        std::cout << "No se encontraron archivos en la ruta especificada.\n";
        return 1;
    }

    // Iniciar proceso de compresion
    std::cout << "Iniciando compresion de " << archivos.size() << " archivo(s)...\n";
    comprimir_archivos(archivos, "backup_out");

    // Crear lista de rutas a los archivos comprimidos (.gz)
    std::vector<std::string> archivos_gz;
    for (const auto &path : archivos)
    {
        std::string nombre = fs::path(path).filename().string();
        archivos_gz.push_back("backup_out/" + nombre + ".gz");
    }

    // Preguntar si el usuario desea encriptar los archivos comprimidos
    std::string opcion;
    std::cout << "\n¿Deseas encriptar los archivos comprimidos? (s/n): ";
    std::cin >> opcion;

    if (opcion == "s" || opcion == "S")
    {
        std::string clave;
        std::cout << "Ingresa una clave de encriptacion: ";
        std::cin >> clave;

        // Encriptar todos los archivos .gz
        encriptar_archivos(archivos_gz, clave, "backup_enc");

        std::cout << "\nArchivos encriptados guardados en 'backup_enc/'\n";
    }

    // Preguntar si se desea restaurar los archivos en este momento
    std::cout << "\n¿Deseas restaurar los archivos ahora? (s/n): ";
    std::cin >> opcion;

    if (opcion == "s" || opcion == "S")
    {
        // Buscar archivos encriptados en la carpeta backup_enc
        std::vector<std::string> archivos_enc;
        for (const auto &file : fs::directory_iterator("backup_enc"))
        {
            if (file.path().extension() == ".enc")
                archivos_enc.push_back(file.path().string());
        }

        std::string clave;
        std::cout << "Ingresa la clave de encriptacion para restaurar: ";
        std::cin >> clave;

        // Desencriptar archivos .enc → .gz
        desencriptar_archivos(archivos_enc, clave, "restored_gz");

        // Buscar archivos .gz desencriptados
        std::vector<std::string> archivos_restaurados_gz;
        for (const auto &file : fs::directory_iterator("restored_gz"))
        {
            if (file.path().extension() == ".gz")
                archivos_restaurados_gz.push_back(file.path().string());
        }

        // Descomprimir archivos .gz → archivos originales
        descomprimir_archivos(archivos_restaurados_gz, "restored_final");

        std::cout << "\nArchivos restaurados correctamente en 'restored_final/'\n";
    }

    // Final del proceso completo
    std::cout << "\nProceso completado.\n";
    return 0;
}

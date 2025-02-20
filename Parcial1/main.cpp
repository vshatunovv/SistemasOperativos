#include "ManejadorArchivo.h"
#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    // Se debe proporcionar al menos una bandera
    if (argc < 2)
    {
        std::cerr << "Uso: " << argv[0] << " <flag> <archivo_entrada> [archivo_salida]\n";
        std::cerr << "Banderas disponibles:\n"
                  << "  -h, --help         Muestra ayuda\n"
                  << "  -v, --version      Muestra la version\n"
                  << "  -e, --encrypt      Encripta el archivo\n"
                  << "  -d, --decrypt      Desencripta el archivo\n";
        return 1;
    }

    std::string flag = argv[1];

    // Procesar banderas que no requieren archivo de entrada
    if (flag == "-h" || flag == "--help")
    {
        std::cout << "Uso: " << argv[0] << " <flag> <archivo_entrada> [archivo_salida]\n";
        std::cout << "Opciones:\n"
                  << "  -h, --help         Muestra esta ayuda\n"
                  << "  -v, --version      Muestra la version del programa\n"
                  << "  -e, --encrypt      Encripta el archivo indicado\n"
                  << "  -d, --decrypt      Desencripta el archivo indicado\n";
        return 0;
    }
    else if (flag == "-v" || flag == "--version")
    {
        std::cout << "Version 1.0\n";
        return 0;
    }

    // Para encriptar o desencriptar se requiere el archivo de entrada
    if (argc < 3)
    {
        std::cerr << "Error: Falta el archivo de entrada\n";
        std::cerr << "Uso: " << argv[0] << " <flag> <archivo_entrada> [archivo_salida]\n";
        return 1;
    }

    std::string archivoEntrada = argv[2];
    std::string archivoSalida = (argc > 3) ? argv[3] : "salida.bin";

    ManejadorArchivo manejador;

    if (flag == "-e" || flag == "--encrypt")
    {
        if (!manejador.leerArchivo(archivoEntrada))
            return 1;

        // Se indica si el archivo es de texto o binario
        if (manejador.esArchivoTexto())
            std::cout << "El archivo es de texto" << std::endl;
        else
            std::cout << "El archivo es binario" << std::endl;

        // Encriptar los datos aplicando XOR con clave fija
        manejador.encriptarDatos();
        manejador.escribirArchivo(archivoSalida);
        std::cout << "Archivo encriptado exitosamente: " << archivoSalida << std::endl;
    }
    else if (flag == "-d" || flag == "--decrypt")
    {
        if (!manejador.leerArchivo(archivoEntrada))
            return 1;

        // Se indica si el archivo es de texto o binario
        if (manejador.esArchivoTexto())
            std::cout << "El archivo es de texto" << std::endl;
        else
            std::cout << "El archivo es binario" << std::endl;

        // Desencriptar los datos aplicando la operacion XOR (simetrica)
        manejador.encriptarDatos();
        manejador.escribirArchivo(archivoSalida);
        std::cout << "Archivo desencriptado exitosamente: " << archivoSalida << std::endl;
    }
    else
    {
        std::cerr << "Opcion no reconocida: " << flag << "\nUse -h o --help para ver las opciones.\n";
        return 1;
    }

    return 0;
}

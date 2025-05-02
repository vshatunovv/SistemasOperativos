// main.cpp
#include "imagen.h"
#include "buddy_allocator.h"
#include <iostream>
#include <chrono>
#include <cstring>
#include <sys/resource.h>

using namespace std;
using namespace std::chrono;

void mostrarUso()
{
    cout << "Uso: ./programa_buddy <entrada.jpg> <salida.jpg> -angulo <grados> -escalar <factor> <-buddy|-no-buddy>" << endl;
}

void mostrarListaChequeo(const string &archivoEntrada, const string &archivoSalida, bool usarBuddy)
{
    cout << "\n=== LISTA DE CHEQUEO ===" << endl;
    cout << "Archivo de entrada: " << archivoEntrada << endl;
    cout << "Archivo de salida: " << archivoSalida << endl;
    cout << "Modo de asignación: " << (usarBuddy ? "Buddy System" : "new/delete") << endl;
    cout << "------------------------" << endl;
}

long getUsoMemoriaKB()
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;
}

int main(int argc, char *argv[])
{
    if (argc != 8)
    {
        cerr << "Error: Número incorrecto de argumentos.\n";
        mostrarUso();
        return 1;
    }

    string archivoEntrada = argv[1];
    string archivoSalida = argv[2];

    if (string(argv[3]) != "-angulo")
    {
        cerr << "Error: Se esperaba '-angulo' como tercer argumento.\n";
        mostrarUso();
        return 1;
    }
    float angulo = stof(argv[4]);

    if (string(argv[5]) != "-escalar")
    {
        cerr << "Error: Se esperaba '-escalar' como quinto argumento.\n";
        mostrarUso();
        return 1;
    }
    float factorEscalado = stof(argv[6]);

    bool usarBuddy = false;
    if (string(argv[7]) == "-buddy")
        usarBuddy = true;
    else if (string(argv[7]) == "-no-buddy")
        usarBuddy = false;
    else
    {
        cerr << "Error: Opción de modo inválida.\n";
        mostrarUso();
        return 1;
    }

    mostrarListaChequeo(archivoEntrada, archivoSalida, usarBuddy);
    cout << "Ángulo de rotación: " << angulo << " grados\n";
    cout << "Factor de escalado: " << factorEscalado << "\n";

    auto inicio = high_resolution_clock::now();
    long memoriaAntes = getUsoMemoriaKB();

    if (usarBuddy)
    {
        cout << "\n[INFO] Usando Buddy System para la asignación de memoria.\n";
        BuddyAllocator allocador(32 * 1024 * 1024);
        Imagen img(archivoEntrada, &allocador);
        img.mostrarInfo();
        img.rotarImagen(angulo);
        img.escalarImagen(factorEscalado);
        img.guardarImagen(archivoSalida);
    }
    else
    {
        cout << "\n[INFO] Usando new/delete para la asignación de memoria.\n";
        Imagen img(archivoEntrada);
        img.mostrarInfo();
        img.rotarImagen(angulo);
        img.escalarImagen(factorEscalado);
        img.guardarImagen(archivoSalida);
    }

    auto fin = high_resolution_clock::now();
    long memoriaDespues = getUsoMemoriaKB();
    auto duracion = duration_cast<milliseconds>(fin - inicio).count();

    cout << "\n------------------------\n";
    cout << "TIEMPO DE PROCESAMIENTO:\n";
    cout << " - " << (usarBuddy ? "Con Buddy System" : "Sin Buddy System") << ": " << duracion << " ms\n";
    cout << "\nMEMORIA UTILIZADA:\n";
    cout << " - " << (usarBuddy ? "Con Buddy System" : "Sin Buddy System") << ": " << (memoriaDespues - memoriaAntes) << " KB\n";
    cout << "------------------------\n";

    cout << "\n[INFO] Proceso completado con éxito.\n";

    return 0;
}

// imagen.cpp
#include "imagen.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>
#include <cmath>

using namespace std;

Imagen::Imagen(const std::string &nombreArchivo, BuddyAllocator *allocador)
    : allocador(allocador)
{

    unsigned char *buffer = stbi_load(nombreArchivo.c_str(), &ancho, &alto, &canales, 0);
    if (!buffer)
    {
        cerr << "Error: No se pudo cargar la imagen '" << nombreArchivo << "'.\n";
        exit(1);
    }

    convertirBufferAMatriz(buffer);
    stbi_image_free(buffer);
}

Imagen::~Imagen()
{
    if (!allocador)
    {
        for (int y = 0; y < alto; y++)
        {
            for (int x = 0; x < ancho; x++)
            {
                delete[] pixeles[y][x];
            }
            delete[] pixeles[y];
        }
        delete[] pixeles;
    }
}

void Imagen::convertirBufferAMatriz(unsigned char *buffer)
{
    int indice = 0;
    pixeles = new unsigned char **[alto];

    for (int y = 0; y < alto; y++)
    {
        pixeles[y] = new unsigned char *[ancho];
        for (int x = 0; x < ancho; x++)
        {
            pixeles[y][x] = new unsigned char[canales];
            for (int c = 0; c < canales; c++)
            {
                pixeles[y][x][c] = buffer[indice++];
            }
        }
    }
}

void Imagen::mostrarInfo() const
{
    cout << "Dimensiones: " << ancho << " x " << alto << endl;
    cout << "Canales: " << canales << endl;
}

void Imagen::guardarImagen(const std::string &nombreArchivo) const
{
    unsigned char *buffer = new unsigned char[alto * ancho * canales];
    int indice = 0;

    for (int y = 0; y < alto; y++)
    {
        for (int x = 0; x < ancho; x++)
        {
            for (int c = 0; c < canales; c++)
            {
                buffer[indice++] = pixeles[y][x][c];
            }
        }
    }

    if (!stbi_write_png(nombreArchivo.c_str(), ancho, alto, canales, buffer, ancho * canales))
    {
        cerr << "Error: No se pudo guardar la imagen en '" << nombreArchivo << "'.\n";
        delete[] buffer;
        exit(1);
    }

    delete[] buffer;
    cout << "[INFO] Imagen guardada correctamente en '" << nombreArchivo << "'.\n";
}

void Imagen::invertirColores()
{
    for (int y = 0; y < alto; y++)
    {
        for (int x = 0; x < ancho; x++)
        {
            for (int c = 0; c < canales; c++)
            {
                pixeles[y][x][c] = 255 - pixeles[y][x][c];
            }
        }
    }
}

float bilinearInterpolate(float q11, float q12, float q21, float q22, float x, float y)
{
    return (1 - x) * (1 - y) * q11 +
           x * (1 - y) * q21 +
           (1 - x) * y * q12 +
           x * y * q22;
}

void Imagen::rotarImagen(float anguloGrados)
{
    float rad = anguloGrados * M_PI / 180.0;

    int nuevoAncho = ancho;
    int nuevoAlto = alto;

    unsigned char ***nuevaImagen = new unsigned char **[nuevoAlto];
    for (int y = 0; y < nuevoAlto; y++)
    {
        nuevaImagen[y] = new unsigned char *[nuevoAncho];
        for (int x = 0; x < nuevoAncho; x++)
        {
            nuevaImagen[y][x] = new unsigned char[canales];
            for (int c = 0; c < canales; c++)
            {
                nuevaImagen[y][x][c] = 0;
            }
        }
    }

    float cx = ancho / 2.0;
    float cy = alto / 2.0;

    for (int y = 0; y < nuevoAlto; y++)
    {
        for (int x = 0; x < nuevoAncho; x++)
        {
            float dx = x - cx;
            float dy = y - cy;

            float origenX = cos(rad) * dx + sin(rad) * dy + cx;
            float origenY = -sin(rad) * dx + cos(rad) * dy + cy;

            int x0 = floor(origenX);
            int x1 = x0 + 1;
            int y0 = floor(origenY);
            int y1 = y0 + 1;

            if (x0 >= 0 && x1 < ancho && y0 >= 0 && y1 < alto)
            {
                float dx = origenX - x0;
                float dy = origenY - y0;

                for (int c = 0; c < canales; c++)
                {
                    float p00 = pixeles[y0][x0][c];
                    float p01 = pixeles[y1][x0][c];
                    float p10 = pixeles[y0][x1][c];
                    float p11 = pixeles[y1][x1][c];

                    float interpolado = bilinearInterpolate(p00, p01, p10, p11, dx, dy);
                    nuevaImagen[y][x][c] = static_cast<unsigned char>(interpolado);
                }
            }
        }
    }

    for (int y = 0; y < alto; y++)
    {
        for (int x = 0; x < ancho; x++)
        {
            delete[] pixeles[y][x];
        }
        delete[] pixeles[y];
    }
    delete[] pixeles;

    pixeles = nuevaImagen;
}

void Imagen::escalarImagen(float factor)
{
    int nuevoAncho = static_cast<int>(ancho * factor);
    int nuevoAlto = static_cast<int>(alto * factor);

    unsigned char ***nuevaImagen = new unsigned char **[nuevoAlto];
    for (int y = 0; y < nuevoAlto; y++)
    {
        nuevaImagen[y] = new unsigned char *[nuevoAncho];
        for (int x = 0; x < nuevoAncho; x++)
        {
            nuevaImagen[y][x] = new unsigned char[canales];
        }
    }

    for (int y = 0; y < nuevoAlto; y++)
    {
        for (int x = 0; x < nuevoAncho; x++)
        {
            float srcX = x / factor;
            float srcY = y / factor;

            int x0 = floor(srcX);
            int x1 = x0 + 1;
            int y0 = floor(srcY);
            int y1 = y0 + 1;

            float dx = srcX - x0;
            float dy = srcY - y0;

            for (int c = 0; c < canales; c++)
            {
                float p00 = (x0 >= 0 && x0 < ancho && y0 >= 0 && y0 < alto) ? pixeles[y0][x0][c] : 0;
                float p10 = (x1 >= 0 && x1 < ancho && y0 >= 0 && y0 < alto) ? pixeles[y0][x1][c] : 0;
                float p01 = (x0 >= 0 && x0 < ancho && y1 >= 0 && y1 < alto) ? pixeles[y1][x0][c] : 0;
                float p11 = (x1 >= 0 && x1 < ancho && y1 >= 0 && y1 < alto) ? pixeles[y1][x1][c] : 0;

                float interpolado = bilinearInterpolate(p00, p01, p10, p11, dx, dy);
                nuevaImagen[y][x][c] = static_cast<unsigned char>(interpolado);
            }
        }
    }

    for (int y = 0; y < alto; y++)
    {
        for (int x = 0; x < ancho; x++)
        {
            delete[] pixeles[y][x];
        }
        delete[] pixeles[y];
    }
    delete[] pixeles;

    pixeles = nuevaImagen;
    ancho = nuevoAncho;
    alto = nuevoAlto;
}

#include "encriptado.h"
#include <string>
#include <openssl/evp.h>
#include <stdexcept> // std::runtime_error

// ! Códigos de error
// ! versión 2.0
// ! modificado por Aether
// ? se agregaron los códigos de error
// ! ErrCif01: Error al inicializar la librería de
// ! ErrCif02: Error al inicializar el contexto de encriptación
// ! ErrCif03: Error al encriptar el texto
// ! ErrCif04: Error al inicializar el contexto de desencriptación
// ! ErrCif05: Error al desencriptar el texto




// ! constructor
// ! versión 2.0
// ! modificado por Aether
// ? se modificó el constructor para que acepte un iv y se inicialice en "0123456789012345" ademas de mejorar la implementación
EncriptadoV2::Encriptado::Encriptado(const std::string& contraseña, const std::string& iv)
{
    this->contraseña = contraseña;
    this->iv = iv;
}


// ! encriptar (método para encriptar un texto)
// ! versión 2.0
// ! modificado por Aether
// ? se mejoró la implementación
std::string EncriptadoV2::Encriptado::encriptar(std::string texto)
{
    // se realiza un cifrado con 128 bits por lo que si la contraseña es más corta se rellena con ceros
    if (contraseña.size() < 32)
    {
        contraseña.append(32 - contraseña.size(), '\0');
    }
    // ? cadena de texto encriptado
    std::string textoEncriptado;
    // ? inicialización de la librería de cifrado
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        //return "";
        //throw std::runtime_error("Error al inicializar la librería de cifrado");
        throw std::runtime_error("ErrCif01: Error al inicializar la librería de cifrado");
    }
    // ? inicialización del contexto de encriptación
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, (const unsigned char*)contraseña.c_str(), (const unsigned char*)iv.c_str()) != 1)
    {
        // ? si no se pudo inicializar el contexto de encriptación, se libera la memoria
        EVP_CIPHER_CTX_free(ctx);
        // ? genera una excepción
        throw std::runtime_error("ErrCif02: Error al inicializar el contexto de encriptación");
    }
    // ? extraemos el tamaño del texto para reservar la memoria del buffer de salida
    int textoTam = texto.size();
    // ? reservamos la memoria para el texto encriptado
    unsigned char *textoEncriptadoBuffer = new unsigned char[textoTam + 16];
    // ? le añadimos 16 bytes adicionales por si acaso
    // creamos una variable para almacenar el tamaño del texto encriptado
    int textoEncriptadoTam = 0;
    // ? encriptamos el texto
    if (EVP_EncryptUpdate(ctx, textoEncriptadoBuffer, &textoEncriptadoTam, (const unsigned char*)texto.c_str(), textoTam) != 1)
    {
        // ? si no se pudo encriptar el texto, se libera la memoria
        delete[] textoEncriptadoBuffer;
        // ? se libera el contexto de encriptación
        EVP_CIPHER_CTX_free(ctx);
        // ? se genera una excepción
        throw std::runtime_error("ErrCif03: Error al encriptar el texto");
    }
    // ? liberamos la memoria del contexto de encriptación
    EVP_CIPHER_CTX_free(ctx);
    // ? asignamos el texto encriptado a la cadena de texto encriptado
    textoEncriptado.assign((char*)textoEncriptadoBuffer, textoEncriptadoTam);
    // ? liberamos la memoria del buffer de texto encriptado
    delete[] textoEncriptadoBuffer;
    // ? retornamos el texto encriptado
    return textoEncriptado;
}

// ! desencriptar (método para desencriptar un texto)
// ! versión 2.0
// ! modificado por Aether
// ? se mejoró la implementación
std::string EncriptadoV2::Encriptado::desencriptar(std::string texto)
{
    // se realiza un cifrado con 128 bits por lo que si la contraseña es más corta se rellena con ceros
    if (contraseña.size() < 32)
    {
        contraseña.append(32 - contraseña.size(), '\0');
    }
    // ? cadena de texto desencriptado
    std::string textoDesencriptado;
    // ? inicialización de la librería de cifrado
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        //return "";
        //throw std::runtime_error("Error al inicializar la librería de cifrado");
        throw std::runtime_error("ErrCif01: Error al inicializar la librería de cifrado");
    }
    // ? inicialización del contexto de desencriptación
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, (const unsigned char*)contraseña.c_str(), (const unsigned char*)iv.c_str()) != 1)
    {
        // ? si no se pudo inicializar el contexto de desencriptación, se libera la memoria
        EVP_CIPHER_CTX_free(ctx);
        // ? genera una excepción
        throw std::runtime_error("ErrCif04: Error al inicializar el contexto de desencriptación");
    }
    // ? extraemos el tamaño del texto para reservar la memoria del buffer de salida
    int textoTam = texto.size();
    // ? reservamos la memoria para el texto desencriptado
    unsigned char *textoDesencriptadoBuffer = new unsigned char[textoTam + 16];
    // ? le añadimos 16 bytes adicionales por si acaso
    // creamos una variable para almacenar el tamaño del texto desencriptado
    int textoDesencriptadoTam = 0;
    // ? desencriptamos el texto
    if (EVP_DecryptUpdate(ctx, textoDesencriptadoBuffer, &textoDesencriptadoTam, (const unsigned char*)texto.c_str(), textoTam) != 1)
    {
        // ? si no se pudo desencriptar el texto, se libera la memoria
        delete[] textoDesencriptadoBuffer;
        // ? se libera el contexto de desencriptación
        EVP_CIPHER_CTX_free(ctx);
        // ? se genera una excepción
        throw std::runtime_error("ErrCif05: Error al desencriptar el texto");
    }
    // ? liberamos la memoria del contexto de desencriptación
    EVP_CIPHER_CTX_free(ctx);
    // ? asignamos el texto desencriptado a la cadena de texto desencriptado
    textoDesencriptado.assign((char*)textoDesencriptadoBuffer, textoDesencriptadoTam);
    // ? liberamos la memoria del buffer de texto desencriptado
    delete[] textoDesencriptadoBuffer;
    // ? retornamos el texto desencriptado
    return textoDesencriptado;
}

// ! getContraseña (método para obtener la contraseña)
// ! versión 2.0
// ! modificado por Aether
// ? se mejoró la implementación
std::string EncriptadoV2::Encriptado::getContraseña()
{
    return contraseña;
}

// ! setContraseña (método para establecer la contraseña)
// ! versión 2.0
// ! modificado por Aether
// ? se mejoró la implementación
void EncriptadoV2::Encriptado::setContraseña(std::string contraseña)
{
    this->contraseña = contraseña;
    // ? si la contraseña es menor a 32 caracteres, se rellena con ceros
    if (this->contraseña.size() < 32)
    {
        this->contraseña.append(32 - this->contraseña.size(), '\0');
    }
}

// ! getIv (método para obtener el iv)
// ! versión 2.0
// ! modificado por Aether
// ? Primera implementación de este método
std::string EncriptadoV2::Encriptado::getIv()
{
    return iv;
}

// ! setIv (método para establecer el iv)
// ! versión 2.0
// ! modificado por Aether
// ? Primera implementación de este método
void EncriptadoV2::Encriptado::setIv(std::string iv)
{
    this->iv = iv;
}


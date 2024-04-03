#include "encriptado.h"
#include <string>
#include <openssl/evp.h>


// ! constructor
// ! versión 1.0
// ! modificado por Aether
// ? no se realizaron cambios a partir de la versión 1.0
Encriptado::Encriptado(std::string contraseña)
{
    this->contraseña = contraseña;
}

// ! método para encriptar un texto
// ! versión 1.0
// ! modificado por Aether
// ? no se realizaron cambios a partir de la versión 1.0
std::string Encriptado::encriptar(std::string texto)
{
    // ? Cadena de texto encriptada
    std::string resultado;
    // ? Inicializamos el contexto de encriptación
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        // ? Si no se pudo inicializar el contexto, se retorna una cadena vacía
        return "";
    }
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, (unsigned char *)contraseña.c_str(), NULL) != 1)
    {
        // ? Si no se pudo inicializar el contexto de encriptación, se libera la memoria y se retorna una cadena vacía
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }
    // Se realiza un cifrado con 128 bits por lo que si la contraseña es más corta se rellena con ceros
    if (contraseña.length() < 32)
    {
        contraseña += std::string(32 - contraseña.length(), '\0');
    }
    //int tam = texto.length();
    unsigned char *textoCifrado = new unsigned char[128];
    int textoCifradoLen = 0;
    // ? Buffer de salida
    if (EVP_EncryptUpdate(ctx, textoCifrado, &textoCifradoLen, (unsigned char *)texto.c_str(), texto.length()) != 1)
    {
        // ? Si no se pudo realizar el cifrado, se libera la memoria y se retorna una cadena vacía
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    // liberamos la memoria
    EVP_CIPHER_CTX_free(ctx);
    delete[] textoCifrado;

    return resultado;
}

// ! método para desencriptar un texto
// ! versión 1.0
// ! modificado por Aether
// ? no se realizaron cambios a partir de la versión 1.0
std::string Encriptado::desencriptar(std::string texto)
{
    // ? Cadena de texto desencriptada
    std::string resultado;
    // ? Inicializamos el contexto de desencriptación
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        // ? Si no se pudo inicializar el contexto, se retorna una cadena vacía
        return "";
    }
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, (unsigned char *)contraseña.c_str(), NULL) != 1)
    {
        // ? Si no se pudo inicializar el contexto de desencriptación, se libera la memoria y se retorna una cadena vacía
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }
    // Se realiza un cifrado con 128 bits por lo que si la contraseña es más corta se rellena con ceros
    if (contraseña.length() < 32)
    {
        contraseña += std::string(32 - contraseña.length(), '\0');
    }
    //int tam = texto.length();
    unsigned char *textoDesencriptado = new unsigned char[128];
    int textoDesencriptadoLen = 0;
    // ? Buffer de salida
    if (EVP_DecryptUpdate(ctx, textoDesencriptado, &textoDesencriptadoLen, (unsigned char *)texto.c_str(), texto.length()) != 1)
    {
        // ? Si no se pudo realizar el cifrado, se libera la memoria y se retorna una cadena vacía
        EVP_CIPHER_CTX_free(ctx);
        return "";
    }

    // liberamos la memoria
    EVP_CIPHER_CTX_free(ctx);
    delete[] textoDesencriptado;

    return resultado;
}


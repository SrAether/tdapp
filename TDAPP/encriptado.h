#ifndef ENCRIPTADO_H
#define ENCRIPTADO_H

#include <string>

// ! clase Encriptado
// ! versión 1.0
// ! modificado por Aether
// ? no se realizaron cambios a partir de la versión 1.0
class Encriptado
{
    // Contraseña de encriptado
    std::string contraseña;
public:
    Encriptado(std::string contrasena = "contraseña");
    std::string encriptar(std::string texto);
    std::string desencriptar(std::string texto);
    std::string getContraseña();
    void setContraseña(std::string contrasena);
};

#endif // ENCRIPTADO_H

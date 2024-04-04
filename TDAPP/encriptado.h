#ifndef ENCRIPTADOV2_H
#define ENCRIPTADOV2_H

#include <string>

// esta clase es una mejor implementación de la clase encriptado (probablemente elimine la implementación anterior)

namespace EncriptadoV2
{
class Encriptado
{
    // Contraseña de encriptado
    std::string contraseña;
    // iv para encriptado (iv significa initialization vector)
    std::string iv; // contendrá "0123456789012345";
public:
    Encriptado(const std::string& contraseña = "contraseña", const std::string& iv = "0123456789012345");
    std::string encriptar(std::string texto);
    std::string desencriptar(std::string texto);
    std::string getContraseña();
    void setContraseña(std::string contraseña);
    std::string getIv();
    void setIv(std::string iv);
};
}




#endif // ENCRIPTADOV2_H

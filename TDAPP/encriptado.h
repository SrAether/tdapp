#ifndef ENCRIPTADO_H
#define ENCRIPTADO_H

#include <QByteArray>

// ! clase Encriptado
// ! versión 1.0
// ! modificado por Aether
// ? no se realizaron cambios a partir de la versión 1.0
class Encriptado
{
    // Contraseña de encriptado
    QByteArray contraseña;
public:
    Encriptado(QByteArray contrasena = "contraseña");
    QByteArray encriptar(QByteArray texto);
    QByteArray desencriptar(QByteArray texto);
};

#endif // ENCRIPTADO_H

#ifndef MANEJADORJSON_H
#define MANEJADORJSON_H

#include <string>
#include <map>
// ! para manejar archivos se usa la clase manejadorArchivos de la librería mArchivos
#include "manejadorArchivos.h"

// ! espacio de nombres mJson
// ! ultima modificación realizada por Aether
namespace mJson {
// ! clase ManejadorJson
// ! versión 1.0
// ! modificado por Aether
// ? no se realizaron cambios a partir de la versión 1.0
class ManejadorJson {
private:
    std::map<std::string, std::string> map;
    std::string ruta;
    mArchivos::manejadorArchivos manejadorArchivos;
    void estructurar(); // ! método para extraer el mapa del string del archivo
public:
    // constructor
    ManejadorJson(const std::string& rutaArchivo = "archivo.txt", bool nuevo=false);
    // operador =
    bool operator=(const std::map<std::string, std::string> &otro);
    bool operator=(const ManejadorJson &otro);
    // operador ==
    bool operator==(const ManejadorJson &otro);
    bool operator==(const std::map<std::string, std::string> &otro);
    // operador !=
    bool operator!=(const ManejadorJson &otro);
    bool operator!=(const std::map<std::string, std::string> &otro);
    // operador []
    std::string& operator[](std::string clave);
    // operador [] para asignar valores
    //void operator[](std::pair<std::string, std::string> par);
    // método para guardar el archivo
    bool guardar();
    // método para modificar la ruta del archivo
    bool modificarRuta(std::string nuevaRuta, bool guardar=false, bool eliminarAnterior=false);
    // método para eliminar el archivo
    bool eliminar(bool eliminarArchivo=true);
    // método para saber si el mapa está vacío
    bool vacio();
    // destructor (llama al método guardar)
    ~ManejadorJson();
};
}


#endif // MANEJADORJSON_H

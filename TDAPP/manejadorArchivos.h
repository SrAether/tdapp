#ifndef MANEJADORARCHIVOS_H
#define MANEJADORARCHIVOS_H

#include <string> // para convertir los archivos a string
//#include <filesystem> // para manejar archivos
#include <vector>

// es necesario recalcar que para que std::filesystem funcione, es necesario contar con la versión de c++17 o superior

namespace mArchivos {
class manejadorArchivos {
public:
    // Constructor
    manejadorArchivos();
    // Métodos para manejar archivos
    bool verificarExistenciaDeArchivo(const std::string& ruta);
    void crearArchivo(const std::string& nombre, const std::string& contenido);
    void eliminarArchivo(const std::string& nombre);
    bool modificarArchivo(const std::string& nombre, const std::string& contenido);
    std::string leerArchivo(const std::string& nombre);
    void copiarArchivo(const std::string& rutaNueva, const std::string& rutaAnterior);
    void moverArchivo(const std::string& rutaNueva, const std::string& rutaAnterior);
    // Métodos para manejar carpetas
    bool verificarExistenciaDeCarpeta(const std::string& ruta);
    void crearCarpeta(const std::string& ruta);
    void eliminarCarpeta(const std::string& ruta);
    void copiarCarpeta(const std::string& rutaNueva, const std::string& rutaAnterior);
    void moverCarpeta(const std::string& rutaNueva, const std::string& rutaAnterior);
    // Métodos para obtener el contenido de una carpeta
    const std::vector<std::string> obtenerContenidoCarpeta(const std::string& ruta, const int tipo); // 0 para archivos, 1 para carpetas, 2 para ambos

};
}

#endif // MANEJADORARCHIVOS_H

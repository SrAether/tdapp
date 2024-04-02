#include <string>     // para convertir los archivos a string
#include <filesystem> // para manejar archivos
#include <fstream>

#include <vector>
#include "manejadorArchivos.h"

// *Constructor de la clase manejadorArchivos
mArchivos::manejadorArchivos::manejadorArchivos() {}

bool mArchivos::manejadorArchivos::verificarExistenciaDeArchivo(const std::string &ruta)
{
    return std::filesystem::exists(ruta);
}

// *Método para crear un archivo
void mArchivos::manejadorArchivos::crearArchivo(const std::string &nombre, const std::string &contenido)
{
    std::ofstream archivo(nombre);
    if (!archivo.is_open())
    {
        throw std::runtime_error("No se pudo abrir el archivo");
    }
    archivo << contenido;
    archivo.close();
}

// *Método para leer un archivo
void mArchivos::manejadorArchivos::eliminarArchivo(const std::string &nombre)
{
    if (!std::filesystem::remove(nombre))
    {
        throw std::runtime_error("No se pudo eliminar el archivo");
    }
}


// !modificar archivo es igual a crear archivo, quizá sea mejor no tenerlo
bool mArchivos::manejadorArchivos::modificarArchivo(const std::string &nombre, const std::string &contenido)
{
    std::ofstream archivo(nombre);
    if (!archivo.is_open())
    {
        throw std::runtime_error("No se pudo abrir el archivo");
        return false;
    }
    archivo << contenido;
    archivo.close();
    return true;
}

// *Método para leer un archivo
std::string mArchivos::manejadorArchivos::leerArchivo(const std::string &nombre)
{
    std::ifstream archivo(nombre);
    if (!archivo.is_open())
    {
        throw std::runtime_error("No se pudo abrir el archivo");
    }

    // ? método corto y eficiente
    std::string contenido((std::istreambuf_iterator<char>(archivo)), std::istreambuf_iterator<char>());

    // ? método un poco más largo
    // std::string contenido;
    // std::string linea;
    // while (std::getline(archivo, linea))
    // {
    //     contenido += linea + "\n";
    // }


    archivo.close();
    return contenido;
}

// *Método para copiar un archivo
void mArchivos::manejadorArchivos::copiarArchivo(const std::string &rutaNueva, const std::string &rutaAnterior)
{
    if (!std::filesystem::copy_file(rutaAnterior, rutaNueva))
    {
        throw std::runtime_error("No se pudo copiar el archivo");
    }
}

// *Método para mover un archivo
void mArchivos::manejadorArchivos::moverArchivo(const std::string &rutaNueva, const std::string &rutaAnterior)
{
    // ! olvide que std::filesystem::rename no retorna un valor
    // if (!std::filesystem::rename(rutaAnterior, rutaNueva))
    // {
    //     throw std::runtime_error("No se pudo mover el archivo");
    // }
    // ? la solución es usar un try catch
    try
    {
        std::filesystem::rename(rutaAnterior, rutaNueva);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("No se pudo mover el archivo");
    }
}

// *Método para verificar la existencia de una carpeta
bool mArchivos::manejadorArchivos::verificarExistenciaDeCarpeta(const std::string &ruta)
{
    return std::filesystem::is_directory(ruta);
}

// *Método para crear una carpeta
void mArchivos::manejadorArchivos::crearCarpeta(const std::string &ruta)
{
    if (!std::filesystem::create_directory(ruta))
    {
        throw std::runtime_error("No se pudo crear la carpeta");
    }
}

// *Método para eliminar una carpeta
void mArchivos::manejadorArchivos::eliminarCarpeta(const std::string &ruta)
{
    if (!std::filesystem::remove_all(ruta))
    {
        throw std::runtime_error("No se pudo eliminar la carpeta");
    }
}

// *Método para copiar una carpeta
void mArchivos::manejadorArchivos::copiarCarpeta(const std::string &rutaNueva, const std::string &rutaAnterior)
{
    // ! olvide que std::filesystem::copy no retorna un valor
    // if (!std::filesystem::copy(rutaAnterior, rutaNueva))
    // {
    //     throw std::runtime_error("No se pudo copiar la carpeta");
    // }
    // ? la solución es usar un try catch
    try
    {
        std::filesystem::copy(rutaAnterior, rutaNueva);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("No se pudo copiar la carpeta");
    }
}

// *Método para mover una carpeta
void mArchivos::manejadorArchivos::moverCarpeta(const std::string &rutaNueva, const std::string &rutaAnterior)
{
    try
    {
        std::filesystem::rename(rutaAnterior, rutaNueva);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("No se pudo mover la carpeta");
    }
}

// *Método para obtener el contenido de una carpeta
const std::vector<std::string> mArchivos::manejadorArchivos::obtenerContenidoCarpeta(const std::string &ruta, const int tipo)
{
    std::vector<std::string> contenido;
    for (const auto &elemento : std::filesystem::directory_iterator(ruta))
    {
        if (tipo == 0 && std::filesystem::is_regular_file(elemento))
        {
            contenido.push_back(elemento.path().string());
        }
        else if (tipo == 1 && std::filesystem::is_directory(elemento))
        {
            contenido.push_back(elemento.path().string());
        }
        else if (tipo == 2)
        {
            contenido.push_back(elemento.path().string());
        }
    }
    return contenido;
}

#include <string>     // para convertir los archivos a string
#include <filesystem> // para manejar archivos
#include <fstream>

#include <vector>
#include "manejadorArchivos.h"

// eliminame
//#include <iostream>


// Lista de códigos de error
// Err:TDAPP01, No se pudo crear el archivo
// Err:TDAPP02, No se encontró el archivo
// Err:TDAPP03, No se pudo modificar el archivo
// Err:TDAPP04, No se encontró el archivo a eliminar
// Err:TDAPP05, No se pudo copiar el archivo
// Err:TDAPP06, No se pudo mover el archivo
// Err:TDAPP07, No se pudo crear la carpeta
// Err:TDAPP08, No se pudo eliminar la carpeta
// Err:TDAPP09, No se pudo copiar la carpeta
// Err:TDAPP10, No se pudo mover la carpeta





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
        //throw std::runtime_error("No se pudo abrir el archivo");
        throw std::runtime_error("Err:TDAPP01, No se pudo crear el archivo");
    }
    // este simbolo "<<" se llama operador de inserción
    archivo << contenido;
    archivo.close();
}

// *Método para eliminar un archivo
void mArchivos::manejadorArchivos::eliminarArchivo(const std::string &nombre)
{
    if (!std::filesystem::remove(nombre))
    {
        //throw std::runtime_error("No se pudo eliminar el archivo");
        throw std::runtime_error("Err:TDAPP04, No se encontró el archivo a eliminar");
    }
}


// !modificar archivo es igual a crear archivo, quizá sea mejor no tenerlo
bool mArchivos::manejadorArchivos::modificarArchivo(const std::string &nombre, const std::string &contenido)
{
    std::ofstream archivo(nombre);
    if (!archivo.is_open())
    {
        //throw std::runtime_error("No se pudo abrir el archivo");
        throw std::runtime_error("Err:TDAPP03, No se pudo modificar el archivo");
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
        throw std::runtime_error("Err:TDAPP02, No se encontró el archivo");
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
        //throw std::runtime_error("No se pudo copiar el archivo");
        throw std::runtime_error("Err:TDAPP05, No se pudo copiar el archivo");
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
        //throw std::runtime_error("No se pudo mover el archivo");
        throw std::runtime_error("Err:TDAPP06, No se pudo mover el archivo");
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
        //throw std::runtime_error("No se pudo crear la carpeta");
        throw std::runtime_error("Err:TDAPP07, No se pudo crear la carpeta");
    }
}

// *Método para eliminar una carpeta
void mArchivos::manejadorArchivos::eliminarCarpeta(const std::string &ruta)
{
    if (!std::filesystem::remove_all(ruta))
    {
        //throw std::runtime_error("No se pudo eliminar la carpeta");
        throw std::runtime_error("Err:TDAPP08, No se pudo eliminar la carpeta");
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
        //throw std::runtime_error("No se pudo copiar la carpeta");
        throw std::runtime_error("Err:TDAPP09, No se pudo copiar la carpeta");
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
        //throw std::runtime_error("No se pudo mover la carpeta");
        throw std::runtime_error("Err:TDAPP10, No se pudo mover la carpeta");
    }
}

// *Método para obtener el contenido de una carpeta
const std::vector<std::string> mArchivos::manejadorArchivos::obtenerContenidoCarpeta(const std::string &ruta, const int tipo)
{
    std::vector<std::string> contenido;
    // for (const auto &elemento : std::filesystem::directory_iterator(ruta))
    // {
    //     if (tipo == 0 && std::filesystem::is_regular_file(elemento))
    //     {
    //         contenido.push_back(elemento.path().string());
    //     }
    //     else if (tipo == 1 && std::filesystem::is_directory(elemento))
    //     {
    //         contenido.push_back(elemento.path().string());
    //     }
    //     else if (tipo == 2)
    //     {
    //         contenido.push_back(elemento.path().string());
    //     }
    // }
    // /home/aether/cosa
    // ? Para este punto ya se tiene el contenido de la carpeta pero se agrego con todo y la ruta, si se quiere solo el nombre se puede hacer un pequeño cambio
    // ? en el for anterior
    for (const auto &elemento : std::filesystem::directory_iterator(ruta))
    {
        if (tipo == 0 && std::filesystem::is_regular_file(elemento))
        {
            contenido.push_back(elemento.path().filename().string());
        }
        else if (tipo == 1 && std::filesystem::is_directory(elemento))
        {
            contenido.push_back(elemento.path().filename().string());
        }
        else if (tipo == 2)
        {
            contenido.push_back(elemento.path().filename().string());
        }
    }

    return contenido;
}

// *Método para obtener la extensión de un archivo
std::string mArchivos::manejadorArchivos::obtenerExtension(const std::string &ruta)
{
    return std::filesystem::path(ruta).extension().string();
}

// *Método para buscar un archivo con coincidencia exacta sin extensión, retorna la extensión
std::string mArchivos::manejadorArchivos::buscarExtensionArchivo(const std::string &ruta, const std::string &nombre)
{
    // creamos un arreglo de extensiones
    std::vector<std::string> extensiones;
    // colocamos las extensiones en el arreglo por el momento de imagen
    extensiones.push_back(".png");
    extensiones.push_back(".jpg");
    extensiones.push_back(".jpeg");
    extensiones.push_back(".gif");


    for (const auto &elemento : std::filesystem::directory_iterator(ruta))
    {
        // // mostramos el nombre del archivo
        // std::cout << elemento.path().filename().string() << std::endl;
        // if (elemento.path().filename().string() == nombre)
        // {

        //     return elemento.path().extension().string();
        // }
        for (const auto &extension : extensiones)
        {
            if (elemento.path().filename().string() == nombre + extension)
            {
                return extension;
            }
        }
    }
    return "";
}


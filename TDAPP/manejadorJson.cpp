#include <string>
#include <map>                 // * para usar mapas como estructuras de datos
//#include <algorithm>           // * para usar algoritmos como remove_if
#include "manejadorArchivos.h" // ! para manejar archivos se usa la clase manejadorArchivos de la librería mArchivos
#include "manejadorJson.h"     // * declaración de la clase ManejadorJson
#include <stdexcept>            // * para lanzar excepciones

// para depuración
//#include <iostream>

// Códigos de error de la clase ManejadorJson parten de TDAPP20
// Err:TDAPP20, El archivo "rutaArchivo" ya existe


// !Constructor de la clase ManejadorJson
// !Versión 1.0
// !Modificado por Aether
// ? no se realizaron cambios a partir de la versión 1.0
mJson::ManejadorJson::ManejadorJson(const std::string& rutaArchivo, bool nuevo)
{
    this->ruta = rutaArchivo;
    // * Si se pasa el parámetro nuevo como true, se crea un archivo nuevo
    if (nuevo)
    {
        // * Verificamos si el archivo ya existe
        if (manejadorArchivos.verificarExistenciaDeArchivo(rutaArchivo))
        {
            //throw std::runtime_error("El archivo ya existe");
            throw std::runtime_error("Err:TDAPP20, El archivo \"" + rutaArchivo + "\" ya existe");
        }
        // * Creamos el archivo
        this->manejadorArchivos.crearArchivo(rutaArchivo, "{}");
        // * Creamos el mapa vacío
        map = std::map<std::string, std::string>{};
        // * No es necesario guardar el archivo, ya que se creó vacío
        return;
    }

    // ! Es necesario desarrollar el método estructurar (completado requiere revisión)
    estructurar();

}

// ! método para extraer el mapa del string del archivo
// ! versión 2.0
// ! modificado por Aether
// ! solo es funcional si el archivo tiene un solo objeto json
// ! para archivos con múltiples objetos json, se debe modificar sutilmente el método ademas de cambiar la estructura del mapa ya que es un solo objeto
// ! se debe cambiar el mapa a un vector de mapas o un mapa de mapas o un vector de pares
// ! vector<map<string, string>> o map<string, map<string, string>> o vector<pair<string, string>>
/* ? Se modifico la forma de iteración del contenido del archivo: pasamos de un for each a
 *  un for con un índice, esto permite un mejor control de la iteración, además se
 *  modificaron los casos de los switch para tomar en cuenta todos los caracteres ascii
 */
void mJson::ManejadorJson::estructurar()
{
    // ? usamos el método leerArchivo de la clase manejadorArchivos
    std::string contenido = manejadorArchivos.leerArchivo(ruta);
    //std::cout << contenido << std::endl;

    // ? comenzamos el proceso de estructuración

    // creamos 2 variables para guardar la clave y el valor
    std::string clave = "";
    std::string valor = "";
    // variable para saber si estamos en la clave
    bool enClave = false;
    // variable para saber si estamos en el valor
    bool enValor = false;
    // variable para saber estado de proceso actual
    bool proceso = false;
    // si ambos son falsos, estamos en el inicio de un objeto
    // si clave es verdadero, estamos en la clave
    // si valor es verdadero, estamos en el valor
    // variable para distinguir las comillas reales de las comillas de los valores

    //std::cout << contenido << std::endl;

    // ? recorremos el contenido del archivo
    for (int i = 0; i < int(contenido.size()); i++)
    {
        // usando un switch para determinar el tipo de carácter
        unsigned char c = contenido[i];
        switch (c)
        {
        case '}':
        {
            // ? fin de objeto
            if (i == contenido.size() - 1)
            {

                break;
            }
            // si estamos en la clave y aun no terminamos el proceso
            if (enClave && !proceso)
            {
                // agregamos el carácter a la clave
                //clave += i;
                clave += c;
            }
            // si estamos en el valor
            if (enValor)
            {
                // agregamos el carácter al valor
                //valor += i;
                valor += c;
            }
            break;
        }
        case '{':
        {
            // inicio de objeto
            if (contenido[i+1] == '\n')
            {
                //std::cout << "siguiente " << contenido[i+1] << std::endl;
                // saltamos al siguiente ciclo
                break;
            }
            // si estamos en la clave y aun no terminamos el proceso
            if (enClave && !proceso)
            {
                // agregamos el carácter a la clave
                //clave += i;
                clave += c;
            }
            // si estamos en el valor
            if (enValor)
            {
                // agregamos el carácter al valor
                //valor += i;
                valor += c;
            }
            break;
        }
        case '"':
        {
            // si enClave es verdadero, quiere decir que la clave ya fue leída
            // si proceso es falso quiere decir que aun no llegamos al final de la clave
            if (enClave && !proceso)
            {
                proceso = true;
                continue;
            }
            // si enClave es verdadero y proceso es verdadero, quiere decir que la clave ya fue leída y estamos en la primera comilla del valor
            if (enClave && proceso)
            {
                enValor = true;
                // ya no estamos en la clave
                enClave = false;
                continue;
            }

            // si enValor es verdadero, quiere decir que ya leímos la clave y estamos en el valor por lo que estamos en la ultima comilla del valor
            if (enValor && (contenido[i+1] == ','))
            {
                //td::cout << "siguiente " << contenido[i+1] << std::endl;
                // agregamos la clave y el valor al mapa
                map[clave] = valor;
                //std::cout << "clave: " << clave << " valor: " << valor << std::endl;
                // limpiamos la clave
                clave = "";
                // limpiamos el valor
                valor = "";
                // cambiamos el estado de proceso
                proceso = false;
                // cambiamos el estado de enValor
                enValor = false;
                // cambiamos el estado de enClave
                enClave = false;
                // saltamos al siguiente ciclo
                continue;
            }
            if (enValor && (contenido[i+1] != ','))
            {
                // si estamos en la clave y aun no terminamos el proceso
                if (enClave && !proceso)
                {
                    // agregamos el carácter a la clave
                    //clave += i;
                    clave += c;
                }
                // si estamos en el valor
                if (enValor)
                {
                    // agregamos el carácter al valor
                    //valor += i;
                    valor += c;
                }
                break;
            }
            // si enClave es falso, quiere decir que aun no leemos la clave por lo que estamos en la primera comilla de la clave
            if (!enClave)
            {
                enClave = true;
                continue;
            }
            break;
        }
        // case ',':
        // {

        //     // saltamos al siguiente ciclo el proceso de agregar la clave y el valor al mapa se hace en la comilla
        //     continue;
        // }
        // salto de linea
        case '\n':
        {
            // saltamos al siguiente ciclo
            //continue;
            // si el salto de linea va despues de , quiere decir que el objeto terminó por lo que damos continue
            if (contenido[i-1] == ',')
            {
                break;
            }
            // si no seguimos al default
            // si estamos en la clave y aun no terminamos el proceso
            if (enClave && !proceso)
            {
                // agregamos el carácter a la clave
                //clave += i;
                clave += c;
            }
            // si estamos en el valor
            if (enValor)
            {
                // agregamos el carácter al valor
                //valor += i;
                valor += c;
            }
            break;
        }
        default:
        {
            // si estamos en la clave y aun no terminamos el proceso
            if (enClave && !proceso)
            {
                // agregamos el carácter a la clave
                //clave += i;
                clave += c;
            }
            // si estamos en el valor
            if (enValor)
            {
                // agregamos el carácter al valor
                //valor += i;
                valor += c;
            }
            break;
        }
        }
    }
    // mostramos lo que quedo en la clave y el valor
    // std::cout << "Residuos" << std::endl;
    // std::cout << "clave: " << clave << std::endl;
    // std::cout << " valor: " << std::endl;
    // std::cout << valor << std::endl;
    // std::cout << "Fin del proceso" << std::endl;

    // std::cout << "Fin del proceso" << std::endl;
    // if (map.empty())
    // {
    //     throw std::runtime_error("El archivo está vacío");
    // }
}

// ! método para guardar el archivo
// ! versión 1.0
// ! modificado por Aether
// ? no se realizaron cambios a partir de la versión 1.0
bool mJson::ManejadorJson::guardar()
{
    // ? creamos una variable para guardar el contenido del archivo
    std::string contenido = "{\n";
    // ? siempre que el mapa no esté vacío
    //if (!map.empty())
    // ? recorremos el mapa
    for (auto i : map)
    {
        // ? agregamos la clave y el valor al contenido
        contenido += "\"" + i.first + "\": \"" + i.second + "\",\n";
    }
    // ! eliminamos la última coma (no implementado)
    // contenido.erase(std::remove_if(contenido.end() - 2, contenido.end(), [](char c) { return c == ','; }), contenido.end());
    // ? cerramos el objeto
    contenido += "\n}";

    // ? usamos el método escribirArchivo de la clase manejador
    return manejadorArchivos.modificarArchivo(ruta, contenido);
}

// ! método para modificar la ruta del archivo
// ! versión 1.0
// ! modificado por Aether
// ? no se realizaron cambios a partir de la versión 1.0
bool mJson::ManejadorJson::modificarRuta(std::string nuevaRuta, bool guardar, bool eliminarAnterior)
{
    // ? verificamos si la nueva ruta ya existe
    if (manejadorArchivos.verificarExistenciaDeArchivo(nuevaRuta))
    {
        throw std::runtime_error("La nueva ruta ya existe");
    }
    // ? si se pasa el parámetro eliminarAnterior como true, eliminamos el archivo anterior
    if (eliminarAnterior)
    {
        // ? usamos el método eliminarArchivo de la clase manejadorArch
        manejadorArchivos.eliminarArchivo(ruta);
    }
    // ? modificamos la ruta
    this->ruta = nuevaRuta;
    // ? si se pasa el parámetro guardar como true, guardamos el archivo
    if (guardar)
    {
        // ? guardamos el archivo
        return this->guardar();
    }
    return true;
}

// ! método para eliminar el objeto y opcionalmente el archivo
// ! versión 1.0
// ! modificado por Aether
// ? no se realizaron cambios a partir de la versión 1.0
bool mJson::ManejadorJson::eliminar(bool eliminarArchivo)
{
    // ? si se pasa el parámetro eliminarArchivo como true, eliminamos el archivo
    if (eliminarArchivo)
    {
        // ? usamos el método eliminarArchivo de la clase manejadorArchivos
        manejadorArchivos.eliminarArchivo(ruta);
    }
    // ? eliminamos el mapa
    map.clear();
    // ? eliminamos la ruta
    ruta = "";
    return true;
}

// ! método para cargar el archivo
// ! versión 1.0
// ! modificado por Aether
// ? no se realizaron cambios a partir de la versión 1.0
bool mJson::ManejadorJson::cargar(const std::string& rutaArchivo, bool nuevo)
{
    // ? eliminamos el objeto anterior
    eliminar(false);
    // ? si la rutaArchivo es diferente de la ruta actual, modificamos la ruta
    if (rutaArchivo != ruta)
    {
        // ? modificamos la ruta
        ruta = rutaArchivo;
    }
    // ? si el archivo no existe, lo creamos
    if (nuevo)
    {
        // ? verificamos si el archivo ya existe
        if (manejadorArchivos.verificarExistenciaDeArchivo(rutaArchivo))
        {
            throw std::runtime_error("El archivo ya existe");
        }
        // ? creamos el archivo
        manejadorArchivos.crearArchivo(rutaArchivo, "{}");
        // ? creamos el mapa vacío
        map = std::map<std::string, std::string>{};
        // ? no es necesario guardar el archivo, ya que se creó vacío
        return true;
    }
    // ? usamos el método estructurar
    estructurar();
    return true;
}

// ! destructor (llama al método guardar)
// ! versión 1.0
// ! modificado por Aether
// ? no se realizaron cambios a partir de la versión 1.0
mJson::ManejadorJson::~ManejadorJson()
{
    // ? guardamos el archivo
    guardar();
}

// ! operador = con objeto ManejadorJson
// ! versión 1.0
// ! modificado por Aether
// ? no se realizaron cambios a partir de la versión 1.0
bool mJson::ManejadorJson::operator=(const mJson::ManejadorJson &otro)
{
    // ? asignamos la ruta
    this->ruta = otro.ruta;
    // ? asignamos el mapa
    this->map = otro.map;
    return true;
}

// ! operador = con mapa
// ! versión 1.0
// ! modificado por Aether
// ? no se realizaron cambios a partir de la versión 1.0
bool mJson::ManejadorJson::operator=(const std::map<std::string, std::string> &otro)
{
    // ? asignamos el mapa
    this->map = otro;
    return true;
}

// ! operador == con objeto ManejadorJson
// ! versión 1.0
// ! modificado por Aether
// ? no se realizaron cambios a partir de la versión 1.0
bool mJson::ManejadorJson::operator==(const mJson::ManejadorJson &otro)
{
    // ? comparamos la ruta
    if (this->ruta != otro.ruta)
    {
        return false;
    }
    // ? comparamos el mapa
    if (this->map != otro.map)
    {
        return false;
    }
    return true;
}

// ! operador == con mapa
// ! versión 1.0
// ! modificado por Aether
// ? no se realizaron cambios a partir de la versión 1.0
bool mJson::ManejadorJson::operator==(const std::map<std::string, std::string> &otro)
{
    // ? comparamos el mapa
    if (this->map != otro)
    {
        return false;
    }
    return true;
}

// ! operador != con objeto ManejadorJson
// ! versión 1.0
// ! modificado por Aether
// ? no se realizaron cambios a partir de la versión 1.0
bool mJson::ManejadorJson::operator!=(const mJson::ManejadorJson &otro)
{
    // ? comparamos la ruta
    if (this->ruta != otro.ruta)
    {
        return true;
    }
    // ? comparamos el mapa
    if (this->map != otro.map)
    {
        return true;
    }
    return false;
}

// ! operador != con mapa
// ! versión 1.0
// ! modificado por Aether
// ? no se realizaron cambios a partir de la versión 1.0
bool mJson::ManejadorJson::operator!=(const std::map<std::string, std::string> &otro)
{
    // ? comparamos el mapa
    if (this->map != otro)
    {
        return true;
    }
    return false;
}

// ! operador [] para obtener un valor del mapa y asignar valores
// ! versión 1.1
// ! modificado por Aether
// ? En un principio solo era para obtener valores, ahora también se puede asignar valores
std::string& mJson::ManejadorJson::operator[](std::string clave)
{
    // ? Si la clave no existe en el mapa, la insertamos con un valor vacío
    if (map.find(clave) == map.end()) {
        map[clave] = "";
    }
    // ? Retornamos una referencia al valor asociado a la clave
    return map[clave];
    // Utilizamos el método find para buscar la clave
    // auto it = map.find(clave);

    // // Si la clave no existe en el mapa, la insertamos con un valor vacío
    // if (it == map.end()) {
    //     it = map.insert({clave, ""}).first;
    // }

    // // Retornamos una referencia mutable al valor asociado a la clave
    // return it->second;
}

// ! metodo para saber si el mapa está vacío
// ! versión 1.0
// ! modificado por Aether
// ? no se realizaron cambios a partir de la versión 1.0
bool mJson::ManejadorJson::vacio()
{
    return map.empty();
    //return true;
}

// ! método para obtener claves
// ! versión 1.0
// ! modificado por Aether
// ? no se realizaron cambios a partir de la versión 1.0
std::vector<std::string> mJson::ManejadorJson::claves()
{
    // ? creamos un vector para guardar las claves
    std::vector<std::string> claves;
    // ? recorremos el mapa
    for (auto i : map)
    {
        // ? agregamos la clave al vector
        claves.push_back(i.first);
    }
    return claves;
}

// ! fin de la clase ManejadorJson

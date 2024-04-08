#include "mainwindow.h"
#include <string>

// para debug de terminal
#include <map>
#include <iostream>

// dependencias de qt

#include <QCalendarWidget> // incluimos el calendario

#include <QFrame> // ? para crear frames
#include <QComboBox> // ? para crear un selector (como en html)
#include <QPushButton> // ? para crear botones
#include <QLineEdit> // ? para crear campos de texto


// ! Rutas de archivos y carpetas necesarios

// obsoleto
//#define RUTA_CONFIGURACIONES "./config/config.json" // ? ruta del archivo de configuraciones

const std::string RUTA_CONFIGURACIONES = "./config/config.json"; // ? ruta del archivo de configuraciones
const std::string RUTA_CARPETA_CONFIGURACIONES = "./config"; // ? ruta de la carpeta de configuraciones
const std::string RUTA_USUARIOS = "./config/usuarios.json"; // ? ruta del archivo de usuarios



MainWindow::MainWindow(QWidget *parent)
    : QWidget{parent}
{
    // Titulo de la ventana
    setWindowTitle("TDApp");
    // Nombre de la ventana
    this->setWindowIconText("TDApp");
    // Icono de la ventana
    // this->setWindowIcon(QIcon(":/images/icono.png"));


    // verificamos la existencia de los archivos y carpetas necesarios
    verificacionInicial();

    // creamos el frame principal
    framePrincipal = new QFrame(this);
    // establecemos el tamaño del frame principal
    framePrincipal->setGeometry(0, 0, 1920, 1080);

    // INICIO DE SESIÓN NO COMPLETADO
    /* Debe contener:
     * un campo para el usuario puede ser un area de texto o un selector de usuario (lista desplegable)
     * un campo para la contraseña debe ser un area de texto
     * un botón para iniciar sesión
     * un botón para registrarse
     */

    // creamos un frame para iniciar sesión
    frameInicioSesion = new QFrame(framePrincipal);

    // en este punto se deberia cargar un estilo para el frame inicio de sesión
    //frameInicioSesion->setStyleSheet("background-color: #000000;"); // estilo para el frame

    // SELECTOR DE USUARIO
    // creamos un selector de usuario
    selectorUsuario = new QComboBox(frameInicioSesion);
    // establecemos la posición del selector
    selectorUsuario->setGeometry(100, 100, 200, 50);
    // establecemos el texto del selector
    selectorUsuario->addItem("Usuario 1");
    selectorUsuario->addItem("Usuario 2");

    // CAMPO DE CONTRASEÑA
    // creamos un campo de contraseña
    campoContraseña = new QLineEdit(frameInicioSesion);
    // establecemos la posición del campo de contraseña
    campoContraseña->setGeometry(100, 200, 200, 50);
    // establecemos el texto del campo de contraseña
    campoContraseña->setPlaceholderText("Contraseña");

    // BOTÓN DE INICIAR SESIÓN
    // creamos un botón para iniciar sesión
    botonIniciarSesion = new QPushButton(frameInicioSesion);
    // establecemos la posición del botón de iniciar sesión
    botonIniciarSesion->setGeometry(100, 300, 200, 50);
    // establecemos el texto del botón de iniciar sesión
    botonIniciarSesion->setText("Iniciar Sesión");

    // BOTÓN DE REGISTRARSE
    // creamos un botón para registrarse
    botonRegistrarse = new QPushButton(frameInicioSesion);
    // establecemos la posición del botón de registrarse
    botonRegistrarse->setGeometry(100, 400, 200, 50);
    // establecemos el texto del botón de registrarse
    botonRegistrarse->setText("Registrarse");


    // REGISTRO DE USUARIOS NO COMPLETADO
    /* Debe contener:
     * un campo para el nombre de usuario debe ser un area de texto
     * un campo para la contraseña debe ser un area de texto
     * un campo para la confirmación de la contraseña debe ser un area de texto
     * un botón para registrarse
     * un botón para cancelar
     */

    // creamos un frame para el registro de usuarios
    frameRegistroUsuario = new QFrame(framePrincipal);

    // en este punto se deberia cargar un estilo para el frame registro de usuarios
    //frameRegistroUsuario->setStyleSheet("background-color: #000000;"); // estilo para el frame






    // creamos un calendario
    //calendario = new QCalendarWidget(framePrincipal);
    // creamos un frame para iniciar sesión
    //frameInicioSesion = new QFrame(framePrincipal);
    // creamos un frame para el registro de emociones
    //frameRegistroEmociones = new QFrame(framePrincipal);
    // creamos el frame para el hiperfoco
    //frameHiperfoco = new QFrame(framePrincipal);
    // creamos el frame para la configuración
    //frameConfiguracion = new QFrame(framePrincipal);



    // activamos el frame de inicio de sesión (interfaz de inicio de sesión) o el frame de registro de usuarios (interfaz de registro de usuarios)
    // si no hay usuarios, se abrirá el formulario de registro de usuarios
    if (usuarios->vacio())
    {
        // * cerramos la ventana de inicio de sesión
        desactivarInterfazInicioSesion(); // * cerramos la ventana de inicio de sesión
        // * abrimos la ventana de registro de usuarios
        //activarInterfazRegistroUsuario(); // * abrimos la ventana de registro de usuarios
    }


}

MainWindow::~MainWindow() {}

// ! método para verificar la existencia de los archivos y carpetas necesarios
void MainWindow::verificacionInicial()
{
    // ? En este método se verificará la existencia de los archivos y carpetas necesarios

    // en el siguiente bloque se verificará la existencia de los archivos y carpetas de configuración

    // En primera instancia debemos cargar las configuraciones del archivo de configuraciones
    // la ruta propuesta es ./config/config.json
    //configuraciones = new mJson::ManejadorJson("./config/config.json");
    // si no existe el archivo, se creará uno nuevo
    if (!manejadorArchivos.verificarExistenciaDeArchivo(RUTA_CONFIGURACIONES))
    {
        // Verificamos que exista la carpeta de configuraciones
        if (!manejadorArchivos.verificarExistenciaDeCarpeta(RUTA_CARPETA_CONFIGURACIONES))
        {
            // si no existe la carpeta, la creamos
            manejadorArchivos.crearCarpeta(RUTA_CARPETA_CONFIGURACIONES);
        }

        // se creará un archivo nuevo con las configuraciones por defecto
        configuraciones = new mJson::ManejadorJson(RUTA_CONFIGURACIONES, true);
        // el archivo de configuraciones contiene las siguientes configuraciones
        // {
        //     "tema": "claro",
        //     "fuente": "Arial",
        //     "sonidoAlarma": "alarma.mp3",
        //     "volumenAlarma": "100",
        //     "sonidoNotificacion": "notificacion.mp3",
        //     "volumenNotificacion": "100",
        // }
        // como lo acabamos de crear se deberán cargar las configuraciones por defecto
        //std::string fuente = "fuente";
        //configuraciones->operator[]("tema") = "claro";
        auto& conf = *configuraciones;// soy dios y si quiero usar el operador [] de esta forma, lo hago
        conf["tema"] = "claro";
        conf["fuente"] = "Arial";
        conf["sonidoAlarma"] = "alarma.mp3";
        conf["volumenAlarma"] = "100";
        conf["sonidoNotificacion"] = "notificacion.mp3";
        conf["volumenNotificacion"] = "100";
        // como nota adicional, se puede usar el operador [] de la siguiente forma con punteros
        // (*configuraciones)["tema"] = "claro"; // pero la verdad tarde en darme cuenta de esto

        // guardamos los cambios
        configuraciones->guardar();
    }
    else
    {
        // se cargarán las configuraciones del archivo

        std::cout << "Cargando configuraciones" << std::endl;
        configuraciones = new mJson::ManejadorJson(RUTA_CONFIGURACIONES);
        // se carga la fuente
        fuente.setFamily(QString::fromStdString((*configuraciones)["fuente"]));
    }


    // ? en el siguiente bloque se verificará la existencia del archivo de usuarios

    verificarExistenciaArchivoUsuario(); // ? se verifica la existencia del archivo de usuarios (el bloque de código se encuentra en el metodo)



}

// ! método para verificar la existencia del archivo de usuarios
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::verificarExistenciaArchivoUsuario()
{
    // ? se verifica la existencia del archivo de usuarios
    if (!manejadorArchivos.verificarExistenciaDeArchivo(RUTA_USUARIOS))
    {
        // ? si no existe el archivo de usuarios, se creará uno nuevo
        // ? se crea un archivo con un usuario por defecto
        // ? el archivo de usuarios no contiene campos adicionales

        // ? se crea un archivo con un usuario por defecto
        //mJson::ManejadorJson usuarios(RUTA_USUARIOS, true);
        std::cout << "Creando archivo de usuarios" << std::endl;
        usuarios = new mJson::ManejadorJson(RUTA_USUARIOS, true);
        std::cout << "Archivo de usuarios creado" << std::endl;
    }
    else
    {
        // ? si existe el archivo de usuarios, se cargarán los usuarios del archivo
        // ? se cargarán los usuarios del archivo
        usuarios = new mJson::ManejadorJson(RUTA_USUARIOS);
        std::cout << "Archivo de usuarios creado" << std::endl;
        std::cout << usuarios->vacio() << std::endl;
    }


}

// ! ACTIVACIÓN Y DESACTIVACIÓN DE LAS INTERFACES (FRAMES)

// ! método para activar el frame de inicio de sesión (interfaz de inicio de sesión)
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::activarInterfazInicioSesion()
{
    // ? se activa el frame de inicio de sesión
    frameInicioSesion->show();
}

// ! método para desactivar el frame de inicio de sesión (interfaz de inicio de sesión)
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::desactivarInterfazInicioSesion()
{
    // ? se desactiva el frame de inicio de sesión
    frameInicioSesion->hide();

    // ? se oculta el contenido del frame de inicio de sesión
    //selectorUsuario->hide();
    //campoContraseña->hide();
    //botonIniciarSesion->hide();
    //botonRegistrarse->hide();
}

// ! método para activar el frame de registro de usuarios (interfaz de registro de usuarios)
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::activarInterfazRegistroUsuario()
{
    // ? se activa el frame de registro de usuarios
    frameRegistroUsuario->show();
}

// ! método para desactivar el frame de registro de usuarios (interfaz de registro de usuarios)
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::desactivarInterfazRegistroUsuario()
{
    // ? se desactiva el frame de registro de usuarios
    frameRegistroUsuario->hide();
}



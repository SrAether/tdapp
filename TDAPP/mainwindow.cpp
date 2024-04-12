#include "mainwindow.h"
#include <string>

// para debug de terminal
#include <map>
#include <iostream>

#include <vector> // para manejar vectores
#include <algorithm> // para usar la función find

// dependencias de qt

#include <QCalendarWidget> // incluimos el calendario

#include <QFrame> // ? para crear frames
#include <QComboBox> // ? para crear un selector (como en html)
#include <QPushButton> // ? para crear botones
#include <QLineEdit> // ? para crear campos de texto
#include <QLabel> // ? para crear etiquetas
#include <QFileDialog> // ? para abrir un cuadro de dialogo para seleccionar un archivo
#include <QMessageBox> // ? para mostrar mensajes emergentes

// para manejar archivos
#include "manejadorArchivos.h"
#include "manejadorJson.h"
// para encriptar
#include "encriptado.h"


// ! Rutas de archivos y carpetas necesarios

// obsoleto
//#define RUTA_CONFIGURACIONES "./config/config.json" // ? ruta del archivo de configuraciones

const std::string RUTA_CONFIGURACIONES = "./config/config.json"; // ? ruta del archivo de configuraciones
const std::string RUTA_CARPETA_CONFIGURACIONES = "./config"; // ? ruta de la carpeta de configuraciones
const std::string RUTA_USUARIOS = "./usuarios/"; // ? ruta de la carpeta de usuarios
const std::string CONTRASEÑA_ENCRIPTADO = "GULRarfiubh12#"; // ? contraseña para el encriptado


MainWindow::MainWindow(QWidget *parent)
    : QWidget{parent}
{
    // Titulo de la ventana
    setWindowTitle("TDApp");
    // Nombre de la ventana
    this->setWindowIconText("TDApp");
    // Icono de la ventana
    // this->setWindowIcon(QIcon(":/images/icono.png"));

    // ? SE CREA EL OBJETO DE ENCRIPTADO
    encriptado = new EncriptadoV2::Encriptado(CONTRASEÑA_ENCRIPTADO);


    // verificamos la existencia de los archivos y carpetas necesarios
    verificacionInicial();

    // creamos el frame principal
    framePrincipal = new QFrame(this);
    // establecemos el tamaño del frame principal
    framePrincipal->setGeometry(0, 0, 1920, 1080);

    // -----------------------------------------------------------------------------
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
    InSeselectorUsuario = new QComboBox(frameInicioSesion);
    // establecemos la posición del selector
    InSeselectorUsuario->setGeometry(100, 100, 200, 50);
    // establecemos el texto del selector
    InSeselectorUsuario->addItem("Usuario 1");
    InSeselectorUsuario->addItem("Usuario 2");

    // CAMPO DE CONTRASEÑA
    // creamos un campo de contraseña
    InSecampoContraseña = new QLineEdit(frameInicioSesion);
    // establecemos la posición del campo de contraseña
    InSecampoContraseña->setGeometry(100, 200, 200, 50);
    // establecemos el texto del campo de contraseña
    InSecampoContraseña->setPlaceholderText("Contraseña");

    // BOTÓN DE INICIAR SESIÓN
    // creamos un botón para iniciar sesión
    InSebotonIniciarSesion = new QPushButton(frameInicioSesion);
    // establecemos la posición del botón de iniciar sesión
    InSebotonIniciarSesion->setGeometry(100, 300, 200, 50);
    // establecemos el texto del botón de iniciar sesión
    InSebotonIniciarSesion->setText("Iniciar Sesión");

    // BOTÓN DE REGISTRARSE
    // creamos un botón para registrarse
    InSebotonRegistrarse = new QPushButton(frameInicioSesion);
    // establecemos la posición del botón de registrarse
    InSebotonRegistrarse->setGeometry(100, 400, 200, 50);
    // establecemos el texto del botón de registrarse
    InSebotonRegistrarse->setText("Registrarse");

    // -----------------------------------------------------------------------------
    // REGISTRO DE USUARIOS NO COMPLETADO
    /* Debe contener:
     * un campo para el nombre de usuario debe ser un area de texto
     * un campo para el apodo (username)
     * un campo para la contraseña debe ser un area de texto
     * un campo para la confirmación de la contraseña debe ser un area de texto
     * un boton para añadir la foto de perfil
     * un campo para el correo electrónico debe ser un area de texto
     * un campo para la contraseña del correo electrónico debe ser un area de texto
     * un botón para registrarse
     * un botón para cancelar
     */
    // inicializamos las variables
    // ? La etiqueta de titulo
    contenidoEtiquetaRegistroUsuario = "Registro de Usuarios"; // contenido de la etiqueta de registro de usuarios
    // ? Boleano que funciona bandera para pedir o no las credenciales de correo electronico del usuario
    banderaRegistroCopiaSeg = 0;

    // creamos un frame para el registro de usuarios
    frameRegistroUsuario = new QFrame(framePrincipal);
    // ocultamos por defecto
    frameRegistroUsuario->hide();

    // en este punto se deberia cargar un estilo para el frame registro de usuarios
    //frameRegistroUsuario->setStyleSheet("background-color: #000000;"); // estilo para el frame

    // ETIQUETA DE REGISTRO DE USUARIOS
    // creamos una etiqueta para el registro de usuarios
    etiquetaRegistroUsuario = new QLabel{QString::fromStdString(contenidoEtiquetaRegistroUsuario), frameRegistroUsuario};
    // establecemos la posición de la etiqueta de registro de usuarios
    etiquetaRegistroUsuario->setGeometry(100, 50, 200, 50);

    // CAMPO PARA INGRESAR NOMBRE
    // creamos un campo para ingresar el nombre
    campoRegistroNombre = new QLineEdit(frameRegistroUsuario);
    // establecemos la posición del campo para ingresar el nombre
    campoRegistroNombre->setGeometry(100, 100, 200, 50);
    // establecemos el texto del campo para ingresar el nombre
    campoRegistroNombre->setPlaceholderText("Nombre");
    // mensaje al pasar el cursor sobre el campo
    campoRegistroNombre->setToolTip("COLOCA TU NOMBRE PEQUEÑO INSECTO");
    campoRegistroNombre->setToolTipDuration(3000); // duración de cosito emergente

    // CAMPO PARA INGRESAR APODO (USERNAME)
    // creamos un campo para ingresar el apodo
    campoRegistroNombreUs = new QLineEdit(frameRegistroUsuario);
    // establecemos la posición del campo para ingresar el apodo
    campoRegistroNombreUs->setGeometry(100, 150, 200, 50);
    // establecemos el texto del campo para ingresar el apodo
    campoRegistroNombreUs->setPlaceholderText("Como le gustaria que lo llamemos?");

    // CAMPO PARA INGRESAR CONTRASEÑA
    // creamos un campo para ingresar la contraseña
    campoRegistroContraseña = new QLineEdit(frameRegistroUsuario);
    // establecemos la posición del campo para ingresar la contraseña
    campoRegistroContraseña->setGeometry(100, 200, 200, 50);
    // establecemos el texto del campo para ingresar la contraseña
    campoRegistroContraseña->setPlaceholderText("Contraseña");
    // establecemos que el campo sea de tipo contraseña
    campoRegistroContraseña->setEchoMode(QLineEdit::Password);

    // CAMPO PARA CONFIRMAR CONTRASEÑA
    // creamos un campo para confirmar la contraseña
    campoRegistroContraseñaConf = new QLineEdit(frameRegistroUsuario);
    // establecemos la posición del campo para confirmar la contraseña
    campoRegistroContraseñaConf->setGeometry(100, 250, 200, 50);
    // establecemos el texto del campo para confirmar la contraseña
    campoRegistroContraseñaConf->setPlaceholderText("Confirmar Contraseña");
    // establecemos que el campo sea de tipo contraseña y que muestre la opción de mostrar la contraseña
    campoRegistroContraseñaConf->setEchoMode(QLineEdit::Password);




    // BOTÓN PARA SELECCIONAR SI DESEA REALIZAR COPIA DE SEGURIDAD
    // creamos un botón para seleccionar si desea realizar copia de seguridad
    botonRegistroCopiaSeguridad = new QPushButton(frameRegistroUsuario);
    // establecemos la posición del botón para seleccionar si desea realizar copia de seguridad
    botonRegistroCopiaSeguridad->setGeometry(300, 350, 400, 50);
    // establecemos el texto del botón para seleccionar si desea realizar copia de seguridad
    botonRegistroCopiaSeguridad->setText("Da click para activar copia de seguridad");
    // Mensaje cuando pasamos el cursor sobre el boton


    // SELECTOR FRECUENCIA COPIA DE SEGURIDAD
    // creamos un selector para la copia de seguridad
    selectorRegistroFrecuenciaCopiaSeguridad = new QComboBox(frameRegistroUsuario);
    // establecemos la posición del selector para la copia de seguridad
    selectorRegistroFrecuenciaCopiaSeguridad->setGeometry(100, 350, 200, 50);
    // establecemos el texto del selector para la copia de seguridad
    selectorRegistroFrecuenciaCopiaSeguridad->addItem("A diario");
    selectorRegistroFrecuenciaCopiaSeguridad->addItem("Cada 3 días");
    selectorRegistroFrecuenciaCopiaSeguridad->addItem("Cada semana");
    selectorRegistroFrecuenciaCopiaSeguridad->addItem("Cada mes");
    selectorRegistroFrecuenciaCopiaSeguridad->addItem("Nunca");
    // ocultamos por defecto
    selectorRegistroFrecuenciaCopiaSeguridad->hide();

    // CAMPO PARA INGRESAR CORREO ELECTRÓNICO (PARA COPIA DE SEGURIDAD)
    // creamos un campo para ingresar el correo electrónico
    campoRegistroCorreoCopiaSeguridad = new QLineEdit(frameRegistroUsuario);
    // establecemos la posición del campo para ingresar el correo electrónico
    campoRegistroCorreoCopiaSeguridad->setGeometry(100, 400, 200, 50);
    // establecemos el texto del campo para ingresar el correo electrónico
    campoRegistroCorreoCopiaSeguridad->setPlaceholderText("Correo Electrónico");
    // ocultamos por defecto
    campoRegistroCorreoCopiaSeguridad->hide();

    // CAMPO PARA INGRESAR CONTRASEÑA DE CORREO ELECTRÓNICO (PARA COPIA DE SEGURIDAD)
    // creamos un campo para ingresar la contraseña del correo electrónico
    campoRegistroContraseñaCopiaSeguridad = new QLineEdit(frameRegistroUsuario);
    // establecemos la posición del campo para ingresar la contraseña del correo electrónico
    campoRegistroContraseñaCopiaSeguridad->setGeometry(100, 450, 200, 50);
    // establecemos el texto del campo para ingresar la contraseña del correo electrónico
    campoRegistroContraseñaCopiaSeguridad->setPlaceholderText("Contraseña del Correo Electrónico");
    // establecemos que el campo sea de tipo contraseña
    campoRegistroContraseñaCopiaSeguridad->setEchoMode(QLineEdit::Password);
    // ocultamos por defecto
    campoRegistroContraseñaCopiaSeguridad->hide();

    // CAMPO PARA CONFIRMAR CONTRASEÑA DE CORREO ELECTRÓNICO (PARA COPIA DE SEGURIDAD)
    // creamos un campo para confirmar la contraseña del correo electrónico
    campoRegistroContraseñaCopiaSeguridadConf = new QLineEdit(frameRegistroUsuario);
    // establecemos la posición del campo para confirmar la contraseña del correo electrónico
    campoRegistroContraseñaCopiaSeguridadConf->setGeometry(100, 500, 200, 50);
    // establecemos el texto del campo para confirmar la contraseña del correo electrónico
    campoRegistroContraseñaCopiaSeguridadConf->setPlaceholderText("Confirmar Contraseña del Correo Electrónico");
    // establecemos que el campo sea de tipo contraseña
    campoRegistroContraseñaCopiaSeguridadConf->setEchoMode(QLineEdit::Password);
    // ocultamos por defecto
    campoRegistroContraseñaCopiaSeguridadConf->hide();

    // BOTÓN PARA INGRESAR FOTO DE PERFIL (avatar)
    // creamos un botón para añadir la foto de perfil
    botonRegistroFotoPerfil = new QPushButton(frameRegistroUsuario);
    // establecemos la posición del botón para añadir la foto de perfil
    botonRegistroFotoPerfil->setGeometry(100, 550, 200, 50);
    // establecemos el texto del botón para añadir la foto de perfil
    botonRegistroFotoPerfil->setText("Añadir Foto de Perfil");

    // SELECTOR DE GÉNERO
    // creamos un selector para el género
    selectorRegistroGenero = new QComboBox(frameRegistroUsuario);
    // establecemos la posición del selector para el género
    selectorRegistroGenero->setGeometry(100, 600, 200, 50);
    // establecemos el texto del selector para el género
    selectorRegistroGenero->addItem("Hombre");
    selectorRegistroGenero->addItem("Mujer");
    selectorRegistroGenero->addItem("No binario");
    selectorRegistroGenero->addItem("Transgénero");
    selectorRegistroGenero->addItem("Fluido");
    selectorRegistroGenero->addItem("Queer");
    selectorRegistroGenero->addItem("Helicóptero Apache");
    selectorRegistroGenero->addItem("Otro");

    // SELECTOR PARA LA EDAD
    // creamos un selector para la edad
    selectorRegistroEdad = new QComboBox(frameRegistroUsuario);
    // establecemos la posición del selector para la edad
    selectorRegistroEdad->setGeometry(100, 650, 200, 50);
    // establecemos el texto del selector para la edad
    selectorRegistroEdad->addItem("Menos de 18 años");
    selectorRegistroEdad->addItem("18 - 24 años");
    selectorRegistroEdad->addItem("25 - 34 años");
    selectorRegistroEdad->addItem("35 - 44 años");
    selectorRegistroEdad->addItem("45 - 54 años");
    selectorRegistroEdad->addItem("55 - 64 años");
    selectorRegistroEdad->addItem("65 - 74 años");
    selectorRegistroEdad->addItem("75 años o más");

    // BOTÓN PARA REGISTRARSE
    // creamos un botón para registrarse
    botonRegistrarUsuario = new QPushButton(frameRegistroUsuario);
    // establecemos la posición del botón para registrarse
    botonRegistrarUsuario->setGeometry(100, 700, 200, 50);
    // establecemos el texto del botón para registrarse
    botonRegistrarUsuario->setText("Registrarse");

    // BOTÓN PARA CANCELAR
    // creamos un botón para cancelar
    botonRegistroCancelar = new QPushButton(frameRegistroUsuario);
    // establecemos la posición del botón para cancelar
    botonRegistroCancelar->setGeometry(100, 750, 200, 50);
    // establecemos el texto del botón para cancelar
    botonRegistroCancelar->setText("Cancelar");







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
    if (usuarios.empty())
    {
        std::cout << "No hay usuarios" << std::endl;
        // * cerramos la ventana de inicio de sesión
        desactivarInterfazInicioSesion(); // * cerramos la ventana de inicio de sesión
        // * abrimos la ventana de registro de usuarios
        activarInterfazRegistroUsuario(); // * abrimos la ventana de registro de usuarios
    }

    // AREA PARA CONECTAR SEÑALES Y SLOTS

    // SEÑALES RELACIONADAS CON EL REGISTRO DE USUARIOS
    // conectamos la señal de cancelar registro de usuario
    connect(botonRegistroCancelar, SIGNAL(clicked()), this, SLOT(cancelarRegistroUsuario()));
    // conectamos la señal de registrar usuario (area de registro de usuarios) con el metodo registrarUsuario
    connect(botonRegistrarUsuario, SIGNAL(clicked()), this, SLOT(registrarUsuario()));
    // conectamos la señal de seleccionar si desea realizar copia de seguridad
    connect(botonRegistroCopiaSeguridad, SIGNAL(clicked()), this, SLOT(seleccionarCopiaSeguridad()));
    // conectamos la señal de botonRegistroFotoPerfil con el metodo seleccionarArchivo
    connect(botonRegistroFotoPerfil, SIGNAL(clicked()), this, SLOT(seleccionarFotoPerfil()));

    // SEÑALES RELACIONADAS CON EL INICIO DE SESIÓN
    // conectamos la señal para activar el frame de registro de usuarios
    connect(InSebotonRegistrarse, SIGNAL(clicked()), this, SLOT(inSeRegistrarUsuarioNuevo()));



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
// ! versión 2.0
// ! modificado por Aether
// ? Se cambio el enfoque de crear un archivo de usuarios por defecto a crear una carpeta de usuarios por defecto
void MainWindow::verificarExistenciaArchivoUsuario()
{
    // // ? se verifica la existencia del archivo de usuarios
    // if (!manejadorArchivos.verificarExistenciaDeArchivo(RUTA_USUARIOS))
    // {
    //     // ? si no existe el archivo de usuarios, se creará uno nuevo
    //     // ? se crea un archivo con un usuario por defecto
    //     // ? el archivo de usuarios no contiene campos adicionales

    //     // ? se crea un archivo con un usuario por defecto
    //     //mJson::ManejadorJson usuarios(RUTA_USUARIOS, true);
    //     std::cout << "Creando archivo de usuarios" << std::endl;
    //     usuarios = new mJson::ManejadorJson(RUTA_USUARIOS, true);
    //     std::cout << "Archivo de usuarios creado" << std::endl;
    // }
    // else
    // {
    //     // ? si existe el archivo de usuarios, se cargarán los usuarios del archivo
    //     // ? se cargarán los usuarios del archivo
    //     usuarios = new mJson::ManejadorJson(RUTA_USUARIOS);
    //     std::cout << "Archivo de usuarios creado" << std::endl;
    //     std::cout << usuarios->vacio() << std::endl;
    // }

    // ? En el enfoque anterior se creaba un archivo de usuarios por defecto
    // ? En este enfoque se verificará si existe la carpeta de usuarios
    // tiene más sentido crear la carpeta de usuarios y no el archivo de usuarios
    // si no existe la carpeta de usuarios, se creará una carpeta de usuarios
    // para obtener los usuarios se verifica la existencia de subcarpetas en la carpeta de usuarios
    // cada subcarpeta es un usuario, dentro de cada subcarpeta se encuentra un archivo con las
    // configuraciones del usuario y todo lo relacionado con el usuario

    // ? se verifica la existencia de la carpeta de usuarios
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(RUTA_USUARIOS))
    {
        // ? si no existe la carpeta de usuarios, se creará una carpeta de usuarios
        manejadorArchivos.crearCarpeta(RUTA_USUARIOS);
    }
    // ? se verifica la existencia de subcarpetas en la carpeta de usuarios
    usuarios = manejadorArchivos.obtenerContenidoCarpeta(RUTA_USUARIOS, 1);



}

// ! ACTIVACIÓN Y DESACTIVACIÓN DE LAS INTERFACES (FRAMES)


// ////////////////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------------------------
// ! RELACIONADOS CON EL INICIO DE SESIÓN

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

// ! método para ir a la interfaz de registro de usuarios
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::inSeRegistrarUsuarioNuevo()
{
    // ? se desactiva el frame de inicio de sesión
    desactivarInterfazInicioSesion();

    // ? se activa el frame de registro de usuarios
    activarInterfazRegistroUsuario();
}


// ////////////////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------------------------
// ! RELACIONADOS CON EL REGISTRO DE USUARIOS

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

// ! método para cancelar el registro de usuario (interfaz de registro de usuarios)
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::cancelarRegistroUsuario()
{
    // ? se desactiva el frame de registro de usuarios
    desactivarInterfazRegistroUsuario();

    // ? se activa el frame de inicio de sesión
    activarInterfazInicioSesion();
}

// ! método para registrar un usuario
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::registrarUsuario()
{
    // ? se registrará un usuario
    // ? se obtendrán los datos del formulario de registro de usuarios
    // ? se verificará que los datos sean correctos
    // ? se guardará el usuario en el archivo de usuarios
    // ? se activará el frame de inicio de sesión

    // ? se obtienen los datos del formulario de registro de usuarios
    std::string nombre = campoRegistroNombre->text().toStdString();
    std::string nombreUs = campoRegistroNombreUs->text().toStdString();
    std::string contraseña = campoRegistroContraseña->text().toStdString();
    std::string contraseñaConf = campoRegistroContraseñaConf->text().toStdString();
    std::string correo = campoRegistroCorreoCopiaSeguridad->text().toStdString();
    std::string contraseñaCorreo = campoRegistroContraseñaCopiaSeguridad->text().toStdString();
    std::string contraseñaCorreoConf = campoRegistroContraseñaCopiaSeguridadConf->text().toStdString();

    // mostramos en terminal los datos
    std::cout << "Nombre: " << nombre << std::endl;
    std::cout << "Nombre de Usuario: " << nombreUs << std::endl;
    std::cout << "Contraseña: " << contraseña << std::endl;
    std::cout << "Confirmación de Contraseña: " << contraseñaConf << std::endl;
    std::cout << "Realizar Copia de Seguridad: " << banderaRegistroCopiaSeg << std::endl;
    std::cout << "Ruta foto de perfil:" << rutaRegistroFotoPerfil << std::endl;
    std::cout << "Genero: " << selectorRegistroGenero->currentText().toStdString() << std::endl;
    std::cout << "Edad: " << selectorRegistroEdad->currentText().toStdString() << std::endl;

    // estos datos son opcionales
    std::cout << "Correo Electrónico: " << correo << std::endl;
    std::cout << "Contraseña de Correo Electrónico: " << contraseñaCorreo << std::endl;
    std::cout << "Confirmación de Contraseña de Correo Electrónico: " << contraseñaCorreoConf << std::endl;
    std::cout << "Frecuencia de Copia de Seguridad: " << selectorRegistroFrecuenciaCopiaSeguridad->currentText().toStdString() << std::endl;



    // ? se verificará que los datos sean correctos
    // ? se verificará que los campos no estén vacíos
    if (nombre.empty() || nombreUs.empty() || contraseña.empty() || contraseñaConf.empty() || rutaRegistroFotoPerfil.empty())
    {
        // ? si los campos están vacíos, se mostrará un mensaje emergente que indica que los campos no pueden estar vacios
        QMessageBox::critical(this, "Error", "Los campos no pueden estar vacíos");
        return;
    }
    // en caso de que el usuario quiera realizar copia de seguridad
    if (banderaRegistroCopiaSeg)
    {
        // ? se verificará que los campos de correo electrónico y contraseña de correo electrónico no estén vacíos
        if (correo.empty() || contraseñaCorreo.empty() || contraseñaCorreoConf.empty())
        {
            // ? si los campos están vacíos, se mostrará un mensaje emergente que indica que los campos no pueden estar vacios
            QMessageBox::critical(this, "Error", "Los campos para el correo electrónico no pueden estar vacíos");
            return;
        }
    }
    // ! Para este punto, los campos no están vacíos y se procederá a verificar las contraseñas
    // ? se verificará que la contraseña y la confirmación de la contraseña sean iguales
    if (contraseña != contraseñaConf)
    {
        // ? si las contraseñas no coinciden, se mostrará un mensaje emergente que indica que las contraseñas no coinciden
        QMessageBox::critical(this, "Error", "Las contraseñas no coinciden");
        return;
    }
    // ? se verificará que la contraseña del correo electrónico y la confirmación de la contraseña del correo electrónico sean iguales
    if (contraseñaCorreo != contraseñaCorreoConf)
    {
        // ? si las contraseñas no coinciden, se mostrará un mensaje emergente que indica que las contraseñas no coinciden
        QMessageBox::critical(this, "Error", "Las contraseñas de correo electrónico no coinciden");
        return;
    }

    // ! Para este punto, las contraseñas coinciden se procederá a registrar el usuario
    // * primero se verificará si el usuario ya existe
    // * si el usuario ya existe, se mostrará un mensaje emergente
    // * si el usuario no existe, se procederá a registrar el usuario
    // * se creará una carpeta con el nombre del usuario
    // * dentro de la carpeta se creará un archivo de configuraciones
    // * se guardarán los datos del usuario en el archivo de configuraciones
    // * se guardará la foto de perfil en la carpeta del usuario
    // * se mostrará un mensaje emergente que indica que el usuario se ha registrado correctamente

    // ? se verificará si el usuario ya existe
    int indice = buscarUsuario(nombreUs);
    if (indice != -1)
    {
        // ? si el usuario ya existe, se mostrará un mensaje emergente
        QMessageBox::critical(this, "Error", "El usuario ya existe");
        return;
    }
    // ? si el usuario no existe, se procederá a registrar el usuario
    // ? se creará una carpeta con el nombre del usuario
    std::string rutaUsuario = RUTA_USUARIOS + nombreUs;
    // ? se creará la carpeta del usuario
    manejadorArchivos.crearCarpeta(rutaUsuario);
    // ? se creará un archivo de configuraciones
    mJson::ManejadorJson usuario(rutaUsuario + "/config.json", true);
    // ? se guardarán los datos del usuario en el archivo de configuraciones
    //usuario["nombre"] = nombre;
    usuario["nombre"] = encriptado->encriptar(nombre);
    //usuario["nombreUsuario"] = nombreUs;
    usuario["nombreUsuario"] = encriptado->encriptar(nombreUs);
    //usuario["contraseña"] = contraseña;
    usuario["contraseña"] = encriptado->encriptar(contraseña);
    //usuario["genero"] = selectorRegistroGenero->currentText().toStdString();
    usuario["genero"] = encriptado->encriptar(selectorRegistroGenero->currentText().toStdString());
    //usuario["edad"] = selectorRegistroEdad->currentText().toStdString();
    usuario["edad"] = encriptado->encriptar(selectorRegistroEdad->currentText().toStdString());
    // ? si el usuario desea realizar copia de seguridad
    if (banderaRegistroCopiaSeg)
    {
        //usuario["correoElectronico"] = correo;
        //usuario["contraseñaCorreoElectronico"] = contraseñaCorreo;
        //usuario["frecuenciaCopiaSeguridad"] = selectorRegistroFrecuenciaCopiaSeguridad->currentText().toStdString();
        usuario["correoElectronico"] = encriptado->encriptar(correo);
        usuario["contraseñaCorreoElectronico"] = encriptado->encriptar(contraseñaCorreo);
        usuario["frecuenciaCopiaSeguridad"] = encriptado->encriptar(selectorRegistroFrecuenciaCopiaSeguridad->currentText().toStdString());

    }
    // ? se guardará la foto de perfil en la carpeta del usuario
    //manejadorArchivos.copiarArchivo(rutaRegistroFotoPerfil, rutaUsuario + "/fotoPerfil.png"); // ? es al reves xd
    manejadorArchivos.copiarArchivo(rutaUsuario + "/fotoPerfil.png", rutaRegistroFotoPerfil);
    //usuario["fotoPerfil"] = "fotoPerfil.png";
    usuario["fotoPerfil"] = encriptado->encriptar("fotoPerfil.png");
    // ? se guardará la configuración
    usuario.guardar();
    // ? se mostrará un mensaje emergente que indica que el usuario se ha registrado correctamente
    QMessageBox::information(this, "Registro de Usuario", "El usuario se ha registrado correctamente");
    // ? se desactiva el frame de registro de usuarios
    desactivarInterfazRegistroUsuario();
    // ? se activa el frame de inicio de sesión
    activarInterfazInicioSesion();



}

// ! método para seleccionar si desea realizar copia de seguridad
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::seleccionarCopiaSeguridad()
{

    // ? invertimos la bandera
    banderaRegistroCopiaSeg = !banderaRegistroCopiaSeg;
    // ? si la bandera es verdadera, se mostrarán los campos para ingresar el correo electrónico y la contraseña del correo electrónico
    if (banderaRegistroCopiaSeg)
    {
        selectorRegistroFrecuenciaCopiaSeguridad->show();
        campoRegistroCorreoCopiaSeguridad->show();
        campoRegistroContraseñaCopiaSeguridad->show();
        campoRegistroContraseñaCopiaSeguridadConf->show();
        // ? cambiamos el texto del botón
        botonRegistroCopiaSeguridad->setText("Da click para desactivar copia de seguridad");
    }
    else
    {
        selectorRegistroFrecuenciaCopiaSeguridad->hide();
        campoRegistroCorreoCopiaSeguridad->hide();
        campoRegistroContraseñaCopiaSeguridad->hide();
        campoRegistroContraseñaCopiaSeguridadConf->hide();
        // ? cambiamos el texto del botón
        botonRegistroCopiaSeguridad->setText("Da click para activar copia de seguridad");
    }
}


// ! método para seleccionar la foto de perfil
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::seleccionarFotoPerfil()
{
    // mostramos en consola el mensaje
    std::cout << "Seleccionando Foto de Perfil" << std::endl;
    // ? se seleccionará la foto de perfil
    // ? se obtendrá la ruta de la foto de perfil
    // ? se mostrará un mensaje emergente que indica que la foto de perfil se ha seleccionado
    // ? se obtendrá la ruta de la foto de perfil
    seleccionarArchivo(rutaRegistroFotoPerfil, "Imagen (*.png *.jpg *.jpeg)", "Seleccionar Foto de Perfil");


}

// ////////////////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------------------------
// ! BUSQUEDA DE USUARIOS

// ! método para buscar un usuario dentro del vector de usuarios
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
int MainWindow::buscarUsuario(const std::string& nombreUsuario)
{

    // ? una expresión lambda es una función anónima que se puede usar para definir funciones dentro de otras funciones
    // ? en este caso, se usará una expresión lambda para buscar un usuario dentro del vector de usuarios
    // ? se buscará un usuario dentro del vector de usuarios mediante el nombre de usuario en una expresión lambda
    // auto it = std::find_if(usuarios.begin(), usuarios.end(), [nombreUsuario](const std::string& nombre) { return nombre == nombreUsuario; });

    // con std::find
    auto it = std::find(usuarios.begin(), usuarios.end(), nombreUsuario);

    // la verdad no se si sea necesario usar una expresión lambda asi que lo cambie por std::find, debemos admitir que la expresión lambda se ve mas cool

    if (it != usuarios.end())
    {
        return std::distance(usuarios.begin(), it);
    }
    return -1;
}

// ////////////////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------------------------
// ! RELACIONADOS CON LA SELECCIÓN DE ARCHIVOS

// ! método para obtener archivos (usando la interfaz gráfica)
// ! versión 1.0
// ! modificado por Aether
// ? El objetivo de este metodo es proporcionar una interfaz gráfica para seleccionar un archivo y obtener su ruta
void MainWindow::seleccionarArchivo(std::string& dondeGuardar, const std::string& extension, const std::string& titulo)
{

    // preparamos un bloque try catch para capturar errores
    try
    {
        // // creamos un cuadro de dialogo para seleccionar un archivo
        // QString rutaArchivo = QFileDialog::getOpenFileName(this, QString::fromStdString(titulo.c_str()), QDir::homePath(), QString::fromStdString(extension.c_str()));
        // // convertimos la ruta a un string
        // dondeGuardar = rutaArchivo.toStdString();

        // creamos un cuadro de dialogo para seleccionar un archivo de no seleccionar se muestra un mensaje emergente al usuario
        QString rutaArchivo = QFileDialog::getOpenFileName(this, QString::fromStdString(titulo.c_str()), QDir::homePath(), QString::fromStdString(extension.c_str()));
        // convertimos la ruta a un string
        if (rutaArchivo.isEmpty())
        {
            // si no se selecciona un archivo, se mostrará un mensaje emergente
            QMessageBox::critical(this, "Error", "No se seleccionó un archivo");
            return;
        }
        dondeGuardar = rutaArchivo.toStdString();

    }
    catch (const std::exception& e)
    {
        // si hay un error, se mostrará un mensaje emergente
        QMessageBox::critical(this, "Error", e.what());
    }
    // mostramos en terminal los datos
    std::cout << "Ruta: " << dondeGuardar << std::endl;
}



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


    // BOTÓN PARA AÑADIR FOTO DE PERFIL (avatar)
    // creamos un botón para añadir la foto de perfil
    botonRegistroFotoPerfil = new QPushButton(frameRegistroUsuario);
    // establecemos la posición del botón para añadir la foto de perfil
    botonRegistroFotoPerfil->setGeometry(100, 300, 200, 50);
    // establecemos el texto del botón para añadir la foto de perfil
    botonRegistroFotoPerfil->setText("Añadir Foto de Perfil");

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
    botonRegistrarse = new QPushButton(frameRegistroUsuario);
    // establecemos la posición del botón para registrarse
    botonRegistrarse->setGeometry(100, 700, 200, 50);
    // establecemos el texto del botón para registrarse
    botonRegistrarse->setText("Registrarse");

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
    if (usuarios->vacio())
    {
        // * cerramos la ventana de inicio de sesión
        desactivarInterfazInicioSesion(); // * cerramos la ventana de inicio de sesión
        // * abrimos la ventana de registro de usuarios
        activarInterfazRegistroUsuario(); // * abrimos la ventana de registro de usuarios
    }

    // AREA PARA CONECTAR SEÑALES Y SLOTS

    // SEÑALES RELACIONADAS CON EL REGISTRO DE USUARIOS
    // conectamos la señal de cancelar registro de usuario
    connect(botonRegistroCancelar, SIGNAL(clicked()), this, SLOT(cancelarRegistroUsuario()));
    // conectamos la señal de registrar usuario
    connect(botonRegistrarse, SIGNAL(clicked()), this, SLOT(registrarUsuario()));
    // conectamos la señal de seleccionar si desea realizar copia de seguridad
    connect(botonRegistroCopiaSeguridad, SIGNAL(clicked()), this, SLOT(seleccionarCopiaSeguridad()));



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
    std::cout << "Correo Electrónico: " << correo << std::endl;
    std::cout << "Contraseña de Correo Electrónico: " << contraseñaCorreo << std::endl;
    std::cout << "Confirmación de Contraseña de Correo Electrónico: " << contraseñaCorreoConf << std::endl;


    // ? se verificará que los datos sean correctos
    // ? se verificará que los campos no estén vacíos
    if (nombre.empty() || nombreUs.empty() || contraseña.empty() || contraseñaConf.empty() || correo.empty() || contraseñaCorreo.empty() || contraseñaCorreoConf.empty())
    {
        // ? si los campos están vacíos, se mostrará un mensaje emergente que indica que los campos no pueden estar vacios
    }

}

// ! método para seleccionar si desea realizar copia de seguridad
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::seleccionarCopiaSeguridad()
{
    // // ? se seleccionará si desea realizar copia de seguridad
    // // ? se mostrarán los campos para ingresar el correo electrónico y la contraseña del correo electrónico
    // // ? se mostrará un selector para seleccionar la frecuencia de la copia de seguridad

    // // ? se mostrarán los campos para ingresar el correo electrónico y la contraseña del correo electrónico
    // campoRegistroCorreoCopiaSeguridad->show();
    // campoRegistroContraseñaCopiaSeguridad->show();
    // campoRegistroContraseñaCopiaSeguridadConf->show();

    // // ? se mostrará un selector para seleccionar la frecuencia de la copia de seguridad
    // selectorRegistroFrecuenciaCopiaSeguridad->show();
    // ? invertimos la bandera
    banderaRegistroCopiaSeg = !banderaRegistroCopiaSeg;
    // ? si la bandera es verdadera, se mostrarán los campos para ingresar el correo electrónico y la contraseña del correo electrónico
    if (banderaRegistroCopiaSeg)
    {
        selectorRegistroFrecuenciaCopiaSeguridad->show();
        campoRegistroCorreoCopiaSeguridad->show();
        campoRegistroContraseñaCopiaSeguridad->show();
        campoRegistroContraseñaCopiaSeguridadConf->show();
    }
    else
    {
        selectorRegistroFrecuenciaCopiaSeguridad->hide();
        campoRegistroCorreoCopiaSeguridad->hide();
        campoRegistroContraseñaCopiaSeguridad->hide();
        campoRegistroContraseñaCopiaSeguridadConf->hide();
    }
}

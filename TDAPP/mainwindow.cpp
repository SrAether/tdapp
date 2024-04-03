#include "mainwindow.h"
#include <string>
#include <map>
#include <QCalendarWidget> // incluimos el calendario

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
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




}

MainWindow::~MainWindow() {}

// ! método para verificar la existencia de los archivos y carpetas necesarios
void MainWindow::verificacionInicial()
{
    // En primera instancia debemos cargar las configuraciones del archivo de configuraciones
    // la ruta propuesta es ./config/config.json
    //configuraciones = new mJson::ManejadorJson("./config/config.json");
    // si no existe el archivo, se creará uno nuevo
    if (!manejadorArchivos.verificarExistenciaDeArchivo("./config/config.json"))
    {
        // Verificamos que exista la carpeta de configuraciones
        if (!manejadorArchivos.verificarExistenciaDeCarpeta("./config"))
        {
            // si no existe la carpeta, la creamos
            manejadorArchivos.crearCarpeta("./config");
        }

        // se creará un archivo nuevo con las configuraciones por defecto
        configuraciones = new mJson::ManejadorJson("./config/config.json", true);
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
        configuraciones = new mJson::ManejadorJson("./config/config.json");
        // se carga la fuente
        fuente.setFamily(QString::fromStdString((*configuraciones)["fuente"]));
    }
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <map> // no se necesita por que lo gestiona el manejadorJson
#include <string>
// ! para cosas de qt
#include <QFrame>

// ! experimental
#include <QCalendarWidget> // incluimos el calendario

// ! para manejar archivos json
#include "manejadorJson.h"
// ! para manejar archivos
#include "manejadorArchivos.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
    // variables
    mArchivos::manejadorArchivos manejadorArchivos; // ! objeto de la clase manejadorArchivos que permite manejar archivos
    mJson::ManejadorJson* configuraciones; // ! objeto de la clase manejadorJson que contiene las configuraciones de la aplicación


    QFont fuente; // fuente de la aplicación

    //! FRAMES Y WIDGETS DE LA VENTANA

    // Frame que contiene todos los elementos de la ventana
    QFrame *framePrincipal; // frame principal
    // Calendario
    QCalendarWidget *calendario; // calendario



    // Métodos privados de la ventana
    void verificacionInicial(); // ! verifica la existencia de los archivos y carpetas necesarios para el funcionamiento de la aplicación y trata de crearlos si no existen



public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H

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
    // Frame para iniciar sesión
    QFrame *frameInicioSesion; // frame para iniciar sesión
    // Frame para el registro de emociones
    QFrame *frameRegistroEmociones; // frame para el registro de emociones
    // Frame para el hiperfoco
    QFrame *frameHiperfoco; // frame para el hiperfoco
    // Frame para la configuración
    QFrame *frameConfiguracion; // frame para la configuración
    // Frame para journaling (diario)
    QFrame *frameJournaling; // frame para journaling
    // Frame para barra de navegación
    QFrame *frameBarraNavegacion; // frame para barra de navegación



    // Métodos privados de la ventana
    void verificacionInicial(); // ! verifica la existencia de los archivos y carpetas necesarios para el funcionamiento de la aplicación y trata de crearlos si no existen

private slots:
    // Métodos que se ejecutan al hacer click en los botones
    // void interfazInicioSesion(); // ! muestra la interfaz de inicio de sesión
    // void interfazRegistroEmociones(); // ! muestra la interfaz de registro de emociones
    // void interfazHiperfoco(); // ! muestra la interfaz de hiperfoco
    // void interfazConfiguracion(); // ! muestra la interfaz de configuración
    // void interfazCalendario(); // ! muestra la interfaz del calendario

    void activarInterfazInicioSesion(); // ! muestra la interfaz de inicio de sesión
    void desactivarInterfazInicioSesion(); // ! oculta la interfaz de inicio de sesión
    void activarInterfazRegistroEmociones(); // ! muestra la interfaz de registro de emociones
    void desactivarInterfazRegistroEmociones(); // ! oculta la interfaz de registro de emociones
    void activarInterfazHiperfoco(); // ! muestra la interfaz de hiperfoco
    void desactivarInterfazHiperfoco(); // ! oculta la interfaz de hiperfoco
    void activarInterfazConfiguracion(); // ! muestra la interfaz de configuración
    void desactivarInterfazConfiguracion(); // ! oculta la interfaz de configuración
    void activarInterfazCalendario(); // ! muestra la interfaz del calendario
    void desactivarInterfazCalendario(); // ! oculta la interfaz del calendario
    void activarInterfazJournaling(); // ! muestra la interfaz de journaling
    void desactivarInterfazJournaling(); // ! oculta la interfaz de journaling

    // DEPENDENCIAS DE LA INTERFAZ CALENDARIO
    void mostrarRegistroEmociones(time_t fecha); // ! muestra la interfaz con la grafica de emociones
    void ocultarRegistroEmociones(); // ! oculta la interfaz con la grafica de emociones
    void mostrarDiaSeleccionado(time_t fecha); // ! muestra la interfaz para agregar un evento al calendario
    void ocultarDiaSeleccionado(); // ! oculta la interfaz para agregar un evento al calendario
    void mostrarIntefazEditarEventoCalendario(int idEvent = 0); // ! muestra la interfaz para editar un evento del calendario, si es 0 es un evento nuevo
    void ocultarIntefazEditarEventoCalendario(); // ! oculta la interfaz para editar un evento del calendario
    void mostrarEventoSeleccionado(int idEvent); // ! muestra la interfaz con la información de un evento seleccionado
    void ocultarEventoSeleccionado(); // ! oculta la interfaz con la información de un evento seleccionado
    // ? boton eliminar evento seleccionado de interfazEventoSeleccionado
    void eliminarEventoCalendario(int idEvent); // ! elimina un evento del calendario
    // ? boton guardar evento usado en la interfaz de editar evento
    void guardarEventoCalendario(int idEvent); // ! guarda un evento del calendario

    // DEPENDENCIAS DE LA INTERFAZ JOURNALING (DIARIO)
    void mostrarPantallaBienvenidaJournaling(); // ! muestra la pantalla de bienvenida de journaling
    void ocultarPantallaBienvenidaJournaling(); // ! oculta la pantalla de bienvenida de journaling
    void mostrarListaEntradasJournaling(); // ! muestra la lista de entradas de journaling
    void ocultarListaEntradasJournaling(); // ! oculta la lista de entradas de journaling
    void mostrarInterfazEditarEntradaJournaling(int idEntrada = 0); // ! muestra la interfaz para editar una entrada de journaling, si es 0 es una entrada nueva
    void ocultarInterfazEditarEntradaJournaling(); // ! oculta la interfaz para editar una entrada de journaling
    void mostrarEntradaSeleccionadaJournaling(int idEntrada); // ! muestra la interfaz con la información de una entrada seleccionada de journaling
    void ocultarEntradaSeleccionadaJournaling(); // ! oculta la interfaz con la información de una entrada seleccionada de journaling
    // ? Boton entrar a journaling de mostrarPantallaBienvenidaJournaling
    void entrarJournaling(); // ! muestra la interfaz de journaling
    // ? Boton nueva entrada de mostrarListaEntradasJournaling
    void nuevaEntradaJournaling(); // ! muestra la interfaz para agregar una nueva entrada de journaling
    // ? Boton guardar entrada de interfazEditarEntradaJournaling
    void guardarEntradaJournaling(int idEntrada); // ! guarda una entrada de journaling
    // ? Boton eliminar entrada de interfaz mostrarEntradaSeleccionadaJournaling
    void eliminarEntradaJournaling(int idEntrada); // ! elimina una entrada de journaling

    // DEPENDENCIAS DE LA INTERFAZ REGISTRO DE EMOCIONES
    void mostrarAreaRegistroEmociones(); // ! muestra la interfaz de registro de emociones
    void ocultarAreaRegistroEmociones(); // ! oculta la interfaz de registro de emociones
    void mostrarMConfEmociones(); // ! muestra la interfaz de confirmación de emociones
    void ocultarMConfEmociones(); // ! oculta la interfaz de confirmación de emociones
    void mostrarEmoConfir(); // ! muestra la interfaz de emoción confirmada
    void ocultarEmoConfir(); // ! oculta la interfaz de emoción confirmada

    // DEPENDENCIAS DE LA INTERFAZ HIPERFOCO
    void mostrarAreaHiperfoco(); // ! muestra la interfaz de hiperfoco
    void ocultarAreaHiperfoco(); // ! oculta la interfaz de hiperfoco
    void mostrarInterfazListaMetodosHiperfoco(); // ! muestra la interfaz de lista de métodos de hiperfoco
    void ocultarInterfazListaMetodosHiperfoco(); // ! oculta la interfaz de lista de métodos de hiperfoco
    void mostrarMetodoHiperfoco(); // ! muestra la interfaz de método de hiperfoco (para un método específico)
    void ocultarMetodoHiperfoco(); // ! oculta la interfaz de método de hiperfoco (para un método específico)
    // ? boton establecer hiperfoco de mostrarAreaHiperfoco
    void establecerHiperfoco(); // ! establece el hiperfoco
    // ? lista de horas de mostrarAreaHiperfoco
    void seleccionarHoraHiperfoco(); // ! selecciona la cantidad de horas del hiperfoco (temporizador)
    // ? lista de minutos de mostrarAreaHiperfoco
    void seleccionarMinutosHiperfoco(); // ! selecciona la cantidad de minutos del hiperfoco (temporizador)
    // ? boton mostrar lista de métodos de mostrarAreaHiperfoco
    void mostrarListaMetodosHiperfoco(); // ! muestra la lista de métodos de hiperfoco
    // ? seleccionar método de gestión de mostrarInterfazListaMetodosHiperfoco
    void seleccionarMetodoHiperfoco(); // ! selecciona el método de gestión de tiempo (hiperfoco)



public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H

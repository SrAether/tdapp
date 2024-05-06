#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <map> // no se necesita por que lo gestiona el manejadorJson
#include <string>
// ! para cosas de qt

#include <QFrame> // ? para crear frames
#include <QComboBox> // ? para crear un selector (como en html)
#include <QPushButton> // ? para crear botones
#include <QLineEdit> // ? para crear campos de texto
#include <QLabel> // ? para crear etiquetas
#include <QListWidget> // ? para crear listas
#include <QListWidgetItem> // ? para crear elementos de la lista
#include <QScrollArea> // ? para crear areas de scroll
#include <QHBoxLayout> // ? para crear layouts horizontales
#include <QVBoxLayout> // ? para crear layouts verticales

// ! experimental
#include <QCalendarWidget> // incluimos el calendario

// ! para manejar archivos json
#include "manejadorJson.h"
// ! para manejar archivos
#include "manejadorArchivos.h"
// ! para encriptar
#include "encriptado.h"

// ! para switch



class MainWindow : public QWidget
{
    Q_OBJECT
    // variables
    mArchivos::manejadorArchivos manejadorArchivos; // ! objeto de la clase manejadorArchivos que permite manejar archivos
    mJson::ManejadorJson* configuraciones; // ! objeto de la clase manejadorJson que contiene las configuraciones de la aplicación
    mJson::ManejadorJson* configuracionesUsuario; // ! objeto de la clase manejadorJson que contiene las configuraciones del usuario
    // ? DADO QUE USUARIOS SOLO SE UTILIZA PARA INICIAR SESIÓN, ES MEJOR USAR UN VECTOR DE STRINGS QUE CONTENGA LOS USUARIOS
    std::vector<std::string> usuarios; // ! vector que contiene los usuarios de la aplicación (nombre de usuario que a su vez es el nombre de la carpeta del usuario)
    QFont fuente; // fuente de la aplicación
    // ! para encriptar
    EncriptadoV2::Encriptado *encriptado; // ! objeto de la clase encriptado que permite encriptar y desencriptar se define como variable global en el cpp



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
    // Frame para la interfaz de registro de usuario
    QFrame *frameRegistroUsuario; // frame para la interfaz de registro de usuario
    // Frame para la interfaz de recuperación de contraseña de usuario
    QFrame *frameRecuperarContra; // frame para la interfaz de recuperación de contraseña de usuario

    // -----------------------------------------
    // COSAS DENTRO DE FRAME INICIO SESION (inSe)
    // Selector de usuario
    QComboBox *inSeSelectorUsuario; // selector de usuario
    // campo de texto para la contraseña
    QLineEdit *inSeCampoContra; // campo de texto para la contraseña
    // Botón para iniciar sesión
    QPushButton *inSeBotonIniciarSesion; // boton para iniciar sesión
    // Botón para registrar usuario
    QPushButton *inSeBotonRegistrarse; // boton para registrar usuario
    // Botón para recuperar contraseña
    QPushButton *inSeBotonRecuperarContra; // boton para recuperar contraseña


    // -----------------------------------------
    // COSAS DENTRO DE FRAME REGISTRO USUARIO
    // variables utiles para registrar usuario
    // boleano que nos indica si el usuario quiere añadir o no su correo electronico
    bool banderaRegistroCopiaSeg; // si es verdadero quiere añadir su correo
    // string que guarda la ruta de la foto de perfil
    std::string rutaRegistroFotoPerfil; // ruta de la foto de perfil
    // etiqueta de registro de usuario
    QLabel *etiquetaRegistroUsuario; // etiqueta de registro de usuario
    // campo de texto para ingresar nombre
    // es relevante mencionar que la etiqueta se tiene que ligar a un objeto de tipo QString el cual voy a castear a std::string
    std::string contenidoEtiquetaRegistroUsuario; // contenido de la etiqueta de registro de usuario
    QLineEdit *campoRegistroNombre; // area para ingresar el nombre de usuario
    // campo de texto para ingresar nombre de usuario (username)
    QLineEdit *campoRegistroNombreUs; // area para ingresar el apodo del usuario
    // campo de texto para ingresar contraseña
    QLineEdit *campoRegistroContra; // campo para ingresar la conraseña
    // campo de texto para confirmar la contraseña
    QLineEdit *campoRegistroContraConf; // campo para confirmar la contraseña
    // Boton para seleccionar si se quiere realizar copia de seguridad
    QPushButton *botonRegistroCopiaSeguridad; // boton para seleccionar si se quiere realizar copia de seguridad
    // Selector de frecuencia copia de seguridad
    QComboBox *selectorRegistroFrecuenciaCopiaSeguridad; // selector de copia de seguridad
    // campo de texto para ingresar correo de copia de seguridad (se usara para google drive)
    QLineEdit *campoRegistroCorreoCopiaSeguridad; // campo para ingresar el correo de copia de seguridad
    // campo de texto para ingresar contraseña de copia de seguridad (se usara para google drive)
    QLineEdit *campoRegistroContraCopiaSeguridad; // campo para ingresar la contraseña de copia de seguridad
    // campo de texto para verificar la contraseña de copia de seguridad
    QLineEdit *campoRegistroContraCopiaSeguridadConf; // campo para verificar la contraseña de copia de seguridad
    // Botón para añadir foto de perfil
    QPushButton *botonRegistroFotoPerfil; // boton para añadir foto de perfil
    // Selector de edad
    QComboBox *selectorRegistroEdad; // selector edad
    // campo para añadir pregunta para recuperar contraseña
    QLineEdit *campoRegistroPreguntaRecuperacion; // campo para añadir pregunta para recuperar contraseña
    // campo para añadir respuesta para recuperar contraseña
    QLineEdit *campoRegistroRespuestaRecuperacion; // campo para añadir respuesta para recuperar contraseña
    // Botón para registrar usuario
    QPushButton *botonRegistrarUsuario; // boton para registrar usuario
    // Botón para cancelar registro de usuario
    QPushButton *botonRegistroCancelar; // boton para cancelar registro de usuario

    // -----------------------------------------
    // COSAS DENTRO DE FRAME Recuperar Contra (reCo)
    // variables
    std::string reCoPreguntaContra; // pregunta para recuperar contraseña
    // selector de usuario
    QComboBox *reCoSelectorUsuario; // selector de usuario
    // QLabel para la pregunta de recuperar contraseña
    QLabel *reCoPreguntaRecuperacion; // etiqueta para la pregunta de recuperar contraseña
    // campo de texto para ingresar la respuesta
    QLineEdit *reCoCampoRespuesta; // campo para ingresar la respuesta
    // boton para recuperar contraseña
    QPushButton *reCoBotonConfirmar; // boton para recuperar contraseña
    // boton para cancelar recuperar contraseña
    QPushButton *reCoBotonCancelar; // boton para cancelar recuperar contraseña


    // -----------------------------------------
    // COSAS DENTRO DE FRAME BARA DE NAVEGACIÓN (barNa)
    // Botón para configuración
    QPushButton *barNaBotonConfiguracion; // boton para configuración
    // Botón para calendario
    QPushButton *barNaBotonCalendario; // boton para calendario
    // Botón para registro de emociones
    QPushButton *barNaBotonRegistroEmociones; // boton para registro de emociones
    // Botón para journaling
    QPushButton *barNaBotonJournaling; // boton para journaling
    // Botón para hiperfoco
    QPushButton *barNaBotonHiperfoco; // boton para hiperfoco
    // Botón para sección actual 0
    QPushButton *barNaBotonSeccionActual0; // boton para sección actual 0
    // Botón para sección actual 1
    QPushButton *barNaBotonSeccionActual1; // boton para sección actual 1
    // Botón para sección actual 2
    QPushButton *barNaBotonSeccionActual2; // boton para sección actual 2
    // Configurar Botones de la barra de navegación
    void barNaConfigurarBotones(const int& nBoton, const bool& activar, const int& nFuncion, const std::string& textoBoton); // ! configura los botones de la barra de navegación


    // -----------------------------------------
    // COSAS DENTRO DE FRAME JOURNALING (jour)
    // Botón para cambiar imagen de bienvenida
    QPushButton *jourBotonCambiarImagenBienvenida; // boton para cambiar imagen de bienvenida
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    // COSAS DENTRO DE FRAME JOURNALING (jour) - PANTALLA LISTA DE NOTAS (jourLiNo)
    // Label para título de la lista de notas
    QLabel *jourLiNoTitulo; // etiqueta para título de la lista de notas
    // Lista de notas
    QListWidget *jourLiNoListaNotas; // lista de notas
    // Area de scroll para la lista de notas
    QScrollArea *jourLiNoScrollArea; // area de scroll para la lista de notas
    // Layout para la lista de notas
    QVBoxLayout *jourLiNoLayout; // layout para la lista de notas





    // Métodos privados de la ventana
    void verificacionInicial(); // ! verifica la existencia de los archivos y carpetas necesarios para el funcionamiento de la aplicación y trata de crearlos si no existen
    void verificarExistenciaArchivoUsuario(); // ! verifica la existencia del archivo de usuarios y trata de crearlo si no existe
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void activarInterfazInicioSesion(); // ! muestra la interfaz de inicio de sesión
    void desactivarInterfazInicioSesion(); // ! oculta la interfaz de inicio de sesión

    // //////////////////////////////////////////////////////////////////////////////////
    // --------------------------- SLOTS Registro Usuario ----------------------------------
    // Relacionados con la interfaz de registro de usuario
    void activarInterfazRegistroUsuario(); // ! muestra la interfaz de registro de usuario
    void desactivarInterfazRegistroUsuario(); // ! oculta la interfaz de registro de usuario
    void registrarUsuario(); // ! registra un nuevo usuario en la aplicación
    void cancelarRegistroUsuario(); // ! cancela el registro de un nuevo usuario en la aplicación
    void seleccionarCopiaSeguridad(); // ! selecciona si se quiere realizar copia de seguridad
    void seleccionarArchivo(std::string& dondeSeGuarda, const std::string& extension, const std::string& titulo); // ! selecciona un archivo
    void seleccionarFotoPerfil(); // ! selecciona la foto de perfil del usuario

    // relacionados con el vector de usuarios
    int buscarUsuario(const std::string& usuario); // ! busca un usuario en el vector de usuarios si no lo encuentra regresa -1

    // //////////////////////////////////////////////////////////////////////////////////
    // -------------------------------- SLOTS Inicio Sesion ----------------------------------
    // Relacionados con la interfaz de inicio de sesión
    void inSeRegistrarUsuarioNuevo(); // ! registra un nuevo usuario en la aplicación
    void inSeCargarUsuarios(); // ! carga los usuarios en el selector de usuarios
    void inSeIniciarSesion(); // ! inicia sesión en la aplicación
    void inSeRecuperarContra(); // ! muestra la interfaz de recuperación de contraseña

    // //////////////////////////////////////////////////////////////////////////////////
    // --------------------------- SLOTS Recuperar Contra ----------------------------------
    // Relacionados con la interfaz de recuperación de contraseña
    void activarInterfazRecuperarContra(); // ! muestra la interfaz de recuperación de contraseña
    void desactivarInterfazRecuperarContra(); // ! oculta la interfaz de recuperación de contraseña
    void reCoCargarDatosUsuario(); // ! carga los datos del usuario en la interfaz de recuperación de contraseña
    void reCoCargarUsuarios(); // ! carga los usuarios en el selector de usuarios
    void reCoRecuperarContra(); // ! recupera la contraseña del usuario
    void reCoCancelarRecuperarContra(); // ! cancela la recuperación de contraseña

    // //////////////////////////////////////////////////////////////////////////////////
    // --------------------------- SLOTS Barra de Navegación ----------------------------------
    // Relacionados con la barra de navegación
    void activarBarraNavegacion(); // ! muestra la barra de navegación
    void desactivarBarraNavegacion(); // ! oculta la barra de navegación
    void barNaMostrarJournaling(); // ! muestra la interfaz de journaling
    void barNaDesactivarTodosLosFrames(); // ! oculta todos los frames de la ventana

    // //////////////////////////////////////////////////////////////////////////////////
    // --------------------------- SLOTS Journaling ----------------------------------
    // Relacionados con la interfaz de journaling
    void activarInterfazJournaling(); // ! muestra la interfaz de journaling
    void desactivarInterfazJournaling(); // ! oculta la interfaz de journaling
    void mostrarPantallaBienvenidaJournaling(); // ! muestra la pantalla de bienvenida de journaling
    void jourOcultarPantallaBienvenida(); // ! oculta la pantalla de bienvenida de journaling
    void jourCambiarImagenBienvenida(); // ! cambia la imagen de bienvenida de journaling
    void jourActivarListaEntradas(); // ! muestra la lista de entradas de journaling


    // // DEPENDENCIAS DE LA INTERFAZ JOURNALING (DIARIO)
    // void mostrarPantallaBienvenidaJournaling(); // ! muestra la pantalla de bienvenida de journaling
    // void ocultarPantallaBienvenidaJournaling(); // ! oculta la pantalla de bienvenida de journaling
    // void mostrarListaEntradasJournaling(); // ! muestra la lista de entradas de journaling
    // void ocultarListaEntradasJournaling(); // ! oculta la lista de entradas de journaling
    // void mostrarInterfazEditarEntradaJournaling(int idEntrada = 0); // ! muestra la interfaz para editar una entrada de journaling, si es 0 es una entrada nueva
    // void ocultarInterfazEditarEntradaJournaling(); // ! oculta la interfaz para editar una entrada de journaling
    // void mostrarEntradaSeleccionadaJournaling(int idEntrada); // ! muestra la interfaz con la información de una entrada seleccionada de journaling
    // void ocultarEntradaSeleccionadaJournaling(); // ! oculta la interfaz con la información de una entrada seleccionada de journaling
    // // ? Boton entrar a journaling de mostrarPantallaBienvenidaJournaling
    // void entrarJournaling(); // ! muestra la interfaz de journaling
    // // ? Boton nueva entrada de mostrarListaEntradasJournaling
    // void nuevaEntradaJournaling(); // ! muestra la interfaz para agregar una nueva entrada de journaling
    // // ? Boton guardar entrada de interfazEditarEntradaJournaling
    // void guardarEntradaJournaling(int idEntrada); // ! guarda una entrada de journaling
    // // ? Boton eliminar entrada de interfaz mostrarEntradaSeleccionadaJournaling
    // void eliminarEntradaJournaling(int idEntrada); // ! elimina una entrada de journaling


    //void activarInterfazRegistroEmociones(); // ! muestra la interfaz de registro de emociones
    //void desactivarInterfazRegistroEmociones(); // ! oculta la interfaz de registro de emociones
    // void activarInterfazHiperfoco(); // ! muestra la interfaz de hiperfoco
    // void desactivarInterfazHiperfoco(); // ! oculta la interfaz de hiperfoco
    // void activarInterfazConfiguracion(); // ! muestra la interfaz de configuración
    // void desactivarInterfazConfiguracion(); // ! oculta la interfaz de configuración
    // void activarInterfazCalendario(); // ! muestra la interfaz del calendario
    // void desactivarInterfazCalendario(); // ! oculta la interfaz del calendario
    // void activarInterfazJournaling(); // ! muestra la interfaz de journaling
    // void desactivarInterfazJournaling(); // ! oculta la interfaz de


    // // DEPENDENCIAS DE LA INTERFAZ CALENDARIO
    // void mostrarRegistroEmociones(time_t fecha); // ! muestra la interfaz con la grafica de emociones
    // void ocultarRegistroEmociones(); // ! oculta la interfaz con la grafica de emociones
    // void mostrarDiaSeleccionado(time_t fecha); // ! muestra la interfaz para agregar un evento al calendario
    // void ocultarDiaSeleccionado(); // ! oculta la interfaz para agregar un evento al calendario
    // void mostrarIntefazEditarEventoCalendario(int idEvent = 0); // ! muestra la interfaz para editar un evento del calendario, si es 0 es un evento nuevo
    // void ocultarIntefazEditarEventoCalendario(); // ! oculta la interfaz para editar un evento del calendario
    // void mostrarEventoSeleccionado(int idEvent); // ! muestra la interfaz con la información de un evento seleccionado
    // void ocultarEventoSeleccionado(); // ! oculta la interfaz con la información de un evento seleccionado
    // // ? boton eliminar evento seleccionado de interfazEventoSeleccionado
    // void eliminarEventoCalendario(int idEvent); // ! elimina un evento del calendario
    // // ? boton guardar evento usado en la interfaz de editar evento
    // void guardarEventoCalendario(int idEvent); // ! guarda un evento del calendario



    // // DEPENDENCIAS DE LA INTERFAZ REGISTRO DE EMOCIONES
    // void mostrarAreaRegistroEmociones(); // ! muestra la interfaz de registro de emociones
    // void ocultarAreaRegistroEmociones(); // ! oculta la interfaz de registro de emociones
    // void mostrarMConfEmociones(); // ! muestra la interfaz de confirmación de emociones
    // void ocultarMConfEmociones(); // ! oculta la interfaz de confirmación de emociones
    // void mostrarEmoConfir(); // ! muestra la interfaz de emoción confirmada
    // void ocultarEmoConfir(); // ! oculta la interfaz de emoción confirmada

    // // DEPENDENCIAS DE LA INTERFAZ HIPERFOCO
    // void mostrarAreaHiperfoco(); // ! muestra la interfaz de hiperfoco
    // void ocultarAreaHiperfoco(); // ! oculta la interfaz de hiperfoco
    // void mostrarInterfazListaMetodosHiperfoco(); // ! muestra la interfaz de lista de métodos de hiperfoco
    // void ocultarInterfazListaMetodosHiperfoco(); // ! oculta la interfaz de lista de métodos de hiperfoco
    // void mostrarMetodoHiperfoco(); // ! muestra la interfaz de método de hiperfoco (para un método específico)
    // void ocultarMetodoHiperfoco(); // ! oculta la interfaz de método de hiperfoco (para un método específico)
    // // ? boton establecer hiperfoco de mostrarAreaHiperfoco
    // void establecerHiperfoco(); // ! establece el hiperfoco
    // // ? lista de horas de mostrarAreaHiperfoco
    // void seleccionarHoraHiperfoco(); // ! selecciona la cantidad de horas del hiperfoco (temporizador)
    // // ? lista de minutos de mostrarAreaHiperfoco
    // void seleccionarMinutosHiperfoco(); // ! selecciona la cantidad de minutos del hiperfoco (temporizador)
    // // ? boton mostrar lista de métodos de mostrarAreaHiperfoco
    // void mostrarListaMetodosHiperfoco(); // ! muestra la lista de métodos de hiperfoco
    // // ? seleccionar método de gestión de mostrarInterfazListaMetodosHiperfoco
    // void seleccionarMetodoHiperfoco(); // ! selecciona el método de gestión de tiempo (hiperfoco)

    // // DEPENDENCIAS DE LA INTERFAZ CONFIGURACIÓN
    // // ? boton modificar tema de mostrarAreaConfiguracion
    // void modificarTema(); // ! modifica el tema de la aplicación
    // // ? boton modificar fuente de mostrarAreaConfiguracion
    // void modificarFuente(); // ! modifica la fuente de la aplicación
    // // ? boton modificar sonido de alarma de mostrarAreaConfiguracion
    // void modificarSonidoAlarma(); // ! modifica el sonido de alarma de la aplicación
    // // ? boton modificar volumen de alarma de mostrarAreaConfiguracion
    // void modificarVolumenAlarma(); // ! modifica el volumen de la alarma de la aplicación
    // // ? boton modificar sonido de notificación de mostrarAreaConfiguracion
    // void modificarSonidoNotificacion(); // ! modifica el sonido de notificación de la aplicación
    // // ? boton modificar volumen de notificación de mostrarAreaConfiguracion
    // void modificarVolumenNotificacion(); // ! modifica el volumen de la notificación de la aplicación
    // // ? boton modificar credenciales de mostrarAreaConfiguracion
    // void modificarCredencialesRespaldo(); // ! modifica las credenciales de la aplicación (usuario y contraseña de google drive)
    // // ? boton modificar información de usuario de mostrarAreaConfiguracion
    // void modificarInformacionUsuario(); // ! modifica la información del usuario de la aplicación


    // // DEPENDENCIAS DE LA BARRA DE NAVEGACIÓN
    // // ? boton mostrar calendario de mostrarBarraNavegacion
    // void mostrarCalendario(); // ! muestra la interfaz del calendario
    // // ? boton mostrar journaling de mostrarBarraNavegacion
    // void mostrarJournaling(); // ! muestra la interfaz de journaling
    // // ? boton mostrar registro de emociones de mostrarBarraNavegacion
    // void mostrarRegistroEmociones(); // ! muestra la interfaz de registro de emociones
    // // ? boton mostrar hiperfoco de mostrarBarraNavegacion
    // void mostrarHiperfoco(); // ! muestra la interfaz de hiperfoco
    // // ? boton mostrar configuración de mostrarBarraNavegacion
    // void mostrarConfiguracion(); // ! muestra la interfaz de configuración

signals:


protected:

};
#endif // MAINWINDOW_H

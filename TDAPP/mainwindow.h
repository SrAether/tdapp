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
#include <QTextEdit> // ? para crear areas de texto
#include <QGridLayout> // ? para crear layouts de cuadricula (CALENDARIO)
#include <QCalendar> // ? para crear calendarios backend
#include <QTimeEdit> // ? para crear campos de tiempo
#include <QSpacerItem>

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
    // Frame para el calendario
    QFrame *frameCalendarioP; // frame para el calendario
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
    //Titulo
    QLabel *inSeLabel;
    std::string contenidoTitulo;
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
    //Layout Vertical
    QVBoxLayout *iSesionLayout;
    //Spacers
    QSpacerItem *inSeSpacer1;
    QSpacerItem *inSeSpacer2;

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
    // Layout para el registro de usuario
    QVBoxLayout *rULayout; // layout para el registro de usuario
    // ScrollArea para el registro de usuario
    QScrollArea *rUScroll; // area de scroll para el registro de usuario
    // QWidget para meter el layout de registro de usuario
    QWidget *rUWidget; // widget para meter el layout de registro de usuario
    // Layout principal
    QVBoxLayout *rUPrincipal; // layout principal
    // Layout etiqueta
    QHBoxLayout *rUHLayout_1;
    //Layout botones finales
    QHBoxLayout *rUHLayout_2;
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
    //Layout
    QVBoxLayout *reCoLayout;


    // -----------------------------------------
    // COSAS DENTRO DE FRAME BARA DE NAVEGACIÓN (barNa)
    // arreglo de 3 enteros que nos indica el numero de función que ejecuta cada boton
    int barNaFunciones[3]; // ! arreglo de 3 enteros que nos indica el numero de función que ejecuta cada boton

    // Botón para configuración
    QSize barNaTamBotones; // ! tamaño de los botones de la barra de navegación
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
    // Ejecutor de funciones de la barra de navegación
    void barNaEjecutorFunciones(const int& nFuncion); // ! ejecuta las funciones de la barra de navegación
    // iconos de la barra de navegación
    QIcon *barNaIconoConfiguracion; // icono para configuración
    QIcon *barNaIconoCalendario; // icono para calendario
    QIcon *barNaIconoRegistroEmociones; // icono para registro de emociones
    QIcon *barNaIconoJournaling; // icono para journaling
    QIcon *barNaIconoHiperfoco; // icono para hiperfoco



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
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    // COSAS DENTRO DE FRAME JOURNALING (jour) - PANTALLA REGISTRO DE ENTRADA (jourReEn)
    mJson::ManejadorJson *entradaSeleccionada; // ! objeto de la clase manejadorJson que contiene la entrada seleccionada
    QLineEdit *jourReEnCampoTitulo; // campo para el título de la entrada
    QTextEdit *jourReEnCampoTexto; // campo para el texto de la entrada
    QVBoxLayout *jourReEnLayout; // layout para la entrada

    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    // ICONOS DE LA INTERFAZ DE JOURNALING PARA BOTONES DE LA BARRA DE NAVEGACIÓN
    QIcon *jourIconoActivarListaEntradas; // icono para activar lista de entradas
    QIcon *jourIconoNuevaEntrada; // icono para nueva entrada
    QIcon *jourIconoGuardarEntrada; // icono para guardar entrada
    QIcon *jourIconoEliminarEntrada; // icono para eliminar entrada
    QIcon *jourIconoEditarEntrada; // icono para editar entrada

    // -----------------------------------------
    // COSAS DENTRO DE FRAME CALENDARIO (cal)
    // Calendario
    //QCalendarWidget *calCalendario; // calendario
    // Layout para el calendario
    QVBoxLayout *calLayout; // layout para el calendario
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    // COSAS DENTRO DE FRAME CALENDARIO (cal) - Grid de calendario
    // Variables utiles para el calendario
    int calMes; // mes actual
    int calAnio; // año actual
    int calDia; // dia actual
    bool editandoEvento; // si se esta editando un evento
    std::vector<std::string> *calIconosMeses; // iconos para los meses
    QStringList *diasSemana; // lista de los días de la semana
    QStringList *meses; // lista de los meses
    QVBoxLayout *calLayoutPrincipal; // layout principal
    QFrame *frameCalendario; // frame para el calendario
    QCalendar *calCalendarioBackend; // calendario backend
    // layout horizontal para el calendario
    QHBoxLayout *calCalendarioLayout; // layout horizontal para el calendario
    // layout vertical para el calendario
    QVBoxLayout *calCalendarioLayoutVertical; // layout vertical para el calendario
    // label para el icono del mes
    QLabel *calIconoMes; // label para el icono del mes
    // label para el el año
    QLabel *calAnioLabel; // label para el año
    // Grid de calendario
    QGridLayout *calCalendario; // grid de calendario
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    // COSAS DE Pantalla dia seleccionado
    // manejador de archivo para dia seleccionado
    mJson::ManejadorJson *calDiaJsonEventos; // ! objeto de la clase manejadorJson que contiene los eventos del dia seleccionado
    // Frame para el dia seleccionado
    QFrame *calDiaFrame; // frame para el dia seleccionado
    // Label de titulo para los eventos del dia
    QLabel *calDiaTitulo; // titulo para los eventos del dia
    // Lista de eventos del dia
    QListWidget *calDiaEventos; // lista de eventos del dia
    // Area de scroll para la lista de eventos del dia
    QScrollArea *calDiaScrollArea; // area de scroll para la lista de eventos del dia
    // Layout para la lista de eventos del dia
    QVBoxLayout *calDiaLayout; // layout para la lista de eventos del dia
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    // COSAS DE Pantalla agregar evento
    // campo de texto para el titulo del evento
    QLineEdit *calAgregarEventoTitulo; // campo para el titulo del evento
    // campo de texto para la hora del evento
    //QLineEdit *calAgregarEventoHora; // campo para la hora del evento
    QTimeEdit *calAgregarEventoHora; // campo para la hora del evento
    // campo de texto para la duración del evento
    QTimeEdit *calAgregarEventoDuracion; // campo para la duración del evento
    // campo de texto para la descripción del evento
    QTextEdit *calAgregarEventoDescripcion; // campo para la descripción del evento
    QHBoxLayout *calAgregarEventoCotenedorTiempo; // contenedor para la hora y duración del evento


    // Métodos privados de la ventana
    void verificacionInicial(); // ! verifica la existencia de los archivos y carpetas necesarios para el funcionamiento de la aplicación y trata de crearlos si no existen
    void verificarExistenciaArchivoUsuario(); // ! verifica la existencia del archivo de usuarios y trata de crearlo si no existe
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // //////////////////////////////////////////////////////////////////////////////////
    // --------------------------- SLOTS Generales ----------------------------------
    //void cerrarAplicacion(); // ! cierra la aplicación

    // //////////////////////////////////////////////////////////////////////////////////
    // --------------------------- SLOTS Inicio ----------------------------------
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
    void barNaMostrarCalendario(); // ! muestra la interfaz del calendario
    void barNaMostrarJournaling(); // ! muestra la interfaz de journaling
    void barNaDesactivarTodosLosFrames(); // ! oculta todos los frames de la ventana
    void barNaBotonSeccionActual0Click(); // ! metodo para el boton de la seccion actual 0
    void barNaBotonSeccionActual1Click(); // ! metodo para el boton de la seccion actual 1
    void barNaBotonSeccionActual2Click(); // ! metodo para el boton de la seccion actual 2
    void barNaLimpiarBotonesSeccionActual(); // ! limpia los botones de la seccion actual

    // //////////////////////////////////////////////////////////////////////////////////
    // --------------------------- SLOTS Journaling ----------------------------------
    // Relacionados con la interfaz de journaling
    void activarInterfazJournaling(); // ! muestra la interfaz de journaling
    void desactivarInterfazJournaling(); // ! oculta la interfaz de journaling
    void mostrarPantallaBienvenidaJournaling(); // ! muestra la pantalla de bienvenida de journaling
    void jourPanBiRedimensionarIconoBoton(); // ! redimensiona el icono de la pantalla de bienvenida de journaling
    void jourOcultarPantallaBienvenida(); // ! oculta la pantalla de bienvenida de journaling
    void jourCambiarImagenBienvenida(); // ! cambia la imagen de bienvenida de journaling
    void jourActivarListaEntradas(); // ! muestra la lista de entradas de journaling
    void jourDesactivarListaEntradas(); // ! oculta la lista de entradas de journaling
    void jourActualizarListaEntradas(); // ! actualiza la lista de entradas de journaling
    void jourLiNoNuevaEntrada(); // ! agrega una entrada a la lista de entradas de journaling
    void jourLiNoCargarEntrada(const int& tipoCarga = 0, const std::string& nombreEntrada = ""); // ! carga una entrada de journaling
    void jourGuardarEntrada(); // ! guarda una entrada de journaling
    void jourEliminarEntrada(); // ! elimina una entrada de journaling
    void jourActivarEditarEntrada(const bool& soloLectura = false); // ! muestra la interfaz para editar una entrada de journaling
    void jourDesactivarEditarEntrada(); // ! oculta la interfaz para editar una entrada de journaling

    // //////////////////////////////////////////////////////////////////////////////////
    // --------------------------- SLOTS Calendario ----------------------------------
    // Relacionados con la interfaz del calendario
    void activarInterfazCalendario(); // ! muestra la interfaz del calendario
    void desactivarInterfazCalendario(); // ! oculta la interfaz del calendario
    void calActivarCalendario(); // ! activa el calendario
    void calDesactivarCalendario(); // ! desactiva el calendario

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // Relacionados el funcionamiento del calendario
    int calObtenerNumeroEventosDia(const int& anio, const int& mes, const int& dia); // ! obtiene el número de eventos de un día
    void calMesAnterior(); // ! muestra el mes anterior en el calendario
    void calMesSiguiente(); // ! muestra el mes siguiente en el calendario
    void calCargarDatosCalendario(int mes = -1, int anio = -1); // ! carga los datos del calendario, es el metodo principal
    void calClickDia(const int& nfila, const int& ncolumna); // ! metodo que se llama al hacer click en un dia del calendario
    void calLimpiarCalendario(); // ! limpia el calendario
    void calClickDiaBackend10(); // ! metodo que se llama al hacer click en el boton 1-0 del calendario
    void calClickDiaBackend11(); // ! metodo que se llama al hacer click en el boton 1-1 del calendario
    void calClickDiaBackend12(); // ! metodo que se llama al hacer click en el boton 1-2 del calendario
    void calClickDiaBackend13(); // ! metodo que se llama al hacer click en el boton 1-3 del calendario
    void calClickDiaBackend14(); // ! metodo que se llama al hacer click en el boton 1-4 del calendario
    void calClickDiaBackend15(); // ! metodo que se llama al hacer click en el boton 1-5 del calendario
    void calClickDiaBackend16(); // ! metodo que se llama al hacer click en el boton 1-6 del calendario
    void calClickDiaBackend20(); // ! metodo que se llama al hacer click en el boton 2-0 del calendario
    void calClickDiaBackend21(); // ! metodo que se llama al hacer click en el boton 2-1 del calendario
    void calClickDiaBackend22(); // ! metodo que se llama al hacer click en el boton 2-2 del calendario
    void calClickDiaBackend23(); // ! metodo que se llama al hacer click en el boton 2-3 del calendario
    void calClickDiaBackend24(); // ! metodo que se llama al hacer click en el boton 2-4 del calendario
    void calClickDiaBackend25(); // ! metodo que se llama al hacer click en el boton 2-5 del calendario
    void calClickDiaBackend26(); // ! metodo que se llama al hacer click en el boton 2-6 del calendario
    void calClickDiaBackend30(); // ! metodo que se llama al hacer click en el boton 3-0 del calendario
    void calClickDiaBackend31(); // ! metodo que se llama al hacer click en el boton 3-1 del calendario
    void calClickDiaBackend32(); // ! metodo que se llama al hacer click en el boton 3-2 del calendario
    void calClickDiaBackend33(); // ! metodo que se llama al hacer click en el boton 3-3 del calendario
    void calClickDiaBackend34(); // ! metodo que se llama al hacer click en el boton 3-4 del calendario
    void calClickDiaBackend35(); // ! metodo que se llama al hacer click en el boton 3-5 del calendario
    void calClickDiaBackend36(); // ! metodo que se llama al hacer click en el boton 3-6 del calendario
    void calClickDiaBackend40(); // ! metodo que se llama al hacer click en el boton 4-0 del calendario
    void calClickDiaBackend41(); // ! metodo que se llama al hacer click en el boton 4-1 del calendario
    void calClickDiaBackend42(); // ! metodo que se llama al hacer click en el boton 4-2 del calendario
    void calClickDiaBackend43(); // ! metodo que se llama al hacer click en el boton 4-3 del calendario
    void calClickDiaBackend44(); // ! metodo que se llama al hacer click en el boton 4-4 del calendario
    void calClickDiaBackend45(); // ! metodo que se llama al hacer click en el boton 4-5 del calendario
    void calClickDiaBackend46(); // ! metodo que se llama al hacer click en el boton 4-6 del calendario
    void calClickDiaBackend50(); // ! metodo que se llama al hacer click en el boton 5-0 del calendario
    void calClickDiaBackend51(); // ! metodo que se llama al hacer click en el boton 5-1 del calendario
    void calClickDiaBackend52(); // ! metodo que se llama al hacer click en el boton 5-2 del calendario
    void calClickDiaBackend53(); // ! metodo que se llama al hacer click en el boton 5-3 del calendario
    void calClickDiaBackend54(); // ! metodo que se llama al hacer click en el boton 5-4 del calendario
    void calClickDiaBackend55(); // ! metodo que se llama al hacer click en el boton 5-5 del calendario
    void calClickDiaBackend56(); // ! metodo que se llama al hacer click en el boton 5-6 del calendario
    void calClickDiaBackend60(); // ! metodo que se llama al hacer click en el boton 6-0 del calendario
    void calClickDiaBackend61(); // ! metodo que se llama al hacer click en el boton 6-1 del calendario
    void calClickDiaBackend62(); // ! metodo que se llama al hacer click en el boton 6-2 del calendario
    void calClickDiaBackend63(); // ! metodo que se llama al hacer click en el boton 6-3 del calendario
    void calClickDiaBackend64(); // ! metodo que se llama al hacer click en el boton 6-4 del calendario
    void calClickDiaBackend65(); // ! metodo que se llama al hacer click en el boton 6-5 del calendario
    void calClickDiaBackend66(); // ! metodo que se llama al hacer click en el boton 6-6 del calendario
    void calClickEmoDia(const int& nfila, const int& ncolumna); // ! metodo que se llama al hacer click en el boton de emociones de un dia del calendario
    void calClickEmoDiaBackend10(); // ! metodo que se llama al hacer click en el boton de emociones 1-0 del calendario
    void calClickEmoDiaBackend11(); // ! metodo que se llama al hacer click en el boton de emociones 1-1 del calendario
    void calClickEmoDiaBackend12(); // ! metodo que se llama al hacer click en el boton de emociones 1-2 del calendario
    void calClickEmoDiaBackend13(); // ! metodo que se llama al hacer click en el boton de emociones 1-3 del calendario
    void calClickEmoDiaBackend14(); // ! metodo que se llama al hacer click en el boton de emociones 1-4 del calendario
    void calClickEmoDiaBackend15(); // ! metodo que se llama al hacer click en el boton de emociones 1-5 del calendario
    void calClickEmoDiaBackend16(); // ! metodo que se llama al hacer click en el boton de emociones 1-6 del calendario
    void calClickEmoDiaBackend20(); // ! metodo que se llama al hacer click en el boton de emociones 2-0 del calendario
    void calClickEmoDiaBackend21(); // ! metodo que se llama al hacer click en el boton de emociones 2-1 del calendario
    void calClickEmoDiaBackend22(); // ! metodo que se llama al hacer click en el boton de emociones 2-2 del calendario
    void calClickEmoDiaBackend23(); // ! metodo que se llama al hacer click en el boton de emociones 2-3 del calendario
    void calClickEmoDiaBackend24(); // ! metodo que se llama al hacer click en el boton de emociones 2-4 del calendario
    void calClickEmoDiaBackend25(); // ! metodo que se llama al hacer click en el boton de emociones 2-5 del calendario
    void calClickEmoDiaBackend26(); // ! metodo que se llama al hacer click en el boton de emociones 2-6 del calendario
    void calClickEmoDiaBackend30(); // ! metodo que se llama al hacer click en el boton de emociones 3-0 del calendario
    void calClickEmoDiaBackend31(); // ! metodo que se llama al hacer click en el boton de emociones 3-1 del calendario
    void calClickEmoDiaBackend32(); // ! metodo que se llama al hacer click en el boton de emociones 3-2 del calendario
    void calClickEmoDiaBackend33(); // ! metodo que se llama al hacer click en el boton de emociones 3-3 del calendario
    void calClickEmoDiaBackend34(); // ! metodo que se llama al hacer click en el boton de emociones 3-4 del calendario
    void calClickEmoDiaBackend35(); // ! metodo que se llama al hacer click en el boton de emociones 3-5 del calendario
    void calClickEmoDiaBackend36(); // ! metodo que se llama al hacer click en el boton de emociones 3-6 del calendario
    void calClickEmoDiaBackend40(); // ! metodo que se llama al hacer click en el boton de emociones 4-0 del calendario
    void calClickEmoDiaBackend41(); // ! metodo que se llama al hacer click en el boton de emociones 4-1 del calendario
    void calClickEmoDiaBackend42(); // ! metodo que se llama al hacer click en el boton de emociones 4-2 del calendario
    void calClickEmoDiaBackend43(); // ! metodo que se llama al hacer click en el boton de emociones 4-3 del calendario
    void calClickEmoDiaBackend44(); // ! metodo que se llama al hacer click en el boton de emociones 4-4 del calendario
    void calClickEmoDiaBackend45(); // ! metodo que se llama al hacer click en el boton de emociones 4-5 del calendario
    void calClickEmoDiaBackend46(); // ! metodo que se llama al hacer click en el boton de emociones 4-6 del calendario
    void calClickEmoDiaBackend50(); // ! metodo que se llama al hacer click en el boton de emociones 5-0 del calendario
    void calClickEmoDiaBackend51(); // ! metodo que se llama al hacer click en el boton de emociones 5-1 del calendario
    void calClickEmoDiaBackend52(); // ! metodo que se llama al hacer click en el boton de emociones 5-2 del calendario
    void calClickEmoDiaBackend53(); // ! metodo que se llama al hacer click en el boton de emociones 5-3 del calendario
    void calClickEmoDiaBackend54(); // ! metodo que se llama al hacer click en el boton de emociones 5-4 del calendario
    void calClickEmoDiaBackend55(); // ! metodo que se llama al hacer click en el boton de emociones 5-5 del calendario
    void calClickEmoDiaBackend56(); // ! metodo que se llama al hacer click en el boton de emociones 5-6 del calendario
    void calClickEmoDiaBackend60(); // ! metodo que se llama al hacer click en el boton de emociones 6-0 del calendario
    void calClickEmoDiaBackend61(); // ! metodo que se llama al hacer click en el boton de emociones 6-1 del calendario
    void calClickEmoDiaBackend62(); // ! metodo que se llama al hacer click en el boton de emociones 6-2 del calendario
    void calClickEmoDiaBackend63(); // ! metodo que se llama al hacer click en el boton de emociones 6-3 del calendario
    void calClickEmoDiaBackend64(); // ! metodo que se llama al hacer click en el boton de emociones 6-4 del calendario
    void calClickEmoDiaBackend65(); // ! metodo que se llama al hacer click en el boton de emociones 6-5 del calendario
    void calClickEmoDiaBackend66(); // ! metodo que se llama al hacer click en el boton de emociones 6-6 del calendario
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // Relacionados la interfaz de dia seleccionado
    void calMostrarDiaSeleccionado(const int& dia); // ! muestra la interfaz para agregar un evento al calendario
    void calOcultarDiaSeleccionado(); // ! oculta la interfaz para agregar un evento al calendario
    void calCargarListaEventosDia(); // ! carga los eventos del dia seleccionado
    void calInterfazEvento(const int& apertura = 0); // ! muestra la interfaz para agregar un evento al calendario
    void calInterfazEventoNuevo(); // ! genera un json para un evento nuevo
    void calInterfazEventoCargarDatos(); // ! carga los datos de un evento
    void calOcultarInterfazEvento(); // ! oculta la interfaz para agregar un evento al calendario
    void calGuardarEvento(); // ! guarda un evento del calendario
    void calEliminarEvento(); // ! elimina un evento del calendario

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


signals:


protected:
    void resizeEvent(QResizeEvent *event) override;

};
#endif // MAINWINDOW_H

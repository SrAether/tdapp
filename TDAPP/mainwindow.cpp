#include "mainwindow.h"
#include <string>

// para debug de terminal
//#include <map>
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
#include <QVBoxLayout> // ? para acomodar los elementos de la interfaz VERTICALMENTE
#include <QHBoxLayout> // ? para acomodar los elementos de la interfaz HORIZONTALMENTE
#include <QtWidgets> // ? para usar las clases de widgets de qt

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
const QRegularExpression NO_VALIDO_EN_ARCHIVOS("[\\/:*?\"<>|]"); // ? caracteres no permitidos en archivos
const std::string RUTA_ICONOS = "./iconos/"; // ? ruta de los iconos


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
    //framePrincipal->setGeometry(0, 0, 1920, 1080);

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
    // ocultamos por defecto
    frameInicioSesion->hide();
    frameInicioSesion->setMinimumWidth(500);
    frameInicioSesion->setMinimumHeight(600);




    // en este punto se deberia cargar un estilo para el frame inicio de sesión
    //frameInicioSesion->setStyleSheet("background-color: #000000;"); // estilo para el frame

    // SELECTOR DE USUARIO
    // creamos un selector de usuario
    inSeSelectorUsuario = new QComboBox(frameInicioSesion);
    // establecemos la posición del selector
    inSeSelectorUsuario->setGeometry(100, 100, 200, 50);
    // establecemos un tamaño minimo
    inSeSelectorUsuario->setMinimumSize(200, 50);

    // CAMPO DE CONTRASEÑA
    // creamos un campo de contraseña
    inSeCampoContra = new QLineEdit(frameInicioSesion);
    // establecemos la posición del campo de contraseña
    inSeCampoContra->setGeometry(100, 200, 200, 50);
    // establecemos el texto del campo de contraseña
    inSeCampoContra->setPlaceholderText("Contraseña");

    // BOTÓN DE INICIAR SESIÓN
    // creamos un botón para iniciar sesión
    inSeBotonIniciarSesion = new QPushButton(frameInicioSesion);
    // establecemos la posición del botón de iniciar sesión
    inSeBotonIniciarSesion->setGeometry(100, 300, 200, 50);
    // establecemos el texto del botón de iniciar sesión
    inSeBotonIniciarSesion->setText("Iniciar Sesión");

    // BOTÓN DE REGISTRARSE
    // creamos un botón para registrarse
    inSeBotonRegistrarse = new QPushButton(frameInicioSesion);
    // establecemos la posición del botón de registrarse
    inSeBotonRegistrarse->setGeometry(100, 400, 200, 50);
    // establecemos el texto del botón de registrarse
    inSeBotonRegistrarse->setText("Registrarse");

    // BOTÓN DE RECUPERAR CONTRASEÑA
    // creamos un botón para recuperar contraseña
    inSeBotonRecuperarContra = new QPushButton(frameInicioSesion);
    // establecemos la posición del botón de recuperar contraseña
    inSeBotonRecuperarContra->setGeometry(100, 500, 200, 50);
    // establecemos el texto del botón de recuperar contraseña
    inSeBotonRecuperarContra->setText("Recuperar Contraseña");

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
    frameRegistroUsuario->setMinimumWidth(500);
    frameRegistroUsuario->setMinimumHeight(800);

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
    campoRegistroContra = new QLineEdit(frameRegistroUsuario);
    // establecemos la posición del campo para ingresar la contraseña
    campoRegistroContra->setGeometry(100, 200, 200, 50);
    // establecemos el texto del campo para ingresar la contraseña
    campoRegistroContra->setPlaceholderText("Contra");
    // establecemos que el campo sea de tipo contraseña
    campoRegistroContra->setEchoMode(QLineEdit::Password);

    // CAMPO PARA CONFIRMAR CONTRASEÑA
    // creamos un campo para confirmar la contraseña
    campoRegistroContraConf = new QLineEdit(frameRegistroUsuario);
    // establecemos la posición del campo para confirmar la contraseña
    campoRegistroContraConf->setGeometry(100, 250, 200, 50);
    // establecemos el texto del campo para confirmar la contraseña
    campoRegistroContraConf->setPlaceholderText("Confirmar Contra");
    // establecemos que el campo sea de tipo contraseña y que muestre la opción de mostrar la contraseña
    campoRegistroContraConf->setEchoMode(QLineEdit::Password);




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
    campoRegistroContraCopiaSeguridad = new QLineEdit(frameRegistroUsuario);
    // establecemos la posición del campo para ingresar la contraseña del correo electrónico
    campoRegistroContraCopiaSeguridad->setGeometry(100, 450, 200, 50);
    // establecemos el texto del campo para ingresar la contraseña del correo electrónico
    campoRegistroContraCopiaSeguridad->setPlaceholderText("Contra del Correo Electrónico");
    // establecemos que el campo sea de tipo contraseña
    campoRegistroContraCopiaSeguridad->setEchoMode(QLineEdit::Password);
    // ocultamos por defecto
    campoRegistroContraCopiaSeguridad->hide();

    // CAMPO PARA CONFIRMAR CONTRASEÑA DE CORREO ELECTRÓNICO (PARA COPIA DE SEGURIDAD)
    // creamos un campo para confirmar la contraseña del correo electrónico
    campoRegistroContraCopiaSeguridadConf = new QLineEdit(frameRegistroUsuario);
    // establecemos la posición del campo para confirmar la contraseña del correo electrónico
    campoRegistroContraCopiaSeguridadConf->setGeometry(100, 500, 200, 50);
    // establecemos el texto del campo para confirmar la contraseña del correo electrónico
    campoRegistroContraCopiaSeguridadConf->setPlaceholderText("Confirmar Contra del Correo Electrónico");
    // establecemos que el campo sea de tipo contraseña
    campoRegistroContraCopiaSeguridadConf->setEchoMode(QLineEdit::Password);
    // ocultamos por defecto
    campoRegistroContraCopiaSeguridadConf->hide();

    // BOTÓN PARA INGRESAR FOTO DE PERFIL (avatar)
    // creamos un botón para añadir la foto de perfil
    botonRegistroFotoPerfil = new QPushButton(frameRegistroUsuario);
    // establecemos la posición del botón para añadir la foto de perfil
    botonRegistroFotoPerfil->setGeometry(100, 550, 200, 50);
    // establecemos el texto del botón para añadir la foto de perfil
    botonRegistroFotoPerfil->setText("Añadir Foto de Perfil");


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

    // Campo para añadir pregunta de recuperación
    // creamos un campo para añadir la pregunta de recuperación
    campoRegistroPreguntaRecuperacion = new QLineEdit(frameRegistroUsuario);
    // establecemos la posición del campo para añadir la pregunta de recuperación
    campoRegistroPreguntaRecuperacion->setGeometry(300, 700, 200, 50);
    // establecemos el texto del campo para añadir la pregunta de recuperación
    campoRegistroPreguntaRecuperacion->setPlaceholderText("Pregunta de Recuperación");

    // Campo para añadir respuesta de recuperación
    // creamos un campo para añadir la respuesta de recuperación
    campoRegistroRespuestaRecuperacion = new QLineEdit(frameRegistroUsuario);
    // establecemos la posición del campo para añadir la respuesta de recuperación
    campoRegistroRespuestaRecuperacion->setGeometry(300, 750, 200, 50);
    // establecemos el texto del campo para añadir la respuesta de recuperación
    campoRegistroRespuestaRecuperacion->setPlaceholderText("Respuesta de Recuperación");

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

    // ----------------------------------------------------------------------------
    // Recuperar Contra NO COMPLETADO
    /* Debe contener:
     * un QComboBox para seleccionar el usuario
     * un QLabel para mostrar la pregunta de recuperación
     * un QLineEdit para responder la pregunta de recuperación
     * un botón para recuperar la contraseña
     * un botón para cancelar
     * mostrara un mensaje emergente con la contraseña
     */
    reCoPreguntaContra = "Hola"; // ? pregunta de recuperación de la contraseña
    frameRecuperarContra = new QFrame(framePrincipal);
    // ocultamos por defecto
    frameRecuperarContra->hide();
    frameRecuperarContra->setMinimumWidth(500);
    frameRecuperarContra->setMinimumHeight(800);

    // en este punto se deberia cargar un estilo para el frame recuperar contraseña
    //frameRecuperarContra->setStyleSheet("background-color: #000000;"); // estilo para el frame

    // SELECTOR DE USUARIO
    // creamos un selector de usuario
    reCoSelectorUsuario = new QComboBox(frameRecuperarContra);
    // establecemos la posición del selector
    reCoSelectorUsuario->setGeometry(100, 100, 200, 50);

    // PREGUNTA DE RECUPERACIÓN
    // creamos una etiqueta para la pregunta de recuperación
    reCoPreguntaRecuperacion = new QLabel{QString::fromStdString(reCoPreguntaContra), frameRecuperarContra};
    // establecemos la posición de la etiqueta de la pregunta de recuperación
    reCoPreguntaRecuperacion->setGeometry(400, 150, 1200, 300);
    // colocamos salto de linea automatico
    reCoPreguntaRecuperacion->setWordWrap(true);

    // CAMPO PARA RESPONDER PREGUNTA DE RECUPERACIÓN
    // creamos un campo para responder la pregunta de recuperación
    reCoCampoRespuesta = new QLineEdit(frameRecuperarContra);
    // establecemos la posición del campo para responder la pregunta de recuperación
    reCoCampoRespuesta->setGeometry(100, 200, 200, 50);
    // establecemos el texto del campo para responder la pregunta de recuperación
    reCoCampoRespuesta->setPlaceholderText("Respuesta de Recuperación");


    // BOTÓN PARA RECUPERAR CONTRASEÑA
    // creamos un botón para recuperar la contraseña
    reCoBotonConfirmar = new QPushButton(frameRecuperarContra);
    // establecemos la posición del botón para recuperar la contraseña
    reCoBotonConfirmar->setGeometry(100, 250, 200, 50);
    // establecemos el texto del botón para recuperar la contraseña
    reCoBotonConfirmar->setText("Recuperar Contraseña");

    // BOTÓN PARA CANCELAR
    // creamos un botón para cancelar
    reCoBotonCancelar = new QPushButton(frameRecuperarContra);
    // establecemos la posición del botón para cancelar
    reCoBotonCancelar->setGeometry(100, 300, 200, 50);
    // establecemos el texto del botón para cancelar
    reCoBotonCancelar->setText("Cancelar");





    // -----------------------------------------------------------------------------
    // JOURNALING NO COMPLETADO
    /* Debe contener:
     * una pantalla de bienvenida <- contiene una imagen seleccionada por el usuario y un boton para continuar
     * una pantalla con la lista de entrada de journaling
     * ... (continuar)
     */

    // creamos un frame para el journaling
    frameJournaling = new QFrame();
    // ocultamos por defecto
    frameJournaling->hide();
    //frameJournaling->setMinimumWidth(400);
    //frameJournaling->setMaximumWidth(2000);
    //frameJournaling->setMinimumHeight(400);
    //frameJournaling->setMaximumHeight(2000);
    frameJournaling->adjustSize(); // ? ajustamos el tamaño del frame

    // Iconos para la barra de navegación del journaling
    jourIconoActivarListaEntradas = new QIcon(QString(RUTA_ICONOS.c_str()) + "activar_lista_entradas.png");
    jourIconoEditarEntrada = new QIcon(QString(RUTA_ICONOS.c_str()) + "editar_entrada.png");
    jourIconoNuevaEntrada = new QIcon(QString(RUTA_ICONOS.c_str()) + "nueva_entrada.png");
    jourIconoEliminarEntrada = new QIcon(QString(RUTA_ICONOS.c_str()) + "eliminar_entrada.png");
    jourIconoGuardarEntrada = new QIcon(QString(RUTA_ICONOS.c_str()) + "guardar_entrada.png");



    // creamos un boton de bienvenida
    jourBotonCambiarImagenBienvenida = new QPushButton(frameJournaling);
    // establecemos la posición del boton de bienvenida
    //jourBotonCambiarImagenBienvenida->setGeometry(0,0,1000,1000);
    //jourBotonCambiarImagenBienvenida->setMinimumWidth(400);
    //jourBotonCambiarImagenBienvenida->setMaximumWidth(2000);
    //jourBotonCambiarImagenBienvenida->setMinimumHeight(400);
    //jourBotonCambiarImagenBienvenida->setMaximumHeight(2000);
    jourBotonCambiarImagenBienvenida->adjustSize();

    // /=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=
    // Relacionado con la Lista de Journaling
    // Label para título de la lista de notas
    jourLiNoTitulo = new QLabel(frameJournaling);
    jourLiNoTitulo->setText("Lista de Notas");
    jourLiNoTitulo->hide();
    // Lista de notas
    jourLiNoListaNotas = new QListWidget(frameJournaling);
    jourLiNoListaNotas->hide();
    // Area de scroll para la lista de notas
    jourLiNoScrollArea = new QScrollArea(frameJournaling);
    jourLiNoScrollArea->setWidget(jourLiNoListaNotas);
    jourLiNoScrollArea->setWidgetResizable(true);
    jourLiNoScrollArea->setMinimumWidth(500);
    jourLiNoScrollArea->hide();
    // Layout para la lista de notas
    jourLiNoLayout = new QVBoxLayout(frameJournaling);
    jourLiNoLayout->addWidget(jourLiNoTitulo);
    jourLiNoLayout->addWidget(jourLiNoScrollArea, 1);
    jourLiNoLayout->addWidget(jourBotonCambiarImagenBienvenida);

    // /=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=
    // Relacionado con la edicion de entradas de journaling
    // campo para el título de la entrada
    jourReEnCampoTitulo = new QLineEdit(frameJournaling);
    jourReEnCampoTitulo->hide();
    jourReEnCampoTitulo->setPlaceholderText("Título de la entrada");
    //jourReEnCampoTitulo->setMinimumSize(600, 100);
    // campo para el texto de la entrada
    jourReEnCampoTexto = new QTextEdit(frameJournaling);
    jourReEnCampoTexto->hide();
    //jourReEnCampoTexto->setMinimumSize(600, 400);
    // layout para la entrada
    // jourReEnLayout = new QVBoxLayout(frameJournaling);
    // jourReEnLayout->addWidget(jourReEnCampoTexto, 1);
    // jourReEnLayout->addWidget(jourReEnCampoTitulo);
    jourLiNoLayout->addWidget(jourReEnCampoTitulo);
    jourLiNoLayout->addWidget(jourReEnCampoTexto, 1);




    // -----------------------------------------------------------------------------
    // BARRA DE NAVEGACIÓN NO COMPLETADO
    /* Debe contener:
     * 3 secciones: Izquierda, Centro, Derecha
     * En la sección izquierda:
     * Boton de configuración
     * En la sección centro:
     * Boton de calendario
     * Boton Registro de emociones
     * Boton Journaling
     * Boton Hiperfoco
     * En la sección derecha:
     * Botones dependientes de la sección actual (configuración, calendario, registro de emociones, journaling, hiperfoco)
     */

    // Creamos un frame para la barra de navegación
    frameBarraNavegacion = new QFrame();
    // ocultamos por defecto
    frameBarraNavegacion->hide();
    // Tamaño de boton
    barNaTamBotones = QSize(50, 50);
    // Icono de configuración
    barNaIconoConfiguracion = new QIcon(QString(RUTA_ICONOS.c_str()) + "configuraciones.svg");
    // Icono de calendario
    barNaIconoCalendario = new QIcon(QString(RUTA_ICONOS.c_str()) + "calendario.svg");
    // Icono de registro de emociones
    barNaIconoRegistroEmociones = new QIcon(QString(RUTA_ICONOS.c_str()) + "registro_emociones.svg");
    // Icono de journaling
    barNaIconoJournaling = new QIcon(QString(RUTA_ICONOS.c_str()) + "journaling.png");
    // Icono de hiperfoco
    barNaIconoHiperfoco = new QIcon(QString(RUTA_ICONOS.c_str()) + "hiperfoco.png");
    // Botón de configuración
    barNaBotonConfiguracion = new QPushButton(frameBarraNavegacion);
    barNaBotonConfiguracion->setIcon(*barNaIconoConfiguracion);
    barNaBotonConfiguracion->setIconSize(barNaTamBotones);
    barNaBotonConfiguracion->setToolTip("Configuración");
    // Botón de calendario
    barNaBotonCalendario = new QPushButton(frameBarraNavegacion);
    barNaBotonCalendario->setToolTip("Calendario");
    barNaBotonCalendario->setIcon(*barNaIconoCalendario);
    barNaBotonCalendario->setIconSize(barNaTamBotones);
    // Botón Registro de emociones
    barNaBotonRegistroEmociones = new QPushButton(frameBarraNavegacion);
    barNaBotonRegistroEmociones->setToolTip("Registro de Emociones");
    barNaBotonRegistroEmociones->setIcon(*barNaIconoRegistroEmociones);
    barNaBotonRegistroEmociones->setIconSize(barNaTamBotones);
    // Botón Journaling
    barNaBotonJournaling = new QPushButton(frameBarraNavegacion);
    barNaBotonJournaling->setToolTip("Journaling");
    barNaBotonJournaling->setIcon(*barNaIconoJournaling);
    barNaBotonJournaling->setIconSize(barNaTamBotones);
    // Botón Hiperfoco
    barNaBotonHiperfoco = new QPushButton(frameBarraNavegacion);
    barNaBotonHiperfoco->setToolTip("Hiperfoco");
    barNaBotonHiperfoco->setIcon(*barNaIconoHiperfoco);
    barNaBotonHiperfoco->setIconSize(barNaTamBotones);
    // Botón para sección actual 0
    barNaBotonSeccionActual0 = new QPushButton(frameBarraNavegacion);
    barNaBotonSeccionActual0->hide();
    barNaBotonSeccionActual0->setIconSize(barNaTamBotones);
    // Botón para sección actual 1
    barNaBotonSeccionActual1 = new QPushButton(frameBarraNavegacion);
    barNaBotonSeccionActual1->hide();
    barNaBotonSeccionActual1->setIconSize(barNaTamBotones);
    // Botón para sección actual 2
    barNaBotonSeccionActual2 = new QPushButton(frameBarraNavegacion);
    barNaBotonSeccionActual2->hide();
    barNaBotonSeccionActual2->setIconSize(barNaTamBotones);

    // /-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/
    // ACOMODO DE LOS ELEMENTOS DE LA INTERFAZ CON LAYOUTS

    // Sección izquierda
    QHBoxLayout* disposicionIzquierda = new QHBoxLayout();
    disposicionIzquierda->addWidget(barNaBotonConfiguracion);
    // Sección centro
    QHBoxLayout* disposicionCentro = new QHBoxLayout();
    disposicionCentro->addWidget(barNaBotonCalendario);
    disposicionCentro->addWidget(barNaBotonRegistroEmociones);
    disposicionCentro->addWidget(barNaBotonJournaling);
    disposicionCentro->addWidget(barNaBotonHiperfoco);
    // Sección derecha
    QHBoxLayout* disposicionDerecha = new QHBoxLayout();
    disposicionDerecha->addWidget(barNaBotonSeccionActual0);
    disposicionDerecha->addWidget(barNaBotonSeccionActual1);
    disposicionDerecha->addWidget(barNaBotonSeccionActual2);
    // Layout principal (MENU)
    QHBoxLayout* disposicionMenu = new QHBoxLayout(frameBarraNavegacion);
    disposicionMenu->addLayout(disposicionIzquierda);
    // colocamos un espacio entre la sección izquierda y la sección central
    disposicionMenu->addStretch();
    disposicionMenu->addLayout(disposicionCentro);
    // colocamos un espacio entre la sección central y la sección derecha
    disposicionMenu->addStretch();
    disposicionMenu->addLayout(disposicionDerecha);



    // -----------------------------------------------------------------------------
    // CALENDARIO NO COMPLETADO
    /* Debe contener:
     * un calendario
     * una pantalla de dia seleccionado donde se muestren las entradas de ese día
     * una pantalla de evento seleccionado donde se muestre la información del evento
     * una pantalla de evento nuevo donde se pueda añadir un evento nuevo
     * una pantalla de editar evento donde se pueda editar un evento
     */

    // creamos un frame para el calendario
    frameCalendario = new QFrame(framePrincipal);
    // ocultamos por defecto
    frameCalendario->hide();

    // /=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=
    // Relacionado con el calendario en sí
    // creamos un calendario
    calCalendario = new QCalendarWidget(frameCalendario);
    calCalendario->hide();
    calCalendario->setMinimumSize(500, 500);
    // añadimos un evento al calendario para el dia 10 de mayo del 2024
    QDate fechaEvento = QDate(2024, 5, 10);
    QTextCharFormat formatoEvento;
    formatoEvento.setBackground(Qt::red);
    formatoEvento.setForeground(Qt::white);
    formatoEvento.setFontWeight(QFont::Bold);
    calCalendario->setDateTextFormat(fechaEvento, formatoEvento);
    // creamos un evento para el dia 12 de mayo del 2024 con el texto "Evento 2" que se mostrará en el calendario
    QDate fechaEvento2 = QDate(2024, 5, 12);
    QTextCharFormat formatoEvento2;
    formatoEvento2.setBackground(Qt::blue);
    formatoEvento2.setForeground(Qt::white);
    formatoEvento2.setFontWeight(QFont::Bold);
    calCalendario->setDateTextFormat(fechaEvento2, formatoEvento2);







    // creamos un layout para el calendario
    calLayout = new QVBoxLayout(frameCalendario);
    calLayout->addWidget(calCalendario);




    // -----------------------------------------------------------------------------
    // AREA DE ACOMODO DE FRAMES

    QVBoxLayout* disposicionPrincipal = new QVBoxLayout(this);

    disposicionPrincipal->addWidget(frameBarraNavegacion);
    // hacemos que los elementos se acomoden de arriba hacía abajo desde la parte superior sin centrar
    //disposicionPrincipal->addStretch();
    disposicionPrincipal->addWidget(frameInicioSesion);
    disposicionPrincipal->addWidget(frameRegistroUsuario);
    disposicionPrincipal->addWidget(frameRecuperarContra);
    disposicionPrincipal->addWidget(frameJournaling);
    disposicionPrincipal->addWidget(frameCalendario);











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
    else
    {
        std::cout << "Hay usuarios" << std::endl;
        // * abrimos la ventana de inicio de sesión
        activarInterfazInicioSesion(); // * abrimos la ventana de inicio de sesión
        // * cerramos la ventana de registro de usuarios
        desactivarInterfazRegistroUsuario(); // * cerramos la ventana de registro de usuarios
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
    connect(inSeBotonRegistrarse, SIGNAL(clicked()), this, SLOT(inSeRegistrarUsuarioNuevo()));
    // conectamos la señal de iniciar sesión
    connect(inSeBotonIniciarSesion, SIGNAL(clicked()), this, SLOT(inSeIniciarSesion()));
    // conectamos la señal de recuperar contraseña
    connect(inSeBotonRecuperarContra, SIGNAL(clicked()), this, SLOT(inSeRecuperarContra()));

    // SEÑALES RELACIONADAS CON EL RECUPERAR CONTRASEÑA
    // conectamos la señal de qcombobox con el metodo reCoCargarDatosUsuario
    connect(reCoSelectorUsuario, SIGNAL(currentIndexChanged(int)), this, SLOT(reCoCargarDatosUsuario()));
    // conectamos la señal de recuperar contraseña
    connect(reCoBotonConfirmar, SIGNAL(clicked()), this, SLOT(reCoRecuperarContra()));
    // conectamos la señal de cancelar
    connect(reCoBotonCancelar, SIGNAL(clicked()), this, SLOT(reCoCancelarRecuperarContra()));

    // SEÑALES RELACIONADAS CON LA BARRA DE NAVEGACIÓN
    // conectamos la señal para activar el frame de calendario
    connect(barNaBotonCalendario, SIGNAL(clicked()), this, SLOT(barNaMostrarCalendario()));
    // conectamos la señal para activar el frame de journaling
    connect(barNaBotonJournaling, SIGNAL(clicked()), this, SLOT(barNaMostrarJournaling()));
    // conectamos la señal del boton 1 de la barra de navegación
    connect(barNaBotonSeccionActual0, SIGNAL(clicked()), this, SLOT(barNaBotonSeccionActual0Click()));
    // conectamos la señal del boton 2 de la barra de navegación
    connect(barNaBotonSeccionActual1, SIGNAL(clicked()), this, SLOT(barNaBotonSeccionActual1Click()));
    // conectamos la señal del boton 3 de la barra de navegación
    connect(barNaBotonSeccionActual2, SIGNAL(clicked()), this, SLOT(barNaBotonSeccionActual2Click()));

    // SEÑALES RELACIONADAS CON EL JOURNALING
    // conectamos la señal para cambiar la imagen de bienvenida
    connect(jourBotonCambiarImagenBienvenida, SIGNAL(clicked()), this, SLOT(jourCambiarImagenBienvenida()));
    // conectamos la señal de click en una entrada de journaling
    connect(jourLiNoListaNotas, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(jourLiNoCargarEntrada()));


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
    // ? carga los usuarios en el selector de usuarios
    inSeCargarUsuarios();
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
    //campoContra->hide();
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

// ! método para cargar los usuarios en el selector de usuarios
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::inSeCargarUsuarios()
{
    // recargamos el arreglo de usuarios
    usuarios = manejadorArchivos.obtenerContenidoCarpeta(RUTA_USUARIOS, 1);
    // * los usuarios se encuentran en el vector usuarios (fueron agregados en el metodo verificarExistenciaArchivoUsuario)
    // * se cargan los usuarios en el selector de usuarios
    // se limpia el selector de usuarios
    inSeSelectorUsuario->clear();
    // ? se cargan los usuarios en el selector de usuarios
    for (const std::string& usuario : usuarios)
    {
        inSeSelectorUsuario->addItem(QString::fromStdString(usuario));
    }

}

// ! método para iniciar sesión
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::inSeIniciarSesion()
{
    // verificacamos que todos los campos estén llenos
    if (inSeSelectorUsuario->currentText().isEmpty() || inSeCampoContra->text().isEmpty())
    {
        // ? si los campos están vacíos, se mostrará un mensaje emergente que indica que los campos no pueden estar vacios
        QMessageBox::critical(this, "Error", "Los campos no pueden estar vacíos");
        return;
    }
    // ? se obtienen los datos del formulario de inicio de sesión
    std::string usuario = inSeSelectorUsuario->currentText().toStdString();
    std::string contraseña = inSeCampoContra->text().toStdString();
    // mostramos en terminal los datos
    std::cout << "Usuario: " << usuario << std::endl;
    std::cout << "Contra: " << contraseña << std::endl;
    // ? verificamos que el usuario exista
    // ? verificamos que la contraseña sea correcta
    // ? si el usuario y la contraseña son correctos, se activará el frame Journaling
    // ? si el usuario y la contraseña son incorrectos, se mostrará un mensaje emergente que indica que los datos son incorrectos

    // ? se verifica que el usuario exista
    if (buscarUsuario(usuario) == -1)
    {
        // ? si el usuario no existe, se mostrará un mensaje emergente que indica que el usuario no existe
        QMessageBox::critical(this, "Error", "El usuario no existe");
        return;
    }
    // ? se verifica que la contraseña sea correcta
    // ? se obtiene la contraseña del usuario (se encuentra en el archivo de configuraciones del usuario)
    //mJson::ManejadorJson configuracionesUsuario(RUTA_USUARIOS + usuario + "/config.json");
    configuracionesUsuario = new mJson::ManejadorJson(RUTA_USUARIOS + usuario + "/config.json");
    // verificamos si el manejador de configuraciones del usuario está vacío
    if (configuracionesUsuario->vacio())
    {
        // si está vacío, se muestra un mensaje emergente que indica que el usuario no existe
        QMessageBox::critical(this, "Error", "El usuario no existe");
        return;
    }
    std::string contraseñaUsuarioEncriptada = (*configuracionesUsuario)["contraseña"];
    // ? se desencripta la contraseña
    std::string contraseñaUsuario = encriptado->desencriptar(contraseñaUsuarioEncriptada);

    // ? se verifica que la contraseña sea correcta
    if (contraseña != contraseñaUsuario)
    {
        // ? si la contraseña es incorrecta, se mostrará un mensaje emergente que indica que la contraseña es incorrecta
        QMessageBox::critical(this, "Error", "La contraseña es incorrecta");
        return;
    }
    // ? si el usuario y la contraseña son correctos, se activará el frame Journaling
    // ? se desactiva el frame de inicio de sesión
    desactivarInterfazInicioSesion();
    // ? se activa el frame de journaling
    activarInterfazJournaling();
    // ? se activa la barra de navegación
    activarBarraNavegacion();

}

// ! método para recuperar la contraseña
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::inSeRecuperarContra()
{
    // ? se desactiva el frame de inicio de sesión
    desactivarInterfazInicioSesion();

    // ? se activa el frame de recuperar contraseña
    activarInterfazRecuperarContra();
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
    std::string contraseña = campoRegistroContra->text().toStdString();
    std::string contraseñaConf = campoRegistroContraConf->text().toStdString();
    std::string correo = campoRegistroCorreoCopiaSeguridad->text().toStdString();
    std::string contraseñaCorreo = campoRegistroContraCopiaSeguridad->text().toStdString();
    std::string contraseñaCorreoConf = campoRegistroContraCopiaSeguridadConf->text().toStdString();
    std::string preguntaRecuperacion = campoRegistroPreguntaRecuperacion->text().toStdString();
    std::string respuestaRecuperacion = campoRegistroRespuestaRecuperacion->text().toStdString();

    // mostramos en terminal los datos
    std::cout << "Nombre: " << nombre << std::endl;
    std::cout << "Nombre de Usuario: " << nombreUs << std::endl;
    std::cout << "Contra: " << contraseña << std::endl;
    std::cout << "Confirmación de Contra: " << contraseñaConf << std::endl;
    std::cout << "Realizar Copia de Seguridad: " << banderaRegistroCopiaSeg << std::endl;
    std::cout << "Ruta foto de perfil:" << rutaRegistroFotoPerfil << std::endl;
    std::cout << "Edad: " << selectorRegistroEdad->currentText().toStdString() << std::endl;
    std::cout << "Pregunta de Recuperación: " << preguntaRecuperacion << std::endl;
    std::cout << "Respuesta de Recuperación: " << respuestaRecuperacion << std::endl;

    // estos datos son opcionales
    std::cout << "Correo Electrónico: " << correo << std::endl;
    std::cout << "Contra de Correo Electrónico: " << contraseñaCorreo << std::endl;
    std::cout << "Confirmación de Contra de Correo Electrónico: " << contraseñaCorreoConf << std::endl;
    std::cout << "Frecuencia de Copia de Seguridad: " << selectorRegistroFrecuenciaCopiaSeguridad->currentText().toStdString() << std::endl;



    // ? se verificará que los datos sean correctos
    // ? se verificará que los campos no estén vacíos
    if (nombre.empty() || nombreUs.empty() || contraseña.empty() || contraseñaConf.empty() || rutaRegistroFotoPerfil.empty() || preguntaRecuperacion.empty() || respuestaRecuperacion.empty())
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
    //usuario["edad"] = selectorRegistroEdad->currentText().toStdString();
    usuario["edad"] = encriptado->encriptar(selectorRegistroEdad->currentText().toStdString());
    //usuario["preguntaRecuperacion"] = preguntaRecuperacion;
    usuario["preguntaRecuperacion"] = encriptado->encriptar(preguntaRecuperacion);
    //usuario["respuestaRecuperacion"] = respuestaRecuperacion;
    usuario["respuestaRecuperacion"] = encriptado->encriptar(respuestaRecuperacion);
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
        campoRegistroContraCopiaSeguridad->show();
        campoRegistroContraCopiaSeguridadConf->show();
        // ? cambiamos el texto del botón
        botonRegistroCopiaSeguridad->setText("Da click para desactivar copia de seguridad");
    }
    else
    {
        selectorRegistroFrecuenciaCopiaSeguridad->hide();
        campoRegistroCorreoCopiaSeguridad->hide();
        campoRegistroContraCopiaSeguridad->hide();
        campoRegistroContraCopiaSeguridadConf->hide();
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
// ! RELACIONADOS CON LA RECUPERACIÓN DE CONTRASEÑA

// ! método para activar el frame de recuperar contraseña
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::activarInterfazRecuperarContra()
{
    // ? se activa el frame de recuperar contraseña
    frameRecuperarContra->show();
    // ? se cargan los usuarios en el selector de usuarios
    reCoCargarUsuarios();
}

// ! método para desactivar el frame de recuperar contraseña
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::desactivarInterfazRecuperarContra()
{
    // ? se desactiva el frame de recuperar contraseña
    frameRecuperarContra->hide();
}

// ! método para cargar usuarios en el selector de usuarios de reco
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::reCoCargarUsuarios()
{
    // primero vaciamos el qcombobox
    reCoSelectorUsuario->clear();
    // cargamos los usuarios en el arreglo de usuarios
    std::cout << "Cargando usuarios" << std::endl;
    usuarios = manejadorArchivos.obtenerContenidoCarpeta(RUTA_USUARIOS, 1);
    std::cout << "Usuarios cargados" << std::endl;
    // cargamos los usuarios en el selector de usuarios
    for (const std::string& usuario : usuarios)
    {
        reCoSelectorUsuario->addItem(QString::fromStdString(usuario));
    }
}

// ! método para cargar datos de usuario
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::reCoCargarDatosUsuario()
{
    // mostramos una pregunta de recuperación de prueba
    //reCoPreguntaRecuperacion->setText("¿Cuál es tu color favorito?");

    // creamos un objeto de tipo mJson::ManejadorJson para cargar las configuraciones del usuario
    // siempre que el usuario sea diferente de ""
    if (reCoSelectorUsuario->currentText().isEmpty())
    {
        return;
    }
    configuracionesUsuario = new mJson::ManejadorJson(RUTA_USUARIOS + reCoSelectorUsuario->currentText().toStdString() + "/config.json");
    // cargamos la pregunta de recuperación en un string
    std::string preguntaRecuperacion = (*configuracionesUsuario)["preguntaRecuperacion"];
    //std::cout << "Pregunta de recuperación: " << preguntaRecuperacion << std::endl;
    //std::cout << "Tamaño de la pregunta de recuperación: " << preguntaRecuperacion.size() << std::endl;
    // desencriptamos la pregunta de recuperación
    std::string preguntaRecuperacionDe = encriptado->desencriptar(preguntaRecuperacion);
    //std::cout << "Pregunta de recuperación desencriptada: " << preguntaRecuperacionDe << std::endl;
    // mostramos la pregunta de recuperación en el label
    reCoPreguntaRecuperacion->setText(QString::fromStdString(preguntaRecuperacionDe));

}

// ! método para recuperar la contraseña
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::reCoRecuperarContra()
{
    // extraemos los datos del formulario
    std::string respuesta = reCoCampoRespuesta->text().toStdString();
    // verificamos que la respuesta no esté vacía
    if (respuesta.empty())
    {
        // si la respuesta está vacía, se mostrará un mensaje emergente
        QMessageBox::critical(this, "Error", "La respuesta no puede estar vacía");
        return;
    }
    // verificamos que la respuesta sea correcta
    // se obtiene la respuesta del archivo de configuraciones del usuario
    std::string respuestaUsuarioEncriptada = (*configuracionesUsuario)["respuestaRecuperacion"];
    // se desencripta la respuesta
    std::string respuestaUsuario = encriptado->desencriptar(respuestaUsuarioEncriptada);
    // se verifica que la respuesta sea correcta
    if (respuesta != respuestaUsuario)
    {
        // si la respuesta es incorrecta, se mostrará un mensaje emergente
        QMessageBox::critical(this, "Error", "La respuesta es incorrecta");
        return;
    }
    // si la respuesta es correcta, se mostrará un mensaje emergente con la contraseña
    // se obtiene la contraseña del archivo de configuraciones del usuario
    std::string contraseñaUsuarioEncriptada = (*configuracionesUsuario)["contraseña"];
    // se desencripta la contraseña
    std::string contraseñaUsuario = encriptado->desencriptar(contraseñaUsuarioEncriptada);
    // se muestra un mensaje emergente con la contraseña
    QMessageBox::information(this, "Recuperación de Contraseña", "La contraseña es: " + QString::fromStdString(contraseñaUsuario));
    // se desactiva el frame de recuperar contraseña
    desactivarInterfazRecuperarContra();
    // se activa el frame de inicio de sesión
    activarInterfazInicioSesion();

}

// ! método para cancelar la recuperación de contraseña
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::reCoCancelarRecuperarContra()
{
    // ? se desactiva el frame de recuperar contraseña
    desactivarInterfazRecuperarContra();

    // ? se activa el frame de inicio de sesión
    activarInterfazInicioSesion();
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


// ////////////////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------------------------
// ! RELACIONADOS CON EL JOURNALING

// ! método para activar el frame de journaling
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::activarInterfazJournaling()
{
    std::cout << "Activando interfaz de journaling" << std::endl;
    // ? se desactiva todo lo relacionado con el journaling
    jourOcultarPantallaBienvenida();
    jourDesactivarListaEntradas();
    jourDesactivarEditarEntrada();
    // ? se activa el frame de journaling
    frameJournaling->show();
    mostrarPantallaBienvenidaJournaling();
}

// ! método para desactivar el frame de journaling
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::desactivarInterfazJournaling()
{
    // ? se desactiva el frame de journaling
    frameJournaling->hide();
    // ? se desactivan los botones de la barra de navegación
    barNaLimpiarBotonesSeccionActual();
}

// ! método para mostrar la pantalla de bienvenida
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::mostrarPantallaBienvenidaJournaling()
{
    // ? se mostrará la pantalla de bienvenida
    // ? se mostrará una imagen seleccionada por el usuario
    // ? se mostrará un botón para continuar
    // verificamos que existe la imagen de bienvenida
    //std::string rutaImagenBienvenida = RUTA_USUARIOS + (*configuracionesUsuario)["nombreUsuario"] + "/imagenBienvenida.png";
    // usuario descifrado
    std::string usuario = encriptado->desencriptar((*configuracionesUsuario)["nombreUsuario"]);
    // obtenemos la extension del archivo
    std::string extension = manejadorArchivos.buscarExtensionArchivo(RUTA_USUARIOS + usuario + "/", "imagenBienvenida");
    // generamos la ruta de la imagen de bienvenida
    std::string rutaImagenBienvenida = RUTA_USUARIOS + usuario + "/imagenBienvenida" + extension;
    std::cout << "Ruta de la imagen de bienvenida: " << rutaImagenBienvenida << std::endl;
    //std::cout << "Ruta de la imagen de bienvenida: " << rutaImagenBienvenida << std::endl;
    // verificamos que exista la imagen de bienvenida
    if (manejadorArchivos.verificarExistenciaDeArchivo(rutaImagenBienvenida))
    {
        // si existe la imagen de bienvenida, la asignamos al boton de bienvenida
        jourBotonCambiarImagenBienvenida->setIcon(QIcon(QPixmap(QString::fromStdString(rutaImagenBienvenida))));
        // redimensionamos la imagen
        jourPanBiRedimensionarIconoBoton();
    }
    else
    {
        // si no existe la imagen de bienvenida, se llama al metodo para seleccionar la imagen de bienvenida
        jourCambiarImagenBienvenida();
    }
    jourBotonCambiarImagenBienvenida->show();
    // activamos el primer boton de la barra de navegacion
    barNaConfigurarBotones(0, true, 0, "Continuar");
}

// ! método para ocultar la pantalla de bienvenida
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::jourOcultarPantallaBienvenida()
{
    // ? se ocultará la pantalla de bienvenida
    // ? se ocultará la imagen de bienvenida
    // ? se ocultará el botón de continuar

    jourBotonCambiarImagenBienvenida->hide();
    // desactivamos el primer boton de la barra de navegacion
    //barNaConfigurarBotones(0, false, -1, "");
    barNaLimpiarBotonesSeccionActual();
}

// ! método para seleccionar la imagen de bienvenida
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::jourCambiarImagenBienvenida()
{
    // mostramos en consola el mensaje
    std::cout << "Seleccionando Imagen de Bienvenida" << std::endl;
    //std::string rutaImagenBienvenida = RUTA_USUARIOS + (*configuracionesUsuario)["nombreUsuario"] + "/imagenBienvenida.png";
    // usuario descifrado
    std::string usuario = encriptado->desencriptar((*configuracionesUsuario)["nombreUsuario"]);
    std::string rutaImagenBienvenida = RUTA_USUARIOS + usuario + "/imagenBienvenida";
    std::string rutaTemp;

    //std::cout << "Ruta de la imagen de bienvenida: " << rutaImagenBienvenida << std::endl;
    // ? se seleccionará la imagen de bienvenida
    // ? se obtendrá la ruta de la imagen de bienvenida
    // ? se mostrará un mensaje emergente que indica que la imagen de bienvenida se ha seleccionado
    // ? se obtendrá la ruta de la imagen de bienvenida
    seleccionarArchivo(rutaTemp, "Imagen (*.png *.jpg *.jpeg)", "Seleccionar Imagen de Bienvenida");
    std::cout << "Ruta Temporal: " << rutaTemp << std::endl;
    // extraemos la extension del archivo
    std::string extension = manejadorArchivos.obtenerExtension(rutaTemp);
    std::cout << "Extension: " << extension << std::endl;
    // Bloque de verificación de existencia de la imagen de bienvenida
    // ? obtenemos la extension de la imagen de bienvenida
    std::string extensionImEx = manejadorArchivos.buscarExtensionArchivo(RUTA_USUARIOS + usuario + "/", "imagenBienvenida");
    std::cout << "Extension ImEx: " << extensionImEx << std::endl;
    // si la imagen de bienvenida ya existe la extension no será vacía
    if (!extensionImEx.empty())
    {
        // si ya existe una imagen de bienvenida, la renombramos a imagenBienvenidaAnterior
        manejadorArchivos.moverArchivo(RUTA_USUARIOS + usuario + "/imagenBienvenidaAnterior" + extensionImEx, rutaImagenBienvenida + extensionImEx);
    }
    // copiamos la imagen a la carpeta del usuario
    try {
         manejadorArchivos.copiarArchivo(rutaImagenBienvenida + extension, rutaTemp);
    }
    catch (const std::exception& e)
    {
        // si hay un error, restauramos la imagen anterior
        manejadorArchivos.moverArchivo(rutaImagenBienvenida + extensionImEx, rutaImagenBienvenida + "Anterior" + extensionImEx);
    }
    // mostramos la imagen en el boton
    jourBotonCambiarImagenBienvenida->setIcon(QIcon(QPixmap(QString::fromStdString(rutaImagenBienvenida + extension))));
    // eliminamos la imagen anterior
    manejadorArchivos.eliminarArchivo(rutaImagenBienvenida + "Anterior" + extensionImEx);
}

// ! método para activar la lista de entradas de journaling
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::jourActivarListaEntradas()
{
    // ? se desactiva la pantalla de bienvenida
    jourOcultarPantallaBienvenida();
    // ? se activará la lista de entradas de journaling
    jourDesactivarEditarEntrada();
    // modifcamos el tamaño de la ventana
    //this->resize(400, 400);

    // ? se activará la lista de entradas de journaling
    jourLiNoTitulo->show();
    jourLiNoListaNotas->show();
    jourLiNoScrollArea->show();
    // ? se actualizará la lista de entradas
    jourActualizarListaEntradas();
    // ? se activará el botón de nueva entrada
    barNaConfigurarBotones(0, true, 1, "Nueva Entrada");

}

// ! método para desactivar la lista de entradas de journaling
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::jourDesactivarListaEntradas()
{
    jourLiNoTitulo->hide();
    jourLiNoListaNotas->hide();
    jourLiNoScrollArea->hide();
    // ? se desactivará el botón de nueva entrada
    //barNaConfigurarBotones(0, false, -1, "");
    barNaLimpiarBotonesSeccionActual();
}

// ! método para actualizar la lista de entradas del journaling
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::jourActualizarListaEntradas()
{
    // limpia la lista de notas
    jourLiNoListaNotas->clear();
    // dentro de un try catch
    try {
        std::string rutaEntradasJournaling = RUTA_USUARIOS + encriptado->desencriptar((*configuracionesUsuario)["nombreUsuario"]) + "/journaling";
        // verificamos que exista la carpeta de notas
        if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaEntradasJournaling))
        {
            // si no existe la carpeta de notas, se crea
            manejadorArchivos.crearCarpeta(rutaEntradasJournaling);
        }
        // cargamos las notas en un arreglo
        std::vector<std::string> notas = manejadorArchivos.obtenerContenidoCarpeta(rutaEntradasJournaling, 1);
        // recorremos el arreglo de notas
        for (const std::string& nota : notas)
        {
            // agregamos la nota a la lista de notas
            jourLiNoListaNotas->addItem(QString::fromStdString(nota));
        }
    } catch (const std::exception &e) {
        // si hay un error, se mostrará un mensaje emergente
        QMessageBox::critical(this, "Error", e.what());
    }
}

// ! método para crear una nueva entrada en el journaling
// ! versión 1.0
// ! modificado por Aether
void MainWindow::jourLiNoNuevaEntrada()
{
    bool ok;
    QString nombreEntrada = QInputDialog::getText(this, "Nueva Entrada", "Nombre de la Entrada:", QLineEdit::Normal, "", &ok);
    try {
        if (nombreEntrada.contains(NO_VALIDO_EN_ARCHIVOS))
        {
            QMessageBox::critical(this, "Error", "El nombre de la entrada no puede contener los siguientes caracteres: \\ / : * ? \" < > |");
            return;
        }
        if (ok && !nombreEntrada.isEmpty())
        {
            std::string rutaEntradaJournaling = RUTA_USUARIOS + encriptado->desencriptar((*configuracionesUsuario)["nombreUsuario"]) + "/journaling/" + nombreEntrada.toStdString();
            // Creamos la carpeta de la entrada
            manejadorArchivos.crearCarpeta(rutaEntradaJournaling);
            // Creamos el archivo de la entrada
            //mJson::ManejadorJson entrada(rutaEntradaJournaling + nombreEntrada.toStdString() + ".tdapp", true);
            manejadorArchivos.crearArchivo(rutaEntradaJournaling + "/" + nombreEntrada.toStdString() + ".tdapp", "");

            // En este punto deberia de abrirse la interfaz de la entrada
            jourLiNoCargarEntrada(2, nombreEntrada.toStdString());
        }

    } catch (const std::exception &e)
    {
        // si hay un error, se mostrará un mensaje emergente
        QMessageBox::critical(this, "Error", e.what());
    }
}

// ! método para cargar una entrada en el journaling
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::jourLiNoCargarEntrada(const int& tipoCarga, const std::string& nombreEntrada)
{
    // tipo carga
    // 0 = cargar entrada de journaling a partir de la lista de entradas esto implica que no se puede modificar
    // 1 = cargar entrada de journaling para modificarla
    // 2 = cargar entrada de journaling como recien creada
    try {
        std::string rutaEntradaJournaling = RUTA_USUARIOS + encriptado->desencriptar((*configuracionesUsuario)["nombreUsuario"]) + "/journaling/";
        // activamos la interfaz de la entrada del journaling
        jourActivarEditarEntrada();
        switch (tipoCarga)
        {
        // ! Este caso solo se cumple si recien se ha creado la entrada
        case 2:
            // ? completamos la ruta de la carpeta de la entrada
            rutaEntradaJournaling += nombreEntrada;
            // ? Cargamos la entrada en el objeto manejadorJson
            entradaSeleccionada = new mJson::ManejadorJson(rutaEntradaJournaling + "/" + nombreEntrada + ".tdapp");
            // ? Agregamos la fecha de creación osea la fecha actual
            (*entradaSeleccionada)["fechaCreacion"] = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss").toStdString();
            break;
        // Este caso solo se cumple si se carga una entrada de la lista de entradas
        case 0 : case 1:
            // verificamos si tenemos un item seleccionado en la lista
            if (jourLiNoListaNotas->currentItem() == nullptr)
            {
                QMessageBox::critical(this, "Error", "Debes seleccionar una entrada");
                return;
            }
            // ? completamos la ruta de la carpeta de la entrada
            rutaEntradaJournaling += jourLiNoListaNotas->currentItem()->text().toStdString();
            // ? Cargamos la entrada en el objeto manejadorJson
            entradaSeleccionada = new mJson::ManejadorJson(rutaEntradaJournaling + "/" + jourLiNoListaNotas->currentItem()->text().toStdString() + ".tdapp");
            // ? Desactivamos la lista de entradas
            //jourDesactivarListaEntradas(); < - ya lo hace jourActivarEditarEntrada
            // ? Activamos la interfaz de la entrada del journaling
            jourActivarEditarEntrada(true); // ? Solo lectura
            // ? Cargar el titulo y el texto de la entrada en la interfaz de la entrada del journaling
            jourReEnCampoTitulo->setText(QString::fromStdString(encriptado->desencriptar((*entradaSeleccionada)["titulo"])));
            //jourReEnCampoTitulo->setReadOnly(true);
            jourReEnCampoTexto->setHtml(QString::fromStdString(encriptado->desencriptar((*entradaSeleccionada)["texto"])));
            //jourReEnCampoTexto->setReadOnly(true);
            break;
        }

    } catch (const std::exception &e)
    {
        // si hay un error, se mostrará un mensaje emergente
        QMessageBox::critical(this, "Error", e.what());
    }
}

// ! método para guardar una entrada en el journaling
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::jourGuardarEntrada()
{
    // ? se obtendrá el titulo y el texto de la entrada
    // ? se guardará el titulo y el texto de la entrada en el archivo de la entrada controlado por el manejadorJson "entradaSeleccionada"
    // ? se mostrará un mensaje emergente que indica que la entrada se ha guardado correctamente

    // obtenemos el titulo y el texto de la entrada
    std::string titulo = jourReEnCampoTitulo->text().toStdString();
    std::string texto = jourReEnCampoTexto->toHtml().toStdString();
    // Guardamos el titulo y el texto de la entrada
    (*entradaSeleccionada)["titulo"] = encriptado->encriptar(titulo);
    (*entradaSeleccionada)["texto"] = encriptado->encriptar(texto);
    // Guardamos la entrada
    entradaSeleccionada->guardar();

}

// ! Método para eliminar una entrada en el journaling
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::jourEliminarEntrada()
{
    // ? se mostrara un mensaje emergente para confirmar si se desea eliminar la entrada
    // ? se eliminara la entrada
    // se actualizara la lista de entradas

    // verificamos si tenemos un item seleccionado en la lista
    if (jourLiNoListaNotas->currentItem() == nullptr)
    {
        QMessageBox::critical(this, "Error", "Debes seleccionar una entrada");
        return;
    }
    // Armamos la ruta de la entrada
    std::string rutaEntradaJournaling = RUTA_USUARIOS + encriptado->desencriptar((*configuracionesUsuario)["nombreUsuario"]) + "/journaling/" + jourLiNoListaNotas->currentItem()->text().toStdString();
    // verificamos si existe la entrada
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaEntradaJournaling))
    {
        QMessageBox::critical(this, "Error", "La entrada no existe");
        return;
    }
    // mostramos un mensaje emergente para confirmar si se desea eliminar la entrada
    QMessageBox::StandardButton respuesta = QMessageBox::question(this, "Eliminar Entrada", "¿Estás seguro de que deseas eliminar la entrada?", QMessageBox::Yes | QMessageBox::No);
    if (respuesta == QMessageBox::Yes)
    {
        // eliminamos la entrada
        manejadorArchivos.eliminarCarpeta(rutaEntradaJournaling);
        // actualizamos la lista de entradas
        //jourActualizarListaEntradas();
        // activamos la lista de entradas
        jourActivarListaEntradas();
    }

}

// ! método para activar la interfaz de la entrada del journaling
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::jourActivarEditarEntrada(const bool& soloLectura)
{
    // ? se desactiva la pantalla de bienvenida
    jourOcultarPantallaBienvenida();
    // ? se desactiva la lista de entradas
    jourDesactivarListaEntradas();
    // ? limpiamos los botones de la barra de navegación
    barNaLimpiarBotonesSeccionActual();
    // ? modificamos el tamaño de la ventana
    //this->resize(400, 400);
    // ? se activa la interfaz de la entrada del journaling
    jourReEnCampoTitulo->show();
    jourReEnCampoTexto->show();
    if (soloLectura)
    {
        jourReEnCampoTexto->setReadOnly(true);
        jourReEnCampoTitulo->setReadOnly(true);
        barNaConfigurarBotones(0, true, 4, "Eliminar");
        barNaConfigurarBotones(1, true, 3, "Editar");
    }
    else
    {
        jourReEnCampoTexto->setReadOnly(false);
        jourReEnCampoTitulo->setReadOnly(false);
        barNaConfigurarBotones(0, true, 2, "Guardar");
    }

}

// ! método para desactivar la interfaz de la entrada del journaling
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::jourDesactivarEditarEntrada()
{
    // ? se desactiva la interfaz de la entrada del journaling
    jourReEnCampoTitulo->hide();
    jourReEnCampoTitulo->clear();
    jourReEnCampoTexto->hide();
    jourReEnCampoTexto->clear();
    // ? se limpian los botones de la barra de navegación
    barNaLimpiarBotonesSeccionActual();
}

// ! método para redimencionar el icono del boton de la pantalla de bienvenida
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::jourPanBiRedimensionarIconoBoton()
{
    // extraemos y mostramos el tamaño del frame de journaling
    QSize size = frameJournaling->size();
    std::cout << "Tamaño del frame de journaling: " << size.width() << " x " << size.height() << std::endl;
    jourBotonCambiarImagenBienvenida->setFixedSize(size.width() - 40, size.height() - 40);
    //jourBotonCambiarImagenBienvenida->size();
    //jourBotonCambiarImagenBienvenida->setIconSize(jourBotonCambiarImagenBienvenida->size());
    QSize BotonSize = jourBotonCambiarImagenBienvenida->size();
    BotonSize.setWidth(BotonSize.width());
    BotonSize.setHeight(BotonSize.height());
    jourBotonCambiarImagenBienvenida->setIconSize(BotonSize);
    std::cout << "Tamaño final del frame de journaling: " << frameJournaling->size().width() << " x " << frameJournaling->size().height() << std::endl;
}


// ////////////////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------------------------
// ! RELACIONADOS CON LA BARRA DE NAVEGACIÓN

// ! método para activar la barra de navegación
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::activarBarraNavegacion()
{
    // ? se activa la barra de navegación
    frameBarraNavegacion->show();
}

// ! método para desactivar la barra de navegación
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::desactivarBarraNavegacion()
{
    // ? se desactiva la barra de navegación
    frameBarraNavegacion->hide();
}

// ! método para mostrar la interfaz de calendario
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::barNaMostrarCalendario()
{
    // ? se desactivan todas las interfaces (frames)
    barNaDesactivarTodosLosFrames();
    // ? se activa el frame de calendario
    activarInterfazCalendario();
}

// ! método para mostrar la interfaz de journaling
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::barNaMostrarJournaling()
{
    // ? se desactivan todas las interfaces (frames)
    barNaDesactivarTodosLosFrames();
    // ? se activa el frame de journaling
    activarInterfazJournaling();
}

// ! método para desactivar todos los frames
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::barNaDesactivarTodosLosFrames()
{
    // ? se desactivan todos los frames
    desactivarInterfazInicioSesion();
    desactivarInterfazRegistroUsuario();
    desactivarInterfazRecuperarContra();
    desactivarInterfazJournaling();
    desactivarInterfazCalendario();
}

// ! método para activar o desactivar los botones de la barra de navegación
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::barNaConfigurarBotones(const int& nBoton, const bool& activar, const int& nFuncion, const std::string& textoBoton)
{
    // ? primero usamos un if para seleccionar el boton a activar o desactivar
    // ? luego usamos un switch para seleccionar la función del boton
    // ? finalmente, activamos o desactivamos el boton y asignamos el texto

    // verificamos que el numero del boton sea correcto
    if (nBoton < 0 || nBoton > 2)
    {
        // si el numero del boton es incorrecto, se mostrará un mensaje emergente
        QMessageBox::critical(this, "Error", "BARNA:01, El número de botón es incorrecto");
        return;
    }
    // creamos un puntero a un boton
    QPushButton* boton;
    // seleccionamos el boton
    switch (nBoton)
    {
        case 0:
            boton = barNaBotonSeccionActual0;
            //std::cout << "Boton 0" << std::endl;
            break;
        case 1:
            boton = barNaBotonSeccionActual1;
            break;
        case 2:
            boton = barNaBotonSeccionActual2;
            break;
    }
    // verificamos que el numero de la función sea correcto
    if (nFuncion < -1)
    {
        // si el numero de la función es incorrecto, se mostrará un mensaje emergente
        QMessageBox::critical(this, "Error", "BARNA:02, El número de función es incorrecto");
        return;
    }

    // seleccionamos la función del boton
    // conectamos la función del boton
    barNaFunciones[nBoton] = nFuncion;
    // asignamos el texto al boton
    boton->setToolTip(QString::fromStdString(textoBoton));
    switch (nFuncion)
    {
        // /-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/
        // ----------------------------JOURNALING-------------------------------------

        // ? Para activar la lista de entradas de journaling (estando en la pantalla de bienvenida)
    case 0:
        boton->setIcon(*jourIconoActivarListaEntradas);
        break;
    // ? Para crear una nueva entrada en journaling
    case 1:
        boton->setIcon(*jourIconoNuevaEntrada);
        break;
    // ? Para guardar una entrada en journaling
    case 2:
        boton->setIcon(*jourIconoGuardarEntrada);
        break;
    // ? Para activar editar entrada en journaling en modo escritura
    case 3:
        boton->setIcon(*jourIconoEditarEntrada);
        break;
    // ? Para eliminar una entrada en journaling
    case 4:
        boton->setIcon(*jourIconoEliminarEntrada);
        break;
    default:
        break;

    }
    // activamos o desactivamos el boton
    if (activar)
    {
        boton->show();
        //std::cout << "Activando boton" << std::endl;
    }
    else
    {
        boton->hide();
        //std::cout << "Desactivando boton" << std::endl;
    }

}

// ! Método encargado de ejecutar las funciones de los botons de la barra de navegación
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::barNaEjecutorFunciones(const int& nFuncion)
{
    switch (nFuncion)
    {
        // /-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/
        // ----------------------------JOURNALING-------------------------------------

        // ? Para activar la lista de entradas de journaling (estando en la pantalla de bienvenida)
    case 0:
        std::cout << "JourActivarListaEntradas" << std::endl;
        //connect(boton, SIGNAL(clicked()), this, SLOT(jourActivarListaEntradas()));
        jourActivarListaEntradas();
        break;
    // ? Para crear una nueva entrada en journaling
    case 1:
        std::cout << "JourLiNoNuevaEntrada" << std::endl;
        //connect(boton, SIGNAL(clicked()), this, SLOT(jourLiNoNuevaEntrada()));
        jourLiNoNuevaEntrada();
        break;
    // ? Para guardar una entrada en journaling
    case 2:
        std::cout << "JourGuardarEntrada" << std::endl;
        //connect(boton, SIGNAL(clicked()), this, SLOT(jourGuardarEntrada()));
        jourGuardarEntrada();
        break;
    // ? Para activar editar entrada en journaling en modo escritura
    case 3:
        std::cout << "JourActivarEditarEntrada" << std::endl;
        //connect(boton, SIGNAL(clicked()), this, SLOT(jourActivarEditarEntrada()));
        jourActivarEditarEntrada();
        break;
    // ? Para eliminar una entrada en journaling
    case 4:
        std::cout << "JourEliminarEntrada" << std::endl;
        //connect(boton, SIGNAL(clicked()), this, SLOT(jourEliminarEntrada()));
        jourEliminarEntrada();
        break;
    default:
        break;

    }
}

// ! Método para ejecutar bóton 1 de la barra de navegación
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::barNaBotonSeccionActual0Click()
{
    barNaEjecutorFunciones(barNaFunciones[0]);
}

// ! Método para ejecutar bóton 2 de la barra de navegación
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::barNaBotonSeccionActual1Click()
{
    barNaEjecutorFunciones(barNaFunciones[1]);
}

// ! Método para ejecutar bóton 3 de la barra de navegación
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::barNaBotonSeccionActual2Click()
{
    barNaEjecutorFunciones(barNaFunciones[2]);
}

// ! Método para limpiar botones de la seleccion actual
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::barNaLimpiarBotonesSeccionActual()
{
    barNaConfigurarBotones(0, false, -1, "");
    barNaConfigurarBotones(1, false, -1, "");
    barNaConfigurarBotones(2, false, -1, "");
}

// ////////////////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------------------------
// ! RELACIONADO CON EL CALENDARIO

// ! método para activar el frame del calendario
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::activarInterfazCalendario()
{
    // desactivamos todos los frames

    frameCalendario->show();
    std::cout << "Activando interfaz de calendario" << std::endl;
    // ? se desactiva todo lo relacionado con el calendario
    // aun no se ha implementado
    // ? se activa el frame de calendario
    calActivarCalendario();
}

// ! método para desactivar el frame del calendario
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::desactivarInterfazCalendario()
{
    frameCalendario->hide();
}

// ! método para mostrar el calendario
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::calActivarCalendario()
{
    calCalendario->show();
}



// ////////////////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------------------------
// ! RELACIONADOS CON LA INTERFAZ GRÁFICA
void MainWindow::resizeEvent(QResizeEvent *event)
{

    QWidget::resizeEvent(event);
    //frame->setFixedSize(this->size());
    jourPanBiRedimensionarIconoBoton();

}

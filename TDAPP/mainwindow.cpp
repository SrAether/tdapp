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
#include <QFormLayout> // ? para crear un layout de formulario

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


    // vector de temas de la aplicación
    // tema por defecto
    temas.push_back(""); // osea que se aplica el tema del sistema
    // tema oscuro
    temas.push_back(
                    "/* Estilos generales */"
                    "QWidget {"
                            "background-color: #222222;" /* Fondo gris oscuro */
                            "color: #ffffff;" /* Texto blanco */
                            "font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;"
                            "font-size: 12pt;"
                        "}"

                        "/* Botones */"
                        "QPushButton {"
                            "background-color: #0056b3;" /* Azul oscuro */
                            "color: white;"
                            "border: none;"
                            "padding: 8px 16px;"
                            "border-radius: 4px;"
                        "}"
                        "QPushButton:hover {"
                            "background-color: #007bff;" /* Azul más claro al pasar el ratón */
                        "}"

                        "/* Etiquetas */"
                        "QLabel {"
                            "color: #e0e0e0;" /* Gris claro para legibilidad */
                        "}"

                        "/* Campos de texto */"
                        "QLineEdit, QTextEdit {"
                            "background-color: #333333;" /* Fondo gris oscuro */
                            "color: #ffffff;"
                            "border: 1px solid #555555;" /* Borde gris más oscuro */
                            "padding: 6px;"
                            "border-radius: 3px;"
                        "}"

                        "/* Tablas */"
                        "QTableView {"
                            "alternate-background-color: #282828;" /* Filas alternadas oscuras */
                            "selection-background-color: #3949AB;" /* Selección azul oscuro */
                            "gridline-color: #555555;" /* Líneas de división oscuras */
                        "}"
                        "QHeaderView::section {"
                            "background-color: #333333;" /* Encabezados gris oscuro */
                            "color: #ffffff;"
                            "padding: 4px;"
                            "border: 1px solid #555555;"
                        "}"

                        "/* Otros widgets */"
                        "QComboBox, QSpinBox, QDoubleSpinBox {"
                            "background-color: #333333;"
                            "color: #ffffff;"
                            "border: 1px solid #555555;"
                            "padding: 5px;"
                            "border-radius: 3px;"
                        "}"
                        "QProgressBar {"
                            "border: 1px solid #555555;"
                            "text-align: center;"
                            "color: #ffffff;"
                        "}"
                        "QProgressBar::chunk {"
                            "background-color: #007bff;" /* Mantiene el azul para la barra */
                        "}"

                        "/* Scrollbars */"
                        "QScrollBar:vertical {"
                            "background: #282828;" /* Fondo oscuro */
                            "width: 15px;"
                            "margin: 0px 0px 0px 0px;"
                        "}"
                        "QScrollBar::handle:vertical {"
                            "background: #555555;" /* Gris oscuro para el deslizador */
                            "min-height: 20px;"
                        "}"
                        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
                            "background: none;"
                            "height: 0px;"
                        "}"
                    );

    // verificamos la existencia de los archivos y carpetas necesarios
    verificacionInicial();



    // cargamos la fuente
    this->setFont(fuente);
    // hacemos que la ventana sea de 720 redimensionable
    this->setMinimumSize(1240, 800);
    // establecemos el estilo de la aplicación
    {
        std::string tm = (*configuraciones)["tema"];
        std::cout << "Tema: " << tm << std::endl;
        if (tm.empty()) {
            this->setStyleSheet(temas[0].c_str());
        } else {
            //this->setStyleSheet(temas[stoi(tm)]);
            int nTema = stoi(tm);
            this->setStyleSheet(temas[nTema].c_str());
        }
    }
    // creamos el frame principal
    framePrincipal = new QFrame(this);


    // -----------------------------------------------------------------------------
    // INICIO DE SESIÓN NO COMPLETADO
    /* Debe contener:
     * un campo para el usuario puede ser un area de texto o un selector de usuario (lista desplegable)
     * un campo para la contraseña debe ser un area de texto
     * un botón para iniciar sesión
     * un botón para registrarse
     */
    {
        // creamos un frame para iniciar sesión
        frameInicioSesion = new QFrame(framePrincipal);
        //frameInicioSesion->setStyleSheet("background-color: #000000;");
        // ocultamos por defecto
        frameInicioSesion->hide();
        frameInicioSesion->setMinimumWidth(500);
        frameInicioSesion->setMinimumHeight(600);




        // en este punto se deberia cargar un estilo para el frame inicio de sesión
        //frameInicioSesion->setStyleSheet("background-color: #000000;"); // estilo para el frame

        // Label foto de perfil
        inSeFotoPerfil = new QLabel(frameInicioSesion);
        inSeFotoPerfil->setMaximumSize(300, 300);
        inSeFotoPerfil->setScaledContents(true);

        // SELECTOR DE USUARIO
        // creamos un selector de usuario
        inSeSelectorUsuario = new QComboBox(frameInicioSesion);
        inSeSelectorUsuario->setMinimumSize(200, 50);

        // CAMPO DE CONTRASEÑA
        // creamos un campo de contraseña
        inSeCampoContra = new QLineEdit(frameInicioSesion);
        inSeCampoContra->setPlaceholderText("Contraseña");
        inSeCampoContra->setEchoMode(QLineEdit::Password);
        inSeCampoContra->setMinimumSize(100, 50);

        // BOTÓN DE INICIAR SESIÓN
        // creamos un botón para iniciar sesión
        inSeBotonIniciarSesion = new QPushButton(frameInicioSesion);
        inSeBotonIniciarSesion->setText("Iniciar Sesión");

        // BOTÓN DE REGISTRARSE
        // creamos un botón para registrarse
        inSeBotonRegistrarse = new QPushButton(frameInicioSesion);
        inSeBotonRegistrarse->setText("Registrarse");

        // BOTÓN DE RECUPERAR CONTRASEÑA
        // creamos un botón para recuperar contraseña
        inSeBotonRecuperarContra = new QPushButton(frameInicioSesion);
        inSeBotonRecuperarContra->setText("Recuperar Contraseña");

        // Layout
        inSeLayout = new QVBoxLayout(frameInicioSesion);
        //inSeLayout->addLayout(inSeImgLayout);
        inSeLayout->addWidget(inSeFotoPerfil);
        inSeLayout->addWidget(inSeSelectorUsuario);
        inSeLayout->addWidget(inSeCampoContra);
        inSeLayout->addWidget(inSeBotonIniciarSesion);
        inSeLayout->addWidget(inSeBotonRegistrarse);
        inSeLayout->addWidget(inSeBotonRecuperarContra);

    }

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
    {
        // inicializamos las variables
        // ? La etiqueta de titulo
        contenidoEtiquetaRegistroUsuario = "Registro de Usuarios"; // contenido de la etiqueta de registro de usuarios
        // ? Boleano que funciona bandera para pedir o no las credenciales de correo electronico del usuario
        banderaRegistroCopiaSeg = 0;

        // creamos un frame para el registro de usuarios
        frameRegistroUsuario = new QFrame(framePrincipal);
        frameRegistroUsuario->hide();
        frameRegistroUsuario->setMinimumWidth(500);
        frameRegistroUsuario->setMinimumHeight(500);

        // creamos un widget para el registro de usuarios
        rUWidget = new QWidget();
        // en este punto se deberia cargar un estilo para el frame registro de usuarios
        //frameRegistroUsuario->setStyleSheet("background-color: #000000;"); // estilo para el frame

        // ETIQUETA DE REGISTRO DE USUARIOS
        // creamos una etiqueta para el registro de usuarios
        etiquetaRegistroUsuario = new QLabel{QString::fromStdString(contenidoEtiquetaRegistroUsuario), rUWidget};
        etiquetaRegistroUsuario->setMaximumHeight(30);

        // CAMPO PARA INGRESAR NOMBRE
        // creamos un campo para ingresar el nombre
        campoRegistroNombre = new QLineEdit(rUWidget);
        campoRegistroNombre->setPlaceholderText("Nombre");
        campoRegistroNombre->setToolTip("Coloca tu nombre");
        campoRegistroNombre->setToolTipDuration(3000); // duración de cosito emergente

        // CAMPO PARA INGRESAR APODO (USERNAME)
        campoRegistroNombreUs = new QLineEdit(rUWidget);
        campoRegistroNombreUs->setPlaceholderText("Como le gustaria que lo llamemos?");

        // CAMPO PARA INGRESAR CONTRASEÑA
        campoRegistroContra = new QLineEdit(rUWidget);
        campoRegistroContra->setPlaceholderText("Contra");
        campoRegistroContra->setEchoMode(QLineEdit::Password);

        // CAMPO PARA CONFIRMAR CONTRASEÑA
        campoRegistroContraConf = new QLineEdit(rUWidget);
        campoRegistroContraConf->setPlaceholderText("Confirmar Contra");
        campoRegistroContraConf->setEchoMode(QLineEdit::Password);




        // BOTÓN PARA SELECCIONAR SI DESEA REALIZAR COPIA DE SEGURIDAD
        // creamos un botón para seleccionar si desea realizar copia de seguridad
        botonRegistroCopiaSeguridad = new QPushButton(rUWidget);
        botonRegistroCopiaSeguridad->setText("Da click para activar copia de seguridad");


        // SELECTOR FRECUENCIA COPIA DE SEGURIDAD
        // creamos un selector para la copia de seguridad
        selectorRegistroFrecuenciaCopiaSeguridad = new QComboBox(rUWidget);
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
        campoRegistroCorreoCopiaSeguridad = new QLineEdit(rUWidget);
        // establecemos el texto del campo para ingresar el correo electrónico
        campoRegistroCorreoCopiaSeguridad->setPlaceholderText("Correo Electrónico");
        // ocultamos por defecto
        campoRegistroCorreoCopiaSeguridad->hide();

        // CAMPO PARA INGRESAR CONTRASEÑA DE CORREO ELECTRÓNICO (PARA COPIA DE SEGURIDAD)
        // creamos un campo para ingresar la contraseña del correo electrónico
        campoRegistroContraCopiaSeguridad = new QLineEdit(rUWidget);
        // establecemos el texto del campo para ingresar la contraseña del correo electrónico
        campoRegistroContraCopiaSeguridad->setPlaceholderText("Contra del Correo Electrónico");
        // establecemos que el campo sea de tipo contraseña
        campoRegistroContraCopiaSeguridad->setEchoMode(QLineEdit::Password);
        // ocultamos por defecto
        campoRegistroContraCopiaSeguridad->hide();

        // CAMPO PARA CONFIRMAR CONTRASEÑA DE CORREO ELECTRÓNICO (PARA COPIA DE SEGURIDAD)
        // creamos un campo para confirmar la contraseña del correo electrónico
        campoRegistroContraCopiaSeguridadConf = new QLineEdit(rUWidget);
        // establecemos el texto del campo para confirmar la contraseña del correo electrónico
        campoRegistroContraCopiaSeguridadConf->setPlaceholderText("Confirmar Contra del Correo Electrónico");
        // establecemos que el campo sea de tipo contraseña
        campoRegistroContraCopiaSeguridadConf->setEchoMode(QLineEdit::Password);
        // ocultamos por defecto
        campoRegistroContraCopiaSeguridadConf->hide();

        // BOTÓN PARA INGRESAR FOTO DE PERFIL (avatar)
        // creamos un botón para añadir la foto de perfil
        botonRegistroFotoPerfil = new QPushButton(rUWidget);
        // establecemos el texto del botón para añadir la foto de perfil
        botonRegistroFotoPerfil->setText("Añadir Foto de Perfil");


        // SELECTOR PARA LA EDAD
        // creamos un selector para la edad
        selectorRegistroEdad = new QComboBox(rUWidget);
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
        campoRegistroPreguntaRecuperacion = new QLineEdit(rUWidget);
        // establecemos el texto del campo para añadir la pregunta de recuperación
        campoRegistroPreguntaRecuperacion->setPlaceholderText("Pregunta de Recuperación");

        // Campo para añadir respuesta de recuperación
        // creamos un campo para añadir la respuesta de recuperación
        campoRegistroRespuestaRecuperacion = new QLineEdit(rUWidget);
        // establecemos el texto del campo para añadir la respuesta de recuperación
        campoRegistroRespuestaRecuperacion->setPlaceholderText("Respuesta de Recuperación");

        // BOTÓN PARA REGISTRARSE
        // creamos un botón para registrarse
        botonRegistrarUsuario = new QPushButton(rUWidget);
        // establecemos el texto del botón para registrarse
        botonRegistrarUsuario->setText("Registrarse");

        // BOTÓN PARA CANCELAR
        // creamos un botón para cancelar
        botonRegistroCancelar = new QPushButton(rUWidget);
        // establecemos el texto del botón para cancelar
        botonRegistroCancelar->setText("Cancelar");

        // Layout
        rULayout = new QVBoxLayout(rUWidget);
        rULayout->addWidget(etiquetaRegistroUsuario);
        rULayout->addWidget(campoRegistroNombre);
        rULayout->addWidget(campoRegistroNombreUs);
        rULayout->addWidget(campoRegistroContra);
        rULayout->addWidget(campoRegistroContraConf);
        rULayout->addWidget(botonRegistroCopiaSeguridad);
        rULayout->addWidget(selectorRegistroFrecuenciaCopiaSeguridad);
        rULayout->addWidget(campoRegistroCorreoCopiaSeguridad);
        rULayout->addWidget(campoRegistroContraCopiaSeguridad);
        rULayout->addWidget(campoRegistroContraCopiaSeguridadConf);
        rULayout->addWidget(botonRegistroFotoPerfil);
        rULayout->addWidget(selectorRegistroEdad);
        rULayout->addWidget(campoRegistroPreguntaRecuperacion);
        rULayout->addWidget(campoRegistroRespuestaRecuperacion);
        rULayout->addWidget(botonRegistrarUsuario);
        rULayout->addWidget(botonRegistroCancelar);



        // Scroll area
        rUScroll = new QScrollArea(frameRegistroUsuario);
        rUScroll->setWidget(rUWidget);
        rUScroll->setWidgetResizable(true);

        // Layout Principal
        rUPrincipal = new QVBoxLayout(frameRegistroUsuario);
        rUPrincipal->addWidget(rUScroll);




    }

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
    {
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


        // PREGUNTA DE RECUPERACIÓN
        reCoPreguntaRecuperacion = new QLabel{QString::fromStdString(reCoPreguntaContra), frameRecuperarContra};
        reCoPreguntaRecuperacion->setWordWrap(true);

        // CAMPO PARA RESPONDER PREGUNTA DE RECUPERACIÓN
        reCoCampoRespuesta = new QLineEdit(frameRecuperarContra);
        reCoCampoRespuesta->setPlaceholderText("Respuesta de Recuperación");


        // BOTÓN PARA RECUPERAR CONTRASEÑA
        reCoBotonConfirmar = new QPushButton(frameRecuperarContra);
        reCoBotonConfirmar->setText("Recuperar Contraseña");

        // BOTÓN PARA CANCELAR
        reCoBotonCancelar = new QPushButton(frameRecuperarContra);
        reCoBotonCancelar->setText("Cancelar");

        // Layout
        reCoLayout = new QVBoxLayout(frameRecuperarContra);
        reCoLayout->addWidget(reCoSelectorUsuario);
        reCoLayout->addWidget(reCoPreguntaRecuperacion);
        reCoLayout->addWidget(reCoCampoRespuesta);
        reCoLayout->addWidget(reCoBotonConfirmar);
        reCoLayout->addWidget(reCoBotonCancelar);
        reCoLayout->addStretch();

    }

    // -----------------------------------------------------------------------------
    // JOURNALING NO COMPLETADO
    /* Debe contener:
     * una pantalla de bienvenida <- contiene una imagen seleccionada por el usuario y un boton para continuar
     * una pantalla con la lista de entrada de journaling
     * ... (continuar)
     */
    {
        // creamos un frame para el journaling
        frameJournaling = new QFrame();
        // ocultamos por defecto
        frameJournaling->hide();
        frameJournaling->adjustSize(); // ? ajustamos el tamaño del frame

        // Iconos para la barra de navegación del journaling
        jourIconoActivarListaEntradas = new QIcon(QString(RUTA_ICONOS.c_str()) + "activar_lista_entradas.png");
        jourIconoEditarEntrada = new QIcon(QString(RUTA_ICONOS.c_str()) + "editar_entrada.png");
        jourIconoNuevaEntrada = new QIcon(QString(RUTA_ICONOS.c_str()) + "nueva_entrada.png");
        jourIconoEliminarEntrada = new QIcon(QString(RUTA_ICONOS.c_str()) + "eliminar_entrada.png");
        jourIconoGuardarEntrada = new QIcon(QString(RUTA_ICONOS.c_str()) + "guardar_entrada.png");



        // creamos un boton de bienvenida
        jourBotonCambiarImagenBienvenida = new QPushButton(frameJournaling);
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

        // campo para el texto de la entrada
        jourReEnCampoTexto = new QTextEdit(frameJournaling);
        jourReEnCampoTexto->hide();

        // layout para la entrada
        jourLiNoLayout->addWidget(jourReEnCampoTitulo);
        jourLiNoLayout->addWidget(jourReEnCampoTexto, 1);
    }

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
    {
        // Creamos un frame para la barra de navegación
        frameBarraNavegacion = new QFrame();
        // ocultamos por defecto
        frameBarraNavegacion->hide();
        frameBarraNavegacion->setMaximumHeight(100);
        // Tamaño de boton
        barNaTamBotones = QSize(50, 50);
        // Icono de configuración
        barNaIconoConfiguracion = new QIcon(QString(RUTA_ICONOS.c_str()) + "configuraciones.svg");
        // Icono de Cerrar Sesión
        barNaIconoCerrarSesion = new QIcon(QString(RUTA_ICONOS.c_str()) + "cerrar_sesion.png");
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
        // Botón de cerrar sesión
        barNaBotonCerrarSesion = new QPushButton(frameBarraNavegacion);
        barNaBotonCerrarSesion->setIcon(*barNaIconoCerrarSesion);
        barNaBotonCerrarSesion->setIconSize(barNaTamBotones);
        barNaBotonCerrarSesion->setToolTip("Cerrar Sesión");
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
        disposicionIzquierda->addWidget(barNaBotonCerrarSesion);
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
    }

    // -----------------------------------------------------------------------------
    // CALENDARIO NO COMPLETADO
    /* Debe contener:
     * un calendario
     * una pantalla de dia seleccionado donde se muestren las entradas de ese día
     * una pantalla de evento seleccionado donde se muestre la información del evento
     * una pantalla de evento nuevo donde se pueda añadir un evento nuevo
     * una pantalla de editar evento donde se pueda editar un evento
     */
    {
        // creamos un frame para el calendario
        frameCalendarioP = new QFrame(framePrincipal);
        // ocultamos por defecto
        frameCalendarioP->hide();
        // establecemos editandoEvento como falso
        editandoEvento = false;

        // creamos un frame para el calendario
        frameCalendario = new QFrame(frameCalendarioP);
        frameCalendario->setFrameShape(QFrame::Box);
        // Le ponemos politica de expansión
        frameCalendario->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        frameCalendario->hide();

        calLayout = new QVBoxLayout(frameCalendarioP);
        calCalendarioBackend = new QCalendar();

        // /=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=
        calCalendarioLayout = new QHBoxLayout(frameCalendario);
        calCalendarioLayoutVertical = new QVBoxLayout(frameCalendario);
        // Relacionado con el calendario en sí
        calCalendario = new QGridLayout(frameCalendario);
        // label para el año
        calAnioLabel = new QLabel(frameCalendario);
        // le metemos un tamaño de fuente grande
        QFont fuenteAnio = calAnioLabel->font();
        fuenteAnio.setPointSize(25);
        calAnioLabel->setFont(fuenteAnio);
        // label para el icono del mes
        calIconoMes = new QLabel(frameCalendario);
        calCalendarioLayoutVertical->addWidget(calAnioLabel);
        // metemos un espaciador
        calCalendarioLayoutVertical->addStretch();
        calCalendarioLayoutVertical->addWidget(calIconoMes);
        //calIconoMes->hide();
        //calIconoMes->setText("hola");
        calCalendarioLayout->addLayout(calCalendarioLayoutVertical);
        calCalendarioLayout->addLayout(calCalendario);
        // Para los días de la semana
        //QStringList diasSemana = {"Domingo", "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado"};
        diasSemana = new QStringList{"Domingo", "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado"};
        meses = new QStringList{"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
        calIconosMeses = new std::vector<std::string>{"Enero_blanco.png", "Febrero_blanco.png", "Marzo_blanco.png", "Abril_blanco.png", "Mayo_blanco.png", "Junio_blanco.png", "Julio_blanco.png", "Agosto_blanco.png", "Septiembre_blanco.png", "Octubre_blanco.png", "Noviembre_blanco.png", "Diciembre_blanco.png"};
        for (int i = 0; i < 7; i++)
        {
            QLabel* calDiaSemana = new QLabel(frameCalendario);
            calDiaSemana->setText((*diasSemana)[i]);
            calCalendario->addWidget(calDiaSemana, 0, i);
        }
        // Creamos la cuadricula del calendario
        for (int nfila = 1; nfila < 7; nfila++)
        {
            for (int ncolumna = 0; ncolumna < 7; ncolumna++)
            {
                // agregamos el cuadro a un boton
                QPushButton* calCuadro = new QPushButton(frameCalendario);
                calCuadro->setMinimumSize(150, 100);
                QPushButton* calEmocionDia = new QPushButton(calCuadro);
                // creamos un grid layout para el cuadro del calendario
                QGridLayout* calCuadroLayout = new QGridLayout(calCuadro);

                calEmocionDia->setIcon(QIcon(QString(RUTA_ICONOS.c_str()) + "sorpresa-icono.svg"));
                calEmocionDia->setIconSize(QSize(12, 12));
                // establecemos el tamaño del icono a 20x20
                calEmocionDia->setFixedSize(30, 30);
                // le ponemos un estilo sin bordes
                calEmocionDia->setFlat(true);
                calEmocionDia->hide();
                // hacemos que el icono se ajuste al tamaño del label
                //calEmocionDia->setScaledContents(true);
                // lo agregamos al layout del cuadro
                calCuadroLayout->addWidget(calEmocionDia, 0, 0);
                // metemos un espacio entre la emoción y el número
                calCuadroLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum), 0, 1);
                // Label para el numero del dia
                QLabel* calNumeroDia = new QLabel(calCuadro);
                // le damos un dia de prueba
                calNumeroDia->setText("");
                // lo agregamos al layout del cuadro
                calCuadroLayout->addWidget(calNumeroDia, 0, 2);
                // Label para la cantidad de eventos
                QLabel* calCantidadEventos = new QLabel(calCuadro);
                // le damos una cantidad de eventos de prueba
                calCantidadEventos->setText("* * *");
                // lo agregamos al layout del cuadro
                calCuadroLayout->addWidget(calCantidadEventos, 1, 0);


                // establecemos la forma del marco exterior
                //calCuadro->setFrameShape(QFrame::Box);
                // Asignamos un identificador unico a cada cuadro
                QString idCuadro = QString::number(nfila) + QString::number(ncolumna);
                calCuadro->setObjectName(idCuadro);
                calEmocionDia->setObjectName("emocion" + idCuadro);
                calNumeroDia->setObjectName("numero" + idCuadro);
                calCantidadEventos->setObjectName("eventos" + idCuadro);
                // agregamos el cuadro a un boton
                //QPushButton* calBotonCuadro = new QPushButton(frameCalendario);
                calCuadro->setObjectName("boton" + idCuadro);
                // establecemos el layout del cuadro como el layout del boton
                //calBotonCuadro->setLayout(calCuadroLayout);
                // boton redimensionable
                calCuadro->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                // Modificamos su señal de click
                //connect(calBotonCuadro, SIGNAL(clicked()), this, SLOT(calClickDia()));
                // agregamos el cuadro al calendario
                calCalendario->addWidget(calCuadro, nfila, ncolumna);
            }
        }
        // Añadimos el calendario al layout principal
        calLayout->addLayout(calCalendario);
        // CONECTAMOS LOS BOTONES DEL CALENDARIO CON SUS RESPECTIVAS FUNCIONES
        {
            QPushButton* calBoton = frameCalendario->findChild<QPushButton*>("boton10");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend10()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton11");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend11()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton12");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend12()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton13");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend13()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton14");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend14()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton15");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend15()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton16");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend16()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton20");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend20()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton21");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend21()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton22");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend22()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton23");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend23()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton24");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend24()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton25");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend25()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton26");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend26()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton30");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend30()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton31");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend31()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton32");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend32()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton33");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend33()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton34");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend34()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton35");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend35()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton36");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend36()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton40");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend40()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton41");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend41()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton42");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend42()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton43");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend43()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton44");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend44()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton45");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend45()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton46");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend46()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton50");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend50()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton51");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend51()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton52");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend52()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton53");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend53()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton54");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend54()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton55");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend55()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton56");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend56()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton60");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend60()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton61");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend61()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton62");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend62()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton63");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend63()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton64");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend64()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton65");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend65()));
            calBoton = frameCalendario->findChild<QPushButton*>("boton66");
            connect(calBoton, SIGNAL(clicked()), this, SLOT(calClickDiaBackend66()));
        }

        // CONECTAMOS LOS BOTONES DE EMOCIONES DEL CALENDARIO CON SUS RESPECTIVAS FUNCIONES
        {
            QPushButton* calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion10");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend10()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion11");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend11()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion12");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend12()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion13");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend13()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion14");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend14()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion15");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend15()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion16");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend16()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion20");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend20()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion21");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend21()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion22");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend22()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion23");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend23()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion24");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend24()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion25");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend25()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion26");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend26()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion30");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend30()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion31");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend31()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion32");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend32()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion33");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend33()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion34");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend34()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion35");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend35()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion36");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend36()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion40");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend40()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion41");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend41()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion42");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend42()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion43");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend43()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion44");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend44()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion45");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend45()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion46");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend46()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion50");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend50()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion51");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend51()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion52");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend52()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion53");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend53()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion54");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend54()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion55");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend55()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion56");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend56()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion60");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend60()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion61");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend61()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion62");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend62()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion63");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend63()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion64");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend64()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion65");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend65()));
            calEmoBoton = frameCalendario->findChild<QPushButton*>("emocion66");
            connect(calEmoBoton, SIGNAL(clicked()), this, SLOT(calClickEmoDiaBackend66()));

        }

        // /=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=
        // Relacionado con la pantalla de día seleccionado
        calDiaJsonEventos = nullptr;
        calDiaFrame = new QFrame(frameCalendarioP);
        calDiaTitulo = new QLabel(calDiaFrame);
        calDiaTitulo->setText("Día seleccionado");
        calDiaTitulo->hide();
        //calDiaTitulo->setAlignment(Qt::AlignCenter);
        calDiaEventos = new QListWidget(calDiaFrame);
        calDiaEventos->hide();
        // hacemos que el tamaño mínimo sea de 600x400
        //calDiaEventos->setMinimumSize(600, 400);
        calDiaEventos->setMinimumWidth(300);
        calDiaEventos->setStyleSheet("color: rgba(0,0,0,0);");
        // lo hacemos redimensionable
        calDiaEventos->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        //calDiaEventos->setMinimumSize(600, 400);
        //calDiaEventos->setSelectionMode(QAbstractItemView::SingleSelection);
        calDiaScrollArea = new QScrollArea(calDiaFrame);
        calDiaScrollArea->setWidget(calDiaEventos);
        calDiaScrollArea->setWidgetResizable(true);
        calDiaScrollArea->hide();
        //calDiaScrollArea->setMinimumSize(600, 400);
        //calDiaScrollArea->setWidgetResizable(true);
        // Lo agregamos al layout
        calDiaLayout = new QVBoxLayout(calDiaFrame);
        calDiaLayout->addWidget(calDiaTitulo);
        calDiaLayout->addWidget(calDiaScrollArea);

        calLayoutPrincipal = new QVBoxLayout(frameCalendarioP);
        calLayoutPrincipal->addWidget(frameCalendario);
        calLayoutPrincipal->addWidget(calDiaFrame);

        // /=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=
        // Relacionado con la pantalla de nuevo evento
        calAgregarEventoTitulo = new QLineEdit(calDiaFrame);
        calAgregarEventoTitulo->hide();
        calAgregarEventoTitulo->setPlaceholderText("Título del evento");
        calAgregarEventoHora = new QTimeEdit(calDiaFrame);
        calAgregarEventoHora->hide();
        calAgregarEventoHora->setDisplayFormat("HH:mm");
        calAgregarEventoDuracion = new QTimeEdit(calDiaFrame);
        calAgregarEventoDuracion->hide();
        calAgregarEventoDuracion->setDisplayFormat("HH:mm");
        calAgregarEventoDescripcion = new QTextEdit(calDiaFrame);
        calAgregarEventoDescripcion->hide();
        calAgregarEventoDescripcion->setPlaceholderText("Descripción del evento");
        calAgregarEventoCotenedorTiempo = new QHBoxLayout(calDiaFrame);
        calAgregarEventoCotenedorTiempo->addWidget(calAgregarEventoHora);
        calAgregarEventoCotenedorTiempo->addWidget(calAgregarEventoDuracion);
        // los agregamos al layout
        calDiaLayout->addWidget(calAgregarEventoTitulo);
        //calDiaLayout->addWidget(calAgregarEventoHora);
        //calDiaLayout->addWidget(calAgregarEventoDuracion);
        calDiaLayout->addLayout(calAgregarEventoCotenedorTiempo);
        calDiaLayout->addWidget(calAgregarEventoDescripcion);


    }

    // -----------------------------------------------------------------------------
    // AREA DE REGISTRO DE EMOCIONES
    /* Debe contener:
     * un botón para cada emoción
     * un botón para cancelar
     */
    {
        /* Vector de vectores de strings que contiene frases motivadoras para cada emoción
         *
         */
        {
            // 0 -> miedo
            // 1 -> tristeza
            // 2 -> alegria
            // 3 -> enojo
            // 4 -> sorpresa
            // 5 -> disgusto
            // Alegría
            std::vector<std::string> frasesAlegria = {
                "¡Qué alegría verte tan feliz! ¡Disfruta este momento al máximo!",
                "Tu sonrisa ilumina el mundo. ¡Sigue brillando!",
                "¡Celebra tu felicidad! Te lo mereces.",
                "La alegría es contagiosa. ¡Compártela con los demás!",
                "¡Qué bueno que te sientes así de bien! ¡Sigue haciendo lo que te hace feliz!",
                "La vida es más hermosa cuando la miras con alegría. ¡Disfrútala!",
                "Tu felicidad es un regalo para ti y para los demás. ¡Sigue irradiando esa energía positiva!",
                "¡Qué emoción! ¡Sigue disfrutando de este momento de felicidad!",
                "La alegría está en las pequeñas cosas. ¡Aprecia cada instante!",
                "¡Felicidades por sentirte tan alegre! ¡Que dure mucho tiempo!",
                "La alegría es un estado mental. ¡Elige ser feliz hoy!",
                "Tu felicidad es inspiradora. ¡Gracias por compartirla!",
                "¡Qué emoción! ¡Disfruta de esta sensación de alegría!",
                "La alegría es un tesoro. ¡Cuídala y compártela!",
                "¡Qué bien te ves cuando estás feliz! ¡Sigue así!",
                "La alegría es la mejor medicina. ¡Ríe, baila y disfruta!",
                "Tu felicidad es un imán para cosas buenas. ¡Sigue atrayéndolas!",
                "¡Qué alegría verte tan radiante! ¡Eres un sol!",
                "La alegría es un superpoder. ¡Úsalo para hacer el bien!",
                "¡Felicidades por elegir la felicidad! ¡Es la mejor decisión!"
            };

            // Tristeza
            std::vector<std::string> frasesTristeza = {
                "Está bien sentirse triste. Permítete sentir y sanar a tu propio ritmo.",
                "No estás solo en esto. Siempre hay alguien dispuesto a escucharte.",
                "La tristeza es temporal. Recuerda que después de la tormenta siempre sale el sol.",
                "Eres fuerte y capaz de superar cualquier obstáculo. No te rindas.",
                "Cuídate con cariño y date el tiempo que necesites para sanar.",
                "La tristeza puede ser una oportunidad para crecer y aprender. Abraza tus emociones.",
                "No te juzgues por sentirte triste. Es una emoción humana y válida.",
                "Busca apoyo en tus seres queridos. Ellos te quieren y te ayudarán a salir adelante.",
                "Permítete sentir tu tristeza, pero no te quedes atrapado en ella. Busca la luz al final del túnel.",
                "Eres valioso y merecedor de amor y felicidad. No dejes que la tristeza te haga olvidar eso.",
                "La tristeza puede ser una maestra sabia. Aprende de ella y saldrás fortalecido.",
                "No te aísles en tu tristeza. Comparte tus sentimientos con alguien de confianza.",
                "Recuerda que eres amado y apoyado. No estás solo en este camino.",
                "La tristeza es una parte de la vida, pero no dejes que te defina. Eres mucho más que eso.",
                "Date permiso para llorar, gritar o hacer lo que necesites para liberar tus emociones.",
                "Busca la belleza en las pequeñas cosas. Te ayudará a encontrar luz en la oscuridad.",
                "Eres resiliente y tienes la fuerza para superar esto. Confía en ti mismo.",
                "No te rindas ante la tristeza. Busca ayuda si la necesitas y recuerda que siempre hay esperanza.",
                "La tristeza es una emoción pasajera. No dejes que te robe la alegría de vivir.",
                "Eres capaz de sanar y encontrar la felicidad de nuevo. Ten paciencia y sé amable contigo mismo."
            };

            // Miedo
            std::vector<std::string> frasesMiedo = {
                "El miedo es una emoción natural. No te sientas mal por sentirlo.",
                "Eres valiente y capaz de enfrentar tus miedos. ¡Confía en ti mismo!",
                "Respira hondo y recuerda que eres más fuerte de lo que crees.",
                "No dejes que el miedo te paralice. ¡Da un paso a la vez y avanza!",
                "Enfrenta tus miedos con valentía y descubrirás la fuerza que llevas dentro.",
                "Recuerda que no estás solo. Busca apoyo en quienes te quieren.",
                "El miedo es una oportunidad para crecer y aprender. ¡Atrévete a superarlo!",
                "No te rindas ante el miedo. ¡Eres capaz de lograr grandes cosas!",
                "Visualiza el éxito y confía en que puedes superar cualquier obstáculo.",
                "El miedo es solo una emoción. No dejes que te controle.",
                "Transforma tu miedo en energía positiva y úsala para avanzar.",
                "Recuerda que el miedo es temporal. ¡La valentía es eterna!",
                "Cada paso que das para enfrentar tus miedos te hace más fuerte.",
                "No te avergüences de tus miedos. Todos los tenemos.",
                "Busca ayuda si la necesitas. No hay nada de malo en pedir apoyo.",
                "El miedo es una señal de que estás saliendo de tu zona de confort. ¡Eso es bueno!",
                "Enfócate en tus fortalezas y recuerda que eres capaz de superar cualquier desafío.",
                "No dejes que el miedo te impida perseguir tus sueños. ¡Ve por ellos!",
                "El miedo es una oportunidad para descubrir tu verdadero potencial. ¡Aprovéchala!",
                "Recuerda que eres valiente y capaz de superar cualquier cosa que se te presente."
            };

            // Enojo
            std::vector<std::string> frasesEnojo = {
                "Está bien sentir enojo, pero no dejes que te controle. Respira hondo y busca soluciones.",
                "Expresa tu enojo de forma saludable. Habla con alguien de confianza o escribe tus sentimientos.",
                "No te guardes el enojo. Busca formas de liberarlo de manera constructiva.",
                "Recuerda que el enojo es una emoción pasajera. No dejes que arruine tu día.",
                "Identifica la causa de tu enojo y busca soluciones para resolver el problema.",
                "No tomes decisiones impulsivas cuando estés enojado. Date tiempo para calmarte.",
                "El enojo puede ser una señal de que algo necesita cambiar. Reflexiona sobre ello.",
                "No descargues tu enojo en los demás. Busca formas de canalizarlo de manera positiva.",
                "Recuerda que tienes el control de tus emociones. No dejes que el enojo te domine.",
                "Busca actividades que te ayuden a relajarte y liberar el enojo, como el ejercicio o la meditación.",
                "No te culpes por sentir enojo. Es una emoción humana y válida.",
                "Aprende a reconocer las señales de que te estás enojando y busca formas de calmarte antes de que se intensifique.",
                "El enojo puede ser una fuente de energía. Úsala para motivarte a hacer cambios positivos.",
                "No te rindas ante el enojo. Busca ayuda si la necesitas y aprende a manejarlo de manera saludable.",
                "Recuerda que tienes el poder de elegir cómo reaccionar ante el enojo. Elige la calma y la serenidad.",
                "No dejes que el enojo te aleje de las personas que te importan. Comunícate abiertamente y busca soluciones juntos.",
                "El perdón puede ser una herramienta poderosa para liberar el enojo. Perdona a los demás y a ti mismo.",
                "Enfócate en las cosas positivas de tu vida y no dejes que el enojo te nuble la visión.",
                "Recuerda que el enojo es una emoción, no una identidad. No te definas por él.",
                "Eres capaz de manejar el enojo de manera constructiva. Confía en ti mismo y busca soluciones."
            };
            std::vector<std::string> frasesDisgusto = {
                "Es normal sentir disgusto a veces. Reconoce tus sentimientos y permítete sentirlos.",
                "No te juzgues por sentir disgusto. Es una emoción válida y todos la experimentamos.",
                "Identifica la causa de tu disgusto y busca formas de alejarte de ella o resolver el problema.",
                "Habla con alguien de confianza sobre lo que te disgusta. Compartir tus sentimientos puede ayudarte a sentirte mejor.",
                "Recuerda que el disgusto es temporal. No dejes que te afecte a largo plazo.",
                "Enfócate en las cosas positivas de tu vida y no dejes que el disgusto te nuble la visión.",
                "Busca actividades que te ayuden a distraerte y a sentirte mejor, como leer, escuchar música o salir a caminar.",
                "No te quedes atrapado en el disgusto. Busca soluciones y sigue adelante.",
                "Recuerda que tienes el control de tus emociones. No dejes que el disgusto te domine.",
                "Aprende a establecer límites saludables para protegerte de situaciones o personas que te disgustan.",
                "El disgusto puede ser una señal de que algo no está bien. Presta atención a tus sentimientos y busca soluciones.",
                "No te culpes por sentir disgusto. Es una reacción natural ante situaciones desagradables.",
                "Busca el lado positivo de las cosas. Incluso en situaciones difíciles, siempre hay algo bueno que aprender.",
                "No dejes que el disgusto te amargue. Busca formas de encontrar alegría y satisfacción en tu vida.",
                "Aprende a perdonar y a dejar ir el disgusto. Guardar rencor solo te hará daño a ti mismo.",
                "Rodéate de personas positivas y que te apoyen. Evita a aquellos que te generan disgusto.",
                "El disgusto puede ser una oportunidad para crecer y cambiar. Aprende de la experiencia y sigue adelante.",
                "Recuerda que tienes el poder de elegir cómo reaccionar ante el disgusto. Elige la paz y la tranquilidad.",
                "No te dejes llevar por el disgusto. Busca soluciones y toma medidas para mejorar la situación.",
                "Eres capaz de superar cualquier disgusto y encontrar la felicidad de nuevo. Ten fe en ti mismo."
            };
            std::vector<std::string> frasesSorpresa = {
                "¡Wow! ¡Esto es increíble!",
                "¡No puedo creerlo! ¡Qué sorpresa!",
                "¡Guau! ¡Esto es más de lo que esperaba!",
                "¡Esto es sorprendente! ¡Nunca lo hubiera imaginado!",
                "¡Qué giro inesperado! ¡Esto cambia todo!",
                "¡Me has dejado sin palabras! ¡Es asombroso!",
                "¡Esto es realmente impresionante! ¡Felicidades!",
                "¡Qué sorpresa tan agradable! ¡Me encanta!",
                "¡Estoy boquiabierto! ¡Esto es increíblemente genial!",
                "¡No me lo esperaba! ¡Qué emocionante!",
                "¡Esto es realmente sorprendente! ¡Estoy impresionado!",
                "¡Qué revelación! ¡Esto cambia mi perspectiva!",
                "¡Esto es increíblemente inesperado! ¡Me encanta la sorpresa!",
                "¡Guau! ¡Esto es mucho mejor de lo que esperaba!",
                "¡Esto es realmente alucinante! ¡No puedo creer lo que veo!",
                "¡Qué sorpresa tan maravillosa! ¡Estoy muy feliz por ti!",
                "¡Esto es realmente sorprendente! ¡Nunca lo hubiera adivinado!",
                "¡Me has dejado atónito! ¡Es increíblemente sorprendente!",
                "¡Wow! ¡Esto es realmente espectacular!",
                "¡Qué sorpresa tan emocionante! ¡No puedo esperar a ver qué sigue!"
            };
            reEmFrasesMotivacionales = {frasesMiedo, frasesTristeza, frasesAlegria, frasesEnojo, frasesSorpresa, frasesDisgusto};
        }
        // creamos un frame para el registro de emociones
        frameRegistroEmociones = new QFrame(framePrincipal);
        // ocultamos por defecto
        frameRegistroEmociones->hide();
        // creamos los qiconos de las emociones
        QStringList reEmIconos = {"Miedo-icono.svg", "Tristeza-icono.svg", "Sorpresa-icono.svg", "Alegria-icono.svg", "Enojo-icono.svg", "Disgusto-icono.svg"};
        reEmIconoEmoMiedo = new QIcon(QString(RUTA_ICONOS.c_str()) + reEmIconos[0]);
        reEmIconoEmoTristeza = new QIcon(QString(RUTA_ICONOS.c_str()) + reEmIconos[1]);
        reEmIconoEmoSorpresa = new QIcon(QString(RUTA_ICONOS.c_str()) + reEmIconos[2]);
        reEmIconoEmoAlegria = new QIcon(QString(RUTA_ICONOS.c_str()) + reEmIconos[3]);
        reEmIconoEmoEnojo = new QIcon(QString(RUTA_ICONOS.c_str()) + reEmIconos[4]);
        reEmIconoEmoDisgusto = new QIcon(QString(RUTA_ICONOS.c_str()) + reEmIconos[5]);

        reEmTitulo = new QLabel(frameRegistroEmociones); // titulo
        //reEmTitulo->setMinimumWidth(500); // tamaño mínimo
        //reEmTitulo->setMaximumWidth(2000); // tamaño máximo
        reEmTitulo->setText("¿Como te sientes?");
        reEmTitulo->setMaximumHeight(100);
        reEmTitulo->setAlignment(Qt::AlignCenter); // centrado

        // // tamaño de los botones
        reEmTamBotones = new QSize(100, 100);

        // Botones de emociones
        reEmBotonMiedo = new QPushButton(frameRegistroEmociones);
        reEmBotonMiedo->setIcon(*reEmIconoEmoMiedo);
        reEmBotonMiedo->setIconSize(*reEmTamBotones);
        reEmBotonTristeza = new QPushButton(frameRegistroEmociones);
        reEmBotonTristeza->setIcon(*reEmIconoEmoTristeza);
        reEmBotonTristeza->setIconSize(*reEmTamBotones);
        reEmBotonSorpresa = new QPushButton(frameRegistroEmociones);
        reEmBotonSorpresa->setIcon(*reEmIconoEmoSorpresa);
        reEmBotonSorpresa->setIconSize(*reEmTamBotones);
        reEmBotonAlegria = new QPushButton(frameRegistroEmociones);
        reEmBotonAlegria->setIcon(*reEmIconoEmoAlegria);
        reEmBotonAlegria->setIconSize(*reEmTamBotones);
        reEmBotonEnojo = new QPushButton(frameRegistroEmociones);
        reEmBotonEnojo->setIcon(*reEmIconoEmoEnojo);
        reEmBotonEnojo->setIconSize(*reEmTamBotones);
        reEmBotonDisgusto = new QPushButton(frameRegistroEmociones);
        reEmBotonDisgusto->setIcon(*reEmIconoEmoDisgusto);
        reEmBotonDisgusto->setIconSize(*reEmTamBotones);



        // Layout principal
        reEmLayout = new QVBoxLayout(frameRegistroEmociones);
        //reEmLayout->addLayout(reEmLayoutTitulo);

        //reEmRedimencionarCosas(); // redimencionamos cosas

        // CONECTAMOS LOS BOTONES DE EMOCIONES CON SUS RESPECTIVAS FUNCIONES
        {
            connect(reEmBotonMiedo, SIGNAL(clicked()), this, SLOT(reEmClickMiedo()));
            connect(reEmBotonTristeza, SIGNAL(clicked()), this, SLOT(reEmClickTristeza()));
            connect(reEmBotonSorpresa, SIGNAL(clicked()), this, SLOT(reEmClickSorpresa()));
            connect(reEmBotonAlegria, SIGNAL(clicked()), this, SLOT(reEmClickAlegria()));
            connect(reEmBotonEnojo, SIGNAL(clicked()), this, SLOT(reEmClickEnojo()));
            connect(reEmBotonDisgusto, SIGNAL(clicked()), this, SLOT(reEmClickDisgusto()));
        }

        // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        // Label para la frase motivacional
        reEmFraseMotivacional = new QLabel(frameRegistroEmociones);
        reEmFraseMotivacional->hide();
        //reEmFraseMotivacional->setAlignment(Qt::AlignCenter);
        // Habilitar ajuste de texto
        reEmFraseMotivacional->setWordWrap(true);

        // Establecer política de ajuste en espacios entre palabras
        reEmFraseMotivacional->setTextFormat(Qt::RichText); // Necesario para aplicar la política
        reEmFraseMotivacional->setTextInteractionFlags(Qt::TextSelectableByMouse); // Opcional, para permitir selección de texto
        reEmFraseMotivacional->setWordWrap(true);
        reEmFraseMotivacional->setAlignment(Qt::AlignCenter);
        //reEmFraseMotivacional->setMinimumSize(400,200);
        // le metemos un estilo con fondo blanco, letras negras y tamaño de letra de 20px bordes redondeados y un espacio a la derecha de 10px
        reEmFraseMotivacional->setStyleSheet("background-color: white; color: black; font-size: 20px; border-radius: 10px; padding-right: 10px;");

        // Label para emocion seleccionada
        reEmIconoFraseMotivacionalLabel = new QLabel(frameRegistroEmociones);
        reEmIconoFraseMotivacionalLabel->hide();
        reEmIconoFraseMotivacionalLabel->setScaledContents(true);

        // label para proximo registro de emocion
        reEmHoraProximaFraseMotivacional = new QLabel(frameRegistroEmociones);
        reEmHoraProximaFraseMotivacional->hide();


        // Grid layout para las emociones
        reEmLayoutBotones = new QGridLayout(frameRegistroEmociones);

        // Título (ocupa las 3 primeras columnas)
        reEmLayoutBotones->addWidget(reEmTitulo, 0, 0, 1, 3);

        // Ícono y frase motivacional (deben ir antes de los botones)
        reEmLayoutBotones->addWidget(reEmIconoFraseMotivacionalLabel, 1, 0, 1, 1);  // 2 filas, 2 columnas
        reEmLayoutBotones->addWidget(reEmFraseMotivacional, 1, 1, 1, 2);         // 2 filas, 1 columna
        reEmLayoutBotones->addWidget(reEmHoraProximaFraseMotivacional, 2, 0, 1, 3); // 2 filas, 2 columnas
        // Botones de emociones
        reEmLayoutBotones->addWidget(reEmBotonMiedo, 3, 0);
        reEmLayoutBotones->addWidget(reEmBotonTristeza, 3, 1);
        reEmLayoutBotones->addWidget(reEmBotonSorpresa, 3, 2);
        reEmLayoutBotones->addWidget(reEmBotonAlegria, 4, 0);
        reEmLayoutBotones->addWidget(reEmBotonEnojo, 4, 1);
        reEmLayoutBotones->addWidget(reEmBotonDisgusto, 4, 2);



        // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        // Cosas de pantalla grafica de emociones
        reEmGraficaSeries = new QPieSeries(frameRegistroEmociones);
        reEmGraficaSeries->append("Miedo", 0);
        reEmGraficaSeries->append("Tristeza", 4);
        reEmGraficaSeries->append("Sorpresa", 2);
        reEmGraficaSeries->append("Alegría", 1);
        reEmGraficaSeries->append("Enojo", 0);
        reEmGraficaSeries->append("Disgusto", 0);
        reEmGrafica = new QChart();
        reEmGrafica->addSeries(reEmGraficaSeries);
        reEmGrafica->setTitle("Emociones del dia");
        reEmGrafica->legend()->setAlignment(Qt::AlignRight);
        reEmGraficaVista = new QChartView(reEmGrafica);
        reEmGraficaVista->setRenderHint(QPainter::Antialiasing);
        reEmGraficaVista->hide();
        // hacemos que la grafica sea redimensionable
        //reEmGraficaVista->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        // lo agregamos al layout de botones
        reEmLayoutBotones->addWidget(reEmGraficaVista, 5, 0, 1, 3);


        // Espaciador (ahora en la última fila)
        reEmLayoutBotones->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 6, 0, 1, 3);
        reEmLayout->addLayout(reEmLayoutBotones);


    }

    // -----------------------------------------------------------------------------
    // AREA DE AJUSTES
    /* Debe contener:
     * Título
     * Selector de Tema
     * Botón Para Cambiar Sonido de Alarma
     * Botón Para Cambiar Sonido de Notificación
     * Botón Para Cambiar fuente
     * Botón Para Cambiar tamaño de fuente
     * Botón Para Cambiar contraseña
     * Botón Para Cambiar pregunta de seguridad
     * Botón Para Cambiar respuesta de seguridad
     * Botón Para Cambiar foto de perfil
     * Botón Para Eliminar Cuenta
     */
    {
        frameAjustes = new QFrame(framePrincipal); // creamos un frame para los ajustes
        frameAjustes->hide(); // lo ocultamos por defecto

        ajusWidget = new QWidget(frameAjustes); // creamos un widget para los ajustes

        // Elementos de la interfaz (graficos)
        ajusTitulo = new QLabel(ajusWidget); // titulo
        ajusTitulo->setText("Ajustes");
        ajusTitulo->setMaximumHeight(100);
        ajusTitulo->setAlignment(Qt::AlignCenter); // centrado
        ajusSelectorTema = new QComboBox(ajusWidget); // selector de tema
        ajusSelectorTema->addItem("Tema: Sistema");
        ajusSelectorTema->addItem("Tema: Oscuro");
        ajusCambiarSonidoAlarma = new QPushButton(ajusWidget); // boton para cambiar sonido de alarma
        ajusCambiarSonidoAlarma->setText("Cambiar Sonido de Alarma");
        //ajusCambiarSonidoNotificacion = new QPushButton(ajusWidget); // boton para cambiar sonido de notificacion
        //ajusCambiarSonidoNotificacion->setText("Cambiar Sonido de Notificación");
        ajusCambiarFuente = new QPushButton(ajusWidget); // boton para cambiar fuente
        ajusCambiarFuente->setText("Cambiar Fuente");
        // ajusCambiarTamFuente = new QPushButton(ajusWidget); // boton para cambiar tamaño de fuente
        // ajusCambiarTamFuente->setText("Cambiar Tamaño de Fuente");
        ajusCambiarContra = new QPushButton(ajusWidget); // boton para cambiar contraseña
        ajusCambiarContra->setText("Cambiar Contraseña");
        ajusCambiarPreguntaContra = new QPushButton(ajusWidget); // boton para cambiar pregunta de seguridad
        ajusCambiarPreguntaContra->setText("Cambiar Pregunta de Seguridad");
        //ajusCambiarRespuestaContra = new QPushButton(ajusWidget); // boton para cambiar respuesta de seguridad
        //ajusCambiarRespuestaContra->setText("Cambiar Respuesta de Seguridad");
        ajusCambiarFotoPerfil = new QPushButton(ajusWidget); // boton para cambiar foto de perfil
        ajusCambiarFotoPerfil->setText("Cambiar Foto de Perfil");
        ajusEliminarCuenta = new QPushButton(ajusWidget); // boton para eliminar cuenta
        ajusEliminarCuenta->setText("Eliminar Cuenta");

        // Layout
        ajusLayout = new QVBoxLayout(ajusWidget); // layout principal
        ajusLayout->addWidget(ajusTitulo); // titulo
        ajusLayout->addWidget(ajusSelectorTema); // selector de tema
        ajusLayout->addWidget(ajusCambiarSonidoAlarma); // boton para cambiar sonido de alarma
        //ajusLayout->addWidget(ajusCambiarSonidoNotificacion); // boton para cambiar sonido de notificacion
        ajusLayout->addWidget(ajusCambiarFuente); // boton para cambiar fuente
        //ajusLayout->addWidget(ajusCambiarTamFuente); // boton para cambiar tamaño de fuente
        ajusLayout->addWidget(ajusCambiarContra); // boton para cambiar contraseña
        ajusLayout->addWidget(ajusCambiarPreguntaContra); // boton para cambiar pregunta de seguridad
        //ajusLayout->addWidget(ajusCambiarRespuestaContra); // boton para cambiar respuesta de seguridad
        ajusLayout->addWidget(ajusCambiarFotoPerfil); // boton para cambiar foto de perfil
        ajusLayout->addWidget(ajusEliminarCuenta); // boton para eliminar cuenta

        // Scroll Area
        ajusScroll = new QScrollArea(frameAjustes); // creamos un scroll area para los ajustes
        ajusScroll->setWidget(ajusWidget); // le asignamos el widget de los ajustes
        ajusScroll->setWidgetResizable(true); // hacemos que el widget sea redimensionable

        // Layout principal
        ajusPrincipal = new QVBoxLayout(frameAjustes); // layout principal
        ajusPrincipal->addWidget(ajusScroll); // agregamos el scroll area al layout principal

    }

    // -----------------------------------------------------------------------------
    // AREA DE HIPERFOCO
    /* Debe contener:
    *
    */
    {
        frameHiperfoco = new QFrame(framePrincipal);    //creamos el Frame de la interfaz de hiperfoco
        frameHiperfoco->hide();                         //lo escondemos por defecto

        banderaHiperfocoActivo = false;                  //bandera para saber si el hiperfoco esta activo o no
        banderaInterfazHiperfoco = false;                //bandera para saber si la interfaz de hiperfoco esta activa o no
        //Iconos
        {
            hipeIconoHiperfocoActivado = new QIcon(QString(RUTA_ICONOS.c_str()) + "hiperfocoActivado.png");
            hipeIconoHiperfocoDesactivado = new QIcon(QString(RUTA_ICONOS.c_str()) + "hiperfoco.png");
            hipeIconoBotonVerMetodos = new QIcon(QString(RUTA_ICONOS.c_str()) + "icono_de_metodos.png");
        }

        hipeLayoutPrincipal = new QVBoxLayout(frameHiperfoco); //creamos el layout principal
        hipeLayoutH1 = new QHBoxLayout(frameHiperfoco);        //creamos el layout para el titulo
        hipeLayoutH2 = new QHBoxLayout(frameHiperfoco);        //creamos el layout para la duracion
        hipeLayoutH3 = new QHBoxLayout(frameHiperfoco);        //creamos el layout para el boton de hiperfoco

        //items de H1
        {
        hipeTituloPrincipal = new QLabel(frameHiperfoco); //creamos un qlabel principal para el titulo de la interfaz
        hipeTituloPrincipal->setText("Botón de Hiperfoco"); //Texto del titulo
        hipeTituloPrincipal->setAlignment(Qt::AlignCenter);  //Alineacion centrada del titulo
        }
        //items de H2
        {
            hipeTextoDuracion = new QLabel(frameHiperfoco); //creamos el texto duracion
            hipeTextoDuracion->setText("Duracion");

            hipeTextoHoras = new QLabel(frameHiperfoco);    //creamos el texto horas
            hipeTextoHoras->setText("Horas:");

            hipeTextoMinutos = new QLabel(frameHiperfoco); //creamos el texto minutos
            hipeTextoMinutos->setText("Minutos:");

            hipeHorasSpinBox = new QSpinBox(frameHiperfoco); //creamos el spinbox para seleccionar horas
            hipeHorasSpinBox->setRange(0,23);                //establecemos el rango

            hipeMinutosSpinBox = new QSpinBox(frameHiperfoco); //creamos el spoinbos para seleccionar minutos
            hipeMinutosSpinBox->setRange(0,59);              //establecemos el rango

            hipeTextoTiempoEstablecido = new QLabel(frameHiperfoco); //creamos el texto que aparecera cuando establescan el tiempo
            hipeTextoTiempoEstablecido->hide();             //lo escondemos por defecto

            timer = new QTimer(frameHiperfoco);         //creamos el temporizador
            timer->setSingleShot(true);                 //solo se activa una vez el temporizador
        }
        //items dd H3
        {
            hipeBotonDeHiperfoco = new QPushButton(frameHiperfoco); //creamos el boton de hiperfoco
            hipeBotonDeHiperfoco->setMinimumSize(420,420);
            hipeBotonDeHiperfoco->setMaximumSize(420,420);
            //hipeBotonDeHiperfoco->setCheckable(true);
            hipeBotonDeHiperfoco->setIcon(*hipeIconoHiperfocoDesactivado);
            hipeBotonDeHiperfoco->setIconSize(QSize(400,400));
        }

        //Interfaz de Metodos
        {
            hipeTituloListaMetodos = new QLabel(frameHiperfoco);
            hipeTituloListaMetodos->setText("Metodos que pueden mejorar tu vida");
            hipeTituloListaMetodos->setAlignment(Qt::AlignCenter);
            hipeTituloListaMetodos->hide();

            hipeListaDeMetodos = new QListWidget(frameHiperfoco);

            hipeItemPomodoro = new QListWidgetItem("Tecnica Pomodoro", hipeListaDeMetodos);
            hipeItem5Segundos = new QListWidgetItem("Regla de los 5 segundos", hipeListaDeMetodos);
            hipeItemMindfulness = new QListWidgetItem("Mindfulness", hipeListaDeMetodos);

            hipeItemPomodoro->setTextAlignment(Qt::AlignCenter);
            hipeItem5Segundos->setTextAlignment(Qt::AlignCenter);
            hipeItemMindfulness->setTextAlignment(Qt::AlignCenter);

            hipeListaDeMetodos->addItem(hipeItemPomodoro);
            hipeListaDeMetodos->addItem(hipeItem5Segundos);
            hipeListaDeMetodos->addItem(hipeItemMindfulness);

            hipeListaDeMetodos->hide();
        }

        //Interfaz Tecnica Pomodoro
        {
            hipeTituloPomodoro = new QLabel(frameHiperfoco);
            hipeTituloPomodoro->setText("Pomodoro");
            hipeTituloPomodoro->setAlignment(Qt::AlignCenter);
            hipeTituloPomodoro->hide();

            hipeTextoPomodoro = new QLabel(frameHiperfoco);
            hipeTextoPomodoro->setText( "Es una técnica de gestión del tiempo desarrollada por Francesco Cirillo a\n"
                                    "finales de los años 80. Toma su nombre de un temporizador con forma\n"
                                    "de tomate de cocina, utilizado para medir intervalos de tiempo\n"
                                    "predefinidos. La técnica consiste en dividir el trabajo en intervalos de\n"
                                    "tiempo generalmente de 25 minutos, llamados \"pomodoros\", separados\n"
                                    "por breves pausas.");
            hipeTextoPomodoro->setAlignment(Qt::AlignCenter);
            hipeTextoPomodoro->hide();

        }
        //Interfaz Tecnica 5 Segundos
        {
            hipeTituloTitulo5Segundos = new QLabel(frameHiperfoco);
            hipeTituloTitulo5Segundos->setText("Regla de los 5 segundos");
            hipeTituloTitulo5Segundos->setAlignment(Qt::AlignCenter);
            hipeTituloTitulo5Segundos->hide();

            hipeTextoTitulo5Segundos = new QLabel(frameHiperfoco);
            hipeTextoTitulo5Segundos->setText( "Popularizada por la autora y entrenadora Mel Robbins, \n"
                                               "es una técnica simple pero poderosa que puede ayudarte a superar la \n"
                                               "procrastinación, el miedo y la duda, y a tomar medidas más rápidamente.\n"
                                               "Identifica un momento en el que te sientas indeciso o procrastinando,\n"
                                               "toma aire y comienza a contar en voz alta de 5 a 1,\n"
                                               "en el momento en que llegues a 1, toma una decisión o realiza una acción.\n"
                                               "No te detengas a pensar o analizar la situación. Actúa de inmediato.\n"
                                               "Observa cómo te sientes después de aplicar la regla.\n"
                                               "¿Te sientes más empoderado, seguro y motivado?");
            hipeTextoTitulo5Segundos->setAlignment(Qt::AlignCenter);
            hipeTextoTitulo5Segundos->hide();
        }
        //Interfaz Tecnicas Mindfulness
        {
            hipeTituloMindfulness = new QLabel(frameHiperfoco);
            hipeTituloMindfulness->setText("Minfulness");
            hipeTituloMindfulness->setAlignment(Qt::AlignCenter);
            hipeTituloMindfulness->hide();

            hipeTextoMindfulness = new QLabel(frameHiperfoco);
            hipeTextoMindfulness->setText( "El mindfulness, también conocido como atención plena, es la práctica de prestar\n"
                                               "atención a la experiencia presente sin juzgar. Implica enfocarse en las sensaciones,\n"
                                               "pensamientos y emociones que surgen en el momento, sin aferrarse a ellos ni\n"
                                               "dejarse llevar por ellos.\n"
                                               "Meditación: La meditación es una práctica formal de mindfulness que implica\n"
                                               "sentarse en silencio y enfocar la atención en la respiración o en un mantra. \n"
                                               "Yoga: El yoga combina posturas físicas, respiración y meditación para promover el\n"
                                               "mindfulness y el bienestar general.");
            hipeTextoMindfulness->setAlignment(Qt::AlignCenter);
            hipeTextoMindfulness->hide();
        }
        //conexiones
        QObject::connect(hipeBotonDeHiperfoco, SIGNAL( clicked() ), this, SLOT( hipeBotonHiperfocoActivado() ) );
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT( hipeBotonHiperfocoDesactivado() ) );
        QObject::connect(hipeListaDeMetodos, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT( hipeManejadorDeListadeItems(QListWidgetItem*)) );
        //hipeBotonDeHiperfoco

        //Estructura del frame
        {
        hipeLayoutH1->addWidget(hipeTituloPrincipal);  //añadimos el titulo a su espacio horizontal

        hipeLayoutH2->addWidget(hipeTextoDuracion);   //añadimos el texto de duracion a su espacio horizontal H2
        hipeLayoutH2->addWidget(hipeTextoTiempoEstablecido); //añadimos el texto de tiempo establecido que aparecera cuando se active el boton
        hipeLayoutH2->addWidget(hipeTextoHoras);      //añadimos el texto de duracion a su espacio horizontal H2
        hipeLayoutH2->addWidget(hipeHorasSpinBox);    //añadimos el espacio de seleccion
        hipeLayoutH2->addWidget(hipeTextoMinutos);    //añadimos el texto minutos
        hipeLayoutH2->addWidget(hipeMinutosSpinBox);  //añadimos el espacio de seleccion

        hipeLayoutH3->addWidget(hipeBotonDeHiperfoco); //añadimos el boton al espacio H3

        hipeLayoutPrincipal->addLayout(hipeLayoutH1);  //añadimos el espacio al layout principal
        hipeLayoutPrincipal->addLayout(hipeLayoutH2);  //añadimos el espacio H2 al layout principal
        hipeLayoutPrincipal->addLayout(hipeLayoutH3);  //añadimos el espacio H3 al layout principal

        //Interfaz de Lista de Metodos
        hipeLayoutPrincipal->addWidget(hipeTituloListaMetodos);
        hipeLayoutPrincipal->addWidget(hipeListaDeMetodos);

        //Interfaz Tecnicas
        hipeLayoutPrincipal->addWidget(hipeTituloPomodoro);
        hipeLayoutPrincipal->addWidget(hipeTextoPomodoro);
        hipeLayoutPrincipal->addWidget(hipeTituloTitulo5Segundos);
        hipeLayoutPrincipal->addWidget(hipeTextoTitulo5Segundos);
        hipeLayoutPrincipal->addWidget(hipeTituloMindfulness);
        hipeLayoutPrincipal->addWidget(hipeTextoMindfulness);

        hipeLayoutPrincipal->addStretch();            //añadimos espacio para que este mas arriba
        }
    }

    // -----------------------------------------------------------------------------
    // AREA DE ACOMODO DE FRAMES

    QVBoxLayout* disposicionPrincipal = new QVBoxLayout(this);

    disposicionPrincipal->addWidget(frameBarraNavegacion);
    // hacemos que los elementos se acomoden de arriba hacía abajo desde la parte superior sin centrar
    //disposicionPrincipal->addStretch();
    disposicionPrincipal->addWidget(frameInicioSesion);
    disposicionPrincipal->addWidget(frameRegistroUsuario); // se oculta por que ahora lo contiene el scrollRegistroUsuario
    disposicionPrincipal->addWidget(frameRecuperarContra);
    disposicionPrincipal->addWidget(frameJournaling);
    disposicionPrincipal->addWidget(frameCalendario);
    disposicionPrincipal->addWidget(calDiaFrame);
    disposicionPrincipal->addWidget(frameRegistroEmociones);
    disposicionPrincipal->addWidget(frameAjustes);
    disposicionPrincipal->addWidget(frameHiperfoco);
    //disposicionPrincipal->addWidget(scrollRegistroUsuario); // modificacion para agregar scroll al frame de registro de usuario




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
    {

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
        // conectamos la señal de seleccionar usuario con inSeCargarFotoPerfil
        connect(inSeSelectorUsuario, SIGNAL(currentIndexChanged(int)), this, SLOT(inSeCargarImagenPerfil(int)));

        // SEÑALES RELACIONADAS CON EL RECUPERAR CONTRASEÑA
        // conectamos la señal de qcombobox con el metodo reCoCargarDatosUsuario
        connect(reCoSelectorUsuario, SIGNAL(currentIndexChanged(int)), this, SLOT(reCoCargarDatosUsuario()));
        // conectamos la señal de recuperar contraseña
        connect(reCoBotonConfirmar, SIGNAL(clicked()), this, SLOT(reCoRecuperarContra()));
        // conectamos la señal de cancelar
        connect(reCoBotonCancelar, SIGNAL(clicked()), this, SLOT(reCoCancelarRecuperarContra()));

        // SEÑALES RELACIONADAS CON LA BARRA DE NAVEGACIÓN
        // conectamos la señal para cerrar sesión
        connect(barNaBotonCerrarSesion, SIGNAL(clicked()), this, SLOT(barNaCerrarSesion()));
        // conectamos la señal para abrir el frame de ajustes
        connect(barNaBotonConfiguracion, SIGNAL(clicked()), this, SLOT(barNaMostrarAjustes()));
        // conectamos la señal para activar el frame de calendario
        connect(barNaBotonCalendario, SIGNAL(clicked()), this, SLOT(barNaMostrarCalendario()));
        // conectamos la señal para activar el frame de journaling
        connect(barNaBotonJournaling, SIGNAL(clicked()), this, SLOT(barNaMostrarJournaling()));
        // conectamos la señal para activar el framde de hiperfoco
        connect(barNaBotonHiperfoco, SIGNAL(clicked()), this, SLOT(barNaMostrarHiperfoco()));
        // conectamos la señal para activar el frame de registro de emociones
        connect(barNaBotonRegistroEmociones, SIGNAL(clicked()), this, SLOT(barNaMostrarRegistroEmociones()));
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

        // SEÑALES RELACIONADAS CON EL CALENDARIO
        // conectamos la señal de itemClicked de la lista de eventos del día
        connect(calDiaEventos, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(calInterfazEvento()));

        // SEÑALES RELACIONADAS CON LA INTERFAZ DE AJUSTES
        // conectamos la señal de cambio de fuente
        connect(ajusCambiarFuente, SIGNAL(clicked()), this, SLOT(ajusClickCambiarFuente()));
        // conectamos la señal de cambio de contraseña
        connect(ajusCambiarContra, SIGNAL(clicked()), this, SLOT(ajusClickCambiarContra()));
        // conectamos la señal de cambio de pregunta de seguridad
        connect(ajusCambiarPreguntaContra, SIGNAL(clicked()), this, SLOT(ajusClickCambiarPreguntaContra()));
        // conectamos la señal de cambio de foto de perfil
        connect(ajusCambiarFotoPerfil, SIGNAL(clicked()), this, SLOT(ajusClickCambiarFotoPerfil()));
        // conectamos la señal de eliminar cuenta
        connect(ajusEliminarCuenta, SIGNAL(clicked()), this, SLOT(ajusClickEliminarCuenta()));
        // conectamos la señal de cambio de sonido de alarma
        connect(ajusCambiarSonidoAlarma, SIGNAL(clicked()), this, SLOT(ajusClickCambiarSonidoAlarma()));
        // conectamos la señal de cambio seleccion de item en el selector de tema
        connect(ajusSelectorTema, SIGNAL(currentIndexChanged(int)), this, SLOT(ajusCambiarTema(int)));
    }


}

MainWindow::~MainWindow() {
    // liberamos la memoria de los objetos creados
    // delete configuraciones;
    // delete configuracionesUsuario;
    // delete encriptado;
    // delete entradaSeleccionada;
    // delete calDiaJsonEventos;
}

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
        //     "tamFuente": "20",
        //     "sonidoAlarma": "alarma.mp3",
        //     "volumenAlarma": "100",
        //     "sonidoNotificacion": "notificacion.mp3",
        //     "volumenNotificacion": "100",
        // }
        // como lo acabamos de crear se deberán cargar las configuraciones por defecto
        //std::string fuente = "fuente";
        //configuraciones->operator[]("tema") = "claro";
        auto& conf = *configuraciones;// soy dios y si quiero usar el operador [] de esta forma, lo hago
        conf["tema"] = "0";
        conf["fuente"] = "Arial";
        conf["tamFuente"] = "20";
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

    }
    // se carga la fuente con tamaño 20
    fuente.setFamily(QString::fromStdString((*configuraciones)["fuente"]));
    //fuente.setPointSize(20);
    //fuente.setPointSize((*configuraciones)["tamFuente"]);
    int tamFuente = std::stoi((*configuraciones)["tamFuente"]);
    fuente.setPointSize(tamFuente);

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
    inSeCargarImagenPerfil(0);

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

// ! Método para cargar la foto de perfil del usuario
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::inSeCargarImagenPerfil(int index)
{
    std::string usuario = inSeSelectorUsuario->itemText(index).toStdString();
    if (usuario.empty())
    {
        return;
    }
    std::cout << "Cargando imagen de perfil: " << usuario << std::endl;
    // ? se carga la foto de perfil del usuario
    // ? se obtiene el nombre del usuario
    //std::string usuario = inSeSelectorUsuario->itemText(index).toStdString();
    // ? se obtiene la ruta de la foto de perfil del usuario
    std::string rutaFotoPerfil = RUTA_USUARIOS + usuario;
    mJson::ManejadorJson confUs(RUTA_USUARIOS + usuario + "/config.json");
    rutaFotoPerfil += "/" + encriptado->desencriptar(confUs["fotoPerfil"]);
    std::cout << "Ruta foto perfil: " << rutaFotoPerfil << std::endl;
    // ? se carga la foto de perfil en el label
    QPixmap fotoPerfil(rutaFotoPerfil.c_str());
    inSeFotoPerfil->setPixmap(fotoPerfil.scaled(1000, 1000, Qt::KeepAspectRatio));
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
    //scrollRegistroUsuario->show();
}

// ! método para desactivar el frame de registro de usuarios (interfaz de registro de usuarios)
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::desactivarInterfazRegistroUsuario()
{
    // ? se desactiva el frame de registro de usuarios
    frameRegistroUsuario->hide();
    //scrollRegistroUsuario->hide();
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
// ! versión 2.0
// ! modificado por Aether
// ? Se modifico el método para que si no puede copiar la imagen capture el throw y muestre un mensaje emergente
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
    usuario["nombre"] = encriptado->encriptar(nombre);
    usuario["nombreUsuario"] = encriptado->encriptar(nombreUs);
    usuario["contraseña"] = encriptado->encriptar(contraseña);
    usuario["edad"] = encriptado->encriptar(selectorRegistroEdad->currentText().toStdString());
    usuario["preguntaRecuperacion"] = encriptado->encriptar(preguntaRecuperacion);
    usuario["respuestaRecuperacion"] = encriptado->encriptar(respuestaRecuperacion);
    // ? si el usuario desea realizar copia de seguridad
    if (banderaRegistroCopiaSeg)
    {
        usuario["correoElectronico"] = encriptado->encriptar(correo);
        usuario["contraseñaCorreoElectronico"] = encriptado->encriptar(contraseñaCorreo);
        usuario["frecuenciaCopiaSeguridad"] = encriptado->encriptar(selectorRegistroFrecuenciaCopiaSeguridad->currentText().toStdString());

    }
    // ? se guardará la foto de perfil en la carpeta del usuario
    try {
        manejadorArchivos.copiarArchivo(rutaRegistroFotoPerfil, rutaUsuario + "/fotoPerfil.png");
    } catch (const std::exception& e) {
        //std::cerr << e.what() << '\n';
        QMessageBox::critical(this, "Error", "Error al copiar la foto de perfil, "
                            "si se encuentra usando windows actualmente no podemos extraer"
                            " fotos que se encuentren en carpetas de one drive, le recomendamos cambiar la imagen a una ubicación que no sea conflictiva");
    }
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
    jourBotonCambiarImagenBienvenida->setText("");
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
        //jourCambiarImagenBienvenida();
        jourBotonCambiarImagenBienvenida->setText("Seleccionar Imagen de Bienvenida");
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
    try {
        // mostramos en consola el mensaje
        std::cout << "Seleccionando Imagen de Bienvenida" << std::endl;
        //std::string rutaImagenBienvenida = RUTA_USUARIOS + (*configuracionesUsuario)["nombreUsuario"] + "/imagenBienvenida.png";
        // usuario descifrado
        std::string usuario = encriptado->desencriptar((*configuracionesUsuario)["nombreUsuario"]);
        std::string rutaImagenBienvenida = RUTA_USUARIOS + usuario + "/imagenBienvenida";
        std::string rutaTemp = "";

        //std::cout << "Ruta de la imagen de bienvenida: " << rutaImagenBienvenida << std::endl;
        // ? se seleccionará la imagen de bienvenida
        // ? se obtendrá la ruta de la imagen de bienvenida
        // ? se mostrará un mensaje emergente que indica que la imagen de bienvenida se ha seleccionado
        // ? se obtendrá la ruta de la imagen de bienvenida
        seleccionarArchivo(rutaTemp, "Imagen (*.png *.jpg *.jpeg)", "Seleccionar Imagen de Bienvenida");
        // si la ruta está vacía, se cancela la operación
        if (rutaTemp.empty())
        {
            return;
        }
        std::cout << "Ruta Temporal: " << rutaTemp << std::endl;
        // extraemos la extension del archivo
        std::string extension = manejadorArchivos.obtenerExtension(rutaTemp);
        std::cout << "Extension: " << extension << std::endl;
        // Bloque de verificación de existencia de la imagen de bienvenida
        // ? obtenemos la extension de la imagen de bienvenida
        std::string extensionImEx = manejadorArchivos.buscarExtensionArchivo(RUTA_USUARIOS + usuario + "/", "imagenBienvenida");
        std::cout << "Extension ImEx: " << extensionImEx << std::endl;
        // si la imagen de bienvenida ya existe la extension no será vacía
        bool Existe = false;
        if (!extensionImEx.empty())
        {
            // si ya existe una imagen de bienvenida, la renombramos a imagenBienvenidaAnterior
            manejadorArchivos.moverArchivo(RUTA_USUARIOS + usuario + "/imagenBienvenidaAnterior" + extensionImEx, rutaImagenBienvenida + extensionImEx);
            Existe = true;
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
        if (Existe)
        {
            manejadorArchivos.eliminarArchivo(rutaImagenBienvenida + "Anterior" + extensionImEx);
        }
        //
    } catch (const std::exception& e) {
        // si hay un error, se mostrará un mensaje emergente
        QMessageBox::critical(this, "Error", "No se pudo seleccionar la imagen de bienvenida, si usas windows no selecciones imagenes que se encuentren dentro de rutas protegidas por one drive");
    }
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
        // las invertimos para que se muestren en orden descendente
        std::reverse(notas.begin(), notas.end());
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
    // creamos un cuadro de dialogo para ingresar el nombre de la nueva entrada
    //QInputDialog dialogo (this, "Nueva Entrada", "Nombre De la entrada:", QLineEdit::Normal, "", &ok);
    QString estilo = "QLineEdit {"
                    "border: 2px solid #000000;"
                    "border-radius: 10px;"
                    "padding: 0 8px;"
                    "background: #FFFFFF;"
                    "color: #000000;"
                    "font-size: 16px;"
                    "font-weight: bold;"
                    "}";
    QInputDialog dialogo;
    dialogo.setInputMode(QInputDialog::TextInput);
    dialogo.setLabelText("Nombre de la Entrada:");
    dialogo.setTextValue("");
    dialogo.setOkButtonText("Aceptar");
    dialogo.setCancelButtonText("Cancelar");
    dialogo.setStyleSheet(estilo);
    dialogo.setWindowTitle("Nueva Entrada");

    //QString nombreEntrada = QInputDialog::getText(this, "Nueva Entrada", "Nombre de la Entrada:", QLineEdit::Normal, "", &ok);
    // Le damos un estilo al cuadro de dialogo
    //QString nombreEntrada = QInputDialog::getText(this, "Nueva Entrada", "Nombre de la Entrada:", QLineEdit::Normal, "", &ok);
    QString nombreEntrada = "";
    if (dialogo.exec())
    {
        nombreEntrada = dialogo.textValue();
        ok = true;
    }
    else
    {
        ok = false;
    }
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
            // ? Guardamos la entrada
            entradaSeleccionada->guardar();
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
            //jourReEnCampoTexto->setHtml(QString::fromStdString(encriptado->desencriptar((*entradaSeleccionada)["texto"])));
            jourReEnCampoTexto->setPlainText(QString::fromStdString(encriptado->desencriptar((*entradaSeleccionada)["texto"])));
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
    //std::string texto = jourReEnCampoTexto->toHtml().toStdString();
    std::string texto = jourReEnCampoTexto->toPlainText().toStdString();
    // Guardamos el titulo y el texto de la entrada
    (*entradaSeleccionada)["titulo"] = encriptado->encriptar(titulo);
    //(*entradaSeleccionada)["titulo"] = encriptado->encriptar(titulo);
    (*entradaSeleccionada)["texto"] = encriptado->encriptar(texto);
    //(*entradaSeleccionada)["texto"] = encriptado->encriptar(texto);
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
    //std::cout << "Tamaño del frame de journaling: " << size.width() << " x " << size.height() << std::endl;
    jourBotonCambiarImagenBienvenida->setFixedSize(size.width() - 20, size.height() - 40);
    //jourBotonCambiarImagenBienvenida->size();
    //jourBotonCambiarImagenBienvenida->setIconSize(jourBotonCambiarImagenBienvenida->size());
    QSize BotonSize = jourBotonCambiarImagenBienvenida->size();
    BotonSize.setWidth(BotonSize.width());
    BotonSize.setHeight(BotonSize.height());
    jourBotonCambiarImagenBienvenida->setIconSize(BotonSize);
    //std::cout << "Tamaño final del frame de journaling: " << frameJournaling->size().width() << " x " << frameJournaling->size().height() << std::endl;
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

// ! Método para mostrar la interfaz de ajustes
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::barNaMostrarAjustes()
{
    // ? se desactivan todas las interfaces (frames)
    barNaDesactivarTodosLosFrames();
    // ? se activa el frame de ajustes
    activarInterfazAjustes();
}

// ! método para cerrar sesión
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::barNaCerrarSesion()
{
    // creamos un mensaje emergente para confirmar si se desea cerrar sesión
    QMessageBox msgBox;
    msgBox.setText("¿Estás seguro de que deseas cerrar sesión?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int respuesta = msgBox.exec();
    // verificamos la respuesta
    if (respuesta == QMessageBox::No)
    {
        return;
    }
    if (configuracionesUsuario != nullptr)
    {
        // ? se eliminan las configuraciones del usuario
        delete configuracionesUsuario;
        configuracionesUsuario = nullptr;
    }
    // ? se desactivan todas las interfaces (frames)
    barNaDesactivarTodosLosFrames();
    desactivarBarraNavegacion();
    // ? se activa el frame de inicio de sesión
    activarInterfazInicioSesion();
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

// ! método para activar la interfaz de hiperfoco
// ! versión 1.0
// ! modificado por mbraulio
// ? Sin cambios primera versión
void MainWindow::barNaMostrarHiperfoco()
{
    // ? se desactivan todas las interfaces (frames)
    barNaDesactivarTodosLosFrames();
    // ? se activa el frame de hiperfoco
    activarInterfazHiperfoco();
}

// ! método para activar la interfaz de registro de emociones
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::barNaMostrarRegistroEmociones()
{
    // ? se desactivan todas las interfaces (frames)
    barNaDesactivarTodosLosFrames();
    // ? se activa el frame de registro de emociones
    activarInterfazRegistroEmociones();
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
    desactivarInterfazRegistroEmociones();
    desactivarInterfazAjustes();
    desactivarInterfazHiperfoco();
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
    // ? Para mostrar el mes anterior en el calendario
    case 5:
        boton->setText("Mes Anterior");
        break;
    // ? Para mostrar el mes siguiente en el calendario
    case 6:
        boton->setText("Mes Siguiente");
        break;
    // ? Para agregar un nuevo evento en el calendario
    case 7:
        boton->setText("Nuevo Evento");
        break;
    // ? Para guardar un evento en el calendario
    case 8:
        //boton->setText("Guardar Evento");
        boton->setIcon(*jourIconoGuardarEntrada);
        break;
    // ? Para mostrar la interfaz de eventos del calendario en modo edición
    case 9:
        //boton->setText("Editar Evento");
        boton->setIcon(*jourIconoEditarEntrada);
        break;
    // ? Para eliminar un evento en el calendario
    case 10:
        //boton->setText("Eliminar Evento");
        boton->setIcon(*jourIconoEliminarEntrada);
        break;
    // ? Para mostrar la interfaz de hiperfoco
    case 11:
            boton->setIcon(*hipeIconoBotonVerMetodos);
            break;
    // ? En el caso default no se asigna ningun icono
    default:
        boton->setIcon(QIcon());
        boton->setText("");
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
        jourActivarListaEntradas();
        break;
    // ? Para crear una nueva entrada en journaling
    case 1:
        std::cout << "JourLiNoNuevaEntrada" << std::endl;
        jourLiNoNuevaEntrada();
        break;
    // ? Para guardar una entrada en journaling
    case 2:
        std::cout << "JourGuardarEntrada" << std::endl;
        jourGuardarEntrada();
        break;
    // ? Para activar editar entrada en journaling en modo escritura
    case 3:
        std::cout << "JourActivarEditarEntrada" << std::endl;
        jourActivarEditarEntrada();
        break;
    // ? Para eliminar una entrada en journaling
    case 4:
        std::cout << "JourEliminarEntrada" << std::endl;
        jourEliminarEntrada();
        break;
        // /-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/
        // ----------------------------CALENDARIO-------------------------------------
    // ? Para mostrar el mes anterior en el calendario
    case 5:
        std::cout << "CalMesAnterior" << std::endl;
        calMesAnterior();
        break;
    // ? Para mostrar el mes siguiente en el calendario
    case 6:
        std::cout << "CalMesSiguiente" << std::endl;
        calMesSiguiente();
        break;
    // ? Para agregar un nuevo evento en el calendario
    case 7:
        std::cout << "CalAgregarEvento" << std::endl;
        calInterfazEvento(2);
        break;
    // ? Para guardar un evento en el calendario
    case 8:
        std::cout << "CalGuardarEvento" << std::endl;
        calGuardarEvento();
        break;
    // ? Para mostrar la interfaz de eventos del calendario en modo edición
    case 9:
        std::cout << "CalEditarEvento modo edición" << std::endl;
        calInterfazEvento(1);
        break;
    // ? Para eliminar un evento en el calendario
    case 10:
        std::cout << "CalEliminarEvento" << std::endl;
        calEliminarEvento();
        break;
    // ? Para mostrar la interfaz de hiperfoco
    case 11:
            std::cout << "hipeIconoBotonVerMetodos" << std::endl;
            hipeBotonVerMetodos();
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

    frameCalendarioP->show();
    std::cout << "Activando interfaz de calendario" << std::endl;
    // ? se activa el frame de calendario
    calActivarCalendario();
}

// ! método para desactivar el frame del calendario
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::desactivarInterfazCalendario()
{
    frameCalendarioP->hide();
    frameCalendario->hide();
    calDiaFrame->hide();
    calDesactivarCalendario();

}

// ! método para mostrar el calendario
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::calActivarCalendario()
{
    // ? se Desactiva la interfaz de calendario antes de activarla para asegurar su correcto funcionamiento
    calDesactivarCalendario();
    frameCalendario->show();
    calCargarDatosCalendario();
    // ? se activa el boton 0 y 1 de la barra de navegación
    barNaConfigurarBotones(0, true, 5, "Mes Anterior");
    barNaConfigurarBotones(1, true, 6, "Mes Siguiente");


}

// ! método para ocultar el calendario
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::calDesactivarCalendario()
{
    frameCalendario->hide();
    // ? se desactiva el boton 0 y 1 de la barra de navegación
    barNaConfigurarBotones(0, false, -1, "");
    barNaConfigurarBotones(1, false, -1, "");
    if (editandoEvento)
    {
        std::cout << "Se guardará el evento" << std::endl;
        editandoEvento = false;
        // guardamos el evento
        calGuardarEvento();
    }
}

// ! método para cargar eventos en el calendario
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
int MainWindow::calObtenerNumeroEventosDia(const int& anio, const int& mes, const int& dia)
{
    int numeroEventos = 0;
    // construimos la ruta de la carpeta de eventos
    std::string rutaEventos = RUTA_USUARIOS + encriptado->desencriptar((*configuracionesUsuario)["nombreUsuario"]) + "/calendario/" + std::to_string(anio) + "/" + std::to_string(mes) + "/" + std::to_string(dia);
    // verificamos si existe la carpeta de eventos
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaEventos))
    {
        // si no existe la carpeta de eventos, retornamos 0
        return 0;
    }
    // obtenemos los eventos de la carpeta
    std::vector<std::string> eventos = manejadorArchivos.obtenerContenidoCarpeta(rutaEventos, 1);
    // obtenemos el numero de eventos
    numeroEventos = eventos.size();

    return numeroEventos;
}

// ! Método para obtener la emocion predominante de un dia
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
int MainWindow::calObtenerEmocionPredominanteDia(const int& anio, const int& mes, const int& dia)
{
    int emocionPredominante = -1; // -1 = Sin emocion predominante
    // construimos la ruta de la carpeta de eventos
    std::string rutaEventos = RUTA_USUARIOS + encriptado->desencriptar((*configuracionesUsuario)["nombreUsuario"]) + "/emociones/" + std::to_string(anio) + "/" + std::to_string(mes) + "/" + std::to_string(dia);
    // verificamos si existe la carpeta de eventos
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaEventos))
    {
        // si no existe la carpeta de eventos, retornamos -1
        return emocionPredominante;
    }
    // obtenemos los eventos de la carpeta
    std::vector<std::string> eventos = manejadorArchivos.obtenerContenidoCarpeta(rutaEventos, 0);
    // vemos que evento tiene un numero mayor de emociones
    int numeroEmociones = 0;
    int numeroEvento = 0;
    int nEvAct = 0;
    for (const auto& evento : eventos)
    {
        // obtenemos el contenido del archivo del evento
        std::string rutaEvento = rutaEventos + "/" + evento;
        std::string contenido = manejadorArchivos.leerArchivo(rutaEvento);
        contenido = contenido.empty() ? "0" : contenido;
        int nEmo = std::stoi(contenido);
        if (nEmo > numeroEmociones)
        {
            numeroEmociones = nEmo;
            numeroEvento = nEvAct;
        }
        nEvAct++;
    }
    // 0 -> miedo
    // 1 -> tristeza
    // 2 -> felicidad
    // 3 -> enojo
    // 4 -> sorpresa
    // 5 -> disgusto
    if (eventos[numeroEvento] == "miedo")
    {
        emocionPredominante = 0;
    }
    else if (eventos[numeroEvento] == "tristeza")
    {
        emocionPredominante = 1;
    }
    else if (eventos[numeroEvento] == "alegria")
    {
        emocionPredominante = 2;
    }
    else if (eventos[numeroEvento] == "enojo")
    {
        emocionPredominante = 3;
    }
    else if (eventos[numeroEvento] == "sorpresa")
    {
        emocionPredominante = 4;
    }
    else if (eventos[numeroEvento] == "disgusto")
    {
        emocionPredominante = 5;
    }
    return emocionPredominante;
}


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Relacionados el funcionamiento del calendario

// ! metodo que muestra el mes anterior en el calendario
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::calMesAnterior()
{
    // ? se cargará el mes anterior en el calendario
    // ? se mostrará un mensaje emergente que indica que se ha cargado el mes anterior
    if (calMes == 1)
    {
        calCargarDatosCalendario(12, calAnio - 1);
    }
    else
    {
        calCargarDatosCalendario(calMes - 1, calAnio);
    }

}

// ! método que muestra el mes siguiente en el calendario
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::calMesSiguiente()
{
    // ? se cargará el mes siguiente en el calendario
    // ? se mostrará un mensaje emergente que indica que se ha cargado el mes siguiente
    if (calMes == 12)
    {
        calCargarDatosCalendario(1, calAnio + 1);
    }
    else
    {
        calCargarDatosCalendario(calMes + 1, calAnio);
    }
}

// ! método para cargar los datos del calendario en el mes y año especificados
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::calCargarDatosCalendario(int mes, int anio)
{
    // primero limpiamos el calendario
    calLimpiarCalendario();


    // Si el mes y el anio son -1 quiere decir que se cargará el mes y el año actual
    if (mes == -1 && anio == -1)
    {
        // Obtenemos el mes y el año actual
        calMes = QDate::currentDate().month();
        calAnio = QDate::currentDate().year();
    }
    else
    {
        // Si el mes y el año son distintos de -1, se cargará el mes y el año especificados
        calMes = mes;
        calAnio = anio;
    }

    //std::cout << "mes: " << (*calIconosMeses)[calMes - 1] << std::endl;
    //calIconoMes->setPixmap()
    std::string rutaIconoMes = RUTA_ICONOS + (*calIconosMeses)[calMes - 1];
    //QPixmap iconoMes(QString::fromStdString(rutaIconoMes));
    //calIconoMes->setPixmap(iconoMes);
    calIconoMes->setPixmap(QPixmap(QString::fromStdString(rutaIconoMes)));
    calAnioLabel->setText(QString::number(calAnio));
    calIconoMes->setScaledContents(true);
    // le damos un tamaño maximo
    calIconoMes->setMaximumSize(100, 500);
    // ? Usaremos la información del mes y año para cargar los datos del calendario
    // ? extraemos el primer dia del mes del calCalendarioBakend
    calCalendarioBackend->dayOfWeek(QDate(calAnio, calMes, 1));
    // ? extraemos el número de días del mes
    int calDiasMes = QDate(calAnio, calMes, 1).daysInMonth();
    //std::cout << "Dias del mes: " << calDiasMes << std::endl;
    // ? dia en el que inicia el mes
    int calDiaInicio = calCalendarioBackend->dayOfWeek(QDate(calAnio, calMes, 1));
    //std::cout << "Dia de inicio: " << calDiaInicio << std::endl;
    //int semIni = calDiaInicio > 6 ? 1 : 0;
    int semIni = 0;

    //std::cout << "Dias Sem Fin: " << diasSemFin << std::endl;
    for (int semana = semIni + 1; semana < 7; semana++)
    {
        for (int dia = 0; dia < 7; dia++)
        {
            QLabel* label = frameCalendario->findChild<QLabel*>("numero" + QString::number(semana) + QString::number(dia));
            QPushButton* botonEmocion = frameCalendario->findChild<QPushButton*>("emocion" + QString::number(semana) + QString::number(dia));
            botonEmocion->hide();
            int diaNum = (semana - semIni - 1) * 7 + dia - calDiaInicio + 1;
            if (diaNum > calDiasMes)
            {
                label->setText("");
            }
            else if (diaNum > 0)
            {
                label->setText(QString::number(diaNum));
                // obtenemos el numero de eventos del dia
                int numeroEventos = calObtenerNumeroEventosDia(calAnio, calMes, diaNum);
                int nEmocion = calObtenerEmocionPredominanteDia(calAnio, calMes, diaNum);
                // si el numero de eventos es mayor a 0
                if (numeroEventos > 0)
                {
                    // mostramos el label de eventos
                    QLabel* labelEventos = frameCalendario->findChild<QLabel*>("eventos" + QString::number(semana) + QString::number(dia));
                    std::string textoEventos = "<html><p>";
                    if (numeroEventos >= 1) textoEventos += "🔵 "; // no es a lo wey es un simbolo solo que el editor no lo puede interpretar
                    if (numeroEventos >= 2) textoEventos += "🔵 ";
                    if (numeroEventos == 3) textoEventos += "🔵 ";
                    else if (numeroEventos > 3) textoEventos += " ⭕";
                    textoEventos += "</p></html>";
                    //labelEventos->setText(QString::fromStdString(textoEventos));
                    //labelEventos->setTextFormat()
                    // le indicamos al label que le vamos a pasar un texto en formato html
                    //labelEventos->setTextFormat(Qt::RichText);
                    // le pasamos el texto
                    labelEventos->setText(QString::fromStdString(textoEventos));
                    //labelEventos->setText(QString(QChar(0x1F6D1)));
                }
                // si la emocion es mayor a -1
                if (nEmocion > -1)
                {
                    // mostramos el label de emocion
                    QPushButton* botonEmocion = frameCalendario->findChild<QPushButton*>("emocion" + QString::number(semana) + QString::number(dia));
                    botonEmocion->show();
                    // mostramos el icono de la emocion
                    //std::string rutaIconoEmocion = RUTA_ICONOS + (*calIconosEmociones)[nEmocion];
                    // 0 -> miedo
                    // 1 -> tristeza
                    // 2 -> felicidad
                    // 3 -> enojo
                    // 4 -> sorpresa
                    // 5 -> disgusto
                    //botonEmocion->setIcon(QIcon(QString::fromStdString(rutaIconoEmocion)));
                    switch (nEmocion)
                    {
                        case 0:
                            botonEmocion->setIcon(QIcon(*reEmIconoEmoMiedo));
                            break;
                        case 1:
                            botonEmocion->setIcon(QIcon(*reEmIconoEmoTristeza));
                            break;
                        case 2:
                            botonEmocion->setIcon(QIcon(*reEmIconoEmoAlegria));
                            break;
                        case 3:
                            botonEmocion->setIcon(QIcon(*reEmIconoEmoEnojo));
                            break;
                        case 4:
                            botonEmocion->setIcon(QIcon(*reEmIconoEmoSorpresa));
                            break;
                        case 5:
                            botonEmocion->setIcon(QIcon(*reEmIconoEmoDisgusto));
                            break;
                    }

                    //botonEmocion->setIconSize(QSize(50, 50));
                    botonEmocion->setIconSize(QSize(30, 30));
                    botonEmocion->show();
                } else
                {
                    QPushButton* botonEmocion = frameCalendario->findChild<QPushButton*>("emocion" + QString::number(semana) + QString::number(dia));
                    botonEmocion->setIcon(QIcon());
                    botonEmocion->hide();
                }
                // obtenemos el dia actual
                QDate fechaActual = QDate::currentDate();
                // si el dia es el dia actual y el mes es el mes actual y el año es el año actual no se muestra el icono de emocion
                if (diaNum == fechaActual.day() && calMes == fechaActual.month() && calAnio == fechaActual.year())
                {
                    QPushButton* botonEmocion = frameCalendario->findChild<QPushButton*>("emocion" + QString::number(semana) + QString::number(dia));
                    botonEmocion->setIcon(QIcon());
                    botonEmocion->hide();
                }
            }
        }
    }

}

void MainWindow::calClickDia(const int& nfila, const int& ncolumna)
{
    //std::cout << "Click en el dia: " << nfila << " - " << ncolumna << std::endl;
    // ? accedemos al label del dia
    QLabel* label = frameCalendario->findChild<QLabel*>("numero" + QString::number(nfila) + QString::number(ncolumna));
    // extraemos el texto y lo mostramos
    //std::cout << "Dia: " << label->text().toStdString() << std::endl;
    // ? si el label no tiene texto implicará que no hay nada que mostrar
    if (label->text() == "")
    {
        return;
    }
    // ? se llama al método para mostrar los eventos del dia, le pasamos el dia
    calMostrarDiaSeleccionado(label->text().toInt());
}

void MainWindow::calClickEmoDia(const int& nfila, const int& ncolumna)
{
    std::cout << "Click en emocion: " << nfila << " - " << ncolumna << std::endl;
    // ? accedemos al label del dia
    QLabel* label = frameCalendario->findChild<QLabel*>("numero" + QString::number(nfila) + QString::number(ncolumna));
    // ? extraemos el texto de label
    std::string diaS = label->text().toStdString();
    diaS = diaS.empty() ? "0" : diaS;
    int dia = std::stoi(diaS);
    // dia mes y año actual
    reEmCargarGraficaEmociones(dia, calMes, calAnio);

}

// ! Metodo para limpiar el calendario
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::calLimpiarCalendario()
{
    for (int semana = 1; semana < 7; semana++)
    {
        for (int dia = 0; dia < 7; dia++)
        {
            QLabel* label = frameCalendario->findChild<QLabel*>("numero" + QString::number(semana) + QString::number(dia));
            label->setText("");
            QPushButton* boton = frameCalendario->findChild<QPushButton*>("emocion" + QString::number(semana) + QString::number(dia));
            boton->setIcon(QIcon());
            label = frameCalendario->findChild<QLabel*>("eventos" + QString::number(semana) + QString::number(dia));
            label->setText("");

        }
    }
}

// Metodos para el backend del calendario
void MainWindow::calClickDiaBackend10() { calClickDia(1, 0);}
void MainWindow::calClickDiaBackend11() { calClickDia(1, 1);}
void MainWindow::calClickDiaBackend12() { calClickDia(1, 2);}
void MainWindow::calClickDiaBackend13() { calClickDia(1, 3);}
void MainWindow::calClickDiaBackend14() { calClickDia(1, 4);}
void MainWindow::calClickDiaBackend15() { calClickDia(1, 5);}
void MainWindow::calClickDiaBackend16() { calClickDia(1, 6);}
void MainWindow::calClickDiaBackend20() { calClickDia(2, 0);}
void MainWindow::calClickDiaBackend21() { calClickDia(2, 1);}
void MainWindow::calClickDiaBackend22() { calClickDia(2, 2);}
void MainWindow::calClickDiaBackend23() { calClickDia(2, 3);}
void MainWindow::calClickDiaBackend24() { calClickDia(2, 4);}
void MainWindow::calClickDiaBackend25() { calClickDia(2, 5);}
void MainWindow::calClickDiaBackend26() { calClickDia(2, 6);}
void MainWindow::calClickDiaBackend30() { calClickDia(3, 0);}
void MainWindow::calClickDiaBackend31() { calClickDia(3, 1);}
void MainWindow::calClickDiaBackend32() { calClickDia(3, 2);}
void MainWindow::calClickDiaBackend33() { calClickDia(3, 3);}
void MainWindow::calClickDiaBackend34() { calClickDia(3, 4);}
void MainWindow::calClickDiaBackend35() { calClickDia(3, 5);}
void MainWindow::calClickDiaBackend36() { calClickDia(3, 6);}
void MainWindow::calClickDiaBackend40() { calClickDia(4, 0);}
void MainWindow::calClickDiaBackend41() { calClickDia(4, 1);}
void MainWindow::calClickDiaBackend42() { calClickDia(4, 2);}
void MainWindow::calClickDiaBackend43() { calClickDia(4, 3);}
void MainWindow::calClickDiaBackend44() { calClickDia(4, 4);}
void MainWindow::calClickDiaBackend45() { calClickDia(4, 5);}
void MainWindow::calClickDiaBackend46() { calClickDia(4, 6);}
void MainWindow::calClickDiaBackend50() { calClickDia(5, 0);}
void MainWindow::calClickDiaBackend51() { calClickDia(5, 1);}
void MainWindow::calClickDiaBackend52() { calClickDia(5, 2);}
void MainWindow::calClickDiaBackend53() { calClickDia(5, 3);}
void MainWindow::calClickDiaBackend54() { calClickDia(5, 4);}
void MainWindow::calClickDiaBackend55() { calClickDia(5, 5);}
void MainWindow::calClickDiaBackend56() { calClickDia(5, 6);}
void MainWindow::calClickDiaBackend60() { calClickDia(6, 0);}
void MainWindow::calClickDiaBackend61() { calClickDia(6, 1);}
void MainWindow::calClickDiaBackend62() { calClickDia(6, 2);}
void MainWindow::calClickDiaBackend63() { calClickDia(6, 3);}
void MainWindow::calClickDiaBackend64() { calClickDia(6, 4);}
void MainWindow::calClickDiaBackend65() { calClickDia(6, 5);}
void MainWindow::calClickDiaBackend66() { calClickDia(6, 6);}

void MainWindow::calClickEmoDiaBackend10() { calClickEmoDia(1, 0);}
void MainWindow::calClickEmoDiaBackend11() { calClickEmoDia(1, 1);}
void MainWindow::calClickEmoDiaBackend12() { calClickEmoDia(1, 2);}
void MainWindow::calClickEmoDiaBackend13() { calClickEmoDia(1, 3);}
void MainWindow::calClickEmoDiaBackend14() { calClickEmoDia(1, 4);}
void MainWindow::calClickEmoDiaBackend15() { calClickEmoDia(1, 5);}
void MainWindow::calClickEmoDiaBackend16() { calClickEmoDia(1, 6);}
void MainWindow::calClickEmoDiaBackend20() { calClickEmoDia(2, 0);}
void MainWindow::calClickEmoDiaBackend21() { calClickEmoDia(2, 1);}
void MainWindow::calClickEmoDiaBackend22() { calClickEmoDia(2, 2);}
void MainWindow::calClickEmoDiaBackend23() { calClickEmoDia(2, 3);}
void MainWindow::calClickEmoDiaBackend24() { calClickEmoDia(2, 4);}
void MainWindow::calClickEmoDiaBackend25() { calClickEmoDia(2, 5);}
void MainWindow::calClickEmoDiaBackend26() { calClickEmoDia(2, 6);}
void MainWindow::calClickEmoDiaBackend30() { calClickEmoDia(3, 0);}
void MainWindow::calClickEmoDiaBackend31() { calClickEmoDia(3, 1);}
void MainWindow::calClickEmoDiaBackend32() { calClickEmoDia(3, 2);}
void MainWindow::calClickEmoDiaBackend33() { calClickEmoDia(3, 3);}
void MainWindow::calClickEmoDiaBackend34() { calClickEmoDia(3, 4);}
void MainWindow::calClickEmoDiaBackend35() { calClickEmoDia(3, 5);}
void MainWindow::calClickEmoDiaBackend36() { calClickEmoDia(3, 6);}
void MainWindow::calClickEmoDiaBackend40() { calClickEmoDia(4, 0);}
void MainWindow::calClickEmoDiaBackend41() { calClickEmoDia(4, 1);}
void MainWindow::calClickEmoDiaBackend42() { calClickEmoDia(4, 2);}
void MainWindow::calClickEmoDiaBackend43() { calClickEmoDia(4, 3);}
void MainWindow::calClickEmoDiaBackend44() { calClickEmoDia(4, 4);}
void MainWindow::calClickEmoDiaBackend45() { calClickEmoDia(4, 5);}
void MainWindow::calClickEmoDiaBackend46() { calClickEmoDia(4, 6);}
void MainWindow::calClickEmoDiaBackend50() { calClickEmoDia(5, 0);}
void MainWindow::calClickEmoDiaBackend51() { calClickEmoDia(5, 1);}
void MainWindow::calClickEmoDiaBackend52() { calClickEmoDia(5, 2);}
void MainWindow::calClickEmoDiaBackend53() { calClickEmoDia(5, 3);}
void MainWindow::calClickEmoDiaBackend54() { calClickEmoDia(5, 4);}
void MainWindow::calClickEmoDiaBackend55() { calClickEmoDia(5, 5);}
void MainWindow::calClickEmoDiaBackend56() { calClickEmoDia(5, 6);}
void MainWindow::calClickEmoDiaBackend60() { calClickEmoDia(6, 0);}
void MainWindow::calClickEmoDiaBackend61() { calClickEmoDia(6, 1);}
void MainWindow::calClickEmoDiaBackend62() { calClickEmoDia(6, 2);}
void MainWindow::calClickEmoDiaBackend63() { calClickEmoDia(6, 3);}
void MainWindow::calClickEmoDiaBackend64() { calClickEmoDia(6, 4);}
void MainWindow::calClickEmoDiaBackend65() { calClickEmoDia(6, 5);}
void MainWindow::calClickEmoDiaBackend66() { calClickEmoDia(6, 6);}


// ! para mostrar un dia seleccionado en el calendario
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::calMostrarDiaSeleccionado(const int& diaSeleccionado)
{
    // Ocultamos el interfaz de nuevo evento
    calOcultarInterfazEvento();
    calDia = diaSeleccionado;
    // ? Desactivamos el calendario
    calDesactivarCalendario();
    calDiaFrame->show();
    // ? Mostramos el dia seleccionado
    calDiaTitulo->setText(QString::number(diaSeleccionado) + " de " + (*meses)[calMes - 1] + " de " + QString::number(calAnio));
    calDiaTitulo->show();
    calDiaEventos->show();
    calDiaScrollArea->show();
    // ? Cargamos los eventos del dia seleccionado
    calCargarListaEventosDia();

    // ? Configuramos los botones de la barra de navegación
    barNaConfigurarBotones(0, true, 7, "Nuevo Evento");
}

// ! Método para ocultar la interfaz del dia seleccionado
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::calOcultarDiaSeleccionado()
{
    calDiaFrame->hide();
    calDiaTitulo->hide();
    calDiaEventos->hide();
    calDiaScrollArea->hide();
    // ? Limpiamos los botones de la barra de navegación
    barNaLimpiarBotonesSeccionActual();
    // ? Activamos el calendario
    //calActivarCalendario();
}

// ! Método para cargar los eventos del dia seleccionado
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::calCargarListaEventosDia()
{
    // ? limpiamos la lista de eventos
    calDiaEventos->clear();
    // ? construimos la ruta a la carpeta de eventos del calendario
    std::string rutaCalendario = RUTA_USUARIOS + encriptado->desencriptar((*configuracionesUsuario)["nombreUsuario"]) + "/calendario";
    //    std::to_string(calAnio) + "/" + std::to_string(calMes) + "/" + std::to_string(dia);
    // ? verificamos si existe la carpeta de calendario
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaCalendario))
    {
        // ? si no existe la carpeta de calendario, la creamos
        manejadorArchivos.crearCarpeta(rutaCalendario);
    }
    // ? construimos la ruta a la carpeta del año
    rutaCalendario += "/" + std::to_string(calAnio);
    // ? verificamos si existe la carpeta del año
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaCalendario))
    {
        // ? si no existe la carpeta del año, la creamos
        manejadorArchivos.crearCarpeta(rutaCalendario);
    }
    // ? construimos la ruta a la carpeta del mes
    rutaCalendario += "/" + std::to_string(calMes);
    // ? verificamos si existe la carpeta del mes
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaCalendario))
    {
        // ? si no existe la carpeta del mes, la creamos
        manejadorArchivos.crearCarpeta(rutaCalendario);
    }
    // ? construimos la ruta a la carpeta del dia
    rutaCalendario += "/" + std::to_string(calDia);
    // ? verificamos si existe la carpeta del dia
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaCalendario))
    {
        // ? si no existe la carpeta del dia, la creamos
        manejadorArchivos.crearCarpeta(rutaCalendario);
    }
    // cada carpeta del dia contendrá un archivo json con un evento
    // ? extraemos todas las carpetas de eventos del dia
    std::vector<std::string> eventos = manejadorArchivos.obtenerContenidoCarpeta(rutaCalendario, 1);
    //std::vector<std::string> eventosJson;
    for (const auto& evento: eventos)
    {
        // los eventos son carpetas, pero cada carpeta se llama como el momento de su creación
        // por lo cual debemos extraer el nombre del archivo json, el nombre es el titulo del evento
        std::string rutaTemporal = rutaCalendario + "/" + evento;
        // creamos un json con la información del evento
        mJson::ManejadorJson eventoJson(rutaTemporal + "/evento.json");
        // cargamos los eventos en la lista
        //calDiaEventos->addItem(QString::fromStdString(evento));
        //calDiaEventos->addItem(QString::fromStdString(eventoJson["titulo"]));
        std::string titulo = eventoJson["titulo"];
        titulo = encriptado->desencriptar(titulo);
        std::string hora = eventoJson["hora"];
        hora = encriptado->desencriptar(hora);
        //calDiaEventos->addItem(QString::fromStdString(titulo));
        //eventosJson.push_back(evento);

        // EXPERIMENTAL
        // creamos un widget
        QWidget* widget = new QWidget();
        widget->setStyleSheet("background-color: #f0f0f0;"
                              "border: 1px solid black;"
                              //"border-radius: 5px;"
                              "padding: 2px;"
                              "margin: 2px;");
        // creamos un layout
        QHBoxLayout* layout = new QHBoxLayout(widget);
        // creamos un label
        QLabel* labelTitulo = new QLabel(widget);
        labelTitulo->setText(QString::fromStdString(titulo));
        // le asignamos un nombre al titulo para poder buscarlo
        labelTitulo->setObjectName("titulo");
        labelTitulo->setStyleSheet("color: black;");
        // creamos otro label para la hora
        QLabel* labelHora = new QLabel(widget);
        labelHora->setText(QString::fromStdString(hora));
        // le damos un tamaño maximo
        labelHora->setMaximumWidth(80);
        // le metemos un estilo
        labelHora->setStyleSheet("background-color: green; border: 1px solid black; border-radius: 5px; padding: 2px; margin: 2px; color: black;");
        // agregamos los labels al layout
        layout->addWidget(labelHora);
        layout->addWidget(labelTitulo);
        QListWidgetItem* item = new QListWidgetItem();
        item->setText(QString::fromStdString(titulo));
        //item->setStyleSheet("color: rgba(0,0,0,0);");
        // // hacemos que el item no muestre el texto
        // item->setHidden(true);
        item->setSizeHint(widget->sizeHint());
        calDiaEventos->addItem(item);
        calDiaEventos->setItemWidget(item, widget);

    }


}

// ! Método para crear un nuevo evento en el calendario
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::calInterfazEvento(const int& modoDeApertura)
{
    std::cout << "Creando nuevo evento en el calendario con modo de apertura: " << modoDeApertura << std::endl;
    // ? ocultamos la interfaz del dia seleccionado
    calOcultarDiaSeleccionado();
    // ? mostramos la interfaz de nuevo evento
    calDiaFrame->show();
    calAgregarEventoTitulo->show();
    calAgregarEventoTitulo->setReadOnly(false);
    //calAgregarEventoTitulo->setText("Nuevo Evento");
    calAgregarEventoHora->show();
    calAgregarEventoHora->setReadOnly(false);
    calAgregarEventoDuracion->show();
    calAgregarEventoDuracion->setReadOnly(false);
    calAgregarEventoDescripcion->show();
    calAgregarEventoDescripcion->setReadOnly(false);
    // ? si el modo de apertura es 2, se mostrará la interfaz en modo nuevo evento
    // ? si el modo de apertura es 1, se mostrará la interfaz en modo editar evento
    // ? si el modo de apertura es 0, se mostrará la interfaz en modo solo lectura
    if (modoDeApertura == 2)
    {

        // ? creamos un nuevo evento
        calInterfazEventoNuevo();
        editandoEvento = true;
        // ? configuramos los botones de la barra de navegación
        barNaConfigurarBotones(0, true, 8, "Guardar Evento");
    }
    else if (modoDeApertura == 1)
    {
        // ? cargamos los datos del evento en la interfaz
        calInterfazEventoCargarDatos();
        editandoEvento = true;
        // ? configuramos los botones de la barra de navegación
        barNaConfigurarBotones(0, true, 8, "Guardar Evento");
    }
    else if (modoDeApertura == 0)
    {
        // ? cargamos los datos del evento en la interfaz
        calInterfazEventoCargarDatos();
        // ? bloqueamos los campos de texto
        calAgregarEventoTitulo->setReadOnly(true);
        calAgregarEventoHora->setReadOnly(true);
        calAgregarEventoDuracion->setReadOnly(true);
        calAgregarEventoDescripcion->setReadOnly(true);
        // ? configuramos los botones de la barra de navegación
        barNaConfigurarBotones(0, true, 9, "Editar Evento");
        barNaConfigurarBotones(1, true, 10, "Eliminar Evento"); // -1 por que aun no se ha implementado

    }



}

// ! Método para crear un nuevo evento e inicializar su json
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::calInterfazEventoNuevo()
{
    // ? El json del evento estara constituido por los siguientes campos
    /* titulo
     * hora
     * duracion
     * descripcion
     * horaDeCreacion <-- este es el más importante ya que se usara para nombrar la carpeta del evento
     */
    std::string rutaEvento = RUTA_USUARIOS + encriptado->desencriptar((*configuracionesUsuario)["nombreUsuario"]) + "/calendario/";
    // verificamos si existe la carpeta de calendario
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaEvento))
    {
        // si no existe la carpeta de calendario, la creamos
        manejadorArchivos.crearCarpeta(rutaEvento);
        // esto es rarísimo, pero por si acaso
    }
    // construimos la ruta a la carpeta del año
    rutaEvento += std::to_string(calAnio) + "/";
    // verificamos si existe la carpeta del año
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaEvento))
    {
        // si no existe la carpeta del año, la creamos
        manejadorArchivos.crearCarpeta(rutaEvento);
    }
    // construimos la ruta a la carpeta del mes
    rutaEvento += std::to_string(calMes) + "/";
    // verificamos si existe la carpeta del mes
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaEvento))
    {
        // si no existe la carpeta del mes, la creamos
        manejadorArchivos.crearCarpeta(rutaEvento);
    }
    // construimos la ruta a la carpeta del dia
    rutaEvento += std::to_string(calDia) + "/";
    // verificamos si existe la carpeta del dia
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaEvento))
    {
        // si no existe la carpeta del dia, la creamos
        manejadorArchivos.crearCarpeta(rutaEvento);
    }
    // ? Para este punto ya tenemos la ruta a la carpeta que contiene los eventos del dia
    // extraemos la hora actual (hora, minutos, segundos) y la usamos para nombrar la carpeta del evento
    std::time_t tiempo = std::time(nullptr);
    std::tm* tiempoLocal = std::localtime(&tiempo);
    std::string horaDeCreacion = std::to_string(tiempoLocal->tm_hour) + std::to_string(tiempoLocal->tm_min) + std::to_string(tiempoLocal->tm_sec);
    // construimos la ruta al archivo del evento
    rutaEvento += horaDeCreacion + "/";
    // verificamos si existe la carpeta del evento
    if (manejadorArchivos.verificarExistenciaDeCarpeta(rutaEvento))
    {
        // si existe la carpeta del evento, mostramos un mensaje de error
        QMessageBox::critical(this, "Error", "Ya existe un evento con la misma hora de creación"
                              ", intente nuevamente"
                            );
        // activamos la interfaz de día seleccionado
        calMostrarDiaSeleccionado(calDia);

    }
    manejadorArchivos.crearCarpeta(rutaEvento);
    // agregamos "evento.json" al final de la ruta del evento
    rutaEvento += "evento.json";
    // creamos un json para el evento
    calDiaJsonEventos = new mJson::ManejadorJson(rutaEvento, true);
    // inicializamos los campos del evento
    //(*calDiaJsonEventos)["titulo"] = "";
    //(*calDiaJsonEventos)["hora"] = "";
    //(*calDiaJsonEventos)["duracion"] = "";
    //(*calDiaJsonEventos)["descripcion"] = "";
    (*calDiaJsonEventos)["horaDeCreacion"] = horaDeCreacion;
}

// ! Método para cargar un evento en la interfaz de evento, para editar o mostrar
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::calInterfazEventoCargarDatos()
{
    // ? este evento se carga a partir de la lista de eventos del dia seleccionado
    // ? extraemos el nombre del evento seleccionado
    std::string nombreEvento = calDiaEventos->currentItem()->text().toStdString();
    std::cout << "Cargando evento: " << nombreEvento << std::endl;
    // ? verificamos si el evento seleccionado tiene contenido
    if (nombreEvento == "")
    {
        return;
    }
    // ? verificamos si calDiaJsonEventos es diferente de nullptr
    if (calDiaJsonEventos != nullptr)
    {
        // ? si es diferente de nullptr, implica que ya se cargó el json de eventos
        // ? guardamos el json de eventos
        calDiaJsonEventos->guardar();
        // ? creamos un puntero a un json
        mJson::ManejadorJson* calDiaJsonEventosAux = calDiaJsonEventos;
        // ? eliminamos el json auxiliar
        delete calDiaJsonEventosAux;
        // ? asignamos nullptr al json de eventos
        calDiaJsonEventos = nullptr;
    }
    // ? construimos la ruta a la carpeta de eventos del calendario
    std::string rutaCalendario = RUTA_USUARIOS + encriptado->desencriptar((*configuracionesUsuario)["nombreUsuario"]) + "/calendario";
    // ? verificamos si existe la carpeta de calendario
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaCalendario))
    {
        // ? si no existe la carpeta de calendario, la creamos
        manejadorArchivos.crearCarpeta(rutaCalendario);
    }
    // ? construimos la ruta a la carpeta del año
    rutaCalendario += "/" + std::to_string(calAnio);
    // ? verificamos si existe la carpeta del año
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaCalendario))
    {
        // ? si no existe la carpeta del año, la creamos
        manejadorArchivos.crearCarpeta(rutaCalendario);
    }
    // ? construimos la ruta a la carpeta del mes
    rutaCalendario += "/" + std::to_string(calMes);
    // ? verificamos si existe la carpeta del mes
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaCalendario))
    {
        // ? si no existe la carpeta del mes, la creamos
        manejadorArchivos.crearCarpeta(rutaCalendario);
    }
    // ? construimos la ruta a la carpeta del dia
    rutaCalendario += "/" + std::to_string(calDia);
    // ? verificamos si existe la carpeta del dia
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaCalendario))
    {
        // ? si no existe la carpeta del dia, la creamos
        manejadorArchivos.crearCarpeta(rutaCalendario);
    }

    // ? construimos la ruta al archivo del evento
    std::string rutaEvento;
    // ? No podemos colocar simplemente el nombre del evento, debido a que la carpeta del
    // evento se llama con la hora de creación, por lo cual debemos extraer el nombre del
    // archivo json del evento y compararlo con el nombre del evento seleccionado para
    // obtener la ruta correcta
    std::vector<std::string> eventos = manejadorArchivos.obtenerContenidoCarpeta(rutaCalendario, 1);
    for (const auto& evento: eventos)
    {
        // los eventos son carpetas, pero cada carpeta se llama como el momento de su creación
        // por lo cual debemos extraer el nombre del archivo json, el nombre es el titulo del evento
        std::string rutaTemporal = rutaCalendario + "/" + evento;
        // creamos un json con la información del evento
        mJson::ManejadorJson eventoJson(rutaTemporal + "/evento.json");
        // si el titulo del evento es igual al nombre del evento seleccionado
        std::string titulo = eventoJson["titulo"];
        titulo = encriptado->desencriptar(titulo);
        if (titulo == nombreEvento)
        {
            // extraemos la hora de creación
            rutaEvento = rutaTemporal;
            break;
        }
    }

    // ? verificamos si existe el archivo del evento
    bool existeEvento = true; // si no existe el archivo evento evitaremos que salte un segundo mensaje
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaEvento))
    {
        // ? si no existe el archivo del evento, mostramos un mensaje de error
        QMessageBox::critical(this, "Error", "No se ha encontrado el evento seleccionado"
                              ", crearemos el evento en su lugar"
                            );
        manejadorArchivos.crearCarpeta(rutaEvento);
        existeEvento = false;
    }
    // ? agregamos "evento.json" al final de la ruta del evento
    rutaEvento += "/evento.json";
    // ? verificamos si existe el archivo del evento
    if (!manejadorArchivos.verificarExistenciaDeArchivo(rutaEvento))
    {
        // ? si no existe el archivo del evento, mostramos un mensaje de error
        if (existeEvento)
        {
            QMessageBox::critical(this, "Error", "No se ha encontrado el evento seleccionado"
                                  ", crearemos el evento en su lugar"
                                );
        }
        // ? si no existe el archivo del evento, lo creamos
        calDiaJsonEventos = new mJson::ManejadorJson(rutaEvento, true);
    }
    // ? cargamos el json del evento
    calDiaJsonEventos = new mJson::ManejadorJson(rutaEvento);
    // ? cargamos los datos del evento en la interfaz
    {
        // ? falta desencritar los datos del json por el momento no estan encriptados
        std::string titulo = (*calDiaJsonEventos)["titulo"];
        titulo = encriptado->desencriptar(titulo);
        calAgregarEventoTitulo->setText(QString::fromStdString(titulo));
        std::string hora = (*calDiaJsonEventos)["hora"];
        hora = encriptado->desencriptar(hora);
        // si hora no esta vacia
        if (hora != "")
        {
            calAgregarEventoHora->setTime(QTime(std::stoi(hora.substr(0, 2)), std::stoi(hora.substr(3, 2))));
        }else{
            calAgregarEventoHora->setTime(QTime(0, 0));
        }
        //calAgregarEventoHora->setTime(QTime(std::stoi(hora.substr(0, 2)), std::stoi(hora.substr(3, 2))));
        std::string duracion = (*calDiaJsonEventos)["duracion"];
        duracion = encriptado->desencriptar(duracion);
        // si duracion no esta vacia
        if (duracion != "")
        {
            calAgregarEventoDuracion->setTime(QTime(std::stoi(duracion.substr(0, 2)), std::stoi(duracion.substr(3, 2))));
        }else{
            calAgregarEventoDuracion->setTime(QTime(0, 0));
        }
        //calAgregarEventoDuracion->setTime(QTime(std::stoi(duracion.substr(0, 2)), std::stoi(duracion.substr(3, 2))));
        std::string descripcion = (*calDiaJsonEventos)["descripcion"];
        descripcion = encriptado->desencriptar(descripcion);
        // ? cargamos la descripción como HTML
        //calAgregarEventoDescripcion->setHtml(QString::fromStdString(descripcion));
        calAgregarEventoDescripcion->setText(QString::fromStdString(descripcion));
    }
}

// ! Método para ocultar la interfaz de evento
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::calOcultarInterfazEvento()
{
    calDiaFrame->hide();
    calAgregarEventoTitulo->hide();
    calAgregarEventoTitulo->clear();
    calAgregarEventoHora->hide();
    calAgregarEventoHora->clear();
    calAgregarEventoDuracion->hide();
    calAgregarEventoDuracion->clear();
    calAgregarEventoDescripcion->hide();
    calAgregarEventoDescripcion->clear();
    // ? limpiamos los botones de la barra de navegación
    barNaLimpiarBotonesSeccionActual();
}

// ! Método para guardar un evento en el calendario
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::calGuardarEvento()
{

    // ? verificamos que se haya ingresado un título para el evento (este titulo será el nombre de la carpeta)
    if (calAgregarEventoTitulo->text() == "")
    {
        QMessageBox::critical(this, "Error", "No se ha ingresado un título para el evento");
        return;
    }
    // ? verificamos que se haya ingresado una hora para el evento
    if (calAgregarEventoHora->text() == "")
    {
        QMessageBox::critical(this, "Error", "No se ha ingresado una hora para el evento");
        return;
    }

    // // ? creamos un json para el evento
    // calDiaJsonEventos = new mJson::ManejadorJson(rutaEvento);
    // ? guardamos los datos del evento
    //(*calDiaJsonEventos)["titulo"] = calAgregarEventoTitulo->text().toStdString();
    std::string titulo = calAgregarEventoTitulo->text().toStdString();
    (*calDiaJsonEventos)["titulo"] = encriptado->encriptar(titulo);
    //(*calDiaJsonEventos)["hora"] = calAgregarEventoHora->text().toStdString();
    std::string hora = calAgregarEventoHora->time().toString().toStdString();
    (*calDiaJsonEventos)["hora"] = encriptado->encriptar(hora);
    //(*calDiaJsonEventos)["duracion"] = calAgregarEventoDuracion->text().toStdString();
    std::string duracion = calAgregarEventoDuracion->time().toString().toStdString();
    (*calDiaJsonEventos)["duracion"] = encriptado->encriptar(duracion);
    //(*calDiaJsonEventos)["descripcion"] = calAgregarEventoDescripcion->toHtml().toStdString();
    //std::string descripcion = calAgregarEventoDescripcion->toHtml().toStdString();
    std::string descripcion = calAgregarEventoDescripcion->toPlainText().toStdString();
    (*calDiaJsonEventos)["descripcion"] = encriptado->encriptar(descripcion);
    // ? guardamos el json de eventos
    calDiaJsonEventos->guardar();
    // ? regresamos a la interfaz de dia seleccionado
    editandoEvento = false;
    calMostrarDiaSeleccionado(calDia);


}

// ! Método para eliminar un evento en el calendario
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::calEliminarEvento()
{
    // verificamos si hay un evento cargado en el json
    if (calDiaJsonEventos != nullptr)
    {
        // guardamos el json
        calDiaJsonEventos->guardar();
        // extraemos el nombre de la carpeta del evento
        std::string nombreEvento = (*calDiaJsonEventos)["horaDeCreacion"];
        //std::cout << "Eliminando evento: " << nombreEvento << std::endl;
        // construimos la ruta a la carpeta de eventos del calendario
        std::string rutaCalendario = RUTA_USUARIOS + encriptado->desencriptar((*configuracionesUsuario)["nombreUsuario"]) + "/calendario" + "/" + std::to_string(calAnio) + "/" + std::to_string(calMes) + "/" + std::to_string(calDia) + "/" + nombreEvento;
        // mostramos un mensaje de confirmación
        QMessageBox::StandardButton respuesta = QMessageBox::question(this, "Eliminar Evento", "¿Estás seguro de que deseas eliminar el evento seleccionado?", QMessageBox::Yes | QMessageBox::No);
        if (respuesta == QMessageBox::Yes)
        {
            // eliminamos el archivo json del evento
            mJson::ManejadorJson* aux = calDiaJsonEventos;
            delete aux;
            calDiaJsonEventos = nullptr;
            // eliminamos la carpeta del evento
            manejadorArchivos.eliminarCarpeta(rutaCalendario);
            // mostramos un mensaje de éxito
            //QMessageBox::information(this, "Evento Eliminado", "El evento ha sido eliminado con éxito");
            // ocultamos la interfaz de evento
            calOcultarInterfazEvento();
            // mostramos la interfaz del dia seleccionado
            calMostrarDiaSeleccionado(calDia);
        }
    }
}


// ////////////////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------------------------
// ! RELACIONADOS CON LA INTERFAZ DE REGISTRO DE EMOCIONES

// ! Método para mostrar la interfaz de registro de emociones
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::activarInterfazRegistroEmociones()
{
    frameRegistroEmociones->show();
    reEmActivarRegistroEmocion();
    reEmRedimencionarCosas();
}

// ! Método para ocultar la interfaz de registro de emociones
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::desactivarInterfazRegistroEmociones()
{
    frameRegistroEmociones->hide();
    reEmDesactivarRegistroEmocion();
    reEmDesactivarGraficaEmociones();
}

// ! Método para que gestiona el proceso de registro de emociones
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::reEmBotonEmocion(const int& emocion)
{
    // 0 -> miedo
    // 1 -> tristeza
    // 2 -> alegria
    // 3 -> enojo
    // 4 -> sorpresa
    // 5 -> disgusto
    std::cout << "Emoción seleccionada: " << emocion << std::endl;
    // Generamos un dialogo de confirmación
    QString estilo = "";
    QMessageBox dialogo;
    dialogo.setWindowTitle("Registro de Emoción");
    dialogo.setText("¿Estás seguro de que deseas registrar esta emoción?");
    dialogo.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    dialogo.setDefaultButton(QMessageBox::No);
    dialogo.setStyleSheet(estilo);
    //dialogo.setIcon()
    switch (emocion) {
    case 0:
        dialogo.setIconPixmap(QPixmap(QString(RUTA_ICONOS.c_str()) + "Miedo-icono.svg"));
        break;
    case 1:
        dialogo.setIconPixmap(QPixmap(QString(RUTA_ICONOS.c_str()) + "Tristeza-icono.svg"));
        break;
    case 2:
        dialogo.setIconPixmap(QPixmap(QString(RUTA_ICONOS.c_str()) + "Alegria-icono.svg"));
        break;
    case 3:
        dialogo.setIconPixmap(QPixmap(QString(RUTA_ICONOS.c_str()) + "Enojo-icono.svg"));
        break;
    case 4:
        dialogo.setIconPixmap(QPixmap(QString(RUTA_ICONOS.c_str()) + "Sorpresa-icono.svg"));
        break;
    case 5:
        dialogo.setIconPixmap(QPixmap(QString(RUTA_ICONOS.c_str()) + "Disgusto-icono.svg"));
        break;
    default:
        break;
    }
    // si el dialogo se niega, no se registrará la emoción
    if (dialogo.exec() == QMessageBox::No)
    {
        // terminamos la función
        std::cout << "Emoción no registrada" << std::endl;
        return;
    }
    // si el dialogo se acepta, se registrará la emoción
    // comenzamos a construir la ruta de la carpeta de emociones
    std::string rutaEmociones = RUTA_USUARIOS + encriptado->desencriptar((*configuracionesUsuario)["nombreUsuario"]) + "/emociones";
    // verificamos si existe la carpeta de emociones
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaEmociones))
    {
        // si no existe la carpeta de emociones, la creamos
        manejadorArchivos.crearCarpeta(rutaEmociones);
    }
    // extraemos la fecha actual
    std::time_t tiempo = std::time(nullptr);
    std::tm* tiempoLocal = std::localtime(&tiempo);
    // extraemos el año, mes y dia
    int anio = tiempoLocal->tm_year + 1900;
    int mes = tiempoLocal->tm_mon + 1;
    int dia = tiempoLocal->tm_mday;
    //std::cout << "Fecha actual: " << dia << "/" << mes << "/" << anio << std::endl;
    // extraemos la hora actual de tiempo local en formato de 12 horas
    std::string hora = std::to_string(tiempoLocal->tm_hour) + ":" + std::to_string(tiempoLocal->tm_min);
    std::cout << "Hora actual: " << hora << std::endl;

    // extraemos la hora de la última emoción registrada
    std::string ultimaEmocionH = (*configuracionesUsuario)["ultimaEmocionH"];
    ultimaEmocionH = ultimaEmocionH.empty() ? "0" : ultimaEmocionH;
    std::string ultimaEmocionM = (*configuracionesUsuario)["ultimaEmocionM"];
    ultimaEmocionM = ultimaEmocionM.empty() ? "0" : ultimaEmocionM;
    // extraemos la emoción registrada
    //std::string ultimaEmocion = (*configuracionesUsuario)["ultimaEmocionRegistrada"];
    //ultimaEmocion = ultimaEmocion.empty() ? "-1" : ultimaEmocion;
    int horaUltimaEmocion = std::stoi(ultimaEmocionH);
    int minutoUltimaEmocion = std::stoi(ultimaEmocionM);
    // verificamos si la última emoción registrada fue hace más de 5 minutos
    if (tiempoLocal->tm_hour - horaUltimaEmocion == 0 && tiempoLocal->tm_min - minutoUltimaEmocion < 5)
    {
        // si la última emoción registrada fue hace menos de 5 minutos, mostramos un mensaje de error
        QMessageBox::critical(this, "Error", "Ya has registrado una emoción hace menos de 5 minutos");
        // terminamos la función
        return;
    }

    // guardamos la hora en el json con la configuración del usuario
    //(*configuracionesUsuario)["ultimaEmocion"] = hora;
    (*configuracionesUsuario)["ultimaEmocionH"] = std::to_string(tiempoLocal->tm_hour);
    (*configuracionesUsuario)["ultimaEmocionM"] = std::to_string(tiempoLocal->tm_min);
    (*configuracionesUsuario)["ultimaEmocionRegistrada"] = std::to_string(emocion);
    // guardamos el json
    configuracionesUsuario->guardar();

    // construimos la ruta a la carpeta del año
    rutaEmociones += "/" + std::to_string(anio);
    // verificamos si existe la carpeta del año
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaEmociones))
    {
        // si no existe la carpeta del año, la creamos
        manejadorArchivos.crearCarpeta(rutaEmociones);
    }
    // construimos la ruta a la carpeta del mes
    rutaEmociones += "/" + std::to_string(mes);
    // verificamos si existe la carpeta del mes
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaEmociones))
    {
        // si no existe la carpeta del mes, la creamos
        manejadorArchivos.crearCarpeta(rutaEmociones);
    }
    // construimos la ruta a la carpeta del dia
    rutaEmociones += "/" + std::to_string(dia);
    // verificamos si existe la carpeta del dia
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaEmociones))
    {
        // si no existe la carpeta del dia, la creamos
        manejadorArchivos.crearCarpeta(rutaEmociones);
    }
    switch (emocion)
    {
        case 0:
            // miedo
            rutaEmociones += "/miedo";
            break;
        case 1:
            // tristeza
            rutaEmociones += "/tristeza";
            break;
        case 2:
            // alegria
            rutaEmociones += "/alegria";
            break;
        case 3:
            // enojo
            rutaEmociones += "/enojo";
            break;
        case 4:
            // sorpresa
            rutaEmociones += "/sorpresa";
            break;
        case 5:
            // disgusto
            rutaEmociones += "/disgusto";
            break;
        default:
            // si la emoción no es válida, terminamos la función
            return;
    }
    int contador = 1;
    // verificamos si existe el archivo de emocion
    if (manejadorArchivos.verificarExistenciaDeArchivo(rutaEmociones))
    {
        // si existe el archivo extraemos su contenido
        std::string contenido = manejadorArchivos.leerArchivo(rutaEmociones);
        // convertimos el contenido a entero
        contador += std::stoi(contenido);
        std::cout << "Contador de emociones: " << contador << std::endl;
    }
    // modificamos el archivo con el nuevo contador
    manejadorArchivos.crearArchivo(rutaEmociones, std::to_string(contador));
    // activamos la ventana de espera
    reEmActivarEmocionConfirmada();
    reEmCargarFraseMotivacional(emocion);


}

// ! Métodos accion click en boton de emocion
void MainWindow::reEmClickMiedo() { reEmBotonEmocion(0);}
void MainWindow::reEmClickTristeza() { reEmBotonEmocion(1);}
void MainWindow::reEmClickAlegria() { reEmBotonEmocion(2);}
void MainWindow::reEmClickEnojo() { reEmBotonEmocion(3);}
void MainWindow::reEmClickSorpresa() { reEmBotonEmocion(4);}
void MainWindow::reEmClickDisgusto() { reEmBotonEmocion(5);}

// ! Método para reactivar el registro de emociones
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::reEmActivarRegistroEmocion()
{
    reEmDesactivarGraficaEmociones();
    reEmDesactivarEmocionConfirmada();
    reEmTitulo->show();
    reEmTitulo->setText("¿Cómo te sientes hoy?");
    reEmBotonMiedo->show();
    reEmBotonTristeza->show();
    reEmBotonAlegria->show();
    reEmBotonEnojo->show();
    reEmBotonSorpresa->show();
    reEmBotonDisgusto->show();
    // extraemos la hora de la última emoción registrada
    std::string ultimaEmocionH = (*configuracionesUsuario)["ultimaEmocionH"];
    ultimaEmocionH = ultimaEmocionH.empty() ? "0" : ultimaEmocionH;
    std::string ultimaEmocionM = (*configuracionesUsuario)["ultimaEmocionM"];
    ultimaEmocionM = ultimaEmocionM.empty() ? "0" : ultimaEmocionM;
    // extraemos la emoción registrada
    std::string ultimaEmocion = (*configuracionesUsuario)["ultimaEmocionRegistrada"];
    ultimaEmocion = ultimaEmocion.empty() ? "-1" : ultimaEmocion;
    // extraemos la hora actual
    std::time_t tiempo = std::time(nullptr);
    std::tm* tiempoLocal = std::localtime(&tiempo);
    // extraemos la hora y minutos
    int hora = tiempoLocal->tm_hour;
    int minuto = tiempoLocal->tm_min;
    // verificamos si la última emoción registrada fue hace más de 5 minutos
    if (hora - std::stoi(ultimaEmocionH) == 0 && minuto - std::stoi(ultimaEmocionM) < 5)
    {
        // si la última emoción registrada fue hace menos de 5 minutos, desactivamos el registro de emociones
        reEmDesactivarRegistroEmocion();
        reEmActivarEmocionConfirmada();
        reEmCargarFraseMotivacional(std::stoi(ultimaEmocion));
    }
    // else
    // {
    //     // si la última emoción registrada fue hace más de 5 minutos, activamos el registro de emociones
    //     reEmActivarRegistroEmocion();
    // }

}

// ! Método para desactivar el registro de emociones
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::reEmDesactivarRegistroEmocion()
{
    reEmTitulo->hide();
    reEmBotonMiedo->hide();
    reEmBotonTristeza->hide();
    reEmBotonAlegria->hide();
    reEmBotonEnojo->hide();
    reEmBotonSorpresa->hide();
    reEmBotonDisgusto->hide();
    reEmDesactivarEmocionConfirmada();

}

// ! Método para redimencionar redimencionar las cosas de reEm
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::reEmRedimencionarCosas()
{
    // OBTENEMOS EL TAMAÑO DEL FRAME DE REGISTRO DE EMOCIONES
    int ancho = frameRegistroEmociones->width();
    int alto = frameRegistroEmociones->height();
    // lo asignamos a reEmTitulo
    //reEmTitulo->setFixedSize(ancho - 10, alto * 0.1);
    reEmIconoFraseMotivacionalLabel->setFixedSize(ancho * 0.4, ancho * 0.4);
    reEmGraficaVista->setFixedSize(ancho * 0.95, alto * 0.90);

}

// ! Método para activar la ventana de espera para añadir una nueva emoción
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::reEmActivarEmocionConfirmada()
{
    // Desactivamos todo lo relacionado con el registro de emociones
    reEmDesactivarRegistroEmocion();
    // Activamos la ventana de espera
    reEmTitulo->show();
    reEmFraseMotivacional->show();
    reEmIconoFraseMotivacionalLabel->show();
    reEmHoraProximaFraseMotivacional->show();
    // extraemos la hora de la próxima frase motivacional
    std::string hora = "";
    //hora += (*configuracionesUsuario)["ultimaEmocionM"];
    // le agregamos 5 minutos a la hora
    int horaInt = std::stoi((*configuracionesUsuario)["ultimaEmocionH"]);
    int minuto = std::stoi((*configuracionesUsuario)["ultimaEmocionM"]) + 5;
    if (minuto >= 60)
    {
        minuto -= 60;
        horaInt++;
        hora = std::to_string(horaInt) + ":" + std::to_string(minuto);
    }
    else
    {
        hora = std::to_string(horaInt) + ":" + std::to_string(minuto);
    }
    reEmHoraProximaFraseMotivacional->setText("La próxima frase motivacional estará disponible a las " + QString::fromStdString(hora));
}

// ! Método para desactivar la ventana de espera para añadir una nueva emoción
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::reEmDesactivarEmocionConfirmada()
{
    // Activamos todo lo relacionado con el registro de emociones
    //reEmActivarRegistroEmocion();
    // Desactivamos la ventana de espera
    reEmTitulo->hide();
    reEmFraseMotivacional->hide();
    reEmIconoFraseMotivacionalLabel->hide();
    reEmHoraProximaFraseMotivacional->hide();
}


// ! Método para cargar una frase motivacional
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::reEmCargarFraseMotivacional(const int& emocion)
{
    // 0 -> miedo
    // 1 -> tristeza
    // 2 -> alegria
    // 3 -> enojo
    // 4 -> sorpresa
    // 5 -> disgusto
    //reEmTitulo->setText("¡Emoción registrada!");
    // generamos un numero aleatorio de 0 a 19
    int numeroAleatorio = rand() % 20;

    // hacemos un switch para seleccionar el tipo de frase motivacional
    switch (emocion)
    {
    case 0:
        reEmTitulo->setText("¡Miedo registrado!");
        reEmFraseMotivacional->setText(QString::fromStdString(reEmFrasesMotivacionales[0][numeroAleatorio]));
        // Metemos un mapa de pixeles al label
        reEmIconoFraseMotivacionalLabel->setPixmap(QPixmap(QString(RUTA_ICONOS.c_str()) + "Miedo-icono.svg").scaled(100, 100));
        break;
    case 1:
        reEmTitulo->setText("¡Tristeza registrada!");
        reEmFraseMotivacional->setText(QString::fromStdString(reEmFrasesMotivacionales[1][numeroAleatorio]));
        // Metemos un mapa de pixeles al label
        reEmIconoFraseMotivacionalLabel->setPixmap(QPixmap(QString(RUTA_ICONOS.c_str()) + "Tristeza-icono.svg").scaled(100, 100));
        break;
    case 2:
        reEmTitulo->setText("¡Alegría registrada!");
        reEmFraseMotivacional->setText(QString::fromStdString(reEmFrasesMotivacionales[2][numeroAleatorio]));
        // Metemos un mapa de pixeles al label
        reEmIconoFraseMotivacionalLabel->setPixmap(QPixmap(QString(RUTA_ICONOS.c_str()) + "Alegria-icono.svg").scaled(100, 100));
        break;
    case 3:
        reEmTitulo->setText("¡Enojo registrado!");
        reEmFraseMotivacional->setText(QString::fromStdString(reEmFrasesMotivacionales[3][numeroAleatorio]));
        // Metemos un mapa de pixeles al label
        reEmIconoFraseMotivacionalLabel->setPixmap(QPixmap(QString(RUTA_ICONOS.c_str()) + "Enojo-icono.svg").scaled(100, 100));
        break;
    case 4:
        reEmTitulo->setText("¡Sorpresa registrada!");
        reEmFraseMotivacional->setText(QString::fromStdString(reEmFrasesMotivacionales[4][numeroAleatorio]));
        // Metemos un mapa de pixeles al label
        reEmIconoFraseMotivacionalLabel->setPixmap(QPixmap(QString(RUTA_ICONOS.c_str()) + "Sorpresa-icono.svg").scaled(100, 100));
        break;
    case 5:
        reEmTitulo->setText("¡Disgusto registrado!");
        reEmFraseMotivacional->setText(QString::fromStdString(reEmFrasesMotivacionales[5][numeroAleatorio]));
        // Metemos un mapa de pixeles al label
        reEmIconoFraseMotivacionalLabel->setPixmap(QPixmap(QString(RUTA_ICONOS.c_str()) + "Disgusto-icono.svg").scaled(100, 100));
        break;
    default:
        break;
    }
}

// ! Método para cargar la grafica de emociones
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::reEmCargarGraficaEmociones(const int& dia, const int& mes, const int& anio)
{
    // verificamos la existencia de la carpeta de emociones
    std::string rutaEmociones = RUTA_USUARIOS + encriptado->desencriptar((*configuracionesUsuario)["nombreUsuario"]) + "/emociones" + "/" + std::to_string(anio) + "/" + std::to_string(mes) + "/" + std::to_string(dia);
    if (!manejadorArchivos.verificarExistenciaDeCarpeta(rutaEmociones))
    {
        // si no existe la carpeta de emociones, mostramos un mensaje de error
        std::cout << "No se han registrado emociones en la fecha seleccionada" << std::endl;
        QMessageBox::critical(this, "Error", "No se han registrado emociones en la fecha seleccionada");
        return;
    }
    reEmGraficaSeries->clear();
    std::cout << "Cargando gráfica de emociones" << std::endl;
    // ? ahora debemos extraer el contenido de cada archivo de emocion para poder cargar la gráfica
    std::vector<std::string> emociones = manejadorArchivos.obtenerContenidoCarpeta(rutaEmociones, 0);
    //int contador = 0;
    for (const auto& emocion: emociones)
    {
        std::string contenido = manejadorArchivos.leerArchivo(rutaEmociones + "/" + emocion);
        //std::cout << "Emoción: " << emocion << " Contenido: " << contenido << std::endl;
        contenido = contenido.empty() ? "0" : contenido;
        // convertimos el contenido a entero
        int cantidad = std::stoi(contenido);
        //contador += cantidad;
        // agregamos la cantidad a la gráfica
        reEmGraficaSeries->append(QString::fromStdString(emocion), cantidad);

        //reEmGraficaSeries->append(contLabel, cantidad);
    }
    for (auto* serie: reEmGraficaSeries->slices())
    {
        // extraemos el contenido de la serie
        QString emocion = serie->label();
        std::string contLabel = "<html>" + emocion.toStdString() + " ";
        if (emocion == "miedo")
        {
            contLabel += "😱";

        }
        else if (emocion == "tristeza")
        {
            contLabel += "😢";
        }
        else if (emocion == "alegria")
        {
            contLabel += "😁";
        }
        else if (emocion == "enojo")
        {
            contLabel += "😡";
        }
        else if (emocion == "sorpresa")
        {
            contLabel += "😱";

        }
        else if (emocion == "disgusto")
        {
            //contLabel += "😖";
            contLabel += "😒";

        }
        contLabel += "  %" + std::to_string(serie->percentage() * 100) + " ";
        contLabel += "</html>";
        serie->setLabel(contLabel.c_str());
        serie->setLabelVisible();

    }
    //std::cout << "Total de emociones: " << contador << std::endl;
    // mostramos la gráfica
    reEmGraficaVista->show();
    reEmActivarGraficaEmociones();

}

// ! Método para mostrar la grafica de emociones
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::reEmActivarGraficaEmociones()
{
    // USAMOS EL METODO DE LA BARRA DE NAVIGACION PARA DESACTIVAR TODAS LAS INTERFACES
    barNaDesactivarTodosLosFrames();
    // activamos la interfaz de registro de emociones
    activarInterfazRegistroEmociones();
    // ocultamos las otras 2 interfaces
    //reEmActivarRegistroEmocion();
    reEmDesactivarRegistroEmocion();
    reEmDesactivarEmocionConfirmada();
    // mostramos la gráfica de emociones
    reEmGraficaVista->show();
}

// ! Método para ocultar la grafica de emociones
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::reEmDesactivarGraficaEmociones()
{
    // ocultamos la gráfica de emociones
    reEmGraficaVista->hide();
    // mostramos la interfaz de registro de emociones
    //activarInterfazRegistroEmociones();
}


// ////////////////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------------------------
// ! RELACIONADOS CON LA INTERFAZ DE AJUSTES

// ! Método para mostrar la interfaz de ajustes
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::activarInterfazAjustes()
{
    frameAjustes->show();
    //ajustesRedimencionarCosas();
}

// ! Método para ocultar la interfaz de ajustes
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::desactivarInterfazAjustes()
{
    frameAjustes->hide();
}

// ! Método para cambiar fuente de la interfaz
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::ajusClickCambiarFuente()
{
    std::cout << "Cambiando fuente" << std::endl;
    // mostramos un dialogo para seleccionar la fuente
    QFontDialog dialogo;
    dialogo.setOption(QFontDialog::DontUseNativeDialog);
    dialogo.show();
    // si se acepta la fuente
    if (dialogo.exec())
    {
        // extraemos la fuente seleccionada
        QFont fuente = dialogo.selectedFont();
        // cambiamos la fuente de la aplicación
        this->setFont(fuente);
        // extraemos el nombre de la fuente
        std::string nombreFuente = fuente.family().toStdString();
        // guardamos la fuente en el json de configuraciones
        (*configuraciones)["fuente"] = nombreFuente;
        (*configuraciones)["tamanoFuente"] = std::to_string(fuente.pointSize());
        // guardamos el json
        configuraciones->guardar();
    }
}

// ! Método para cambiar la contraseña de la interfaz
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::ajusClickCambiarContra()
{
    //std::cout << "Cambiando contraseña" << std::endl;
    // mostramos un dialogo para cambiar la contraseña
    QDialog dialogo;
    QFormLayout formDialogo(&dialogo);
    dialogo.setWindowTitle("Cambiar Contraseña");
    QLabel label;
    label.setText("Contraseña:");
    QLineEdit input;
    input.setEchoMode(QLineEdit::Password);
    formDialogo.addRow(&label, &input);
    QLabel label2("De nuevo");
    QLineEdit input2;
    input2.setEchoMode(QLineEdit::Password);
    formDialogo.addRow(&label2, &input2);
    QDialogButtonBox botones(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialogo);
    formDialogo.addRow(&botones);
    QObject::connect(&botones, &QDialogButtonBox::accepted, &dialogo, &QDialog::accept);
    if (dialogo.exec() == QDialog::Accepted)
    {
        // extraemos la contraseña
        std::string contra = input.text().toStdString();
        std::string contra2 = input2.text().toStdString();
        // verificamos que las contraseñas sean iguales
        if (contra != contra2)
        {
            QMessageBox::critical(this, "Error", "Las contraseñas no coinciden");
            return;
        }
        std::cout << "Contraseña: " << contra << std::endl;

        // guardamos la contraseña en el json de configuraciones
        (*configuracionesUsuario)["contraseña"] = encriptado->encriptar(contra);
        // guardamos el json
        configuraciones->guardar();
    }

}

// ! Método para cambiar la pregunta de recuperación de contraseña
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::ajusClickCambiarPreguntaContra()
{
    // mostramos un dialogo para cambiar la pregunta de recuperación de contraseña
    QDialog dialogo;
    QFormLayout formDialogo(&dialogo);
    dialogo.setWindowTitle("Cambiar Pregunta de Recuperación de Contraseña");
    QLabel label;
    label.setText("Pregunta:");
    QLineEdit input;
    formDialogo.addRow(&label, &input);
    QLabel label2("Respuesta");
    QLineEdit input2;
    formDialogo.addRow(&label2, &input2);
    QDialogButtonBox botones(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialogo);
    formDialogo.addRow(&botones);
    QObject::connect(&botones, &QDialogButtonBox::accepted, &dialogo, &QDialog::accept);
    if (dialogo.exec() == QDialog::Accepted)
    {
        // extraemos la pregunta
        std::string pregunta = input.text().toStdString();
        std::string respuesta = input2.text().toStdString();
        std::cout << "Pregunta: " << pregunta << std::endl;
        std::cout << "Respuesta: " << respuesta << std::endl;
        // verificamos que no estén vacías
        if (pregunta.empty() || respuesta.empty())
        {
            QMessageBox::critical(this, "Error", "La pregunta y respuesta no pueden estar vacías");
            return;
        }
        // guardamos la pregunta y respuesta en el json de configuraciones
        (*configuracionesUsuario)["preguntaRecuperacion"] = encriptado->encriptar(pregunta);
        (*configuracionesUsuario)["respuestaRecuperacion"] = encriptado->encriptar(respuesta);
        // guardamos el json
        configuracionesUsuario->guardar();
    }

}

// ! Método para cambiar la imagen de perfil
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::ajusClickCambiarFotoPerfil()
{
    // mostramos un dialogo para seleccionar la imagen de perfil
    QString rutaImagen = QFileDialog::getOpenFileName(this, "Seleccionar Imagen", QDir::homePath(), "Imágenes (*.png *.jpg *.jpeg)");
    // verificamos si se seleccionó una imagen
    if (rutaImagen.isEmpty())
    {
        return;
    }
    // verificamos si existe el archiv
    if (!manejadorArchivos.verificarExistenciaDeArchivo(rutaImagen.toStdString()))
    {
        QMessageBox::critical(this, "Error", "No se pudo cargar la imagen");
        return;
    }
    // verificamos si ya existe una imagen de perfil
    std::string rutaImagenPerfil = (*configuracionesUsuario)["fotoPerfil"];
    // la desencriptamos
    rutaImagenPerfil = encriptado->desencriptar(rutaImagenPerfil);
    std::cout << "Ruta imagen perfil: " << rutaImagenPerfil << std::endl;
    std::string rutaUsuario = RUTA_USUARIOS + encriptado->desencriptar((*configuracionesUsuario)["nombreUsuario"]);
    std::cout << "Ruta usuario: " << rutaUsuario << std::endl;
    // si existe la imagen de perfil, la renombramos temporalmente
    if (manejadorArchivos.verificarExistenciaDeArchivo(rutaUsuario + "/" + rutaImagenPerfil))
    {
        manejadorArchivos.moverArchivo(rutaUsuario + "/a" + rutaImagenPerfil, rutaUsuario + "/" + rutaImagenPerfil);
    }
    try{
        // extraemos la extensión de la imagen
        std::string extension = rutaImagen.toStdString().substr(rutaImagen.toStdString().find_last_of("."));
        std::cout << "Extensión: " << extension << std::endl;
        // copiamos la imagen seleccionada a la carpeta del usuario
        manejadorArchivos.copiarArchivo(rutaUsuario + "/fotoPerfil" + extension, rutaImagen.toStdString());
        // guardamos la ruta de la imagen en el json de configuraciones
        (*configuracionesUsuario)["fotoPerfil"] = encriptado->encriptar("fotoPerfil" + extension);
        // guardamos el json
        configuracionesUsuario->guardar();
        // eliminamos la imagen anterior
        if (manejadorArchivos.verificarExistenciaDeArchivo(rutaUsuario + "/a" + rutaImagenPerfil))
        {
            manejadorArchivos.eliminarArchivo(rutaUsuario + "/a" + rutaImagenPerfil);
        }
    } catch (std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        // restauramos la imagen
        manejadorArchivos.moverArchivo(rutaUsuario + "/" + rutaImagenPerfil, rutaUsuario + "/a" + rutaImagenPerfil);
    }
}

// ! Método para cambiar el nombre de usuario
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::ajusClickEliminarCuenta()
{
    // mostramos un dialogo de confirmación
    QMessageBox dialogo;
    dialogo.setWindowTitle("Eliminar Cuenta");
    dialogo.setText("¿Estás seguro de que deseas eliminar tu cuenta?");
    dialogo.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    dialogo.setDefaultButton(QMessageBox::No);
    if (dialogo.exec() == QMessageBox::Yes)
    {
        // extraemos el nombre de usuario
        std::string nombreUsuario = encriptado->desencriptar((*configuracionesUsuario)["nombreUsuario"]);
        // eliminamos la carpeta del usuario
        manejadorArchivos.eliminarCarpeta(RUTA_USUARIOS + nombreUsuario);
        // eliminamos el json de configuraciones
        //manejadorArchivos.eliminarArchivo(RUTA_USUARIOS + nombreUsuario + "/configuraciones.json");
        // mostramos un mensaje de éxito
        QMessageBox::information(this, "Éxito", "Cuenta eliminada con éxito");
        // activamos la interfaz de inicio de sesión
        barNaDesactivarTodosLosFrames();
        activarInterfazInicioSesion();
        // cerramos la aplicación
        //this->close();
    }
}

// ! Método para cambiar el sonido de alarma
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::ajusClickCambiarSonidoAlarma()
{
    // mostramos un dialogo para seleccionar el sonido de alarma
    QString rutaSonido = QFileDialog::getOpenFileName(this, "Seleccionar Sonido", QDir::homePath(), "Sonidos (*.mp3 *.wav)");
    // verificamos si se seleccionó un sonido
    if (rutaSonido.isEmpty())
    {
        return;
    }
    // verificamos si existe el archivo
    if (!manejadorArchivos.verificarExistenciaDeArchivo(rutaSonido.toStdString()))
    {
        QMessageBox::critical(this, "Error", "No se pudo cargar el sonido");
        return;
    }
    // extraemos la extensión del sonido
    std::string extension = rutaSonido.toStdString().substr(rutaSonido.toStdString().find_last_of("."));
    std::cout << "Extensión: " << extension << std::endl;
    // verificamos si ya existe un sonido de alarma
    std::string rutaSonidoAlarma = (*configuraciones)["sonidoAlarma"];
    if (manejadorArchivos.verificarExistenciaDeArchivo(rutaSonidoAlarma))
    {
        // si existe, lo eliminamos
        manejadorArchivos.eliminarArchivo(rutaSonidoAlarma);
    }
    // copiamos el sonido seleccionado a la carpeta de sonidos
    std::string ruta = RUTA_USUARIOS + encriptado->desencriptar((*configuracionesUsuario)["nombreUsuario"]);
    //manejadorArchivos.copiarArchivo(RUTA_SONIDOS + "/sonidoAlarma" + extension, rutaSonido.toStdString());
    manejadorArchivos.copiarArchivo(ruta + "/sonidoAlarma" + extension, rutaSonido.toStdString());
}

// ! Método para cambiar tema
// ! versión 1.0
// ! modificado por Aether
// ? Sin cambios primera versión
void MainWindow::ajusCambiarTema(int nItem)
{
    std::cout << "Cambiando tema a : " << nItem << std::endl;
    // aplicamos el tema seleccionado
    this->setStyleSheet(temas[nItem].c_str());
    // guardamos el tema en el json de configuraciones
    (*configuraciones)["tema"] = std::to_string(nItem);
    // guardamos el json
    configuraciones->guardar();
}


// ////////////////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------------------------
// RELACIONADO CON LA INTERFAZ HIPERFOCO

// ! Metodo para activar la interfaz de hiperfoco.
// ! versión 1.0
// ! modificado por mbraulio
// ? Sin cambios primera versión
void MainWindow::activarInterfazHiperfoco()
{
    frameHiperfoco->show();
    barNaConfigurarBotones(0, true, 11, "Ver lista de Metodos");
    hipeMostrarComponentesPorDefecto();
    if (banderaHiperfocoActivo)
    {
        hipeTextoHoras->hide();                     //escondemos
        hipeHorasSpinBox->hide();
        hipeTextoMinutos->hide();
        hipeMinutosSpinBox->hide();
        hipeTextoTiempoEstablecido->show();
    }
    banderaInterfazHiperfoco = true; //activamos la bandera para saber que la interfaz de hiperfoco esta activa

}

// ! Metodo para desactivar la interfaz de hiperfoco.
// ! versión 1.0
// ! modificado por mbraulio
// ? Sin cambios primera versión
void MainWindow::desactivarInterfazHiperfoco()
{
    frameHiperfoco->hide();
    banderaInterfazHiperfoco = false; //desactivamos la bandera para saber que la interfaz de hiperfoco esta desactiva
}

// ! Metodo que establece el cambio en H2 para colocar el tiempo establecido por el usuario y activar el temporizador.
// ! versión 1.0
// ! modificado por mbraulio
// ? Sin cambios primera versión
void MainWindow::hipeBotonHiperfocoActivado()
{
    banderaInterfazHiperfoco = true; //activamos la bandera para saber que la interfaz de hiperfoco esta activa
    banderaHiperfocoActivo = true; //activamos la bandera para saber que el hiperfoco esta activo
    int horaValor = hipeHorasSpinBox->value(); //obtenemos las horas que establecio el usuario
    int minutosValor = hipeMinutosSpinBox->value(); //obtenemos los minutos que establecio el usuario

    if (timer->isActive())
    {
        hipeMostrarComponentesPorDefecto();
        timer->stop();
        QMessageBox::information(frameHiperfoco,"Información", "Temporizador interrumpido");
        banderaHiperfocoActivo = false;
        return;
    }

    QMessageBox mensajeDeConfirmacion;
    mensajeDeConfirmacion.setWindowTitle("Establecer");
    mensajeDeConfirmacion.setText(QString("Hora: %1\tMinutos: %2").arg(horaValor).arg(minutosValor));
    mensajeDeConfirmacion.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    QPushButton *okButton = qobject_cast<QPushButton*>(mensajeDeConfirmacion.button(QMessageBox::Ok));
    if (okButton) {
        okButton->setText("Aceptar");
        okButton->setStyleSheet("background-color: green; color: white;");
    }

    QPushButton *cancelButton = qobject_cast<QPushButton*>(mensajeDeConfirmacion.button(QMessageBox::Cancel));
    if (cancelButton) {
        cancelButton->setText("Cancelar");
        cancelButton->setStyleSheet("background-color: red; color: white;");
    }

    int reply = mensajeDeConfirmacion.exec();
    if (reply == QMessageBox::Ok)
    {
        hipeTextoHoras->hide();                     //escondemos
        hipeHorasSpinBox->hide();
        hipeTextoMinutos->hide();
        hipeMinutosSpinBox->hide();
        hipeTextoTiempoEstablecido->show();
        int tiempoTotal = 0;
        //QMessageBox::information(this, "Confirmado", "Has aceptado.");
        hipeBotonDeHiperfoco->setIcon(*hipeIconoHiperfocoActivado);
        // Calcular el tiempo en milisegundos
        tiempoTotal = (horaValor * 3600 + minutosValor * 60) * 1000;
        timer->start(tiempoTotal);
        hipeTextoTiempoEstablecido->setText(QString("Tiempo Restante: %1:%2 hrs").arg(horaValor, 2, 10, QChar('0')).arg(minutosValor, 2, 10, QChar('0')));
    }
    else
    {
        //QMessageBox::information(this, "Cancelado", "Has cancelado.");
        hipeMostrarComponentesPorDefecto();
        timer->stop();
    }
}

// ! Metodo para regresar los items cuando se termina el temporizador.
// ! versión 1.0
// ! modificado por mbraulio
// ? Sin cambios primera versión
void MainWindow::hipeBotonHiperfocoDesactivado()
{
    banderaHiperfocoActivo = false;
    QMessageBox::information(hipeBotonDeHiperfoco->parentWidget(), "Tiempo de Hiperfoco terminado", "Toma un descanso :) ");
    hipeBotonDeHiperfoco->setIcon(*hipeIconoHiperfocoDesactivado);
    if (banderaInterfazHiperfoco)
    {
        hipeHorasSpinBox->show();
        hipeMinutosSpinBox->show();
        hipeTextoHoras->show();
        hipeTextoMinutos->show();
    }
    hipeTextoTiempoEstablecido->hide();
}

// ! Metodo para ver la lista de metodos para mindfulness.
// ! versión 1.0
// ! modificado por mbraulio
// ? Sin cambios primera versión
void MainWindow::hipeBotonVerMetodos()
{
    banderaInterfazHiperfoco = false; //desactivamos la bandera para saber que la interfaz de hiperfoco esta desactiva
    hipeTituloPrincipal->hide();
    hipeTextoDuracion->hide();
    hipeTextoHoras->hide();
    hipeTextoMinutos->hide();
    hipeHorasSpinBox->hide();
    hipeMinutosSpinBox->hide();
    hipeBotonDeHiperfoco->hide();
    hipeTextoTiempoEstablecido->hide();

    hipeTituloListaMetodos->show();
    hipeListaDeMetodos->show();

    hipeTituloPomodoro->hide();
    hipeTextoPomodoro->hide();
    hipeTituloTitulo5Segundos->hide();
    hipeTextoTitulo5Segundos->hide();
    hipeTituloMindfulness->hide();
    hipeTextoMindfulness->hide();
}

// ! Metodo para tener los componentes que por defecto pertenecen a la interfaz de hiperfoco.
// ! versión 1.0
// ! modificado por mbraulio
// ? Sin cambios primera versión
void MainWindow::hipeMostrarComponentesPorDefecto()
{
    banderaInterfazHiperfoco = true; //activamos la bandera para saber que la interfaz de hiperfoco esta activa
    hipeTituloPrincipal->show();
    hipeTextoDuracion->show();
    hipeTextoHoras->show();
    hipeTextoMinutos->show();
    hipeHorasSpinBox->show();
    hipeMinutosSpinBox->show();
    hipeBotonDeHiperfoco->show();
    hipeBotonDeHiperfoco->setIcon(*hipeIconoHiperfocoDesactivado);
    hipeTextoTiempoEstablecido->hide();

    hipeTituloListaMetodos->hide();
    hipeListaDeMetodos->hide();

    hipeTituloPomodoro->hide();
    hipeTextoPomodoro->hide();
    hipeTituloTitulo5Segundos->hide();
    hipeTextoTitulo5Segundos->hide();
    hipeTituloMindfulness->hide();
    hipeTextoMindfulness->hide();
}

// ! Metodo para manejar los items de la lista de metodos.
// ! versión 1.0
// ! modificado por mbraulio
// ? Sin cambios primera versión
void MainWindow::hipeManejadorDeListadeItems(QListWidgetItem *item)
{
    if (item == hipeItemPomodoro) {
        hipeInfoTecnicaPomodoro();
    } else if (item == hipeItem5Segundos) {
        hipeInfoTecnica5segundos();
    } else if (item == hipeItemMindfulness) {
        hipeInfoMindfulness();
    }

}

// ! Metodo para mostrar los items del metodo de Tecnica Pomodoro.
// ! versión 1.0
// ! modificado por mbraulio
// ? Sin cambios primera versión
void MainWindow::hipeInfoTecnicaPomodoro()
{
    hipeTituloListaMetodos->hide();
    hipeListaDeMetodos->hide();
    hipeTituloPomodoro->show();
    hipeTextoPomodoro->show();
    banderaInterfazHiperfoco = false; //desactivamos la bandera para saber que la interfaz de hiperfoco esta desactiva
}

// ! Metodo para mostrar los items del metodo de Tecnica dede 5 segundos.
// ! versión 1.0
// ! modificado por mbraulio
// ? Sin cambios primera versión
void MainWindow::hipeInfoTecnica5segundos()
{
    hipeTituloListaMetodos->hide();
    hipeListaDeMetodos->hide();
    hipeTituloTitulo5Segundos->show();
    hipeTextoTitulo5Segundos->show();
    banderaInterfazHiperfoco = false; //desactivamos la bandera para saber que la interfaz de hiperfoco esta desactiva
}

// ! Metodo para mostrar los items del metodo de Tecnica de Mindfulness.
// ! versión 1.0
// ! modificado por mbraulio
// ? Sin cambios primera versión
void MainWindow::hipeInfoMindfulness()
{
    hipeTituloListaMetodos->hide();
    hipeListaDeMetodos->hide();
    hipeTituloMindfulness->show();
    hipeTextoMindfulness->show();
    banderaInterfazHiperfoco = false; //desactivamos la bandera para saber que la interfaz de hiperfoco esta desactiva
}


// ////////////////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------------------------
// ! RELACIONADOS CON LA INTERFAZ GRÁFICA
void MainWindow::resizeEvent(QResizeEvent *event)
{

    QWidget::resizeEvent(event);
    //frame->setFixedSize(this->size());
    jourPanBiRedimensionarIconoBoton();
    reEmRedimencionarCosas();
    //calDiaFrame->setFixedSize(this->size());
    //std::cout << "calDiaFrame: " << calDiaFrame->size().width() << " " << calDiaFrame->size().height() << std::endl;
    // mostramos el tamaño de la ventana
    //std::cout << "Tamaño de la ventana: " << this->size().width() << " " << this->size().height() << std::endl;

}

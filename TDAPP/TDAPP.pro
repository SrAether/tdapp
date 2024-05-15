QT       += core gui

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

CONFIG += c++17 -fpermissive

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    encriptado.cpp \
    estilos.cpp \
    main.cpp \
    mainwindow.cpp \
    manejadorArchivos.cpp \
    manejadorJson.cpp

HEADERS += \
    encriptado.h \
    estilos.h \
    mainwindow.h \
    manejadorArchivos.h \
    manejadorJson.h

TRANSLATIONS += \
    TDAPP_es_MX.ts
CONFIG += lrelease
CONFIG += embed_translations

# Incluir la biblioteca OpenSSL para encriptar y desencriptar
LIBS += -LC:/OpenSSL-Win64/lib/VC/x64/MD -lssl -lcrypto
INCLUDEPATH += C:/OpenSSL-Win64/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

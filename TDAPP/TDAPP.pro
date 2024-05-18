QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

CONFIG += c++17 -fpermissive

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    encriptado.cpp \
    main.cpp \
    mainwindow.cpp \
    manejadorArchivos.cpp \
    manejadorJson.cpp

HEADERS += \
    encriptado.h \
    mainwindow.h \
    manejadorArchivos.h \
    manejadorJson.h

TRANSLATIONS += \
    TDAPP_es_MX.ts
CONFIG += lrelease
CONFIG += embed_translations

# Incluir la biblioteca OpenSSL para encriptar y desencriptar
LIBS += -lssl -lcrypto

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

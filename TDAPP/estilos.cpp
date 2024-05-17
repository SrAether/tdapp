#include "estilos.h"

const QString Clasico = R"(
    *{
        border: none !important;
        margin: none !important;
    }

    QPushButton {
        background-color: #ABDDB5; /* Color de fondo verde */
        color: white; /* Color del texto blanco */
        font-size: 16px; /* Tamaño de la fuente 16px */
        border-radius: 5px; /* Bordes redondeados */
        max-height: 30px;
        max-width: 200 px;
        font-size: 25 px;
        font-family: "Arial";
        min-height: 20px;
    }

    QTextEdit {
        background-color: white;
        font-family: "Schadow BT";
        font-size: 24px;
    }

    QMessageBox{
        background-color: #F1AEAA;
    }

    QMessageBox QLabel {
        background-color: #F1AEAA;
        color: black;
    }

    QMessageBox QPushButton{
        min-height: 25px;
        min-width: 100px;
        border: 1px solid black;
    }

    QLineEdit{
        background-color: white;
        border: 0px;
        border-radius: 10px;
        padding: 5px;
        max-width: 300px;
    }

    QComboBox{
        background-color: white;
        border: 0px;
        border-radius: 10px;
        padding: 5px;
        max-width: 300px;
    }

    QFrame {
        background-color: #F1AEAA; /* Color de fondo para todos los QFrame */
        border: none;
        margin: 0px;
        padding: 0px;
    }

    QLabel{
        font-size: 24px;
        font-family: "Schadow BT";
        max-height: 50px;
        background-color: #ffffff;
        border-radius: 10px;
    }

    QVBoxLayout{
        border: none;
        margin: 0px;
        padding: 0px;
    }

    QHBoxLayout{
        border: none;
        margin: 0px;
        padding: 0px;
    }

    QScrollArea{
        border: none;
        margin: 0px;
        padding: 0px;
    }


            QListWidget {
                background-color: white;
                border: 2px solid #000000;
                border-radius: 10px;
                font-family: 'Schadow BT';
                margin: 0px;
                padding: 0px;
                font-size: 20pt;
            }
            QListWidget::item {
                padding: 10px;
                border-bottom: 1px solid #dcdcdc;
            }
            QListWidget::item:selected {
                background-color: #0078d7;
                color: white;
            }
            QListWidget::item:hover {
                background-color: #e0e0e0;
                color: black;
            }
            QListWidget::item:disabled {
                color: #808080;
            }


)";

const QString Oscuro = R"(
    *{
        border: none !important;
        margin: none !important;
    }

    QPushButton {
        background-color: #636179; /* Color de fondo verde */
        color: white; /* Color del texto blanco */
        font-size: 16px; /* Tamaño de la fuente 16px */
        border-radius: 5px; /* Bordes redondeados */
        max-height: 30px;
        max-width: 200 px;
        font-size: 25 px;
        font-family: "Arial";
        min-height: 20px;
    }

    QTextEdit {
        background-color: #DEDEE2;
        font-family: "Schadow BT";
        font-size: 24px;
    }

    QMessageBox{
        background-color: #6B65A2;
    }

    QMessageBox QLabel {
        background-color: #6B65A2;
        color: black;
    }

    QMessageBox QPushButton{
        min-height: 25px;
        min-width: 100px;
        border: 1px solid black;
    }

    QLineEdit{
        background-color: #DEDEE2;
        border: 0px;
        border-radius: 10px;
        padding: 5px;
        max-width: 300px;
    }

    QComboBox{
        background-color: #DEDEE2;
        border: 0px;
        border-radius: 10px;
        padding: 5px;
        max-width: 300px;
    }

    QFrame {
        background-color: #6B65A2; /* Color de fondo para todos los QFrame */
        border: none;
        margin: 0px;
        padding: 0px;
    }

    QLabel{
        font-size: 24px;
        font-family: "Schadow BT";
        max-height: 50px;
        background-color: #DEDEE2;
        border-radius: 10px;
    }

    QVBoxLayout{
        border: none;
        margin: 0px;
        padding: 0px;
    }

    QHBoxLayout{
        border: none;
        margin: 0px;
        padding: 0px;
    }

    QScrollArea{
        border: none;
        margin: 0px;
        padding: 0px;
    }

            QListWidget {
                background-color: #DEDEE2;
                border: 2px solid #000000;
                border-radius: 10px;
                font-family: 'Schadow BT';
                margin: 0px;
                padding: 0px;
                font-size: 20pt;
            }
            QListWidget::item {
                padding: 10px;
                border-bottom: 1px solid #606060;
            }
            QListWidget::item:selected {
                background-color: #0078d7;
                color: white;
            }
            QListWidget::item:hover {
                background-color: #e0e0e0;
                color: black;
            }
            QListWidget::item:disabled {
                color: #808080;
            }


)";


const QString Claro = R"(
    *{
        border: none !important;
        margin: none !important;
    }

    QPushButton {
        background-color: #42BFDD; /* Color de fondo verde */
        color: white; /* Color del texto blanco */
        font-size: 16px; /* Tamaño de la fuente 16px */
        border-radius: 5px; /* Bordes redondeados */
        max-height: 30px;
        max-width: 200 px;
        font-size: 25 px;
        font-family: "Arial";
        min-height: 20px;
    }

    QTextEdit {
        background-color: #F8F3FF;
        font-family: "Schadow BT";
        font-size: 24px;
    }

    QMessageBox{
        background-color: #F9FBB2;
    }

    QMessageBox QLabel {
        background-color: #F9FBB2;
        color: black;
    }

    QMessageBox QPushButton{
        min-height: 25px;
        min-width: 100px;
        border: 1px solid black;
    }

    QLineEdit{
        background-color: #F8F3FF;
        border: 0px;
        border-radius: 10px;
        padding: 5px;
        max-width: 300px;
    }

    QComboBox{
        background-color: #F8F3FF;
        border: 0px;
        border-radius: 10px;
        padding: 5px;
        max-width: 300px;
    }

    QFrame {
        background-color: #F9FBB2; /* Color de fondo para todos los QFrame */
        border: none;
        margin: 0px;
        padding: 0px;
    }

    QLabel{
        font-size: 24px;
        font-family: "Schadow BT";
        max-height: 50px;
        background-color: white;
        border-radius: 10px;
    }

    QVBoxLayout{
        border: none;
        margin: 0px;
        padding: 0px;
    }

    QHBoxLayout{
        border: none;
        margin: 0px;
        padding: 0px;
    }

    QScrollArea{
        border: none;
        margin: 0px;
        padding: 0px;
    }

            QListWidget {
                background-color: #F8F3FF;
                border: 2px solid #000000;
                border-radius: 10px;
                font-family: 'Schadow BT';
                margin: 0px;
                padding: 0px;
                font-size: 20pt;
            }
            QListWidget::item {
                padding: 10px;
                border-bottom: 1px solid #606060;
            }
            QListWidget::item:selected {
                background-color: #0078d7;
                color: white;
            }
            QListWidget::item:hover {
                background-color: #e0e0e0;
                color: black;
            }
            QListWidget::item:disabled {
                color: #808080;
            }


)";


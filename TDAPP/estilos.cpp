#include "estilos.h"

const QString StylesQSS1 = R"(
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
)";

const QString StylesQSS2 = R"(
    QPushButton {
        background-color: #FFD1DC; /* Color de fondo verde */
        color: white; /* Color del texto blanco */
        font-size: 16px; /* Tamaño de la fuente 16px */
        border-radius: 5px; /* Bordes redondeados */
    }
)";


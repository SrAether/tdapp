# tdapp

diseño de archivos
{
      "clave1": "valor1",
      "clave2": "valor2",
      "clave3": "valor3",
      "clave4": "valor4",
      "clave5": "valor5", 
},


// diseño de archivo de ajustes
{
      "tema": "valor1",
      "fuente": "valor2",
      "sonidoAlarma": "valor3",
      "sonidoNotificacion": "valor4",
}

// diseño de archivo usuario
{
      "nombre": "valor1",
      "apodo": "valor2",
      "sexo": "valor3",
      "edad": "valor4",
      "imagenUsuario":"rutaImagen",
      "contraseña": "valor5",
      "claveDrive": "valor5",
      "contraseñaDrive": "valor5",
      "portadaJournaling": "rutaArchivoPortada",
      "tecnicaDeGestion": "nombreDeLaTecnica",
      "respaldo": 3,
      "ruta": "rutaCarpetaArchivos",
},



diseño de archivos Notas (journaling)
{
      "fechaDeCreacion": "valor1",
      "fechaDeModificacion": "valor2",
      "titulo": "valor3",
      "descripcion": "valor4",
},

diseño de archivos dia calendario
{
      "fecha": "valor1",
      "evento1": "id",
      "evento2": "id",
},

diseño de archivos evento
{
      "fecha": "valor1",
      "horaInicio": "valor2",
      "horaFin": "valor3",
      "titulo": "valor4",
      "descripcion": "valor5", 
},


diseño de archivos emociones por dia emo12092024.tak
{
      "dia": ["f","t",],
},

diseño de archivos emociones grafica
{
      "f": "numero de rep",
      "t": "valor2",
      "d": "valor3",
      "i": "valor4",
      "m": "valor5",
      "s": "valor5",
},

diseño de archivos metodos
{
      "tecnica 1": "descripcion",
      "tecnica 2": "descripcion",
      "tecnica 3": "descripcion",
      "tecnica 4": "descripcion",
},

diseño de archivos frases
{
      "emocionf": ["frase1", "frase2"],
      "emociont": ["frase1", "frase2"],
      "emociond": ["frase1", "frase2"],
      "emocioni": ["frase1", "frase2"],
      "emocionm": ["frase1", "frase2"],
      "emocions": ["frase1", "frase2"],
},

clase de manejo de archivos
      metodos
      crearArchivo(string nombre, string contenido)
      eliminarArchivo(string nombre)
      modificarArchivo(string nombre, string contenido)
      string leerArchivo(string nombre)
      bool verificarExistenciaDeCarpeta(string ruta)
      bool verificarExistenciaDeArchivo(string ruta)
      copiarFichero(string rutaNueva, string rutaAnterior)
    
clase de manejo de jsons
      atributo
            map (public)
            ruta (private)
      metodos
            contructor(std::string rutaArchivo, bool nuevo=false),
            modificar(std::map nuevoMap),
            guardar(),
            modificarRuta(std::string nuevaRuta),
            eliminar(),
            estructurar()

clase de encriptado o de cifrado, como quieran llamarle
      metodos
            cifrar(std::string contrasena, std::string contenido) -> std::string contenidoCifrado
            decifrar(std::string contrasena, std::contenido) -> std::string contenidoDecifrado
            cambiarCifrado(std::string contrasenaAntigua, std::string contrasenaNueva, std::string contenido) -> std::string contenidoCifrado 

clase de respaldo
      atributo
            std::string correo
            std::string contraseñaDrive
      metodo
            respaldo(std::string correo, std::string contraseñaDrive),
            setCorreo(std::string correo);
            getCorreo() -> string;
            setContrasenaDrive(std::string contrasenaDrive);
            getContrasenaDrive() -> string;
            Ingreso() -> bool
            respaldar(ahi vamos viendo)
            recuperar()
            verificar()
            eliminar()



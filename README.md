# Laboratorio de sistemas inteligentes sobre el internet de las cosas

Este repositorio recoge los programas del proyecto desarrollado en el laboratorio de sistemas del máster de internet de las cosas de la Universidad Complutense de Madrid. El proyecto se encuentra dividido en tres grandes bloques:
-	Lectura y escritura de tags usando un modulo NFC y el ESP8266.
-	Desarrollo de aplicación Android para comunicar un Smartphone y el módulo ESP8266 usando NFC.
-	Control de sensor conectado a ESP8266 usando el protocolo MQTT.

## Material utilizado

- Smartphone con NFC

- Modulo NFC v3

- SoC ESP8266

- Tarjetas o tags NFC 

- 4 cables

- 1 cable Micro USB

## Dependencias 

Arduino IDE 1.8.5 con:
  - Plugin [ESP8266](https://github.com/esp8266/Arduino)
  
  - Libreria [NFC](https://github.com/elechouse/PN532)


Android Studio 1.5.1 con:
  - Android SDK 27
  
  - Android Build Tools v27.0.2
  
  - Android Support Repository

## Parte I - Lectura y escritura de tags usando un modulo NFC y el ESP8266

### Preparación hardware

La comunicacion entre el SoC ESP8266 y el modulo NFC se va a llevar a cabo usando el protocolo I2C. Para ello es necesario usar 4 cables que unan los puertos que apaceren en la siguiente tabla:


| ESP8266       | NFC v3        | 
| ------------- |:-------------:| 
| GND           | GND           | 
| 3V            | VCC           | 
| D7            | SDA           | 
| D6            | SCL           | 

Hay que tener en cuenta que no es obligatorio usar los puertos D6 y D7 del ESP8266,para usar otros puertos solo es necesario modificar la linea de codigo **Wire.begin(D7,D6)** que aparece tanto en los programas de [escribir](https://github.com/jorgejarne/Laboratorio/blob/master/nfcEsp8266Write.ino) como en el de [leer](https://github.com/jorgejarne/Laboratorio/blob/master/nfcEsp8266ReadPass.ino).

### Escritura

Este [codigo](https://github.com/jorgejarne/Laboratorio/blob/master/nfcEsp8266Write.ino) se encarga de escribir cualquier tipo de informacion en la memoria libre del tag que se acerca al modulo NFC. En este proyecto se ha decidido guardar las credenciales de una red WiFi pero se puede guardar otro tipo de informacion cambiando la linea 48 del codigo **message.addUriRecord("texto_a_grabar");**


### Lectura

Este [programa](https://github.com/jorgejarne/Laboratorio/blob/master/nfcEsp8266ReadPass.ino) realiza la lectura de toda información que contiene el tag y busca las credenciales de la red Wifi, una vez que las tiene intenta conectarse a dicha red. **El programa entra en bucle comprobando si se ha conectado o no a la red, en la mayoria de ocasiones este proceso dura varias iteracciones hasta que se conecta. Dependiendo de la version del SoC de ESP8266 que se tenga, podria tardar mas o menos en establecer la conexion con la red, por lo que seria necesario modificar la constante MAX_TRIES_CONNECTING que esta definida en el codigo** 

## Parte II - Desarrollo de aplicación Android para comunicar un Smartphone y el módulo ESP8266 usando NFC

El objetivo que tiene esta parte es realizar el intercambio de informacion entre un smartphone con NFC y el ESP8266 con su modulo NFC. El smartphone va a actuar como si fuera una tarjeta o tag, este modo de funcionamiento tambien es conocido como Host-based card emulation (HCE) y esta disponible desde la version Android 4.4. EL HCE puede tener diferentes servicios funcionando internamente por eso a cada uno de estos se le asigna un ApplicationID (AID), entonces cuando el otro dispositvo NFC se acerca, le envia el AID del servicio del HCE con el que quiere hablar. En este caso no ha sido necesario usar ningun AID concreto ya que solo se tiene un servicio funcionando en la app.

La [aplicacion](https://github.com/jorgejarne/Laboratorio/blob/master/appNFC.apk) de android se encuentra ya compilada en formato **apk** para instalarla directamente en el smartphone. Tambien se encuentra en el repositorio el [proyecto](https://github.com/jorgejarne/Laboratorio/blob/master/androidNfcHce.zip) completo de android studio por si el usuario quiere añadir mas servicios o hacer otro tipo de modificaciones.

Tras instalar la aplicacion Android y ejecutarla, aparecen hay dos ventanas, la superior muestra que datos se han intercambiado y en la venta inferior hay dos campos llamados **Nombre del wifi(SSID)** y **Contrasena del wifi(PASS)** que el usuario tiene que rellenar. La informacion introducida en estos campos es la que se enviara al modulo NFC cuando se acerque el movil a el.

El SoC ESP8266 debe tener cargado este [sketch](https://github.com/jorgejarne/Laboratorio/blob/master/lecturaHCE.ino). El programa esta a la espera de que se acerque algun dispositivo NFC para iniciar la conversacion enviandole el AID. Este valor se encunetra en la variable **uint8_t selectApdu[]** del codigo, sin embargo no influye, ya que se usaria solo en el caso de que se tuvieran diferentes AID en la app de Android desarrollada. Si el intercambio de informacion entre los dos dispositivos se a realizado correctamente, el modulo ESP8266 intentara concectarse a la red con el nombre y contraseña que se escribieron en los campos de la app del movil.





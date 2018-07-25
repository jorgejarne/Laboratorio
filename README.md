# Laboratorio de sistemas inteligentes sobre el internet de las cosas

Este repositorio recoge los programas del proyecto desarrollado en el laboratorio de sistemas del máster de internet de las cosas de la Universidad Complutense de Madrid. El proyecto se encuentra dividido en tres grandes bloques:
-	Lectura y escritura de tags usando un modulo NFC y el ESP8266.
-	Desarrollo de aplicación Android para comunicar un Smartphone y el módulo ESP8266 usando NFC.
-	Control de sensor conectado a ESP8266 usando el protocolo MQTT.

## Material utilizado

- PC ubuntu 16.04 LTS

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
  
Mosquitto 1.4.14

**IMPORTANTE: todos los programas que se cargan en el SoC ESP8266 envian mensajes por serial para que se pueda visualizar si todo esta funncionando correctamente. Para ver estos mensajes en el PC es necesario abrir el Arduino IDE y usar el monitor serie. El monitor tiene una ventana en el lado inferior derecho para selecionar los baudios de la comunicacion, este numero debera coincidir con el numero que se introduce en cada programa cuando se llama a la funcion "Serial.begin(_BAUDIOS_)" **

## Parte I - Lectura y escritura de tags usando un modulo NFC y el ESP8266

### Preparación hardware

La comunicacion entre el SoC ESP8266 y el modulo NFC se va a llevar a cabo usando el protocolo I2C. Para ello es necesario usar 4 cables que unan los puertos que apaceren en la siguiente tabla:


| ESP8266       | NFC v3        | 
| ------------- |:-------------:| 
| GND           | GND           | 
| 3V            | VCC           | 
| D7            | SDA           | 
| D6            | SCL           | 

Hay que tener en cuenta que no es obligatorio usar los puertos D6 y D7 del ESP8266, para usar otros puertos solo es necesario modificar la linea de codigo **Wire.begin(D7,D6)** que aparece tanto en los programas de [escribir](https://github.com/jorgejarne/Laboratorio/blob/master/nfcEsp8266Write.ino) como en el de [leer](https://github.com/jorgejarne/Laboratorio/blob/master/nfcEsp8266ReadPass.ino).

### Escritura

Este [codigo](https://github.com/jorgejarne/Laboratorio/blob/master/nfcEsp8266Write.ino) se encarga de escribir cualquier tipo de informacion en la memoria libre del tag que se acerca al modulo NFC. En este proyecto se ha decidido guardar las credenciales de una red WiFi pero se puede guardar otro tipo de informacion cambiando la linea 48 del codigo **message.addUriRecord("texto_a_grabar");**


### Lectura

Este [programa](https://github.com/jorgejarne/Laboratorio/blob/master/nfcEsp8266ReadPass.ino) realiza la lectura de toda información que contiene el tag y busca las credenciales de la red Wifi, una vez que las tiene intenta conectarse a dicha red. **El programa entra en bucle comprobando si se ha conectado o no a la red, en la mayoria de ocasiones este proceso dura varias iteracciones hasta que se conecta. Dependiendo de la version del SoC de ESP8266 que se tenga, podria tardar mas o menos en establecer la conexion con la red, por lo que seria necesario modificar la constante MAX_TRIES_CONNECTING que esta definida en el codigo** 

## Parte II - Desarrollo de aplicación Android para comunicar un Smartphone y el módulo ESP8266 usando NFC

El objetivo que tiene esta parte es realizar el intercambio de informacion entre un smartphone con NFC y el ESP8266 con su modulo NFC. El smartphone va a actuar como si fuera una tarjeta o tag, este modo de funcionamiento tambien es conocido como Host-based card emulation (HCE) y esta disponible desde la version Android 4.4. EL HCE puede tener diferentes servicios funcionando internamente por eso a cada uno de estos se le asigna un ApplicationID (AID), entonces cuando el otro dispositvo NFC se acerca, le envia el AID del servicio del HCE con el que quiere hablar. En este caso no ha sido necesario usar ningun AID concreto ya que solo se tiene un servicio funcionando en la app.

La [aplicacion](https://github.com/jorgejarne/Laboratorio/blob/master/appNFC.apk) de android se encuentra ya compilada en formato **apk** para instalarla directamente en el smartphone. Tambien se encuentra en el repositorio el [proyecto completo](https://github.com/jorgejarne/Laboratorio/blob/master/androidNfcHce.zip) de Android Studio por si se quiere añadir mas servicios o hacer otro tipo de modificaciones.

Tras instalar la aplicacion Android y ejecutarla, aparecen dos ventanas, la superior muestra que datos se han intercambiado y en la ventana inferior hay dos campos llamados **Nombre del wifi(SSID)** y **Contrasena del wifi(PASS)** que el usuario tiene que rellenar. La informacion introducida en estos campos es la que se enviara al modulo NFC cuando se acerque el movil a el.

El SoC ESP8266 debe tener cargado este [sketch](https://github.com/jorgejarne/Laboratorio/blob/master/lecturaHCE.ino). El programa esta a la espera de que se acerque algun dispositivo NFC para iniciar la conversacion enviandole el AID. Este valor se encuentra en la variable **uint8_t selectApdu[]** del codigo, sin embargo, no influye ya que se usaria solo en el caso de que se tuvieran diferentes AID en la app de Android desarrollada. Si el intercambio de informacion entre los dos dispositivos se ha realizado correctamente, el modulo ESP8266 intentara concectarse a la red con el nombre y contraseña que se escribieron en los campos de la app del movil.


## Parte III - Control de sensor conectado a ESP8266 usando el protocolo MQTT

Se va a controlar el led que tiene integrado el modulo ESP8266 usando el protocolo MQTT. Este protocolo esta compuesto por un *publisher* que sera en este caso un ordenador Ubuntu con moquitto instalado, el *subscriber*  que sera el propio ESP8266 y el *broker* la pagina web que sera *test.mosquitto.org*. El *topic* al que esta a la escucha el *subscriber* y por donde escribe el *publisher* se ha llamado "estadoLed". 

Para instalar el programa mosquitto en Ubunutu hay que abrir una terminal y escribir: **sudo apt-get install mosquitto mosquitto-clients**. Una vez instalado, en la misma terminal se puede introducir: **mosquitto_pub -h test.mosquitto.org -t estadoLed -m "1"**, el parametro que acompaña a *-m* indica el valor que se va a enviar, en este caso solo se usa el 0 y el 1 para apagar y encender, respectivamente, el led del ESP8266. Si se quiere usar un broker distinto hay que sustituir el argumento que va junto a *-h* por el host a usar.

Respecto al [programa](https://github.com/jorgejarne/Laboratorio/blob/master/mqttesp8266.ino) que se carga en el ESP8266, es necesario modificar las variables globales **ssid** y **password** por las credenciales de la red wifi a la que se vaya a conectar. Si se quiere usar otro broker, hay que cambiar tambien la variable **mqtt_server**. Otro parametro a tener en cuenta es el numero de puerto que se usa para conectarse al broker, lo habitual es usar el 1883, si su host usa otro debera cambiarlo en la parte del codigo donde se llama a la funcion **client.setServer(mqtt_server, 1883)**.

Si todo va correctamente, se puede ver como al ir publicando valores (0 o 1) desde la terminal de Ubuntu se va cambiando el estado del led.






#if 0
  #include <SPI.h>
  #include <PN532_SPI.h>
  #include "PN532.h"

  PN532_SPI pn532spi(SPI, 10);
  PN532 nfc(pn532spi);
#elif 0
  #include <PN532_HSU.h>
  #include <PN532.h>
      
  PN532_HSU pn532hsu(Serial1);
  PN532 nfc(pn532hsu);
#else 
  #include <Wire.h>
  #include <PN532_I2C.h>
  #include <PN532.h>
   
  PN532_I2C pn532_i2c(Wire);
  PN532 nfc(pn532_i2c);
#endif

  #include <ESP8266WiFi.h>
  char* ssid  = "mqtt_wifi";
  char* password = "XXXXXXXXXXXXXXXXXXXX";
  #define MAX_SIZE_PASS 20 
  #define MAX_TRIES_CONNECTING 30 

void setup()
{    
    
    Serial.begin(115200);
    Serial.println("-------Peer to Peer HCE--------");
    Wire.begin(D7,D6);
    nfc.begin();
    WiFi.disconnect();
    
    
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (! versiondata) {
      Serial.print("Didn't find PN53x board");
      while (1); // halt
    }
    
    // Got ok data, print it out!
    Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
    Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
    Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
    
    // Set the max number of retry attempts to read from a card
    // This prevents us from waiting forever for a card, which is
    // the default behaviour of the PN532.
    //nfc.setPassiveActivationRetries(0xFF);
    
    // configure board to read RFID tags
    nfc.SAMConfig();
}

void loop()
{
  bool success;
  
  uint8_t responseLength = 32;
  
  Serial.println("Waiting for an ISO14443A card");
  
  // set shield to inListPassiveTarget
  success = nfc.inListPassiveTarget();

  if(success) {
   
     Serial.println("Found something!");
                  
    uint8_t selectApdu[] = { 0x00, /* CLA */
                              0xA4, /* INS */
                              0x04, /* P1  */
                              0x00, /* P2  */
                              0x05, /* Length of AID  */
                              0xF2, 0x22, 0x22, 0x22, 0x22, //0x11, //0x11, /* AID defined on Android App */
                              0x00  /* Le  */ };
                              
    uint8_t response[32];  
     
    success = nfc.inDataExchange(selectApdu, sizeof(selectApdu), response, &responseLength);
    
    if(success) {
      
      Serial.print("responseLength: "); Serial.println(responseLength);
       
      nfc.PrintHexChar(response, responseLength);
      
     // do {
        
        //uint8_t apdu[] = "Hello World";
        uint8_t apdu[] = "[B@930a8fd";
        uint8_t back[32];
        uint8_t length = 32; 

        success = nfc.inDataExchange(apdu, sizeof(apdu), back, &length);
        
        if(success) {
         
          Serial.print("responseLength: "); Serial.println(length);
           
          //nfc.PrintHexChar(back, length);
          //imprimir_hex(back, length);
          String prueba=getString(back,100,40);
          //Serial.println(prueba);
          //Serial.println("splitenado texto...");
          getAuth(prueba,'|');
          Serial.println("auth: ");
          Serial.println(ssid);
          Serial.println(password);
          WiFi.begin(ssid, password);
          int tries=0;
          while ((WiFi.status() != WL_CONNECTED) && (tries<MAX_TRIES_CONNECTING)) {
            delay(500);
            Serial.print(".");
            tries=tries+1;
          }
          if(tries< MAX_TRIES_CONNECTING){
          
            Serial.println("Contraseña correcta --> WiFi conectado");  
            Serial.print("IP --> ");
            Serial.println(WiFi.localIP()); 
          }
        else{
       
          Serial.println("Contraseña incorrecta");  
          
          }
        }
        else {
          
          Serial.println("Broken connection?"); 
        }
      }
     // while(success);
      
    
    else {
     
      Serial.println("Failed sending SELECT AID"); 
    }
  }
  else {
   
    Serial.println("Didn't find anything!");
  }

  delay(1000);
}

void printResponse(uint8_t *response, uint8_t responseLength) {
  
   String respBuffer;

    for (int i = 0; i < responseLength; i++) {
      
      if (response[i] < 0x10) 
        respBuffer = respBuffer + "0"; //Adds leading zeros if hex value is smaller than 0x10
      
      respBuffer = respBuffer + String(response[i], HEX) + " ";                        
    }

    Serial.print("response: "); Serial.println(respBuffer);
}

void setupNFC() {
 
  nfc.begin();
    
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // configure board to read RFID tags
  nfc.SAMConfig(); 
}

String getString(const byte * data,const long numBytes,int characters)
{
  uint32_t szPos;
  int cont=0;
  String texto="";
  for (szPos=0; szPos < numBytes; szPos++)
  {
    if (data[szPos] > 0x1F && cont<characters){
     cont=cont+1;
     texto+=(char)data[szPos];  
    } 
  }
  return texto;
}

void imprimir_hex(const byte * data, const long numBytes)
{
  uint32_t szPos;
  for (szPos=0; szPos < numBytes; szPos++)
  {
    // Append leading 0 for small values
    if (data[szPos] <= 0xF)
      Serial.print("0");
    Serial.print(data[szPos], HEX);
    if ((numBytes > 1) && (szPos != numBytes - 1))
    {
      Serial.print(" ");
    }
  }
  Serial.print("  ");
  for (szPos=0; szPos < numBytes; szPos++)
  {
    if (data[szPos] <= 0x1F)
      Serial.print(".");
    else
      Serial.print((char)data[szPos]);
  }
  Serial.println("");
}

String split(String data, char delimiter) 
{
  uint32_t szPos=0;
  String texto="";
  while (data[szPos]!=delimiter){szPos++;}
  szPos++;
  while (data[szPos]!=' '){
     texto+=(char)data[szPos];
     szPos++;
    }
  return texto;
  }

  void getAuth(String data, char delimiter) 
{
  uint32_t szPos=0;
  String texto="";
   //Serial.println("entrando en get AUTH");
  while (data[szPos]!=delimiter){
    
    texto+=(char)data[szPos];
    szPos++;
    }
  szPos++;
  texto.toCharArray(ssid,25);
  //Serial.print(texto);
  //Serial.print(ssid);
  //º
  //ssid=(char)texto;
  texto="";

  //while (data[szPos]!= ' ')
  while (isalpha(data[szPos]) | isdigit(data[szPos]) ){
     texto+=(char)data[szPos];
     szPos++;
  }
  texto.toCharArray(password,25);  
  Serial.println(password);
  }


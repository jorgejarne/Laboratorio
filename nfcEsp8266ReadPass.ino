/*********************************************************************************************************************************************************
 *                                                                                                                                                      *
 *                               CODIGO CREADO POR JORGE JARNE                                                                                                                       *
 *                                                                                                                                                      *
 *                                                                                                                                                      *
 *********************************************************************************************************************************************************

*/
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <ESP8266WiFi.h>
//NfcAdapter nfc;
//TwoWire cables();
//PN532_I2C pn532_i2c(&cables);
// nfc = NfcAdapter(pn532_i2c);
#define MAX_SIZE_PASS 20 
#define MAX_TRIES_CONNECTING 30 


 unsigned int cont=0;
 unsigned char lectura;
 const char* ssid  = "mqtt_wifi";
 char* password = "XXXXXXXXXXXXXXXXXXXX";

void setup(void) {
   Serial.begin(9600);
   Serial.println("NDEF Reader");
   Serial.println();
   WiFi.disconnect();
}

void loop(void) {
 
   Wire.begin(D7,D6);
   PN532_I2C pn532_i2c(Wire);
   NfcAdapter nfc = NfcAdapter(pn532_i2c);
   nfc.begin();
   delay(100);
   //NdefRecord record();
   while(1){
     while (!nfc.tagPresent()) {}
        Serial.println("Tarjeta NFC encontrada....");
        Serial.println("Leyendo memoria....");
        NfcTag tag = nfc.read();
        NdefMessage text=tag.getNdefMessage();
        NdefRecord record=text.getRecord(0);
        record.print();
        int id=record.getEncodedSize();
        record.getPayload(&lectura);
        Serial.println("imprimiendo en hexadicmal...");
        //PrintHexChar(&lectura,10);  
        imprimir_hex(&lectura,100);
        Serial.println("cogiendo String texto...");
        String prueba=getString(&lectura,100,40);
        Serial.println(prueba);
        Serial.println("splitenado texto...");
        String pass=split(prueba,'=');
        Serial.println(pass);
        delay(500);   
        pass.toCharArray(password,25);
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
  }

/*
void imprimir_hex(const byte * data, const long numBytes)
{
  uint32_t szPos;
  for (szPos=0; szPos < numBytes; szPos++)
  {
    if (data[szPos] <= 0x1F)
      Serial.print(".");
    else
      Serial.print((char)data[szPos]);
  }
  Serial.println("");
}

String split(const byte * data,const long numBytes, char delimiter,int characters)
{
  uint32_t szPos;
  String texto="";
  for (szPos=0; szPos < numBytes; szPos++)
  {
    if (data[szPos] > 0x1F && data[szPos]==delimiter )
     
      texto+=(char)data[szPos];
      for(int i=szPos;i<(characters+szPos);i++){
         texto+=(char)data[i];
        }
      return texto;
      
  }
}*/
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



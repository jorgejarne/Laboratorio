
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

//NfcAdapter nfc;
//TwoWire cables();
//PN532_I2C pn532_i2c(&cables);
// nfc = NfcAdapter(pn532_i2c);
bool first=true; 


void setup(void) {
  //Wire.begin(D7,D6);
  
   
   Serial.begin(9600);
   Serial.println("NDEF Reader");
   Serial.println();
   Serial.println("prueba.......");
  
  
}

void loop(void) {
 //if(first){ //pensar en cambiarlo y definirlo una vez arriba y en el "setup" redefinirlos
   first=false;
   Wire.begin(D7,D6);
   PN532_I2C pn532_i2c(Wire);
   NfcAdapter nfc = NfcAdapter(pn532_i2c);
   nfc.begin();
   delay(100);
 
   while (!nfc.tagPresent()) {}
        NdefMessage message = NdefMessage();
        message.addUriRecord(" CONTRASENA=wifi_pass ");

        bool success = nfc.write(message);
        if (success) {
          Serial.println("Success. Try reading this tag with your phone.");        
        } else {
          Serial.println("Write failed.");
        }
    }
//}


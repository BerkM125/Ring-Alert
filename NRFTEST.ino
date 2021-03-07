//nrf2401 transmitter:
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define PINBASE 30
#define BUZZERPIN 10
RF24 nrf(9, 8);  // CE, CSN
const byte linkAddress[6] = "link1";

float val1;   
float val2;  
float val3; 
float data[3];
char charVal[17];
////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  Serial.println("BMP280/NRF24L01 link");
  nrf.begin();   
  nrf.openWritingPipe(linkAddress);  //set the address 
  //nrf.setPALevel(RF24_PA_LOW);   //keep tx level low to stop psu noise, can remove this but then put in decoupling caps
  // Options are: RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm,  RF24_PA_HIGH=0dBm.
  nrf.setPALevel(RF24_PA_LOW); 
  nrf.stopListening();  //act as a transmitter
}
///////////////////////////////////////////////////
void loop()
{  
 val1 = 1;
 val2 = 2;
 val3 = 3;
  
 data[0] = val1;
 data[1] = val2;
 data[2] = val3;
 if(digitalRead(7) == HIGH) {
    Serial.println("HIGH!");
    tone(BUZZERPIN, 262, 250);
    nrf.write(data, sizeof(data)); //spit out the data 
    delay(250*1.3);
    noTone(BUZZERPIN);
 }
 //----- display on local monitor: ------------
 Serial.print(data[0]); Serial.print("  ");
 Serial.print(data[1]); Serial.print("  ");
 Serial.print(data[2]); Serial.print("  ");
 //---------------------------------------------
 delay(300);
}
/////////////////////////////////////////////////

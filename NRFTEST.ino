/*Copyright Berkan Mertan 2021*/
//nrf2401 transmitter:
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//Buzzer/speaker pin and transmission button pin. Can be adjusted for different designs.
#define BUZZERPIN 10
#define BUTTONPIN 7
RF24 nrf(9, 8);  // CE, CSN pins for transmitter. Can be adjusted for different designs.
const byte linkAddress[6] = "link1"; //linkAddress buffer for specifying the pairing channel
//Sample data value holders and buffers
float val1;   
float val2;  
float val3; 
float data[3];
char charVal[17];
////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  Serial.println("BMP280/NRF24L01 link");
  nrf.begin();   
  nrf.openWritingPipe(linkAddress);  //set the address 
  //nrf.setPALevel(RF24_PA_LOW);   //keep tx level low to stop psu noise, can remove this but then put in decoupling caps
  // Options are: RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm,  RF24_PA_HIGH=0dBm.
  nrf.setPALevel(RF24_PA_LOW);  //-12dB, optimal for lower power consumption. To increase range, 0dBm or RF24_PA_HIGH recommended
  nrf.stopListening();  //act as a transmitter
}
///////////////////////////////////////////////////
void loop() {
 //Sample data buffer values, here to be changed to whatever data you want to transmit
 val1 = 1;
 val2 = 2;
 val3 = 3;
  
 data[0] = val1;
 data[1] = val2;
 data[2] = val3;
 //Check for current reading on pin 7
 if(digitalRead(BUTTONPIN) == HIGH) {
    Serial.println("HIGH!"); //For debugging
    tone(BUZZERPIN, 262, 250); //Just sample frequency and tone length
    nrf.write(data, sizeof(data)); //spit out the data 
    delay(250*1.3); //For tone
    noTone(BUZZERPIN); //For tone
 }
 //----- display on local monitor: ------------
 Serial.print(data[0]); Serial.print("  ");
 Serial.print(data[1]); Serial.print("  ");
 Serial.print(data[2]); Serial.print("  ");
 //---------------------------------------------
 delay(300); //Recommended delay time for checking pushbutton current state
}
/////////////////////////////////////////////////

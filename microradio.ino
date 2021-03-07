/*Copyright Berkan Mertan 2021*/
//receiver/paired device code
#include <nRF24L01.h>
#include <RF24.h>
#include "pitches.h"
#define BUZZERPIN 10
RF24 nrf(9, 8);  // CE, CSN pins by default. Can be changed for adjusted design.
const byte linkAddress[6] = "link1"; //Channel name within the byte-sized buffer can be changed but must be the same as 
// notes in the ringtone melody:
int melody[] = {

  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {

  4, 8, 8, 4, 4, 4, 4, 4
};
//Transmitted data buffer
float data[3];
void setup() {
  Serial.begin(9600);
  //Set MISO nrf pin as an output, becomes useful for different designs with different MISO pinout
  pinMode(MISO, OUTPUT);
  //Play the ringtone on startup, the loop can be adjusted to be the length of the melody buffer
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZERPIN, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZERPIN);
  }
  //Begin nrf reciever
  nrf.begin();    
  nrf.openReadingPipe(0, linkAddress);  //set the address for pairing and listening
  nrf.setPALevel(RF24_PA_LOW); //recommended for low power consumption, but for increased range RF24_PA_HIGH is more recommended
  nrf.startListening();   //Set nrf as receiver
}

void loop() {
   //Check for nrf transmission or signal
   if (nrf.available()) {
     //Check data and make sure it isn't a high reading of current going through the NRF24 module
     nrf.read(&data, sizeof(data));
     Serial.println(data[0]); //Debugging. Also for this one it only checks the first part of the data array because that's all the data being transmitted.
     if(data[0] == 1) {
       //Play melody, should be the same as the one in setup() loop
       for (int thisNote = 0; thisNote < 8; thisNote++) {
          int noteDuration = 1000 / noteDurations[thisNote];
          tone(BUZZERPIN, melody[thisNote], noteDuration);
          int pauseBetweenNotes = noteDuration * 1.30;
          delay(pauseBetweenNotes);
          noTone(BUZZERPIN);
        }
     }
   }
  //No delay or wait time to prevent a missed transmission of some kind, but having one could be better for efficiency 
}

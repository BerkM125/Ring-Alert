#include <nRF24L01.h>
#include <RF24.h>
#include "pitches.h"
#define BUZZERPIN 10
RF24 nrf(9, 8);  // CE, CSN
const byte linkAddress[6] = "link1";
// notes in the melody:
int melody[] = {

  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {

  4, 8, 8, 4, 4, 4, 4, 4
};
float data[3];
void setup() {
  Serial.begin(9600);
  pinMode(MISO,OUTPUT);
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZERPIN, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZERPIN);
  }
  nrf.begin();    
  nrf.openReadingPipe(0, linkAddress);  //set the address 
  nrf.setPALevel(RF24_PA_LOW);
  nrf.startListening();   //Set nrf as receiver
}

void loop() {
   if (nrf.available()) {
     nrf.read(&data, sizeof(data));
     Serial.println(data[0]);
     if(data[0] == 1) {
       for (int thisNote = 0; thisNote < 8; thisNote++) {
          int noteDuration = 1000 / noteDurations[thisNote];
          tone(BUZZERPIN, melody[thisNote], noteDuration);
          int pauseBetweenNotes = noteDuration * 1.30;
          delay(pauseBetweenNotes);
          noTone(BUZZERPIN);
        }
     }
   }
}

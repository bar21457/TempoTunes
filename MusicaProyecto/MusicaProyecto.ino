#include <stdint.h>

#include "pitches.h"

//Bandera para saber si ya inició el juego o no
int x = 0;

//Notas del Rick Roll
int RickRoll[] = {
  NOTE_C5, NOTE_D5, NOTE_G4, NOTE_D5, NOTE_E5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_G4, 0,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_C5, 0, NOTE_C5,
  NOTE_C5, NOTE_D5, NOTE_G4, NOTE_D5, NOTE_E5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_G4, NOTE_B4, NOTE_C5, 0, NOTE_C5, 0, NOTE_C5, NOTE_C5, 0, NOTE_C5, 0,
  NOTE_A4, 0, NOTE_B4, 0, NOTE_C4, 0, NOTE_C4, 0, NOTE_D4, 0, NOTE_B4, 0, NOTE_A4, NOTE_G4, 0,
  NOTE_A4, 0, NOTE_A4, 0, NOTE_B4, 0, NOTE_C5, 0, NOTE_A4, 0, NOTE_G4, 0, NOTE_G5, 0, NOTE_G5, NOTE_D5, 0,
  NOTE_A4, 0, NOTE_A4, 0, NOTE_B4, 0, NOTE_C5, 0, NOTE_A4, 0, NOTE_C5, 0, NOTE_D5, 0, NOTE_B4, 0, NOTE_A4, 0, NOTE_G4, 0,
  NOTE_A4, 0, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_A4, NOTE_G4, 0, NOTE_D5, 0, NOTE_D5, 0, NOTE_D5, 0, NOTE_E5, 0, NOTE_D5, 0,
  NOTE_C5, 0, NOTE_D5, 0, NOTE_E5, 0, NOTE_C5, 0, NOTE_D5, 0, NOTE_D5, 0, NOTE_D5, 0, NOTE_E5, 0, NOTE_D5, NOTE_G4, 0,
  NOTE_A4, 0, NOTE_B4, 0, NOTE_C5, 0, NOTE_A4, 0, NOTE_D5, 0, NOTE_E5, 0, NOTE_D5, 0,
  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, NOTE_D5, 0,
  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_D5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4, 0,
  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_D5, NOTE_B4, NOTE_A4, NOTE_G4, 0, NOTE_G4, NOTE_D5, NOTE_C5, 0,
  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, NOTE_D5, 0,
  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_G5, NOTE_B4, NOTE_C5, NOTE_B4, NOTE_A4, 0,
  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_D5, NOTE_B4, NOTE_A4, NOTE_G4, 0, NOTE_G4, NOTE_D5, NOTE_C5, 0
};

//Notas del Pokemon
int Pokemon[] = {
  NOTE_C3, NOTE_C3, NOTE_E4, NOTE_G3,
  NOTE_C3, NOTE_C3, NOTE_F4, NOTE_A3,
  NOTE_C3, NOTE_C3, NOTE_G4, NOTE_B3,
  NOTE_C3, NOTE_C3, NOTE_F4, NOTE_A3,
  NOTE_G5, NOTE_F5, NOTE_E5, NOTE_F5, NOTE_E5, NOTE_C5, NOTE_G4,
  NOTE_A4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_G5,
  NOTE_A5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6, NOTE_G5,
  NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A5,
  NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5,
  NOTE_E5, NOTE_C5, NOTE_E5, NOTE_C5, NOTE_E5, NOTE_C5, NOTE_E5, NOTE_C5,
  NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_G4, NOTE_D5,
  NOTE_E5, NOTE_C5, NOTE_E5, NOTE_C5, NOTE_E5, NOTE_C5, NOTE_E5, NOTE_C5,
  NOTE_G4, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_G4, NOTE_C5,
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5,
  NOTE_C6, NOTE_B5, NOTE_A5, NOTE_G5, NOTE_E5, NOTE_D5, NOTE_E5, NOTE_C5,
  NOTE_A5, NOTE_G5, NOTE_C6, NOTE_B5, NOTE_A5, NOTE_G5, NOTE_E5, NOTE_C5, NOTE_G4, NOTE_D5, NOTE_C5,
  NOTE_C3, NOTE_C3, NOTE_E4, NOTE_G3,
  NOTE_C3, NOTE_C3, NOTE_F4, NOTE_A3,
  NOTE_C3, NOTE_C3, NOTE_G4, NOTE_B3,
  NOTE_C3, NOTE_C3, NOTE_F4, NOTE_A3
};

//Duración de las notas del Rick Roll
int RickRollDuraciones[] = {
  2, 2, 3, 2, 2, 7, 7, 7, 2, 2, 1, 2,
  7, 7, 7, 7, 7, 7,
  2, 2, 3, 2, 2, 7, 7, 7, 2, 2, 2, 4, 7, 7, 2, 7, 7, 7, 7, 7, 2,
  6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 3, 7, 6, 1, 1,
  7, 7, 7, 7, 7, 7, 7, 7, 4, 7, 7, 7, 5, 5, 5, 1, 1,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 7, 7, 7, 7, 1, 2,
  7, 7, 4, 4, 4, 4, 4, 4, 7, 7, 7, 7, 7, 7, 7, 7, 3, 4,
  1, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 3, 3, 2,
  7, 7, 7, 7, 7, 7, 7, 4, 7, 7, 7, 7, 5, 2,
  8, 8, 8, 8, 4, 4, 4, 4, 
  8, 8, 8, 8, 4, 4, 4, 8, 8, 4,
  8, 8, 8, 8, 2, 4, 3, 6, 6, 8, 4, 2, 1, 4,
  8, 8, 8, 8, 4, 4, 4, 4,
  8, 8, 8, 8, 3, 4, 4, 8, 8, 4,
  8, 8, 8, 8, 2, 4, 3, 6, 6, 8, 4, 2, 1, 4
};

//Duración de las notas del Pokemon
int PokemonDuraciones[] = {
  5, 5, 5, 5,
  5, 5, 5, 5,
  5, 5, 5, 5,
  5, 5, 5, 5,
  5, 5, 5, 5, 5, 5, 3,
  5, 5, 5, 5, 5, 2,
  5, 5, 5, 5, 5, 2,
  5, 5, 5, 5, 3,
  8, 8, 8, 8,
  5, 5, 5, 5, 5, 5, 5, 5,
  5, 5, 5, 5, 5, 5, 5, 5,
  5, 5, 5, 5, 5, 5, 5, 5,
  5, 5, 5, 5, 5, 5,
  7, 7, 7, 7, 2,
  2, 3, 5, 5, 5, 5, 5, 3,
  5, 2, 2, 3, 5, 5, 5, 5, 5, 5, 3
};

void setup() {

  pinMode(PF_0, INPUT_PULLUP);          //Pin que recibe si ya se cambia la canción
  
}

void loop() {

    for (int thisNote = 0; (thisNote < 207) && (x == 0) ; thisNote++)     //Recorrer las listas de notas y duración de las notas para reproducir el Rick Roll
    { 
      int noteDuration = 1000 / RickRollDuraciones[thisNote];
      tone(PD_7, RickRoll[thisNote], noteDuration);
      
      int pauseBetweenNotes = noteDuration * 1.25;
      delay(pauseBetweenNotes);
      
      noTone(PD_7);
  
      int ButtonState1 = digitalRead(PF_0);
      if (ButtonState1 == LOW) 
      {
         x = 1;
      }
    }
  
    for (int thisNote = 0; (thisNote < 98) && (x == 1) ; thisNote++)      //Recorrer las listas de notas y duración de las notas para reproducir la canción de Pokemon
    {
  
      int noteDuration = 1000 / PokemonDuraciones[thisNote];
      tone(PD_7, Pokemon[thisNote], noteDuration);
  
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
  
      noTone(PD_7);
    }
}

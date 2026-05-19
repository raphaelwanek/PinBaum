#include <Arduino.h>
#include "Variablen.h"
#include "ZylinderControlService.h"
#include "ButtonListener.h"
#include "SerialCommunicationService.h"
#include <Servo.h>

ButtonListener button(I1);
ZylinderControlService zylinder(O4, O5);
SerialCommunicationService serialComm;

int Time_Pos2 = 4616;
int Time_Pos3 = 5340;
int Time_Pos3_ende = 8800;


enum Schritte {
  INITIAL,
  SCHRITT1,
  SCHRITT2,
  SCHRITT3,
  SCHRITT4,
  SCHRITT5,
  SCHRITT6,
  SCHRITT7,
  SCHRITT8,
  SCHRITT9,
  SCHRITT10,
  SCHRITT11,
  SCHRITT12,
  SCHRITT13,
  SCHRITT14,
  SCHRITT15,
  SCHRITT16,
  SCHRITT17
};

Schritte currentStep = INITIAL;

unsigned long Timer0 = 0;
unsigned long Timer1 = 0;
unsigned long Timer2 = 0;
unsigned long Timer3 = 0;
unsigned long Timer4 = 0;
unsigned long Timer5 = 0;

bool emergencyStopActive = false;
bool isPaused = false;
unsigned long pauseStart = 0;
unsigned long totalPausedTime = 0;

bool isbuttonPressed() { return (digitalRead(I1) == LOW); };
bool isEndlageActive() { return (digitalRead(I2) == HIGH); };



void setup() {
  
  
  pinMode(I1, INPUT_PULLUP);
  pinMode(I2, INPUT_PULLUP);
  pinMode(I3, INPUT_PULLUP);
  pinMode(O1, OUTPUT);
  pinMode(O2, OUTPUT);
  pinMode(O3, OUTPUT);
  pinMode(O4, OUTPUT);
  pinMode(O5, OUTPUT);
Serial.begin(9600);
Variablen_init();
zylinder.Init();
zylinder.Retract();
}

void loop() {
  zylinder.Update();

  while(!isbuttonPressed() && currentStep == INITIAL) {
    Serial.println("Warte auf Knopfdruck...");
    zylinder.Update();
  }


  digitalWrite(O2, HIGH); // Bremsen deaktivieren
  Serial.println(zylinder.getPosition());
  if(currentStep == INITIAL && zylinder.getPosition() != RETRACTED) {
    Serial.println("Initialschritt: Zylinder einfahen");
    zylinder.Retract();
  }
  else if(currentStep == INITIAL && zylinder.getPosition() == RETRACTED) {
    Serial.println("Initialschritt: Zylinder eingefahren");
    currentStep = SCHRITT1;
  }
  Serial.println(zylinder.getPosition());
  


  if(currentStep == SCHRITT1 && !isEndlageActive()) {
    Serial.println("Schritt 1: Fahre Motor in Endlage");
    digitalWrite(O3, LOW); // Richtung vorwärts
    analogWrite(O1, 30);   // Geschwindigkeit max
  }else if (currentStep == SCHRITT1 && isEndlageActive()) {
    Serial.println("Schritt 1: Endlage erreicht, Motor anhalten");
    analogWrite(O1, 0);    // Geschwindigkeit 0
    currentStep = SCHRITT2;
    Timer0 = millis();
  }


  if(currentStep == SCHRITT2 && millis() - Timer0 < Time_Pos2  ) {
    Serial.println("Schritt 2: Motor zu Pos 2 fahren");
    digitalWrite(O3, HIGH); // Richtung rückwärts
    analogWrite(O1, 69);   // Geschwindigkeit mittel
  }else if (currentStep == SCHRITT2 && millis() - Timer0 >= Time_Pos2) {
    Serial.println("Schritt 2: Pos 2 erreicht, Motor anhalten");
    analogWrite(O1, 0);    // Geschwindigkeit 0
    currentStep = SCHRITT3;
  }


  if(currentStep == SCHRITT3 && zylinder.getPosition() != EXTENDED) {
    Serial.println("Schritt 3: Zylinder ausfahren");
    zylinder.setUseLongMoveDuration(false);
    zylinder.Expand();
  }else if (currentStep == SCHRITT3 && zylinder.getPosition() == EXTENDED) {
    Serial.println("Schritt 3: Zylinder ausgefahren");
    currentStep = SCHRITT4;
  }


  if(currentStep == SCHRITT4 && !isEndlageActive()) {
    Serial.println("Schritt 4: Fahre Motor in Endlage");
    digitalWrite(O3, LOW); // Richtung vorwärts
    analogWrite(O1, 40);   // Geschwindigkeit max
  } else if (currentStep == SCHRITT4 && isEndlageActive()) {
    Serial.println("Schritt 4: Endlage erreicht, Motor anhalten");
    digitalWrite(O3, HIGH); // Richtung vorwärts
    analogWrite(O1, 20);   // Geschwindigkeit mittel
    delay(5);
    analogWrite(O1, 0);    // Geschwindigkeit 0
    currentStep = SCHRITT5;
  }


  if(currentStep == SCHRITT5 && zylinder.getPosition() != RETRACTED) {
    Serial.println("Schritt 5: Zylinder einfahren");
    zylinder.setUseLongMoveDuration(false);
    zylinder.Retract();
  }else if (currentStep == SCHRITT5 && zylinder.getPosition() == RETRACTED) {
    Serial.println("Schritt 5: Zylinder eingefahren");
    currentStep = SCHRITT6;
    Timer1 = millis();
  }


  if(currentStep == SCHRITT6 && millis() - Timer1 < Time_Pos3) {
    Serial.println("Schritt 6: Motor zu Pos 3 fahren");
    digitalWrite(O3, HIGH); // Richtung rückwärts
    analogWrite(O1, 80);   // Geschwindigkeit mittel
  }else if( currentStep == SCHRITT6 && millis() - Timer1 >= Time_Pos3) {
    Serial.println("Schritt 6: Pos 3 erreicht, Motor anhalten");
    analogWrite(O1, 0);    // Geschwindigkeit 0
    currentStep = SCHRITT7;
  }


  if(currentStep == SCHRITT7 && zylinder.getPosition() != EXTENDED) {
    Serial.println("Schritt 7: Zylinder ausfahren");
    zylinder.setUseLongMoveDuration(true);
    zylinder.Expand();
  }else if (currentStep == SCHRITT7 && zylinder.getPosition() == EXTENDED) {
    Serial.println("Schritt 7: Zylinder ausgefahren");
    currentStep = SCHRITT8;
    Timer2 = millis();
  }


  if(currentStep == SCHRITT8 && millis() - Timer2 < Time_Pos3_ende) {
    Serial.println("Schritt 8: Fahre Motor in Endlage");
    digitalWrite(O3, LOW); // Richtung vorwärts
    analogWrite(O1, 40);   // Geschwindigkeit max
  } else if (currentStep == SCHRITT8 && millis() - Timer2 >= Time_Pos3) {
    Serial.println("Schritt 8: Endlage erreicht, Motor anhalten");
    digitalWrite(O3, HIGH); // Richtung vorwärts
    analogWrite(O1, 41);   // Geschwindigkeit mittel
    delay(3);
    analogWrite(O1, 0);    // Geschwindigkeit 0
    Serial.println("Automatikvorgang abgeschlossen.");
    currentStep = SCHRITT9;
  }


  if(currentStep == SCHRITT9 && zylinder.getPosition() != RETRACTED) {
    Serial.println("Schritt 9: Zylinder einfahren");
    zylinder.setUseLongMoveDuration(true);
    zylinder.Retract();
  }else if (currentStep == SCHRITT9 && zylinder.getPosition() == RETRACTED) {
    Serial.println("Schritt 9: Zylinder eingefahren");
    currentStep = SCHRITT10;
  }


  if(currentStep == SCHRITT10 && !isEndlageActive()) {
    Serial.println("Schritt 10: Fahre Motor in Endlage");
    digitalWrite(O3, LOW); // Richtung vorwärts
    analogWrite(O1, 80);   // Geschwindigkeit max
  } else if (currentStep == SCHRITT10 && isEndlageActive()) {
    Serial.println("Schritt 10: Endlage erreicht, Motor anhalten");
    analogWrite(O1, 0);    // Geschwindigkeit 0
    Serial.println("Automatikvorgang komplett abgeschlossen.");
    currentStep = SCHRITT11;
    Timer3 = millis();
  }


  if(currentStep == SCHRITT11 && millis() - Timer3 < 3810) {
    Serial.println("Schritt 11: Fahre zurück zu Pos 2");
    digitalWrite(O3, HIGH); //Richtung rückwärts
    analogWrite(O1, 30);   // Geschwindigkeit mittel
  }
  else if (currentStep == SCHRITT11 && millis() - Timer3 >= 3810) {
    Serial.println("Schritt 11: Pos 2 erreicht, Motor anhalten");
    analogWrite(O1, 0);    // Geschwindigkeit 0
    currentStep = SCHRITT12;
  }


  if(currentStep == SCHRITT12 && zylinder.getPosition() != EXTENDED) {
    Serial.println("Schritt 12: Zylinder ausfahren");
    zylinder.setUseLongMoveDuration(true);
    zylinder.Expand();
  }else if (currentStep == SCHRITT12 && zylinder.getPosition() == EXTENDED) {
    Serial.println("Schritt 12: Zylinder ausgefahren");
    currentStep = SCHRITT13;
    Timer4 = millis();
  }


  if(currentStep == SCHRITT13 && millis() - Timer4 < 12000) {
    Serial.println("Schritt 13: Fahre zurück zu Endlage");
    digitalWrite(O3, HIGH); //Richtung rückwärts
    analogWrite(O1, 30);   // Geschwindigkeit mittel
  }
  else if (currentStep == SCHRITT13 && millis() - Timer4 >= 12000) {
    Serial.println("Schritt 13: Endlage erreicht, Motor anhalten");
    digitalWrite(O3, HIGH); // Richtung vorwärts
    analogWrite(O1, 41);   // Geschwindigkeit mittel
    delay(4);
    analogWrite(O1, 0);    // Geschwindigkeit 0
    currentStep = SCHRITT14;
  }


  if (currentStep == SCHRITT14 && zylinder.getPosition() != RETRACTED) {
    Serial.println("Schritt 14: Zylinder einfahren");
    zylinder.setUseLongMoveDuration(true);
    zylinder.Retract();
  } else if (currentStep == SCHRITT14 && zylinder.getPosition() == RETRACTED) {
    Serial.println("Schritt 14: Zylinder eingefahren");
    currentStep = SCHRITT15;
  }


  if(currentStep == SCHRITT15 && !isEndlageActive()) {
    Serial.println("Schritt 15: Fahre Motor in Endlage");
    digitalWrite(O3, LOW); // Richtung vorwärts
    analogWrite(O1, 30);   // Geschwindigkeit max
  } else if (currentStep == SCHRITT15 && isEndlageActive()) {
    Serial.println("Schritt 15: Endlage erreicht, Motor anhalten");
    analogWrite(O1, 0);    // Geschwindigkeit 0
    currentStep = SCHRITT16;
  }


  if(currentStep == SCHRITT16 && zylinder.getPosition() != EXTENDED) {
    Serial.println("Schritt 16: Zylinder ausfahren");
    zylinder.setUseLongMoveDuration(false);
    zylinder.Expand();
  } else if (currentStep == SCHRITT16 && zylinder.getPosition() == EXTENDED) {
    Serial.println("Schritt 16: Zylinder ausgefahren");
    currentStep = SCHRITT17;
    Timer5 = millis();
  }

  
  if(currentStep == SCHRITT17 && millis() - Timer5 < 11566) {
    Serial.println("Schritt 17: Motor zu Pos 2 fahren");
    digitalWrite(O3, HIGH); // Richtung rückwärts
    analogWrite(O1, 30);   // Geschwindigkeit mittel
  }else if (currentStep == SCHRITT17 && millis() - Timer5 >= 11566) {
    Serial.println("Schritt 17: Pos 2 erreicht, Motor anhalten");
    digitalWrite(O3, LOW); // Richtung vorwärts
    analogWrite(O1, 40);   // Geschwindigkeit mittel
    delay(3);
    analogWrite(O1, 0);  // Geschwindigkeit 0
    zylinder.setUseLongMoveDuration(false);
    zylinder.Retract();
    currentStep = INITIAL;
  }
}



  
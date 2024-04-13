

// steve's classes
#include "StevesAwesomeButton.h"
#include "StevesAwesomePotentiometer.h"
#include "StevesAwesomeRotaryEncoder.h"
#include "Stepper.h"
//other libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
//Adafruit_NeoPixel neopixel = Adafruit_NeoPixel(8, 32, NEO_RGB);





// automatic control based on time
Stepper stepper(20, 250);            //stepper for smoothing the fader when we are pushing it, we call lock() to lock the pot value every 0.25 second
bool somethingJustDisplayed = true;  // a boolean for screen graph, once a displayReading() is called, it will return true
// pots
int potPins[18] = { A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17 };
int potVals[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int potChannels[18] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };
int *pots = potVals;
int lastPotVals[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

StevesAwesomePotentiometer pot0(A0, 1);
StevesAwesomePotentiometer pot1(A1, 2);
StevesAwesomePotentiometer pot2(A2, 3);
StevesAwesomePotentiometer pot3(A3, 4);
StevesAwesomePotentiometer pot4(A4, 5);
StevesAwesomePotentiometer pot5(A5, 6);
StevesAwesomePotentiometer pot6(A6, 7);
StevesAwesomePotentiometer pot7(A7, 8);
StevesAwesomePotentiometer pot8(A8, 9);
StevesAwesomePotentiometer pot9(A9, 10);
StevesAwesomePotentiometer pot10(A10, 11);
StevesAwesomePotentiometer pot11(A11, 12);
StevesAwesomePotentiometer pot12(A12, 13);
StevesAwesomePotentiometer pot13(A13, 14);
StevesAwesomePotentiometer pot14(A14, 15);
StevesAwesomePotentiometer pot15(A15, 16);
StevesAwesomePotentiometer pot16(A16, 17);
StevesAwesomePotentiometer pot17(A17, 18);
StevesAwesomePotentiometer *potObjects[18] = { &pot0, &pot1, &pot2, &pot3, &pot4, &pot5, &pot6, &pot7, &pot8, &pot9, &pot10, &pot11, &pot12, &pot13, &pot14, &pot15, &pot16, &pot17 };


void setup() {

  //Serial
  Serial.begin(9600);
  // Serial.println("Serial.begin()");
  stepper.stepHandler(onStep);
  stepper.maxStepHandler(onMaxStep);  



  //pots/faders
  for (int i = 0; i < 18; i++) {
    potObjects[i]->turnHandler(potTurn);
    potObjects[i]->lock();
    lastPotVals[i] = potVals[i];
  }

  Serial.println("---system now online---");
}

void loop() {
  updatePots();
  stepper.process();
  checkButtons();
}

void updatePots() {
  for (int i = 0; i < 18; i++) {
    potObjects[i]->update();
  }
}

void potTurn(int number, int analogVal) {
  int a = map(analogVal, 0, 1023, 0, 126);
  usbMIDI.sendControlChange(potChannels[number], a, 1);
}

void onStep(int k) {
  onStepCheckPotPushed();
}

void onStepCheckPotPushed() {
  for (int i = 0; i < 18; i++) {
    lastPotVals[i] = potVals[i];
    if (abs(potVals[i] - lastPotVals[i] < 8)) {
      potObjects[i]->lock();
    }
  }
}

void checkButtons(){
  if (digitalRead(12)==HIGH){
    for (int i = 0; i < 18; i++){
       potObjects[i]->lock();
    }
  }
}

void onMaxStep() {
  
}


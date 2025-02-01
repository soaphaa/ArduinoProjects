/*
 Program name: LED Animation Speed
 Author: Sophia
 Date: November 28, 2024
 Description: A program that allows for a potentiometer to control the speed of a sequence of LEDs lighting up.
              the adcValue determines the speed the LEDs animation goes at.
*/
const int ledPins[] = {3, 4, 5, 6, 7}; //declare and initialize the pins connected to LEDs
int adcValue; // Define a variable to save ADC value
float delayMs; //delay time


void setup() {
  //ledPins setup
  for(int i=0; i<5; i++){
     pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  adcValue = analogRead(A0); // Convert analog of pin A0 to digital
  delayMs = adcValue; // the delay time is the adcValue
  delay(50);
  //the lights shining in sequence
  for(int i=0; i<5; i++){
    digitalWrite(ledPins[i], HIGH);
    digitalWrite(ledPins[i-1], LOW);
    if(digitalRead(ledPins[0])== HIGH){ // ensure the last ledPin turns off in sequence
  		digitalWrite(ledPins[4], LOW);
  	}    
  	delay(delayMs);//determines the speed of the for loop.
  }
  
}

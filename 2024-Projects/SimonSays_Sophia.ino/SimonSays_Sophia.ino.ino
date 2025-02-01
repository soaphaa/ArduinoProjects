/*
 Program name: Simon Says
 Author: Sophia
 Date: November 26, 2024
 Description: A program that plays a specific 4 colour sequence on a RGB Led when the start button is clicked. 
 				* The user is required to "Play back" the 4 colour sequence using the led's connected to buttons.
        * If the user gets the sequence correct, the RGB Led will shine green, otherwise it will shine red.
*/
//initializing the pins.
const int ledPins[]={8,7,6,5};
const int buttonPins[] = {12,11,10,9};
const int startButton = 13;
int redPin = 4;
int bluePin = 3;
int greenPin = 2;

int sequence[4];
int userInput[4];

boolean startPressed = false;

void setup() {

  //led setups
  for(int i = 0; i < 4; i++){
    pinMode(ledPins[i], OUTPUT);
  }


  //button setups
  for(int i = 0; i<4; i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
  }


  pinMode(startButton, INPUT);
  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.begin(9600);
}


void loop() {
  Serial.println("\nplease click start button");
  while (digitalRead(startButton) == HIGH) { //loops nothing when buttonPin is high. It will only execute the code below once it is clicked.
  }
  delay(1000);
  generateSequence(); //function to generate the specific sequence
  playRGBsequence(); //function to play the specific sequence
  if (userInputs()) {//user input function detects whether it's the same as the sequence.
    Serial.println("CONGRATULATIONS! YOU WIN!");
    digitalWrite(greenPin, LOW);
    delay(100);
    digitalWrite(greenPin, HIGH);
    delay(500);
    digitalWrite(greenPin, LOW);
    delay(500);
    digitalWrite(greenPin, HIGH);
    delay(500);
    digitalWrite(greenPin, LOW);
  }
  else{
    Serial.println("GAME OVER");
    delay(500);
    digitalWrite(redPin, HIGH);
    delay(500);
    digitalWrite(redPin, LOW);
    delay(500);
    digitalWrite(redPin, HIGH);
    delay(500);
    digitalWrite(redPin, LOW);
  }
}
 


void generateSequence() {
  
  sequence[0] = 2;
  sequence[1] = 1;
  sequence[2] = 3;
  sequence[3] = 0;
}

void playSequence() {
  for(int i=0; i<4; i++){
    int led = sequence[i];
    digitalWrite(ledPins[led], HIGH);
    delay(500);
    digitalWrite(ledPins[led], LOW);
    delay(250);
    Serial.println(led);
  }
}  

void playRGBsequence(){
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, HIGH);
  delay(500);
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
  delay(500);
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, LOW);
  delay(500);
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
  delay(500);
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, LOW);
  delay(500);
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
  delay(500);
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
  delay(500);
  digitalWrite(redPin, LOW);
}

int buttonState(){
  int button1State = digitalRead(buttonPins[0]);
  int button2State = digitalRead(buttonPins[1]);
  int button3State = digitalRead(buttonPins[2]);
  int button4State = digitalRead(buttonPins[3]);
//returns the number corresponding to the button in sequence.
  if(button1State == LOW){
    return 0;
  }
  else if(button2State == LOW){
    return 1;
  }
  else if(button3State == LOW){
    return 2;
  }
  else if(button4State == LOW){
    return 3;
  }
  else{
    return 200;
  }
}

bool userInputs() {
  bool verdict = true;
  for (int i=0; i<4; i++){
    int bn = 100;
    while (bn > 10) {
      bn = buttonState();
      delay(10);
    }
    Serial.println(bn);
    if (bn != sequence[i]) { //equal to the sequence
      verdict = false;
    }
    digitalWrite(ledPins[bn], HIGH); //light up the led when the button is being pressed
    delay(250);
    digitalWrite(ledPins[bn], LOW);

    // reset for the next button click
    bn = 100;
    delay(1000);
  }
  return verdict;
}
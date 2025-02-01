#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
int buttonPin = 13; // Pin for Morse input button
int ledPin = 11; // Pin for LED output
int buzzerPin = 12; // Pin for buzzer output

int togglePin = 8; // Pin for mode toggle button

int currentMode = 0; // Set to default mode

int RED = A1; // Pin for red LED
int GREEN = A2; // Pin for green LED
int BLUE = A0; // Pin for blue LED

int pos = -1; // Current mode position (-1 indicates no mode selected)

// Toggle button states
int toggleState;
int lastToggleState = HIGH; // Last toggle button state

int debounceDelay = 30; // Debounce delay for button presses

int ditLength = 240; // Length of a dot (dit) in Morse code

// Other Morse timing values computed from dit length
int ditSpace = ditLength; // Space between dots within a letter
int dahLength = (ditLength * 3); // Length of a dash (3 times the dot length)
int letterSpace = (ditLength * 3); // Space between letters
int wordSpace = (ditLength * 6); // Space between words
int gap; // Gap between letters or words

// Variables to track the button press duration and timing
int pressStartTime = 0; // Start time of a button press
int pressDuration = 0; // Duration of the button press

int lastButtonReleaseTime; // Time of the last button release for gap calculation

unsigned long lastDebounceTime = 0; // Time of the last debounce event

int option = -1; //variable to decide which word to use
String chosenMorse = ""; //stores the chosen morse code for the minigame
String chosenLetter = ""; //stores the chosen letter 
String attempt = ""; //stores the attempted answer for the minigame

String inputStr = ""; // Input string for text-to-Morse conversion
String morseOutput = ""; // Morse code output string

String morseInput = ""; // Input Morse sequence for decoding
String keyOutput = ""; // Translated English output from Morse
char ch; // Character placeholder

// Flags to track various states in the program
bool startScreen, togglePressed, buttonPressed, newLetter, newWord, letterFound, morseFound, morseDetected, wordsDetected, randomChosen;

// Morse code dictionary and corresponding English alphabet
char* morseDict[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", 
                    ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",
                     "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."}; 

char alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Initialize flags
  newLetter = false;
  newWord = false;
  letterFound = false;
  morseFound = false;
  togglePressed = false; // Toggle button state
  buttonPressed = false;
  morseDetected = false;
  wordsDetected = false;
  startScreen = true;
  randomChosen = false;

   // Print a message to the LCD to ensure it functions
  pinMode(buttonPin, INPUT); // Set button pin as input
  pinMode(togglePin, INPUT); // Set toggle pin as input
  pinMode(buzzerPin, OUTPUT); // Set buzzer pin as output
  pinMode(ledPin, OUTPUT); // Set LED pin as output

  // Set RGB LED pins as output
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  // Initialize RGB LED to off
  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);

  Serial.begin(9600); // Start serial communication

  lcd.print("Press the button"); // Initial message on LCD
  Serial.println("____________________________________");
  Serial.println("Morse code Arduino Project");
  Serial.println("Options: \n1.Morse Code decoder \n2.Morse Code encoder \n3.Minigame");
  
  delay(500); // Pause to allow the user to read the options

  Serial.println("____________________________________");
  Serial.println("Click the button to toggle between the modes");
}

void loop() {
  lcd.setCursor(0, 1); // Move cursor to second row of LCD
  int reading = digitalRead(togglePin); // Read the toggle button state

  // Checking the toggle button state
  if (reading != lastToggleState) {
    lastDebounceTime = millis(); // Reset the debounce timer
  }

  // If the debounce time has passed, update the toggle button state
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != toggleState) {
      lastDebounceTime = millis(); // Update the debounce timer
      toggleState = reading;

      if (toggleState == LOW && !togglePressed) {
        togglePressed = true; // Mark the toggle button as pressed
        
        // Update the position state (`pos`)
        if (pos == -1) {
          pos = 0; // Start at 0 when first pressed
        } else {
          pos++;
          if (pos > 2) {
            pos = 0;  // Reset to 0 if it exceeds 2
          }
        }

        // Clear the LCD when switching modes
        lcd.clear();

        Serial.print("Mode switched to: ");
        Serial.println(pos);
      }
    }
  }

  // Reset togglePressed state when toggle button is released
  if (toggleState == HIGH) {
    togglePressed = false;
  }

  // Perform mode change based on pos
  switch (pos) {
    case 0:  // Mode 0: Read Morse
      analogWrite(RED, 255); // Set LED to red
      analogWrite(GREEN, 0);
      analogWrite(BLUE, 0);
      if(startScreen){
        lcd.setCursor(0,0);
        lcd.print("mode 1: Type"); // Display mode description
        lcd.setCursor(0,1);
        lcd.print("on the screen");
      }
      startScreen = false;
      readMonitor(); // Call readMonitor method
      break;

    case 1:  // Mode 1: Write Morse
      analogWrite(RED, 0);
      analogWrite(GREEN, 255); // Set LED to green
      analogWrite(BLUE, 0);
      if(startScreen){
        lcd.setCursor(0,0);
        lcd.print("mode 2: Input"); // Display mode description
        lcd.setCursor(0,1);
        lcd.print("morse code");
      }
      startScreen = false;
      writeMorse(); // Call writeMorse method
      break;

    case 2:  // Mode 2: Minigame
      analogWrite(RED, 0);
      analogWrite(GREEN, 0);
      analogWrite(BLUE, 255); // Set LED to blue
      if(startScreen){
        lcd.setCursor(0,0);
        lcd.print("mode 3: minigame"); // Display mode description
        lcd.setCursor(0,1);
        lcd.print("read monitor:");
        Serial.println("Welcome to the minigame! \nTo play: \n1.Look at the morse code written on the LCD screen \n2.Decode it to your best ability and enter your guess into the Monitor");
        Serial.println("____________________________________");
        Serial.println("press the button on breadboard #2 to begin. Good luck! (PS: The morse code is one letter");
      }
      startScreen = false;
      minigame(); // Call minigame method
      break;

    default:  // Handle invalid `pos` values
      // Ensure `pos` is reset to a valid state
      pos = -1;
      break;
  }

  lastToggleState = reading; // Save the reading for next loop
  delay(100); // Small delay to reduce loop speed
}

// Converts typed text to Morse code and plays it
void readMonitor() {
  morseInput = ""; // Clear previous Morse input
  keyOutput = ""; // Clear previous translated text

  serialEvent(); // Read input from the serial monitor
  if(inputStr != ""){
      int len = inputStr.length();
      inputStr.toUpperCase(); // Convert the string to uppercase for comparison
      
      // Convert input to Morse code
      for (int i = 0; i < len; i++) {
        for(int j = 0; j < 26; j++){
          if(inputStr[i] == alpha[j]){ // Find matching letter
            morseOutput += morseDict[j]; // Append Morse code for the letter
            morseOutput += " ";
            morseFound = true;
            break; // Exit loop once match is found
          }
        }
      }

    // Display input and Morse output on the LCD
    lcd.setCursor(0,0);
    lcd.print(inputStr);
    lcd.setCursor(0,1);
    lcd.print(morseOutput);
    delay(1000); // Wait before playing Morse code

    // Play the Morse code using LED and buzzer
    for(int k = 0; k < morseOutput.length(); k++){
      if(morseOutput[k] == '.'){ // Dot
        digitalWrite(ledPin, HIGH);
        digitalWrite(buzzerPin, HIGH);
        delay(ditLength);
        digitalWrite(ledPin, LOW);
        digitalWrite(buzzerPin, LOW);
        delay(ditLength);
      }
      else if(morseOutput[k] == '-'){ // Dash
        digitalWrite(ledPin, HIGH);
        digitalWrite(buzzerPin, HIGH);
        delay(dahLength);
        digitalWrite(ledPin, LOW);
        digitalWrite(buzzerPin, LOW);
        delay(ditLength);
      }
      else{ // Space
        delay(ditSpace);
      }
    }

    inputStr = ""; // Reset the input string after processing
  } 
}

// Reads serial input and appends it to inputStr
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read(); // Read a character from serial
    inputStr += inChar; // Append to inputStr
    attempt += inChar; // Append to the attempt for the minigame
  }
}

// Converts Morse code input to English and handles button presses
void writeMorse() {
  inputStr = ""; // Reset input string
  morseOutput = ""; // Reset Morse output
  int morseButtonReading = digitalRead(buttonPin); // Read the Morse input button state

  // Handle debounce logic for the Morse input button
  if (morseButtonReading != buttonPressed) {
    lastDebounceTime = millis(); // Reset debounce timer
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (morseButtonReading == LOW && !buttonPressed) {
      buttonPressed = true;
      pressStartTime = millis(); // Record the press start time
      digitalWrite(ledPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
    }

    if (morseButtonReading == HIGH && buttonPressed) {
      buttonPressed = false; // Reset button press state
      pressDuration = millis() - pressStartTime; // Calculate press duration
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, LOW);

      // Determine if DOT or DASH
      if (pressDuration < ditLength * 1.5) {
        morseInput += ".";  
      } else {
        morseInput += "-"; 
      }

      lcd.setCursor(0,1);
      lcd.print(morseInput);

      // Update last button release time
      lastButtonReleaseTime = millis();

      // Set newLetter to true, indicating the start of a new sequence
      newLetter = true;
    }

    // Calculate the gap between button presses
    gap = millis() - lastButtonReleaseTime;

    // Check if the gap exceeds the letter space
    if (newLetter && gap >= letterSpace) {
      Serial.println("Processing Morse sequence...");
      letterFound = false;

      // Compare the Morse sequence to the dictionary
      for (int i = 0; i < 26; i++) {
        if (morseInput == morseDict[i]) {
          keyOutput += alpha[i]; // Add the letter to the translated string
          letterFound = true;
          break; // Stop searching once a match is found
        }
      }

      // Handle unknown Morse sequences
      if (!letterFound) {
        digitalWrite(buzzerPin, HIGH); // Indicate error with a beep
        delay(100);
        digitalWrite(buzzerPin, LOW);
      }

      // Reset the Morse sequence
      morseInput = ""; // Clear the current Morse sequence
      newLetter = false;

      // Update the LCD with the current translation
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(keyOutput); // Print the translated text
    }
  }

  // Debugging: Print the current Morse sequence
  Serial.print("Morse Sequence: ");
  Serial.println(morseInput);
}

void minigame() {
  lcd.setCursor(0, 0);
  morseInput = ""; // Reset Morse input
  keyOutput = ""; // Reset translated text
  inputStr = ""; // Reset input string
  morseOutput = ""; // Reset Morse output

  if(!randomChosen){
    generateMorseQuiz();
    lcd.print(chosenMorse);
    for(int i = 0; i<chosenMorse.length(); i++){
        if(chosenMorse[i] == '.'){ // Dot
          digitalWrite(ledPin, HIGH);
          digitalWrite(buzzerPin, HIGH);
          delay(ditLength);
          digitalWrite(ledPin, LOW);
          digitalWrite(buzzerPin, LOW);
          delay(ditLength);
        }
        else if(chosenMorse[i] == '-'){ // Dash
          digitalWrite(ledPin, HIGH);
          digitalWrite(buzzerPin, HIGH);
          delay(dahLength);
          digitalWrite(ledPin, LOW);
          digitalWrite(buzzerPin, LOW);
          delay(ditLength);
        }
        else{ // Space
          delay(ditSpace);
        }
      }
    randomChosen = true;
  }
  serialEvent();
  if(attempt != ""){
    attempt.toUpperCase(); // Convert the string to uppercase for comparison
    for(int i = 0; i<26; i++){
      lcd.setCursor(0,1);
      lcd.print(attempt);
      if(attempt == chosenLetter){
        Serial.println("CORRECT CHOSEN!");
        randomChosen = false;
        attempt = "";
        chosenLetter = "";
        chosenMorse = "";
        delay(500);
        lcd.clear();
        lcd.print("CORRECT!");
        delay(500);
        lcd.clear();
        
        break;
      }
      else if(attempt!= chosenLetter && attempt!= ""){
        Serial.println("WRONG CHOSEN :(");
        attempt = "";
        chosenLetter = "";
       	chosenMorse = "";
        randomChosen = false;
        
        delay(500);
        lcd.clear();
        lcd.print("Wrong!");
        delay(500);
        lcd.clear();
        break;
      }
    }
  }
}

void generateMorseQuiz(){
  option = random(0, 25); // Generates a random number between 0 and 24
  for(int i = 0; i<26; i++){
    if(option == i){
      chosenMorse = morseDict[option];
      chosenLetter += alpha[option];
      Serial.println(chosenLetter);
      Serial.println(chosenMorse);
      
    }
    
    // else{
    //   Serial.println("ERROR CHOOSING RANDOM");
    // }
  }
}
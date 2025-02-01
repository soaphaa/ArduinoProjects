# Morse code translator and minigame
My Arduino final project for TEJ2O  
<br>
  <img src="https://github.com/soaphaa/ArduinoProjects/blob/main/2024-Projects/MorseCodeTEJ2OSummative_Sophia.ino/images/morseCodeProjectImage.PNG" 
       alt="Morse Code Project" 
       width="400" 
       style="vertical-align: middle; display: inline;">
<br>       
## Parts needed: 
* Arduino UNO board
* LCD screen
* Potentiometer
* 5x 220 ohm resistors
* 4x 10k ohm resistors
* 1x 1k ohm resistor
* LED
* RGB LED
* 1x NPN Transistor
* 1x Active buzzer
* 2x push buttons
* Wires
## Pictorial & Circuit Diagram: 
<br>
![Alt text](https://github.com/soaphaa/ArduinoProjects/blob/main/2024-Projects/MorseCodeTEJ2OSummative_Sophia.ino/images/morseCodeProjectTinkercadDiagram.PNG)
<br>
![Alt text](https://github.com/soaphaa/ArduinoProjects/blob/main/2024-Projects/MorseCodeTEJ2OSummative_Sophia.ino/images/morseCodeProjectCircuitDiagram.PNG)

## Project description:

### Modes:
My Arduino project replicates the functionality of a typical Morse code machine. The user is greeted with instructions on the monitor to press a push button to toggle between three modes, each corresponding to a specific color on an RGB LED:
* Green: Translate text from the monitor to Morse code.
* Blue: Translate Morse code input into English.
* Red: Play a Morse code minigame.

#### Mode 1: Morse Code Input and Translation
Users can input Morse code via a push button, with real-time feedback on an LCD screen. A buzzer and LED provide auditory and visual signals, making it accessible for visually and audibly impaired users.

#### Mode 2: English to Morse Code Translation
Users can enter English text through the Serial Monitor, which is displayed on the LCD along with the Morse code translation. The buzzer and LED flash Morse code in real time to reinforce learning.

#### Mode 3: Morse Code Minigame
A quiz mode that tests Morse code proficiency. The user must translate words shown on the LCD or Serial Monitor. Correct answers trigger a green flash and a congratulatory message, with an option to continue or exit.



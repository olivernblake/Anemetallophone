
// SOLENOID VARIABLES
int led = 13;                           // Inbuilt LED for signalling                       
int numSols;                            // How many Solenoids in Array
int sols[] = {3, 4, 5, 6, 7, 8};        // Set up Solenoid array
int tempo;                              // Declare Tempo
int tempoMin = 400;                     // Initialise Tempo Minimum
int tempoMax = 80;                      // Initialise Tempo Maximum
int playChance;                         // Chance that a Solenoid will play

// ANEMOMETER VARIABLES
int sensorPin = A0;                     // Initialise the Sensor Pin
int sensorValue = 0;                    // Initialise the Sensor Value
// for Constraining Sensor Values
int sensorMax;                          // Declare the Sensor Max
int sensorMin;                          // Declare the Sensor Min
int sensorThresh;                       // Declare the Sensor Threshold

// BUTTON AND MODE SELECT VARIABLES
int buttonPin = 2;                      // Initialise the Button Pin
int buttonVal = 1;                      // Unpressed button value is 1
int pButtonVal = 1;                     // Store unpressed button value as default
int modeNum;                            // Declare the Mode Number

void setup() {

  pinMode(led, OUTPUT);                 // Set up Inbuild LED pin
  pinMode(sensorPin, INPUT);            // Set up Anemometer Pin
  pinMode(buttonPin, INPUT_PULLUP);     // Set up Button Pin as Input Pullup
  
  numSols = 6;                          // Set Number of Solenoids in Array
  for (int i = 0; i < numSols; i++) {   // Set up Solenoid Pins
    pinMode(sols[i], OUTPUT);
  }

  modeNum = 0;                          // Set up Mode Number at boot (default 0)
                                        // Mode 0: Gallery Space with Small Fan
                                        // Mode 1: Gallery Space with Crank Fan
                                        // Mode 2: Gallery Space with the Mega Fan from Hatchlabs
 
  // Run Mode Select function to set Sensor expected values
  modeSelect();
    
}

void modeSelect() {
   if (modeNum == 0) {
    // Set vars for play with Small Fan
    sensorMin = 83;
    sensorMax = 110;
    sensorThresh = 87;
    playChance = 90; 
   }
   
   if (modeNum == 1) {
    // Set vars for play with Crank Fan
    sensorMin = 83;
    sensorMax = 110;
    sensorThresh = 87;
    playChance = 95;
   }

   if (modeNum == 2) {
    // Set vars for play with Mega fan
    sensorMin = 83;
    sensorMax = 125;
    sensorThresh = 90;
    playChance = 95;
   }
}

void loop() {
  
  // READ THE ANEMOMETER DATA
    // Read the sensor value and constrain it within the range of our chosen mode
    sensorValue = analogRead(sensorPin);
    sensorValue = constrain(sensorValue, sensorMin, sensorMax);
    
    // Use the sensor value to map our tempo, and then constrain those values
    tempo = map(sensorValue, sensorThresh, sensorMax, tempoMin, tempoMax);
    tempo = constrain(tempo, tempoMax, tempoMin);

  // PLAY THE INSTRUMENT (or not)
    // If the value from the sensor is above the treshold, play the instrument
    if (sensorValue > sensorThresh) {
      
      // Turn on LED indicator to signal that we're above the treshold
      digitalWrite(led, HIGH);
      
      // Use loop to generate random chance vars for each solenoid to play or not
      for (int i = 0; i < numSols; i++) {
        int chance = random(100);
        if (chance > playChance) digitalWrite(sols[i], HIGH);
      }
      // Amount of time Solenoid is activated for
      delay(50);
      
      // Use loop to send an off message to ALL solenoids
      for (int i = 0; i < numSols; i++) {
        digitalWrite(sols[i], LOW);
      }  
      // Use the TEMPO variable to wait controlled amount of time before triggering again
      delay(tempo);
    
    } else { // DO NOT play the instrument
      
      // Turn off LED indicator to signal that we're below the threshold
      digitalWrite(led, LOW);
      
      // To make us fetch the Anemometer value 10 times a second
      delay(50);
    
    }

  // SELECT MODES
      // Read the Button Value
      buttonVal = digitalRead(buttonPin);
      
      // Check if the Button value is different (lower) than previous loop
      if (buttonVal < pButtonVal) {
        // If button is pressed, change the Mode Number
        modeNum++;
        // Make it reset if Mode Number is higher than 2
        if (modeNum > 2) modeNum = 0;
        // Run Mode Select fuction to update values
        modeSelect();
      }
      
      // Update the Previous Button Value with current for next comparison
      pButtonVal = buttonVal;
} 

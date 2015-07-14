

// 3 and 4 have 47ohm resisters the rest have 
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Uses the default address of 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

const char          pinNumbers[] = {3,5,6,9,10};
//const char          digitalInputPins[] = {12, 13};
//const char          analogInputPins[] = {4,5};
//unsigned char       digitalInputPinsAmt = 1;
//unsigned char       analagInputPinsAmt = 2;
unsigned char       outputPinsAmt = 5;
 

int                 timeCharIsOn = 400;
int                 timeCharIsOff = 150;
char                lEDBrightness = 15;
String              sentance = "Hello World! abcdefghijklmnopqrstuvwxyz 0123456789 now agian :}"; 

 char               lowerCaseArray[] = {96,'e','t','n','a','s','h','f','o','r','d','g','l','y',
                         'p','z','i','c','u','b','m','v','k',123 ,'w','j','x',124,'q',125,126,127};
                         
 char               upperCaseArray[] = {64,'E','T','N','A','S','H','F','O','R','D','G','L','Y',
                         'P','Z','I','C','U','B','M','V','K',95,'W','J','X',91,'Q',92,93,94};
                        
 char               symbolArray[] = {32,33,34,35,36,37,38,39,40,41,42,43,44,45,
                         46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63};

String              message = "";

bool                motorStates[] = {false,false,false,false,false,false,false,false};
bool                inputIsActive = true;
bool                readingMessage = false;
// used in the pause function
bool                pauseIsActive = false;
// pause button variables
long                lastStateChangePause = 0;



void parseSentance(String sentanceToParse,bool &pauseVariable) {
  int messageIndex = 0; // keeps track of you place in the string/message  
  String sentanceCopy = sentanceToParse;    
  while (sentanceCopy.length() > 0) {
//    handleBtn_On_Off(digitalInputPins[1], lastStateChangePause, 399, pauseIsActive, true, 8);      // pause btn    
    if (!pauseVariable) { 
      char currentChar = sentanceCopy.charAt(0);
      if (currentChar != '\0') {
        messageIndex++;        
        byte charByte = charToByte(currentChar);
        setMotorStates(charByte);
        sendToMotors();         
        delay(timeCharIsOn);
        resetStates();          
        delay(timeCharIsOff);
      };
        sentanceCopy.remove(0,1);
    };  // if pause isn't active
  }; // while loop
};  // end of function
 
byte charToByte(char character) {
  if (character < 64) {  // if char is a symbol         
    for(byte i=0; i<sizeof(symbolArray); i++) {
      if(character == symbolArray[i]) {
        i = bitSet(i,5); // modify the 5th bit of the byte to on so if 00000011 now its 00100011
        return byte(i);            
      };                                     
    };
  } else if (character < 96) { // if char is uppercase4
    for(byte i=0; i<sizeof(upperCaseArray); i++) {
      if(character == upperCaseArray[i]) {
        i = bitSet(i, 6);  // modify the 6th bit of the byte to on so if its 00000011 now its 01000011
        return byte(i);
      };                                     
    };
  } else if (character < 128) {  // if char is lowercase
    for(byte i=0; i<sizeof(lowerCaseArray); i++) {
      if(character == lowerCaseArray[i]) {
        return byte(i);
      };                                     
    };  
  };       
};

void setMotorStates(byte charNum) {  
  for (int i=0; i<outputPinsAmt; i++) {
    int currentBit = bitRead(charNum, i);
    if (bitRead(charNum, i) == 1) { 
      motorStates[i] = true;
    };
  };
};

void sendToMotors() { 
  for (int i=0; i<outputPinsAmt; i++) { 
          if (motorStates[i] == 1) {
            if (i == 3) {
              analogWrite(pinNumbers[i], 250);
            }
             analogWrite(pinNumbers[i], 150);   
          };    

  };
};

void resetStates() {
  for (int i = 0; i<outputPinsAmt; i++) {
    motorStates[i] = false;
    analogWrite(pinNumbers[i], 0);
  };
};

//void handleBtn_On_Off(char inputPinNumber, long &lastStateChange, int debounceTime ,bool &stateOfBtn,bool btnHasOutput, char outputPin) {
//  byte pauseBtnReading = digitalRead(inputPinNumber);
//  if (pauseBtnReading == 1) {      
//    long currentMillis = millis();
//    if (currentMillis - lastStateChange > debounceTime) {
//      stateOfBtn = !stateOfBtn;
//      if (btnHasOutput) {
//        digitalWrite(outputPin, pauseIsActive);
//      };        
//      lastStateChange = currentMillis;
//    };
//  };
//}; // end handlePause

void readIncomingSerial() {
  while(Serial.available() > 0) {
    inputIsActive = true;
    readingMessage = false;
    message += char(Serial.read());  
  };
};

void setup() {
  Serial.begin(9600);
  
// PWM Shield setup
  pwm.begin();
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency
  // Initialize Inputs and OutPuts  
    // save I2C bitrate
  uint8_t twbrbackup = TWBR;
  // must be changed after calling Wire.begin() (inside pwm.begin())
  TWBR = 12; // upgrade to 400KHz!
  
  for (int i=0; i<outputPinsAmt; i++) {
    pinMode(pinNumbers[i], OUTPUT);
    analogWrite(10, 10);       
  };
//  for (int i=0; i<digitalInputPinsAmt; i++) {
//    pinMode(digitalInputPins[i], INPUT);
//    digitalWrite(digitalInputPins[i], LOW);    
//  };
//  for (int i=0; i<analagInputPinsAmt; i++) {
//    pinMode(digitalInputPins[i], INPUT);       
//  };
};

void loop() {  
  if (Serial.available() > 0) {    
    readIncomingSerial();
    inputIsActive = false;
    readingMessage = true;  
  };
  if ((!inputIsActive) && (readingMessage)) {
    parseSentance(message, pauseIsActive);
    message.remove(0, message.length());
  };
  inputIsActive = true;
  readingMessage = false; 
  delay(200);  
};

  

  
 

  

  
 


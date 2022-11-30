// Elif Naz Gürerman - Yiğit Erdinç
//         200202004 - 200202110

#include "LiquidCrystal.h"
#include <Keypad.h>

LiquidCrystal lcd(13, 12, 11, 10, 9, 8, 7);

float temperature = 0.0;
float flame = 0.0;
float motion = 0.0;

bool didDisplayTemperatureChangeInfo = false;

int buzzerPin = 22;
int lampPin = 23;
int greenLedPin = 24;
int redLedPin = 25;

const byte rowCount = 4;
const byte columnCount = 3;

byte rowPins[rowCount] = {29, 30, 31, 32};
byte columnPins[columnCount] = {28, 27, 26};

char keysOnKeypad[rowCount][columnCount] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};

char password[]= {'0','0','0','0'};


char pressedKey;

short i = 0;
short j = 0;

short timesValid = 0;

Keypad keypad = Keypad( makeKeymap(keysOnKeypad), rowPins, columnPins, rowCount, columnCount);

void setup() {
   lcd.begin(16, 2);
   setOutputPins();
}

void setOutputPins() {
   pinMode(22, OUTPUT);
   pinMode(23, OUTPUT);
   pinMode(24, OUTPUT);
   pinMode(25, OUTPUT);
}

void loop() {  
  checkTemp();
  checkFlame();
  checkMotion();
  delay(100);
  checkKeypad();
  delay(200);
}

void checkTemp() {
  int analog_value = analogRead(A0);
  temperature = (analog_value); 
  displayTemp();
}

void checkFlame() {
  int analog_value = analogRead(A1);
  flame = (analog_value); 
  if (flame > 1) {
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  }
}

void checkMotion() {
  int analog_value = analogRead(A2);
  motion = (analog_value); 
  if (motion > 1) {
    digitalWrite(lampPin, HIGH);
  } else {
    digitalWrite(lampPin, LOW);
  }
}

void displayTemp() {
  lcd.clear();
  if (temperature < 20.0) {
    if (didDisplayTemperatureChangeInfo == true) {
      lcd.setCursor(0, 0);
      lcd.print("AMBIENT TEMP IS: ");
      lcd.setCursor(0, 1);
      lcd.print(temperature);
    } else {
      lcd.setCursor(0, 0);
      lcd.print("TEMP DROPPED TO: ");
      lcd.setCursor(0, 1);
      lcd.print(temperature);
      didDisplayTemperatureChangeInfo = true;
    }
  } else if (temperature > 30.0) {
    if (didDisplayTemperatureChangeInfo == true) {
      lcd.setCursor(0, 0);
      lcd.print("AMBIENT TEMP IS: ");
      lcd.setCursor(0, 1);
      lcd.print(temperature);
    } else {
      lcd.setCursor(0, 0);
      lcd.print("TEMP RAISED TO: ");
      lcd.setCursor(0, 1);
      lcd.print(temperature);
      didDisplayTemperatureChangeInfo = true;
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.print("AMBIENT TEMP IS: ");
    lcd.setCursor(0, 1);
    lcd.print(temperature);
    didDisplayTemperatureChangeInfo = false;
  }
}

void checkKeypad() {
  pressedKey = NO_KEY;
  pressedKey = keypad.getKey();
  if(pressedKey == '*'){
    checkPassword();
  } else if(pressedKey == '#'){
    changePassword();
  }  
}

void checkPassword() {
  i = 0, j = 0, timesValid = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ENTER PASSWORD: ");
  while(pressedKey != '#'){
    pressedKey = keypad.getKey();
    if(pressedKey != NO_KEY && pressedKey != '#'){
      lcd.setCursor(j, 1);
      lcd.print("*");
      j++;
      if(pressedKey == password[i] && i < sizeof(password)){
        timesValid++;
        i++;
      } else {
        timesValid--;
      }
    }
  }
  pressedKey = NO_KEY;
  if(timesValid==sizeof(password)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PASS IS CORRECT");
    digitalWrite(greenLedPin, HIGH);
    delay(100);
    digitalWrite(greenLedPin, LOW);
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PASS IS WRONG");
    digitalWrite(redLedPin, HIGH);
    delay(100);
    digitalWrite(redLedPin, LOW);
  }
}

void changePassword() {
  i = 0, j = 0, timesValid = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ENTER PASSWORD: ");
  while(pressedKey != '*'){
    pressedKey = keypad.getKey();
    if(pressedKey != NO_KEY && pressedKey != '*'){
      lcd.setCursor(j, 1);
      lcd.print("*");
      j++;
      if(pressedKey == password[i] && i < sizeof(password)){
        timesValid++;
        i++;
      } else {
        timesValid--;
      }
    }
  }
  pressedKey = NO_KEY;
  if(timesValid == sizeof(password)) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PASS IS CORRECT");
  digitalWrite(greenLedPin, HIGH);
  delay(100);
  digitalWrite(greenLedPin, LOW);
  i=0;
  j=0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ENTER NEW PASS: ");
  while(pressedKey != '*'){
    pressedKey = keypad.getKey();
    if(pressedKey != NO_KEY && pressedKey != '*' ){
      password[i] = pressedKey;
      lcd.setCursor(j, 1);
      lcd.print("*");
      i++;
      j++;
    }
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("NEW PASS IS SET");
  digitalWrite(greenLedPin, HIGH);
  delay(100);
  digitalWrite(greenLedPin, LOW);
  pressedKey = NO_KEY;
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PASS IS WRONG");
    digitalWrite(redLedPin, HIGH);
    delay(100);
    digitalWrite(redLedPin, LOW);
  }
}

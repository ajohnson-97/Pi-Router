// Pi-Router Display/Password System
// https://github.com/ajohnson-97/Pi-Router
// Written By: Anthony Johnson

// Upload while bypassing the boot-loader to speed up onset after powering on the device.
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD displays using the hardware address of the displays (in my case thats 0x23/27) check for your devices and adjust accordingly.
LiquidCrystal_I2C lcd1(0x27, 16, 2);
LiquidCrystal_I2C lcd2(0x23, 16, 2);

Servo myServos;
int servoLocked = 90;
int servoUnLocked = 0;

// Set a variable for the pin connected to the key-switch that will act as the master switch (must be digital pin 1 or 2 on an arduino if using it as a hardware interrupt).
int keySwitch = 2;
int ledPins[] = {3, 4, 5, 6, 7, 8, 9, 10}; // Assigned pin numbers to variables, stored in a list. 
int correctPattern[] = {HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW, HIGH}; // Reference pattern to compare the list of input pins to determine if that password is correct.
int redPinLED = 12;
int greenPinLED = 13;

// Create the custom characters for the LCD display by setting the bytes manually.
byte smallxChar[8] = {0b00000,0b00000,0b00000,0b10001,0b01010,0b00100,0b01010,0b10001};
byte bigxChar1[8] = {0b01000,0b00100,0b00010,0b00001,0b00001,0b00010,0b00100,0b01000};
byte bigxChar2[8] = {0b00010,0b00100,0b01000,0b10000,0b10000,0b01000,0b00100,0b00010};
byte leftDash[8] = {0b00000,0b00000,0b00000,0b00111,0b00111,0b00000,0b00000,0b00000};
byte rightDash[8] = {0b00000,0b00000,0b00000,0b11100,0b11100,0b00000,0b00000,0b00000};

void setup() 
{
  lcd1.init();
  lcd2.init();
  lcd1.backlight();
  lcd2.backlight();
  lcd1.clear();
  lcd2.clear();

  // Set the LED pins as digital inputs, using the onboard pull-up resistors to pull the values up to 5V and out of a floating state.
  for (int i = 0; i < 8; i++) {
    pinMode(ledPins[i], INPUT_PULLUP);
  }
  pinMode(keySwitch, INPUT_PULLUP);
  pinMode(redPinLED, OUTPUT);
  pinMode(greenPinLED, OUTPUT);

  myServos.attach(11); // Must attach the servo data pin to a PWM capable pin.
  myServos.write(servoLocked);

  // Index the custom LCD characters
  lcd1.createChar(0, smallxChar);
  lcd1.createChar(1, bigxChar1);
  lcd1.createChar(2, bigxChar2);
  lcd1.createChar(3, leftDash);
  lcd1.createChar(4, rightDash);
}

void loop() 
{
  myServos.write(servoLocked);
  digitalWrite(greenPinLED, LOW);
  digitalWrite(redPinLED, LOW);
  welcomeMessage();

  if (digitalRead(keySwitch) == LOW) { // If the key-switch is engaged (value will register as low when engaged because the pull-up resistor keeps the pin at 5V when the circuit is open).
    if (patternCheck()) { // If the password is incorrect.
      myServos.write(servoUnLocked);
      successMessage();
      digitalWrite(greenPinLED, HIGH);
      digitalWrite(redPinLED, LOW);
    } else { // If the password is incorrect.
      myServos.write(servoLocked);
      failureMessage();
      digitalWrite(greenPinLED, LOW);
      redLEDFlash();
    }
  } 
}

bool patternCheck() 
{
  for (int i = 0; i < 8; i++) {
    int pinState = digitalRead(ledPins[i]);
    int referencePattern = correctPattern[i];
    if (pinState != referencePattern) {
      return false;
    }
  }
  return true;
}

void welcomeMessage()
{
  lcd1.setCursor(0,0);
  lcd1.write((byte)0);
  lcd1.setCursor(1,0);
  lcd1.write((byte)1);
  lcd1.setCursor(2,0);
  lcd1.write((byte)2);
  
  lcd1.setCursor(3,0);
  lcd1.print("PI");
  
  lcd1.setCursor(5,0);
  lcd1.write((byte)3);
  lcd1.setCursor(6,0);
  lcd1.write((byte)4);
  
  lcd1.setCursor(7,0);
  lcd1.print("ROUTER");
  
  lcd1.setCursor(13,0);
  lcd1.write((byte)1);
  lcd1.setCursor(14,0);
  lcd1.write((byte)2);
  lcd1.setCursor(15,0);
  lcd1.write((byte)0);
  
  lcd1.setCursor(0,1);
  lcd1.print("2.5Gb & WIFI 6e ");
  lcd2.setCursor(0,0);
  lcd2.print("    Made by:    ");
  lcd2.setCursor(0,1);
  lcd2.print("Anthony Johnson ");
}

void successMessage() 
{
  lcd1.setCursor(0, 0);
  lcd1.print("Access Granted! ");
  lcd1.setCursor(0,1);
  lcd1.print("WPA2: Password! ");
  lcd2.setCursor(0,0);
  lcd2.print("SSH: 10.7.7.1   ");
  lcd2.setCursor(0,1);
  lcd2.print("DNS: 10.7.7.2   ");
}

void failureMessage() 
{
  lcd1.setCursor(0, 0);
  lcd1.print("Error Wrong Code");
  lcd1.setCursor(0, 1);
  lcd1.print("  Try Again...  ");
  lcd2.setCursor(0,0);
  lcd2.print("Or try using a..");
  lcd2.setCursor(0,1);
  lcd2.print("    Hammer?     ");
}

void redLEDFlash() 
{
  analogWrite(redPinLED, 255);
  delay(500);
  analogWrite(redPinLED, 0);
  delay(500);
}

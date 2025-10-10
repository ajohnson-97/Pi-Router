// Pi-Router Display/Password System
// https://github.com/ajohnson-97/Pi-Router
// Written By: Anthony Johnson

#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Servo myServo;

LiquidCrystal_I2C lcd1(0x23, 16, 2);
LiquidCrystal_I2C lcd2(0x27, 16, 2);

const int ledPins[8] = {3, 4, 5, 6, 7, 8, 12, 13}; // Gap in numbering due to the RGB LED needing the PWM pins.
const int keySwitch = 2;

const bool correctPattern[8] = {0, 0, 0, 1, 0, 0, 1, 1};

// PWM pins for the RGB inputs
const int rgbRed = 9;
const int rgbGreen = 10;

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

  // Clear the LCD feed
  lcd1.clear();
  lcd2.clear();

  // Index the custom ASCII characters
  lcd1.createChar(0, smallxChar);
  lcd1.createChar(1, bigxChar1);
  lcd1.createChar(2, bigxChar2);
  lcd1.createChar(3, leftDash);
  lcd1.createChar(4, rightDash);

  pinMode(rgbRed, OUTPUT);
  pinMode(rgbGreen, OUTPUT);

  myServo.attach(11);

  // Set up LED switches as inputs with pull-up resistors
  for (int i = 0; i < 8; i++) {
      pinMode(ledPins[i], INPUT_PULLUP);
  }

  // Set up key switch as input with pull-up resistor
  pinMode(keySwitch, INPUT_PULLUP);

  // Display the welcome message initially on both LCDs
  welcomeMessage();  
}

void loop() 
{
  if (digitalRead(keySwitch) == LOW) { // If key switch is turned ON
    myServo.write(90);
    if (checkCorrectPattern()) {
      rgbStatusSuccess();
      secretMessage();
    } else {
      rgbStatusError();
      errorMessage();
    }
  } else { // If key switch is OFF
    welcomeMessage();
  }
}

// Function to check if LEDs match the correct pattern
bool checkCorrectPattern() 
{
  for (int i = 0; i < 8; i++) {
      if (digitalRead(ledPins[i]) == correctPattern[i]) {
          return false; // Pattern does not match
      }
  }
  return true;
}

// Function to display the welcome message
void welcomeMessage() 
{
  static bool firstDisplay = true;  // Flag to check if we need to clear the screen

  if (firstDisplay) {  // Only clear the LCD once
    lcd1.clear();
    lcd2.clear();
    firstDisplay = false;
  }

  analogWrite(rgbRed, 0);
  analogWrite(rgbGreen, 0);

  // Custom ASCII symbols and title - 1st line top screen
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
  lcd2.print("Anthony Johnson.");

  delay(2000);

  if (digitalRead(keySwitch) == LOW) { // If key switch is turned ON
    loop();
  }

  // 1st line of the bottom screen
  lcd2.setCursor(0,0);
  lcd2.print("SSID: WIFI!     ");
  // 2nd line of the bottom screen
  lcd2.setCursor(0,1);
  lcd2.print("WPA2: **********");

  // Display the network configuration
  // 1st line of the top screen
  lcd1.setCursor(0,0);
  lcd1.print("SSH: ***********");
  //2nd line of the top screen
  lcd1.setCursor(0,1);
  lcd1.print("DNS: ***********");

  delay(2000);

  if (digitalRead(keySwitch) == LOW) { // If key switch is turned ON
    loop();
  }
}

// Function to display the secret message
void secretMessage() 
{
  static bool firstDisplay = true;  // Flag to check if we need to clear the screen

  if (firstDisplay) {  // Only clear the LCD once
    lcd1.clear();
    lcd2.clear();
    firstDisplay = false;
  }

  lcd1.setCursor(0, 0);
  lcd1.print("Access Granted! ");
  
  lcd1.setCursor(0,1);
  lcd1.print("WPA2: Password! ");

  lcd2.setCursor(1,0);
  lcd2.print("SSH:192.168.1.1 ");

  lcd2.setCursor(1,1);
  lcd2.print("DNS: 10.7.8.1   ");
}

// Function to display an error message
void errorMessage() 
{
  static bool firstDisplay = true;  // Flag to check if we need to clear the screen

  if (firstDisplay) {  // Only clear the LCD once
    lcd1.clear();
    lcd2.clear();
    firstDisplay = false;
  }

  lcd1.setCursor(0, 0);
  lcd1.print("Error Wrong Code");
  lcd1.setCursor(0, 1);
  lcd1.print("Try Again...    ");

  lcd2.setCursor(0, 0);
  lcd2.print("HINT: What is   ");
  lcd2.setCursor(0, 1);
  lcd2.print("PI in binary    ");
}

void rgbStatusError() 
{
  analogWrite(rgbRed, 255);
  analogWrite(rgbGreen, 0);
  delay(1000);

  if (digitalRead(keySwitch) == HIGH) { // If key switch is turned OFF
    loop();
    }

  analogWrite(rgbRed, 0);
  analogWrite(rgbGreen, 0);
  delay(1000);
}

void rgbStatusSuccess() 
{
  analogWrite(rgbRed, 0);
  analogWrite(rgbGreen, 255);
}

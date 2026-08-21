#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <Keypad.h>

// --- Pin Definitions ---
#define OLED_RESET -1
Adafruit_SH1106 display(OLED_RESET);

// Software Serial for HC-05 Bluetooth (Rx: Pin 10, Tx: Pin 11)
SoftwareSerial BTSerial(10, 11); 

#define IR_PIN A0
#define BUZZER_PIN 12

// --- Keypad Setup ---
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// --- Variables ---
String messageBuffer = "";
bool displayOn = false;

void setup() {
  pinMode(IR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  BTSerial.begin(9600);
  
  // OLED Setup (Address 0x3C or 0x3D)
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Master Ready...");
  display.display();
  delay(1000);
}

void loop() {
  // 1. Check IR Sensor Proximity for Power Saving
  int irState = digitalRead(IR_PIN);
  
  // Active LOW or HIGH depending on sensor (Assuming LOW when object detected)
  if (irState == LOW) { 
    if (!displayOn) {
      displayOn = true;
      tone(BUZZER_PIN, 1000, 100); // Proximity alert tone
    }
  } else {
    displayOn = false;
    display.clearDisplay();
    display.display();
  }

  // 2. Handle Keypad Input
  char key = keypad.getKey();
  if (key) {
    tone(BUZZER_PIN, 1500, 50); // Key press tone
    
    if (key == '#') { 
      // '#' acts as SEND key
      if (messageBuffer.length() > 0) {
        BTSerial.print(messageBuffer); // Send via Bluetooth
        tone(BUZZER_PIN, 2000, 150);   // Transmission tone
        
        if (displayOn) {
          display.clearDisplay();
          display.setCursor(0, 0);
          display.print("Sent: ");
          display.println(messageBuffer);
          display.display();
        }
        messageBuffer = ""; // Reset buffer
      }
    } 
    else if (key == '*') { 
      // '*' acts as CLEAR key
      messageBuffer = "";
      if (displayOn) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Cleared!");
        display.display();
      }
    } 
    else {
      // Append character to message
      if (messageBuffer.length() < 16) { 
        messageBuffer += key;
      }
      
      if (displayOn) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Type: ");
        display.println(messageBuffer);
        display.display();
      }
    }
  }
}

#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

// --- Pin Definitions ---
#define OLED_RESET -1
Adafruit_SH1106 display(OLED_RESET);

// Software Serial for HC-05 Bluetooth (Rx: Pin 10, Tx: Pin 11)
SoftwareSerial BTSerial(10, 11); 

#define BUZZER_PIN 12
#define LED_PIN 13

// --- Variables ---
String incomingMessage = "";

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  BTSerial.begin(9600);
  
  // OLED Setup (Address 0x3C or 0x3D)
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Slave Ready...");
  display.println("Waiting for msg...");
  display.display();
  delay(1000);
}

void loop() {
  // Check if data is received via Bluetooth
  if (BTSerial.available()) {
    incomingMessage = "";
    
    // Read the complete incoming message
    while (BTSerial.available()) {
      char c = BTSerial.read();
      incomingMessage += c;
      delay(5); // Small delay to allow buffer to fill
    }

    // Audio & Visual Alert for Incoming Message
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, 2000, 200); // Beep tone
    delay(200);
    digitalWrite(LED_PIN, LOW);

    // Display received message on OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("Incoming Message:");
    display.setTextSize(2); // Bigger font for message
    display.println(incomingMessage);
    display.display();
  }
}

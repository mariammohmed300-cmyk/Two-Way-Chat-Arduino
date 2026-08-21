#include <SoftwareSerial.h>

// Software Serial for HC-05 (Rx: Pin 10, Tx: Pin 11)
SoftwareSerial BTSerial(10, 11); 

void setup() {
  // Start serial communication with PC
  Serial.begin(9600);
  Serial.println("Enter AT Commands:");

  // HC-05 default baud rate in AT Command mode is usually 38400
  BTSerial.begin(38400); 
}

void loop() {
  // Read from HC-05 and send to Serial Monitor
  if (BTSerial.available()) {
    Serial.write(BTSerial.read());
  }

  // Read from Serial Monitor and send to HC-05
  if (Serial.available()) {
    BTSerial.write(Serial.read());
  }
}

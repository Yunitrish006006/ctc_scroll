#include <Arduino.h>

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200); // Initialize serial communication at 115200 baud rate
  while (!Serial && millis() < 5000)
  {
    ; // Wait for Serial to initialize, but don't wait forever
  }
  delay(1000); // Extra delay for stability
  Serial.println("=== ESP32-C3 Started ===");
}

void loop()
{
  Serial.print("Loop count: ");
  Serial.println(millis());
  delay(5000); // Wait for 2 seconds
}
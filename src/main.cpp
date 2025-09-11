#include <Arduino.h>
#include "ec11_module.h"

// 設定針腳
#define CLK_PIN 2
#define DT_PIN 3
#define SW_PIN 10
#define LED_PIN 8
#define DEBUG true

EC11Module ec11(CLK_PIN, DT_PIN, SW_PIN);

void setup()
{
  // 初始化串口通訊
  Serial.begin(115200);
  while (!Serial && millis() < 5000)
  {
    if (DEBUG)
      Serial.println("=== 等待串口初始化 ===");
  }
  delay(1000);
  if (DEBUG)
    Serial.println("=== EC11 模組化測試 ===");
  ec11.begin();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop()
{
  static bool ledState = LOW;
  static bool lastBtn = false;
  ec11.update();
  bool press = ec11.press();
  if (ec11.pressCount > 0 && press != lastBtn)
  {
    if (press)
    {
      ledState = !ledState;
      digitalWrite(LED_PIN, !ledState);
      if (DEBUG)
        Serial.printf("[main] 按鈕觸發LED切換，LED狀態: %s\n", ledState ? "ON" : "OFF");
    }

    lastBtn = press;
  }

  ec11.update();
  delay(5);
}

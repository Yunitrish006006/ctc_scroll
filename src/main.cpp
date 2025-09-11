#include <Arduino.h>
#include "ec11_module.h"

// 設定針腳
#define CLK_PIN 2
#define DT_PIN 3
#define SW_PIN 10
#define LED_PIN 8

EC11Module ec11(CLK_PIN, DT_PIN, SW_PIN, true);

void setup()
{
  // 初始化串口通訊
  Serial.begin(115200);
  while (!Serial && millis() < 5000)
  {
    ; // 等待串口初始化
  }
  delay(1000);

  Serial.println("=== EC11 模組化測試 ===");
  ec11.begin();
}

void loop()
{
  // 按鈕觸發LED切換
  static bool lastBtn = HIGH;
  bool nowBtn = digitalRead(ec11.swPin);
  if (nowBtn == LOW && lastBtn == HIGH)
  {
    // led on
    if (ec11.debug)
      Serial.println("[main] 按鈕觸發LED切換");
  }
  lastBtn = nowBtn;

  ec11.update();
  delay(5);
}

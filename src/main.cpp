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
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // 預設LED關閉
}

void loop()
{
  // 按鈕觸發LED切換
  static bool lastBtn = HIGH;
  static bool ledState = LOW;
  ec11.update();
  bool nowBtn = ec11.press();
  if (nowBtn && !lastBtn)
  {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    if (ec11.debug)
      Serial.printf("[main] 按鈕觸發LED切換，LED狀態: %s\n", ledState ? "ON" : "OFF");
  }
  lastBtn = nowBtn;

  // 顯示狀態擴充變數
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 1000)
  {
    Serial.printf("[main] 累計滾動: %d, 累計按壓: %d\n", ec11.scrollCount, ec11.pressCount);
    lastPrint = millis();
  }

  ec11.update();
  delay(5);
}

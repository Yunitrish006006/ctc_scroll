#include <Arduino.h>
#include "ec11_module.h"

// 設定針腳
#define CLK_PIN 2
#define DT_PIN 3
#define SW_PIN 10
#define LED_PIN 8

#define RED_LED1_PIN 9
#define RED_LED2_PIN 10
#define RED_LED3_PIN 11
#define RED_LED4_PIN 12
#define RED_LED5_PIN 13
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

  // 初始化紅色LED燈條
  // pinMode(RED_LED1_PIN, OUTPUT);
  // pinMode(RED_LED2_PIN, OUTPUT);
  // pinMode(RED_LED3_PIN, OUTPUT);
  // pinMode(RED_LED4_PIN, OUTPUT);
  // pinMode(RED_LED5_PIN, OUTPUT);
  // digitalWrite(RED_LED1_PIN, HIGH);
  // digitalWrite(RED_LED2_PIN, HIGH);
  // digitalWrite(RED_LED3_PIN, HIGH);
  // digitalWrite(RED_LED4_PIN, HIGH);
  // digitalWrite(RED_LED5_PIN, HIGH);
}
// --- LED 與按鈕處理函數 ---
static bool ledState = LOW;
void toggleLedState()
{
  ledState = !ledState;
  digitalWrite(LED_PIN, !ledState);
  if (DEBUG)
    Serial.printf("[main] 按鈕觸發LED切換，LED狀態: %s\n", ledState ? "ON" : "OFF");
}

void handleButtonPress()
{
  static bool lastBtn = false;
  bool press = ec11.press();
  if (ec11.pressCount > 0 && press != lastBtn)
  {
    if (press)
    {
      toggleLedState();
    }
    lastBtn = press;
  }
}

void loop()
{
  ec11.update();
  handleButtonPress();
  ec11.update();
  delay(5);
}

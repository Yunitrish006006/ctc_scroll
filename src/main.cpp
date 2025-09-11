#include <Arduino.h>

// EC11 按鈕接腳定義
#define SW_PIN 10 // 按鈕開關針腳（已確認可用）
#define LED_PIN 8 // LED針腳（已確認可用）

// 按鈕相關變數
bool lastButtonState = HIGH;    // 上次按鈕狀態
bool currentButtonState = HIGH; // 當前按鈕狀態
int buttonPressCount = 0;       // 按鈕按下計數

// LED相關變數
bool ledState = false; // LED開關狀態

void setup()
{
  // 初始化串口通訊
  Serial.begin(115200);
  while (!Serial && millis() < 5000)
  {
    ; // 等待串口初始化
  }
  delay(1000);

  Serial.println("=== EC11 按鈕控制LED程式 ===");
  Serial.println("接腳配置:");
  Serial.println("SW  -> GPIO10 (按鈕) ✓ 已確認");
  Serial.println("LED -> GPIO8  (LED燈) ✓ 已確認");
  Serial.println("================================");

  // 設定針腳模式
  pinMode(SW_PIN, INPUT_PULLUP); // 按鈕針腳，內部上拉
  pinMode(LED_PIN, OUTPUT);      // LED針腳，輸出模式

  // 讀取初始狀態
  lastButtonState = digitalRead(SW_PIN);

  // 初始化LED為關閉狀態
  digitalWrite(LED_PIN, LOW);
  ledState = false;

  Serial.println("初始化完成！");
  Serial.print("初始按鈕狀態: ");
  Serial.println(lastButtonState == HIGH ? "未按下" : "按下");
  Serial.print("初始LED狀態: ");
  Serial.println(ledState ? "開啟" : "關閉");
  Serial.println("按下編碼器按鈕來切換LED開關 (無防抖動)");
  Serial.println("---");
}

void loop()
{
  // 讀取當前按鈕狀態
  currentButtonState = digitalRead(SW_PIN);

  // 直接檢測按鈕按下事件（從HIGH到LOW）
  if (currentButtonState == LOW && lastButtonState == HIGH)
  {
    // 按鈕被按下 - 切換LED狀態
    buttonPressCount++;
    ledState = !ledState; // 切換LED狀態

    // 更新LED
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);

    // 輸出狀態資訊
    Serial.print("*** 按鈕被按下 *** (第 ");
    Serial.print(buttonPressCount);
    Serial.println(" 次)");
    Serial.print("LED狀態切換為: ");
    Serial.println(ledState ? "開啟 💡" : "關閉 ⚫");
    Serial.print("時間戳: ");
    Serial.println(millis());
    Serial.println("---");
  }
  else if (currentButtonState == HIGH && lastButtonState == LOW)
  {
    // 按鈕被釋放
    Serial.println("按鈕被釋放");
  }

  // 更新上次狀態
  lastButtonState = currentButtonState;

  // 短暫延遲
  delay(5); // 減少延遲讓反應更快
}

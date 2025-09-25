#include <Arduino.h>
#include "ec11_module.h"
#include "mqtt_manager.h"
#include "wifi_manager.h"

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
#define RANGE 10

EC11Module ec11(CLK_PIN, DT_PIN, SW_PIN, DEBUG, RANGE, 800); // 最大滾動次數50，重置時間800ms

// MQTT 設定（請根據你的實際 broker 設定修改）
MQTTConfig mqttConfig("192.168.1.5", 1883, "ctc_scroll_client", "mqtt", "mqtt");
MQTTManager mqtt(mqttConfig, DEBUG);

WiFiManager wifi("CTC_Deco", "53537826", DEBUG);

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

  wifi.connect();
  mqtt.begin();
}
// --- LED 與按鈕處理函數 ---
static bool ledState = LOW;
void toggleLedState()
{
  ledState = !ledState;
  digitalWrite(LED_PIN, !ledState);
  if (DEBUG)
    Serial.printf("[main] 按鈕觸發LED切換，LED狀態: %s\n", ledState ? "ON" : "OFF");
  if (mqtt.isConnected())
  {
    mqtt.publish("homeassistant/light/dual/11/b/set", ledState ? "ON" : "OFF");
  }
  else
  {
    Serial.println("❌ MQTT 未連接，無法發送訊息");
    mqtt.connect(); // 嘗試重連
  }
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

void handleScrollChange()
{
  static int lastScroll = 0;
  int scroll = ec11.scrollCount;
  if (scroll != lastScroll)
  {
    // 將 scroll -25~+25 轉成 0~100
    int brightness = map(scroll, -10, 10, 0, 100);
    brightness = constrain(brightness, 0, 100);
    if (mqtt.isConnected())
    {
      char buf[8];
      snprintf(buf, sizeof(buf), "%d", brightness);
      mqtt.publish("homeassistant/light/dual/11/b/set", "ON");
      mqtt.publish("homeassistant/light/dual/11/b/set/brightness", buf);
    }
    else
    {
      Serial.println("❌ MQTT 未連接，無法發送亮度");
      mqtt.connect();
    }
    lastScroll = scroll;
  }
}

void loop()
{
  ec11.update();
  handleButtonPress();
  handleScrollChange();
  mqtt.loop();
  ec11.update();
  delay(5);
}

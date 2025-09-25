#include <Arduino.h>
#include <wifi_manager.h>
#define DEBUG true

const char *SSID = "CTC_Deco";
const char *PASSWORD = "53537826";

WiFiManager wifi(SSID, PASSWORD, DEBUG);
void setup()
{
    Serial.begin(115200);
    delay(6000);
    while (!Serial && millis() < 5000)
    {
        if (DEBUG)
            Serial.println("=== 等待串口初始化 ===");
    }
    delay(1000);
    if (DEBUG)
        Serial.println("=== EC11 模組化測試 ===");

    wifi.connect();
}

void loop()
{
    wifi.printStatus();
    delay(1000);
}
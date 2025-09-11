#include "ec11_module.h"

EC11Module::EC11Module(uint8_t clk, uint8_t dt, uint8_t sw, bool debugMode, uint8_t range, uint16_t time_reset)
    : EC11Core(clk, dt, sw, debugMode), scrollCount(0), pressCount(0), lastPressState(false), scrollRange(range), resetTime(time_reset) {}

void EC11Module::update()
{
    EC11Core::update();
    // 累計滾動次數
    int s = scroll();
    bool p = press();
    if (s != 0)
    {
        if (!p)
        {
            int nextScroll = scrollCount + s;
            if (nextScroll > scrollRange)
            {
                scrollCount = scrollRange;
                if (debug)
                    Serial.printf("[EC11Module] [未按壓] 旋鈕累計超過最大值，已限制為 %d\n", scrollRange);
            }
            else if (nextScroll < -scrollRange)
            {
                scrollCount = -scrollRange;
                if (debug)
                    Serial.printf("[EC11Module] [未按壓] 旋鈕累計低於最小值，已限制為 %d\n", -scrollRange);
            }
            else
            {
                scrollCount = nextScroll;
                if (debug)
                    Serial.printf("[EC11Module] [未按壓] 旋鈕移動: %d, 累計: %d\n", s, scrollCount);
            }
        }
        else
        {
            int nextScroll = pressedScrollCount + s;
            if (nextScroll > scrollRange)
            {
                pressedScrollCount = scrollRange;
                if (debug)
                    Serial.printf("[EC11Module] [按壓中] 旋鈕累計超過最大值，已限制為: %d\n", scrollRange);
            }
            else if (nextScroll < -scrollRange)
            {
                pressedScrollCount = -scrollRange;
                if (debug)
                    Serial.printf("[EC11Module] [按壓中] 旋鈕累計低於最小值，已限制為: %d\n", -scrollRange);
            }
            else
            {
                pressedScrollCount = nextScroll;
                if (debug)
                    Serial.printf("[EC11Module] [按壓中] 旋鈕移動: %d, 累計: %d\n", s, pressedScrollCount);
            }
        }
        unsigned long now = millis();
        if (p && !lastPressState)
        {
            pressedScrollCount = scrollCount;
            if (debug)
                Serial.printf("[EC11Module] 按壓時的滾動次數: %d\n", pressedScrollCount);
            if (now - lastPressMillis > resetTime)
            {
                pressCount = 1;
                if (debug)
                    Serial.printf("[EC11Module] 按壓事件: 新計數開始 (resetTime: %d ms)\n", resetTime);
            }
            else
            {
                pressCount++;
                if (debug)
                    Serial.printf("[EC11Module] 按壓事件: 快速連擊, 當前計數: %d\n", pressCount);
            }
            lastPressMillis = now;
        }
        // 若超過resetTime未按壓，則自動歸零
        if (now - lastPressMillis > resetTime && pressCount != 0)
        {
            if (debug)
                Serial.printf("[EC11Module] 按壓計數歸零 (距離上次: %lu ms)\n", now - lastPressMillis);
            pressCount = 0;
        }
        lastPressState = p;
    }
}

void EC11Module::reset()
{
    scrollCount = 0;
    pressCount = 0;
    lastPressMillis = 0;
    pressedScrollCount = 0;
    if (debug)
        Serial.println("[EC11Module] 狀態已重置");
}

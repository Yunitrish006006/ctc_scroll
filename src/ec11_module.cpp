#include "ec11_module.h"

EC11Module::EC11Module(uint8_t clk, uint8_t dt, uint8_t sw, bool debugMode, uint8_t range, uint16_t time_reset)
    : EC11Core(clk, dt, sw, debugMode), scrollCount(0), pressCount(0), lastPressState(false), scrollRange(range), resetTime(time_reset) {}

void EC11Module::update()
{
    EC11Core::update();
    // 累計滾動次數
    int s = scroll();
    if (s != 0)
    {
        if (s < scrollRange && s > -scrollRange)
        {
            scrollCount += s;
            if (debug)
                Serial.printf("[EC11Module] 旋鈕移動: %d, 累計: %d\n", s, scrollCount);
        }
        else
        {
            if (debug)
                Serial.printf("[EC11Module] 旋鈕移動超出範圍: %d (允許範圍: %d ~ %d)\n", s, -scrollRange, scrollRange);
        }
    }
    // resetTime 內的按壓次數
    bool p = press();
    unsigned long now = millis();
    if (p && !lastPressState)
    {
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

void EC11Module::reset()
{
    scrollCount = 0;
    pressCount = 0;
    lastPressMillis = 0;
    if (debug)
        Serial.println("[EC11Module] 狀態已重置");
}

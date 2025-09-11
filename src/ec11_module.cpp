#include "ec11_module.h"

EC11Module::EC11Module(uint8_t clk, uint8_t dt, uint8_t sw, bool debugMode, uint8_t range, uint8_t time_reset)
    : EC11Core(clk, dt, sw, debugMode), scrollCount(0), pressCount(0), lastPressState(false), scrollRange(range), resetTime(time_reset) {}

void EC11Module::update()
{
    EC11Core::update();
    // 累計滾動次數
    int s = scroll();
    if (s != 0)
    {
        if (s <= scrollRange && s >= -scrollRange)
        {
            scrollCount += s;
            Serial.printf("[EC11Module] 旋鈕位置: %d\n", scrollCount);
        }
        else
            Serial.println("[EC11Module] 超出範圍\n");
    }
    // 累計按壓次數
    bool p = press();
    if (p && !lastPressState)
        pressCount++;
    lastPressState = p;
}

void EC11Module::reset()
{
    scrollCount = 0;
    pressCount = 0;
}

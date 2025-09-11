#include "ec11_module.h"

EC11Module::EC11Module(uint8_t clk, uint8_t dt, uint8_t sw, bool debugMode)
    : clkPin(clk), dtPin(dt), swPin(sw), debug(debugMode),
      lastButtonState(HIGH), lastClkState(HIGH), lastScroll(0), lastPress(false) {}

void EC11Module::begin()
{
    pinMode(clkPin, INPUT_PULLUP);
    pinMode(dtPin, INPUT_PULLUP);
    pinMode(swPin, INPUT_PULLUP);
    lastButtonState = digitalRead(swPin);
    lastClkState = digitalRead(clkPin);
    if (debug)
    {
        Serial.println("[EC11Module] 初始化完成");
    }
}

void EC11Module::update()
{
    // 處理旋鈕
    int clkState = digitalRead(clkPin);
    int dtState = digitalRead(dtPin);
    lastScroll = 0;
    if (clkState != lastClkState)
    {
        if (clkState == LOW)
        {
            if (dtState == HIGH)
            {
                lastScroll = 1;
                if (debug)
                {
                    Serial.println("[EC11Module] 旋鈕: 順時針");
                }
            }
            else
            {
                lastScroll = -1;
                if (debug)
                {
                    Serial.println("[EC11Module] 旋鈕: 逆時針");
                }
            }
        }
        lastClkState = clkState;
    }

    // 處理按鈕
    bool currentButtonState = digitalRead(swPin);
    lastPress = (currentButtonState == LOW);
    if (debug)
    {
        if (currentButtonState == LOW && lastButtonState == HIGH)
        {
            Serial.println("[EC11Module] *** 按鈕被按下 ***");
        }
        else if (currentButtonState == HIGH && lastButtonState == LOW)
        {
            Serial.println("[EC11Module] 按鈕被釋放");
        }
    }
    lastButtonState = currentButtonState;
}

int EC11Module::scroll()
{
    return lastScroll;
}

bool EC11Module::press()
{
    return lastPress;
}

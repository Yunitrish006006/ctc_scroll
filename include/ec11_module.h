#pragma once
#include <Arduino.h>

class EC11Module
{
public:
    // 針腳定義
    uint8_t clkPin;
    uint8_t dtPin;
    uint8_t swPin;
    bool debug;

    // 狀態
    bool lastButtonState;
    int lastClkState;
    int lastScroll;
    bool lastPress;

    EC11Module(uint8_t clk, uint8_t dt, uint8_t sw, bool debugMode = false);
    void begin();
    void update();
    int scroll(); // 右轉+1，左轉-1，無動作0
    bool press(); // 按下true，未按下false
};

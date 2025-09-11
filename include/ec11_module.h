#pragma once
#include "ec11_core.h"

// EC11Module: 擴充狀態紀錄的EC11模組
class EC11Module : public EC11Core
{
public:
    // 擴充狀態變數
    int scrollCount;               // 累計滾動次數
    int pressedScrollCount;        // 按壓時的滾動次數
    int pressCount;                // resetTime 內的按壓次數
    unsigned long lastPressMillis; // 上一次按壓的時間
    bool lastPressState;           // 上一次的按壓狀態
    uint8_t scrollRange;           // 最大滾動次數
    uint16_t resetTime;            // 重置時間

    EC11Module(uint8_t clk, uint8_t dt, uint8_t sw, bool debugMode = false, uint8_t scrollRange = 10, uint16_t resetTime = 800);
    void update();
    void reset();
};

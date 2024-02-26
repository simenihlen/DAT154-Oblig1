#pragma once
#include "framework.h"

class Car {
private:
    bool horizontal;
    float progress;
    COLORREF color;

public:
    explicit Car(bool _horizontal, COLORREF _color = RGB(255, 0, 0));
    float getProgress();
    bool isHorizontal();
    void move(float amount);
    void draw(HWND hWnd, HDC hdc);
};
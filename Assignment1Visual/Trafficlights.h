#pragma once
#include "framework.h"
class Trafficlights
{
private:
    int state;
    bool horizontal;

    int RED = 0;
    int YELLOW = 1;
    int GREEN = 2;

    COLORREF colorStates[4][3] = {
            {RGB(255,0,0), RGB(125,125,125), RGB(125,125,125)},
            {RGB(255,0,0), RGB(255,255,0), RGB(125,125,125)},
            {RGB(125,125,125), RGB(125,125,125), RGB(0,255,0)},
            {RGB(125,125,125), RGB(255,255,0), RGB(125,125,125)},
    };

public:
    int x;
    int y;

    Trafficlights(int _state, int _x, int _y, bool _horizontal);

    int getState();

    COLORREF getStateColor(int position);

    bool isHorizontal();

    void progressState();
    void draw(HDC hdc);
};
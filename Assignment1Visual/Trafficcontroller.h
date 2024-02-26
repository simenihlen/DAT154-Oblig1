#pragma once
#include "Trafficlights.h"
#include "Car.h"
#include <vector>
#include <deque>

using namespace std;

class Trafficcontroller {
private:
    float right, left, top, bottom = 0;

    float pw, pn = 0;

    int cycleState, cycleProgress = 0;

    int cycleLength[4] = {4, 1, 4, 1};

    deque<Trafficlights> tl = {};
    deque<Car> cars = {};

public:
    Trafficcontroller();

    Trafficcontroller(float _right, float _left, float _top, float _bottom);

    void addLight(int state, int x, int y, bool horizontal);
    void addCar(bool horizontal);

    void cycle();

    bool withinBounds(Car car);
    bool withinStoppingBounds(Car car);

    void setCarFlow(float change, bool horizontal);
    float getProbability(bool horizontal);

    void progressLights();
    void progressCars(float amount);

    Trafficlights findLight(Car car);

    void draw(HWND hWnd, HDC hdc);
};
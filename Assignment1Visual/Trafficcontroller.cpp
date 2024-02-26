//
// Created by Bruker on 09/02/2024.
//

#include "Trafficcontroller.h"
#include <iostream>
#include <cstring>

Trafficcontroller::Trafficcontroller() {};

Trafficcontroller::Trafficcontroller(float _right, float _left, float _top, float _bottom) {
    right = _right;
    left = _left;
    top = _top;
    bottom = _bottom;
}

void Trafficcontroller::addCar(bool horizontal) {
    cars.emplace_front(Car(horizontal)); //emplace instead of push
}

void Trafficcontroller::addLight(int state, int x, int y, bool horizontal) {
    tl.emplace_back(Trafficlights(state, x, y, horizontal));
}

void Trafficcontroller::cycle() {
    if (cycleLength[cycleState] == cycleProgress) {
        cycleState = (cycleState + 1) % 4;
        cycleProgress = 0;
    }
    else {
        cycleProgress += 1;
    }
    OutputDebugStringW(L"state: ");
    OutputDebugStringW(std::to_wstring(cycleState).c_str());
    OutputDebugStringW(L"\n");

    OutputDebugStringW(L"stateP: ");
    OutputDebugStringW(std::to_wstring(cycleProgress).c_str());
    OutputDebugStringW(L"\n");
}

void Trafficcontroller::progressLights() {
    bool progressable = true;
    for (auto& c : cars) {
        if (withinBounds(c)) {
            progressable = false;
        }
    }
    if (progressable) {
        cycle();
        if (cycleProgress == 0) {
            for (auto& lights : tl) {
                lights.progressState();
            }
        }
    }
}

Trafficlights Trafficcontroller::findLight(Car car) {
    for (auto& lights : tl) {
        if (lights.isHorizontal() == car.isHorizontal()) {
            return lights;
        }
    }
}

void Trafficcontroller::setCarFlow(float change, bool horizontal) {
    if (horizontal) {
        pw += change;
    }
    else {
        pn += change;
    }
}

float Trafficcontroller::getProbability(bool horizontal) {
    if (horizontal) {
        return pw;
    }
    else {
        return pn;
    }
}

bool Trafficcontroller::withinBounds(Car car) {
    if ((car.getProgress() >= left && car.getProgress() <= right) || (car.getProgress() >= top && car.getProgress() <= bottom)) {
        return true;
    }
    else {
        return false;
    }
}

bool Trafficcontroller::withinStoppingBounds(Car car) {
    if ((car.getProgress() >= left - 0.02 && car.getProgress() <= right + 0.02) || (car.getProgress() >= top - 0.02 && car.getProgress() <= bottom + 0.02)) {
        return true;
    }
    else {
        return false;
    }
}

void Trafficcontroller::progressCars(float amount) {
    for (auto car = cars.begin(); car != cars.end(); ++car) {
        if (!withinStoppingBounds(*car) || findLight(*car).getState() == 2) {
            bool movable = true;
            for (auto carsInFront = car + 1; carsInFront != cars.end(); ++carsInFront) {
                if ((car->isHorizontal() == carsInFront->isHorizontal()) && (carsInFront->getProgress() - car->getProgress()) < 0.1) {
                    movable = false;
                }
            }
            if (movable) {
                car->move(amount);
            }
        }
        if (car->getProgress() > 1) {
            car = cars.erase(car);
            if (cars.end() == car) {
                break;
            }
        }
    }
}

void Trafficcontroller::draw(HWND hWnd, HDC hdc) {
    for (auto& lights : tl) {
        lights.draw(hdc);
        wstring probability = std::to_wstring(getProbability(lights.isHorizontal()));
        TextOut(hdc, lights.x, lights.y - 20, reinterpret_cast<LPCSTR>(probability.c_str()), wcslen(probability.c_str()));
    }
    for (auto& cars : cars) {
        cars.draw(hWnd, hdc);
    }
}
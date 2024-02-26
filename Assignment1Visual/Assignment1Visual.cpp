#include "Main.h"
#include "framework.h"
#include <iostream>
#include <cstring>
#include "deque"

using namespace std;

#define MAX_LOADSTRING 100

HINSTANCE hIns;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

Trafficcontroller trafficcontroller = Trafficcontroller(0.45, 0.55, 0.45, 0.55);

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

const int SWITCH_TRAFFIC_LIGHT = 100;
const int MOVE_CAR = 200;
const int ADD_CAR = 300;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_OBLIG1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OBLIG1));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OBLIG1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCE(IDC_OBLIG1);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hIns = hInstance;

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (hWnd) {
        RECT client;
        GetClientRect(hWnd, &client);

        trafficcontroller.addLight(0, 20, 40, true);
        trafficcontroller.addLight(2, 95, 40, false);

        SetTimer(hWnd, SWITCH_TRAFFIC_LIGHT, 1000, (TIMERPROC)NULL);
        SetTimer(hWnd, MOVE_CAR, 6, (TIMERPROC)NULL);
        SetTimer(hWnd, ADD_CAR, 1000, (TIMERPROC)NULL);

        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);
    }
    return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_LBUTTONDOWN: {
        trafficcontroller.addCar(true);
        InvalidateRect(hWnd, NULL, false);
    }
    break;
    case WM_RBUTTONDOWN: {
        trafficcontroller.addCar(false);
        InvalidateRect(hWnd, NULL, false);
    }
    break;
    case WM_KEYDOWN: {
        switch (wParam) {
            case VK_LEFT:
                trafficcontroller.setCarFlow(-0.1, true);
                break;
            case VK_RIGHT:
                trafficcontroller.setCarFlow(0.1, true);
                break;
            case VK_UP:
                trafficcontroller.setCarFlow(0.1, false);
                break;
            case VK_DOWN:
                trafficcontroller.setCarFlow(-0.1, false);
                break;
        }
        break;
    }
                   break;
    case WM_TIMER: {
        switch (wParam) {
            case SWITCH_TRAFFIC_LIGHT:
            {
                trafficcontroller.progressLights();
                InvalidateRect(hWnd, NULL, false);
            }
            break;
            case MOVE_CAR:
            {
                trafficcontroller.progressCars(0.0025);
                InvalidateRect(hWnd, NULL, false);
            }
            break;
            case ADD_CAR:
            {
                if ((float)(rand() % 100) / 100 < trafficcontroller.getProbability(true))
                {
                    trafficcontroller.addCar(true);
                }
                if ((float)(rand() % 100) / 100 < trafficcontroller.getProbability(false))
                {
                    trafficcontroller.addCar(false);
                }
            }
            break;
        }
    }
    break;
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);

        switch (wmId) {
            case IDM_ABOUT:
                DialogBox(hIns, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT screen;
        if (GetClientRect(hWnd, &screen))
        {
            int wWidth = (&screen)->right;
            int wHeight = (&screen)->bottom;
            float roadWidth = 0.03;

            // lag vei
            HBRUSH hRoad = CreateSolidBrush(RGB(126, 126, 126));
            HPEN hDeletePen = CreatePen(PS_NULL, 0, 0);
            auto horg = SelectObject(hdc, hRoad); //husk å lagre SelectObject i en variabel
            SelectObject(hdc, hDeletePen);
            Rectangle(hdc, -1, wHeight / 2 - wHeight * roadWidth, wWidth + 1, wHeight / 2 + wHeight * roadWidth);
            Rectangle(hdc, wWidth / 2 - wWidth * roadWidth, -1, wWidth / 2 + wWidth * roadWidth, wHeight + 1);

            // sletting av objects
            DeleteObject(hDeletePen);
            DeleteObject(hRoad);
        }
        trafficcontroller.draw(hWnd, hdc);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
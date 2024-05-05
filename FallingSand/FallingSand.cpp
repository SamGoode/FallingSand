#include <iostream>
#include <windows.h>
#include "Screen.h"
#include "CellAuto.h"

int main() {
    SetConsoleOutputCP(437);

    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);

    DWORD consoleMode;
    GetConsoleMode(hout, &consoleMode);
    SetConsoleMode(hout, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    SetConsoleMode(hin, ENABLE_EXTENDED_FLAGS | ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

    SHORT width = 200;
    SHORT height = 50;

    SetConsoleScreenBufferSize(hout, { width, height });

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);

    GetCurrentConsoleFontEx(hout, false, &cfi);

    cfi.dwFontSize.Y = 20;
    cfi.dwFontSize.X = 11;

    SetCurrentConsoleFontEx(hout, false, &cfi);

    SMALL_RECT dim = { 0, 0, width - 1, height - 1 };

    if (!SetConsoleWindowInfo(hout, true, &dim)) {
        cfi.dwFontSize.Y = 16;
        cfi.dwFontSize.X = 9;

        SetCurrentConsoleFontEx(hout, false, &cfi);

        SetConsoleWindowInfo(hout, true, &dim);
    }

    INPUT_RECORD inputRecord;
    DWORD events;

    Screen screen = Screen(width, height);

    //game ticks per second
    int tickrate = 40;

    int mouseX = 0;
    int mouseY = 0;
    bool mousePressed = false;

    CellAuto cellAuto = CellAuto(50, 5, 60, 40);

    srand(449139516);

    while (true) {
        GetNumberOfConsoleInputEvents(hin, &events);

        if (events > 0) {
            ReadConsoleInput(hin, &inputRecord, 1, &events);
            switch (inputRecord.EventType) {
                case MOUSE_EVENT:
                    mouseX = inputRecord.Event.MouseEvent.dwMousePosition.X;
                    mouseY = inputRecord.Event.MouseEvent.dwMousePosition.Y;

                    if (inputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                        mousePressed = true;
                    }
                    else {
                        mousePressed = false;
                    }
                    break;
            }
        }

        if (mousePressed) {
            cellAuto.toggleCell(cellAuto.getMouseGridPos(mouseX, mouseY));
        }

        screen.reset();

        cellAuto.display(screen);

        pos gridPos = cellAuto.getMouseGridPos(mouseX, mouseY);
        screen.text("Mouse screen pos: x:" + std::to_string(mouseX) + ", y:" + std::to_string(mouseY), 2, 1);
        screen.text("Mouse grid pos: x:" + std::to_string(gridPos.x) + ", y:" + std::to_string(gridPos.y), 2, 2);

        screen.text("Hovered cell state: " + std::to_string(cellAuto.getCellState(cellAuto.getMouseGridPos(mouseX, mouseY))), 2, 7);
        
        screen.print();

        cellAuto.updateCells();

        Sleep(1000 / tickrate);
    }
}
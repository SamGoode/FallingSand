#include "CellAuto.h"

CellAuto::CellAuto() {
    x = 0;
    y = 0;
    width = 0;
    height = 0;

    cells = new bool* [height];
    for (int i = 0; i < height; i++) {
        cells[i] = new bool[width];
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cells[i][j] = false;
        }
    }
}

CellAuto::CellAuto(const CellAuto& cellAuto) {
    x = cellAuto.x;
    y = cellAuto.y;
    width = cellAuto.width;
    height = cellAuto.height;

    cells = new bool* [height];
    for (int i = 0; i < height; i++) {
        cells[i] = new bool[width];
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cells[i][j] = cellAuto.cells[i][j];
        }
    }
}

CellAuto::CellAuto(int x, int y, int width, int height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    cells = new bool* [height];
    for (int i = 0; i < height; i++) {
        cells[i] = new bool[width];
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cells[i][j] = false;
        }
    }
}

CellAuto::~CellAuto() {
    for (int i = 0; i < height; i++) {
        delete[] cells[i];
    }
    delete[] cells;
}

//returns {-1, -1} if out of cell grid
pos CellAuto::getMouseGridPos(int mouseX, int mouseY) {
    if (mouseX < x || mouseX > x + (width * 2) - 1 || mouseY < y || mouseY > y + height - 1) {
        return { -1, -1 };
    }

    return { (mouseX - x) / 2, mouseY - y };
}

bool CellAuto::toggleCell(int gridPosX, int gridPosY) {
    if (gridPosX < 0 || gridPosY < 0) {
        return false;
    }

    cells[gridPosY][gridPosX] = !cells[gridPosY][gridPosX];
    return true;
}

bool CellAuto::toggleCell(pos gridPos) {
    if (gridPos.x < 0 || gridPos.y < 0) {
        return false;
    }

    cells[gridPos.y][gridPos.x] = !cells[gridPos.y][gridPos.x];
    return true;
}

//returns -1 if gridPos is out of cell grid bounds
int CellAuto::getCellState(pos gridPos) {
    if (gridPos.x < 0 || gridPos.x >(width - 1) || gridPos.y < 0 || gridPos.y >(height - 1)) {
        return -1;
    }

    return cells[gridPos.y][gridPos.x];
}

//by rewriting the logic here, I can reproduce any kind of cellular automata
//including falling sand
void CellAuto::updateCells() {
    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            if (!cells[i][j]) {
                continue;
            }
            
            if (getCellState({ j, i + 1 }) == 0) {
                cells[i][j] = false;
                cells[i + 1][j] = true;
                continue;
            }
            
            switch (rand() / (RAND_MAX / 2)) {
                case 0:
                    if (getCellState({ j - 1, i + 1 }) == 0) {
                        cells[i][j] = false;
                        cells[i + 1][j - 1] = true;
                    }
                    else if (getCellState({ j + 1, i + 1 }) == 0) {
                        cells[i][j] = false;
                        cells[i + 1][j + 1] = true;
                    }
                    break;
                case 1:
                    if (getCellState({ j + 1, i + 1 }) == 0) {
                        cells[i][j] = false;
                        cells[i + 1][j + 1] = true;
                    }
                    else if (getCellState({ j - 1, i + 1 }) == 0) {
                        cells[i][j] = false;
                        cells[i + 1][j - 1] = true;
                    }
                    break;
            }
        }
    }
}

void CellAuto::display(Screen& screen) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (cells[i][j]) {
                screen.input(219, x + (j * 2), y + i);
                screen.input(219, x + 1 + (j * 2), y + i);
                continue;
            }

            screen.input('[', x + (j * 2), y + i);
            screen.input(']', x + 1 + (j * 2), y + i);
        }
    }
}
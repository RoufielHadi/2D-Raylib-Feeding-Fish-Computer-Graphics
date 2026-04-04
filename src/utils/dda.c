/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "dda.h"
#include <math.h>
#include <stdlib.h>

/* ======================
Fungsi DrawHorizontalPixels
=======================
Fungsi ini digunakan untuk menggambar horizontal pixels.
*/
static void DrawHorizontalPixels(int x1, int x2, int y, Color color) {
    if (x2 < x1) {
        int tmp = x1;
        x1 = x2;
        x2 = tmp;
    }
    for (int x = x1; x <= x2; x++) {
        DrawPixel(x, y, color);
    }
}

/* ======================
Fungsi DrawVerticalPixels
=======================
Fungsi ini digunakan untuk menggambar vertical pixels.
*/
static void DrawVerticalPixels(int x, int y1, int y2, Color color) {
    if (y2 < y1) {
        int tmp = y1;
        y1 = y2;
        y2 = tmp;
    }
    for (int y = y1; y <= y2; y++) {
        DrawPixel(x, y, color);
    }
}

/* ======================
Fungsi DDALine
=======================
Fungsi ini digunakan untuk menjalankan proses DDALine.
*/
void DDALine(int x1, int y1, int x2, int y2, Color color) {
    if (y1 == y2) {
        DrawHorizontalPixels(x1, x2, y1, color);
        return;
    }
    if (x1 == x2) {
        DrawVerticalPixels(x1, y1, y2, color);
        return;
    }

    int dx = x2 - x1, dy = y2 - y1;
    int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);
    if (steps == 0) { DrawPixel(x1, y1, color); return; }
    float xInc = (float)dx / steps;
    float yInc = (float)dy / steps;
    float x = (float)x1, y = (float)y1;
    for (int i = 0; i <= steps; i++) {
        DrawPixel((int)roundf(x), (int)roundf(y), color);
        x += xInc; y += yInc;
    }
}

/* ======================
Fungsi DDA_DashedLine
=======================
Fungsi ini digunakan untuk menjalankan proses DDA_DashedLine.
*/
void DDA_DashedLine(int x1, int y1, int x2, int y2,
                    int dashLen, int gapLen, Color color) {
    int dx = x2 - x1, dy = y2 - y1;
    int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);
    if (steps == 0) { DrawPixel(x1, y1, color); return; }

    float xInc = (float)dx / steps;
    float yInc = (float)dy / steps;
    float x = (float)x1, y = (float)y1;

    int counter = 0;
    int drawing = 1;
    int current_limit = dashLen;

    for (int i = 0; i <= steps; i++) {
        if (drawing) {
            DrawPixel((int)roundf(x), (int)roundf(y), color);
        }
        x += xInc; y += yInc;

        counter++;
        if (counter >= current_limit) {
            counter = 0;
            drawing = !drawing;
            current_limit = drawing ? dashLen : gapLen;
        }
    }
}

/* ======================
Fungsi DDA_ThickLine
=======================
Fungsi ini digunakan untuk menjalankan proses DDA_ThickLine.
*/
void DDA_ThickLine(int x1, int y1, int x2, int y2, int thick, Color color) {
    float dx = (float)(x2-x1), dy = (float)(y2-y1);
    float len = sqrtf(dx*dx + dy*dy);
    if (len == 0) return;
    float px = -dy/len, py = dx/len;
    int half = thick/2;
    for (int t = -half; t <= half; t++) {
        int ox = (int)roundf(px*t), oy = (int)roundf(py*t);
        DDALine(x1+ox, y1+oy, x2+ox, y2+oy, color);
    }
}

/* ======================
Fungsi DDA_DashDotLine
=======================
Fungsi ini digunakan untuk menjalankan proses DDA_DashDotLine.
*/
void DDA_DashDotLine(int x1, int y1, int x2, int y2, Color color) {
    int dx = x2 - x1, dy = y2 - y1;
    int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);
    if (steps == 0) { DrawPixel(x1, y1, color); return; }

    float xInc = (float)dx / steps;
    float yInc = (float)dy / steps;
    float x = (float)x1, y = (float)y1;

    int phases[] = {18, 5, 5, 5};
    int drawPh[] = {1, 0, 1, 0};
    int phase = 0;
    int counter = 0;

    for (int i = 0; i <= steps; i++) {
        if (drawPh[phase % 4]) {
            DrawPixel((int)roundf(x), (int)roundf(y), color);
        }
        x += xInc; y += yInc;

        counter++;
        if (counter >= phases[phase % 4]) {
            counter = 0;
            phase++;
        }
    }
}

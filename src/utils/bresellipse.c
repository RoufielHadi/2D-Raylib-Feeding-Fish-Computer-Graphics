#include "bresellipse.h"

static void DrawEllipsePoints(int cx, int cy, int x, int y, Color color) {
    DrawPixel(cx + x, cy + y, color);
    DrawPixel(cx - x, cy + y, color);
    DrawPixel(cx + x, cy - y, color);
    DrawPixel(cx - x, cy - y, color);
}

void BresenhamEllipse(int cx, int cy, int rx, int ry, Color color) {
    if (rx == 0 || ry == 0) return;

    long rx2 = (long)rx * rx;
    long ry2 = (long)ry * ry;

    int x = 0;
    int y = ry;
    long d1 = ry2 - rx2 * ry + rx2 / 4;

    while (ry2 * x < rx2 * y) {
        DrawEllipsePoints(cx, cy, x, y, color);

        x++;
        if (d1 < 0) {
            d1 += ry2 * (2 * x + 3);
        } else {
            y--;
            d1 += ry2 * (2 * x + 3) - rx2 * (2 * y + 2);
        }
    }

    long d2 = ry2 * (x * x + x) + rx2 * (y * y - 2 * y + 1) - rx2 * ry2;

    while (y >= 0) {
        DrawEllipsePoints(cx, cy, x, y, color);

        y--;
        if (d2 > 0) {
            d2 += rx2 * (3 - 2 * y);
        } else {
            x++;
            d2 += ry2 * (2 * x + 2) + rx2 * (3 - 2 * y);
        }
    }
}

#include "midellips.h"

static void DrawEllipsePoints(int cx, int cy, int x, int y, Color color) {
    DrawPixel(cx + x, cy + y, color);
    DrawPixel(cx - x, cy + y, color);
    DrawPixel(cx + x, cy - y, color);
    DrawPixel(cx - x, cy - y, color);
}

void MidpointEllipse(int cx, int cy, int rx, int ry, Color color) {
    if (rx == 0 || ry == 0) return;

    long rx2 = (long)rx * rx;
    long ry2 = (long)ry * ry;

    int x = 0;
    int y = ry;
    long p1 = 4 * ry2 - 4 * rx2 * ry + rx2;

    while (ry2 * x < rx2 * y) {
        DrawEllipsePoints(cx, cy, x, y, color);

        x++;
        if (p1 < 0) {
            p1 += 4 * ry2 * (2 * x + 3);
        } else {
            y--;
            p1 += 4 * ry2 * (2 * x + 3) - 4 * rx2 * (2 * y + 2);
        }
    }

    long p2 = ry2 * (2 * x + 1) * (2 * x + 1) + 4 * rx2 * (y - 1) * (y - 1) - 4 * rx2 * ry2;

    while (y >= 0) {
        DrawEllipsePoints(cx, cy, x, y, color);

        y--;
        if (p2 > 0) {
            p2 += 4 * rx2 * (-2 * y + 3);
        } else {
            x++;
            p2 += 4 * ry2 * (2 * x + 2) + 4 * rx2 * (-2 * y + 3);
        }
    }
}

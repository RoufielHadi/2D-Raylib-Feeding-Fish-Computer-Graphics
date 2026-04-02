#define PRIMITIVES_NO_RAYLIB_ALIASES
#include "primitives.h"

#include "bresenham.h"
#include "bresellipse.h"
#include "dda.h"
#include "midcircle.h"
#include "midellips.h"

#include <math.h>

typedef struct {
    float x;
    float y;
} SortablePoint;

static int RoundToInt(float value) {
    return (int)lroundf(value);
}

static void SwapPoint(SortablePoint *a, SortablePoint *b) {
    SortablePoint tmp = *a;
    *a = *b;
    *b = tmp;
}

static Color LerpColor(Color from, Color to, float t) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    Color out = {0};
    out.r = (unsigned char)lroundf(from.r + (to.r - from.r) * t);
    out.g = (unsigned char)lroundf(from.g + (to.g - from.g) * t);
    out.b = (unsigned char)lroundf(from.b + (to.b - from.b) * t);
    out.a = (unsigned char)lroundf(from.a + (to.a - from.a) * t);
    return out;
}

static void DrawHorizontalSpan(int x1, int x2, int y, Color color) {
    if (x2 < x1) {
        int tmp = x1;
        x1 = x2;
        x2 = tmp;
    }

    for (int x = x1; x <= x2; x++) {
        DrawPixel(x, y, color);
    }
}

static void DrawVerticalSpan(int x, int y1, int y2, Color color) {
    if (y2 < y1) {
        int tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    for (int y = y1; y <= y2; y++) {
        DrawPixel(x, y, color);
    }
}

static void FillEllipseMidpoint(int cx, int cy, int rx, int ry, Color color) {
    if (rx <= 0 || ry <= 0) return;

    long rx2 = (long)rx * rx;
    long ry2 = (long)ry * ry;
    int x = 0;
    int y = ry;
    long p1 = 4 * ry2 - 4 * rx2 * ry + rx2;

    while (ry2 * x < rx2 * y) {
        DrawHorizontalSpan(cx - x, cx + x, cy + y, color);
        if (y != 0) DrawHorizontalSpan(cx - x, cx + x, cy - y, color);

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
        DrawHorizontalSpan(cx - x, cx + x, cy + y, color);
        if (y != 0) DrawHorizontalSpan(cx - x, cx + x, cy - y, color);

        y--;
        if (p2 > 0) {
            p2 += 4 * rx2 * (-2 * y + 3);
        } else {
            x++;
            p2 += 4 * ry2 * (2 * x + 2) + 4 * rx2 * (-2 * y + 3);
        }
    }
}

static void FillFlatBottomTriangle(SortablePoint top, SortablePoint left, SortablePoint right, Color color) {
    float invSlopeLeft = (left.y != top.y) ? (left.x - top.x) / (left.y - top.y) : 0.0f;
    float invSlopeRight = (right.y != top.y) ? (right.x - top.x) / (right.y - top.y) : 0.0f;

    float curLeft = top.x;
    float curRight = top.x;
    int yStart = RoundToInt(top.y);
    int yEnd = RoundToInt(left.y);

    for (int y = yStart; y <= yEnd; y++) {
        DrawHorizontalSpan(RoundToInt(curLeft), RoundToInt(curRight), y, color);
        curLeft += invSlopeLeft;
        curRight += invSlopeRight;
    }
}

static void FillFlatTopTriangle(SortablePoint left, SortablePoint right, SortablePoint bottom, Color color) {
    float invSlopeLeft = (bottom.y != left.y) ? (bottom.x - left.x) / (bottom.y - left.y) : 0.0f;
    float invSlopeRight = (bottom.y != right.y) ? (bottom.x - right.x) / (bottom.y - right.y) : 0.0f;

    float curLeft = left.x;
    float curRight = right.x;
    int yStart = RoundToInt(left.y);
    int yEnd = RoundToInt(bottom.y);

    for (int y = yStart; y <= yEnd; y++) {
        DrawHorizontalSpan(RoundToInt(curLeft), RoundToInt(curRight), y, color);
        curLeft += invSlopeLeft;
        curRight += invSlopeRight;
    }
}

static void DrawTriangleFilled(Vector2 v1, Vector2 v2, Vector2 v3, Color color) {
    SortablePoint p1 = {v1.x, v1.y};
    SortablePoint p2 = {v2.x, v2.y};
    SortablePoint p3 = {v3.x, v3.y};

    if (p2.y < p1.y) SwapPoint(&p1, &p2);
    if (p3.y < p1.y) SwapPoint(&p1, &p3);
    if (p3.y < p2.y) SwapPoint(&p2, &p3);

    if (RoundToInt(p1.y) == RoundToInt(p3.y)) {
        float minX = fminf(p1.x, fminf(p2.x, p3.x));
        float maxX = fmaxf(p1.x, fmaxf(p2.x, p3.x));
        DrawHorizontalSpan(RoundToInt(minX), RoundToInt(maxX), RoundToInt(p1.y), color);
        return;
    }

    if (RoundToInt(p2.y) == RoundToInt(p3.y)) {
        if (p3.x < p2.x) SwapPoint(&p2, &p3);
        FillFlatBottomTriangle(p1, p2, p3, color);
        return;
    }

    if (RoundToInt(p1.y) == RoundToInt(p2.y)) {
        if (p2.x < p1.x) SwapPoint(&p1, &p2);
        FillFlatTopTriangle(p1, p2, p3, color);
        return;
    }

    float splitT = (p2.y - p1.y) / (p3.y - p1.y);
    SortablePoint split = {p1.x + (p3.x - p1.x) * splitT, p2.y};

    if (split.x < p2.x) {
        FillFlatBottomTriangle(p1, split, p2, color);
        FillFlatTopTriangle(split, p2, p3, color);
    } else {
        FillFlatBottomTriangle(p1, p2, split, color);
        FillFlatTopTriangle(p2, split, p3, color);
    }
}

static int RoundedRadius(Rectangle rec, float roundness) {
    float minSide = (rec.width < rec.height) ? rec.width : rec.height;
    int radius = RoundToInt((minSide * roundness) * 0.5f);
    int halfWidth = (int)(rec.width * 0.5f);
    int halfHeight = (int)(rec.height * 0.5f);
    if (radius < 0) radius = 0;
    if (radius > halfWidth) radius = halfWidth;
    if (radius > halfHeight) radius = halfHeight;
    return radius;
}

static int RoundedInsetForRow(int radius, int yOffset) {
    if (radius <= 0) return 0;
    if (yOffset < 0) yOffset = -yOffset;
    if (yOffset >= radius) return 0;

    float dy = (float)(radius - 1 - yOffset) + 0.5f;
    float dx = sqrtf(fmaxf(0.0f, (float)(radius * radius) - dy * dy));
    int inset = radius - (int)floorf(dx + 0.0001f);
    if (inset < 0) inset = 0;
    if (inset > radius) inset = radius;
    return inset;
}

static void DrawRoundedRectFilled(Rectangle rec, int radius, Color color) {
    int left = RoundToInt(rec.x);
    int top = RoundToInt(rec.y);
    int width = RoundToInt(rec.width);
    int height = RoundToInt(rec.height);
    if (width <= 0 || height <= 0) return;

    if (radius <= 0) {
        for (int y = top; y < top + height; y++) {
            DrawHorizontalSpan(left, left + width - 1, y, color);
        }
        return;
    }

    for (int row = 0; row < height; row++) {
        int y = top + row;
        int inset = 0;
        if (row < radius) inset = RoundedInsetForRow(radius, row);
        else if (row >= height - radius) inset = RoundedInsetForRow(radius, height - 1 - row);

        DrawHorizontalSpan(left + inset, left + width - 1 - inset, y, color);
    }
}

static void DrawRoundedRectOutline(Rectangle rec, int radius, int thickness, Color color) {
    int left = RoundToInt(rec.x);
    int top = RoundToInt(rec.y);
    int width = RoundToInt(rec.width);
    int height = RoundToInt(rec.height);
    if (width <= 0 || height <= 0 || thickness <= 0) return;

    if (radius <= 0) {
        for (int t = 0; t < thickness; t++) {
            int x1 = left + t;
            int x2 = left + width - 1 - t;
            int y1 = top + t;
            int y2 = top + height - 1 - t;
            if (x2 < x1 || y2 < y1) break;
            DrawHorizontalSpan(x1, x2, y1, color);
            if (y2 != y1) DrawHorizontalSpan(x1, x2, y2, color);
            if (y2 - y1 > 1) {
                DrawVerticalSpan(x1, y1 + 1, y2 - 1, color);
                if (x2 != x1) DrawVerticalSpan(x2, y1 + 1, y2 - 1, color);
            }
        }
        return;
    }

    for (int row = 0; row < height; row++) {
        int y = top + row;
        int outerInset = 0;
        int innerInset = 0;
        bool hasInner = (width - thickness * 2 > 0) && (height - thickness * 2 > 0);

        if (row < radius) outerInset = RoundedInsetForRow(radius, row);
        else if (row >= height - radius) outerInset = RoundedInsetForRow(radius, height - 1 - row);

        if (hasInner && row >= thickness && row < height - thickness) {
            int innerRadius = radius - thickness;
            if (innerRadius < 0) innerRadius = 0;
            int innerRow = row - thickness;
            int innerHeight = height - thickness * 2;
            if (innerRadius > 0 && innerRow < innerRadius) innerInset = RoundedInsetForRow(innerRadius, innerRow);
            else if (innerRadius > 0 && innerRow >= innerHeight - innerRadius) innerInset = RoundedInsetForRow(innerRadius, innerHeight - 1 - innerRow);
        } else {
            hasInner = false;
        }

        int outerLeft = left + outerInset;
        int outerRight = left + width - 1 - outerInset;
        if (!hasInner) {
            DrawHorizontalSpan(outerLeft, outerRight, y, color);
            continue;
        }

        int innerLeft = left + thickness + innerInset;
        int innerRight = left + width - thickness - 1 - innerInset;
        if (innerLeft > outerLeft) DrawHorizontalSpan(outerLeft, innerLeft - 1, y, color);
        if (innerRight < outerRight) DrawHorizontalSpan(innerRight + 1, outerRight, y, color);
    }
}

static int AngleWithinSweep(float angleDeg, float startAngle, float endAngle) {
    while (angleDeg < 0.0f) angleDeg += 360.0f;
    while (angleDeg >= 360.0f) angleDeg -= 360.0f;
    while (startAngle < 0.0f) startAngle += 360.0f;
    while (startAngle >= 360.0f) startAngle -= 360.0f;
    while (endAngle < 0.0f) endAngle += 360.0f;
    while (endAngle >= 360.0f) endAngle -= 360.0f;

    if (fabsf(startAngle - endAngle) < 0.001f) return 1;
    if (startAngle < endAngle) return angleDeg >= startAngle && angleDeg <= endAngle;
    return angleDeg >= startAngle || angleDeg <= endAngle;
}

static void DrawRingFilled(Vector2 center, int innerRadius, int outerRadius, float startAngle, float endAngle, Color color) {
    int cx = RoundToInt(center.x);
    int cy = RoundToInt(center.y);
    int fullSweep = (fabsf(endAngle - startAngle) >= 359.9f) || (fabsf(endAngle - startAngle) < 0.001f);

    for (int y = -outerRadius; y <= outerRadius; y++) {
        float outerDxF = sqrtf(fmaxf(0.0f, (float)(outerRadius * outerRadius - y * y)));
        int outerDx = (int)floorf(outerDxF);
        int innerDx = -1;

        if (innerRadius > 0 && y >= -innerRadius && y <= innerRadius) {
            float innerDxF = sqrtf(fmaxf(0.0f, (float)(innerRadius * innerRadius - y * y)));
            innerDx = (int)ceilf(innerDxF);
        }

        int py = cy + y;
        for (int px = cx - outerDx; px <= cx + outerDx; px++) {
            int dx = px - cx;
            if (innerDx >= 0 && dx > -innerDx && dx < innerDx) continue;
            if (!fullSweep) {
                float angle = atan2f((float)-y, (float)dx) * (180.0f / PI);
                if (angle < 0.0f) angle += 360.0f;
                if (!AngleWithinSweep(angle, startAngle, endAngle)) continue;
            }
            DrawPixel(px, py, color);
        }
    }
}

void Prim_DrawLine(int startX, int startY, int endX, int endY, Color color) {
    BresenhamLine(startX, startY, endX, endY, color);
}

void Prim_DrawLineEx(Vector2 startPos, Vector2 endPos, float thick, Color color) {
    int thickness = RoundToInt(thick);
    if (thickness < 1) thickness = 1;
    Bres_ThickLine(RoundToInt(startPos.x), RoundToInt(startPos.y), RoundToInt(endPos.x), RoundToInt(endPos.y), thickness, color);
}

void Prim_DrawLineBezier(Vector2 startPos, Vector2 endPos, float thick, Color color) {
    int thickness = RoundToInt(thick);
    if (thickness < 1) thickness = 1;
    DDA_ThickLine(RoundToInt(startPos.x), RoundToInt(startPos.y), RoundToInt(endPos.x), RoundToInt(endPos.y), thickness, color);
}

void Prim_DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color) {
    DrawTriangleFilled(v1, v2, v3, color);
}

void Prim_DrawCircle(int centerX, int centerY, float radius, Color color) {
    int r = RoundToInt(radius);
    if (r < 1) r = 1;
    MidcircleFilled(centerX, centerY, r, color);
}

void Prim_DrawCircleV(Vector2 center, float radius, Color color) {
    Prim_DrawCircle(RoundToInt(center.x), RoundToInt(center.y), radius, color);
}

void Prim_DrawCircleLines(int centerX, int centerY, float radius, Color color) {
    int r = RoundToInt(radius);
    if (r < 1) r = 1;
    Midcircle(centerX, centerY, r, color);
}

void Prim_DrawEllipse(int centerX, int centerY, float radiusH, float radiusV, Color color) {
    int rx = RoundToInt(radiusH);
    int ry = RoundToInt(radiusV);
    if (rx < 1 || ry < 1) return;
    FillEllipseMidpoint(centerX, centerY, rx, ry, color);
}

void Prim_DrawRectangle(int posX, int posY, int width, int height, Color color) {
    if (width <= 0 || height <= 0) return;
    for (int y = posY; y < posY + height; y++) {
        DrawHorizontalSpan(posX, posX + width - 1, y, color);
    }
}

void Prim_DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color) {
    (void)segments;
    int radius = RoundedRadius(rec, roundness);
    DrawRoundedRectFilled(rec, radius, color);
}

void Prim_DrawRectangleRoundedLines(Rectangle rec, float roundness, int segments, Color color) {
    Prim_DrawRectangleRoundedLinesEx(rec, roundness, segments, 1.0f, color);
}

void Prim_DrawRectangleRoundedLinesEx(Rectangle rec, float roundness, int segments, float lineThick, Color color) {
    (void)segments;
    int radius = RoundedRadius(rec, roundness);
    int thickness = RoundToInt(lineThick);
    if (thickness < 1) thickness = 1;
    DrawRoundedRectOutline(rec, radius, thickness, color);
}

void Prim_DrawRectangleGradientV(int posX, int posY, int width, int height, Color top, Color bottom) {
    if (width <= 0 || height <= 0) return;
    if (height == 1) {
        DrawHorizontalSpan(posX, posX + width - 1, posY, top);
        return;
    }

    for (int row = 0; row < height; row++) {
        float t = (float)row / (float)(height - 1);
        DrawHorizontalSpan(posX, posX + width - 1, posY + row, LerpColor(top, bottom, t));
    }
}

void Prim_DrawRing(Vector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, Color color) {
    (void)segments;
    int inner = RoundToInt(innerRadius);
    int outer = RoundToInt(outerRadius);
    if (inner < 0) inner = 0;
    if (outer < inner) outer = inner;
    if (outer <= 0) return;
    DrawRingFilled(center, inner, outer, startAngle, endAngle, color);
}

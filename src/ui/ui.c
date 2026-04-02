#include "ui.h"

void UI_GetButtonRects(Rectangle outRects[6]) {
    int w = GetScreenWidth();
    float margin = 18.0f;
    float top = 14.0f;
    float btnH = 72.0f;
    float gap = 16.0f;
    float leftBtnW = 170.0f;
    float resetBtnW = 178.0f;
    float rightBtnW = 166.0f;
    float backBtnW = 122.0f;

    outRects[0] = (Rectangle){margin + 0.0f * (leftBtnW + gap), top, leftBtnW, btnH};
    outRects[1] = (Rectangle){margin + 1.0f * (leftBtnW + gap), top, leftBtnW, btnH};
    outRects[2] = (Rectangle){margin + 2.0f * (leftBtnW + gap), top, leftBtnW, btnH};
    outRects[3] = (Rectangle){margin + 3.0f * (leftBtnW + gap), top, resetBtnW, btnH};

    outRects[4] = (Rectangle){(float)w - margin - backBtnW - gap - rightBtnW, top, rightBtnW, btnH};
    outRects[5] = (Rectangle){(float)w - margin - backBtnW, top, backBtnW, btnH};
}

UIButtonId UI_HitTest(Vector2 point) {
    Rectangle rects[6];
    UI_GetButtonRects(rects);
    for (int i = 0; i < 6; i++) {
        if (!CheckCollisionPointRec(point, rects[i])) continue;
        switch (i) {
            case 0: return UI_BTN_ADD_GUPPY;
            case 1: return UI_BTN_ADD_CARNIVORE;
            case 2: return UI_BTN_ADD_ULTRAVORE;
            case 3: return UI_BTN_RESET;
            case 4: return UI_BTN_VOLUME;
            case 5: return UI_BTN_BACK;
            default: break;
        }
    }
    return UI_BTN_NONE;
}

static bool s_volumeOpen = false;
static float s_volume = 0.6f;

void UI_ToggleVolume(void) { s_volumeOpen = !s_volumeOpen; }
bool UI_IsVolumeOpen(void) { return s_volumeOpen; }
float UI_GetVolume(void) { return s_volume; }
void UI_SetVolume(float v) { if (v < 0.0f) v = 0.0f; if (v > 1.0f) v = 1.0f; s_volume = v; }

Rectangle UI_GetVolumeSliderRect(void) {
    Rectangle rects[6];
    UI_GetButtonRects(rects);
    return (Rectangle){rects[4].x, 92.0f, rects[4].width + rects[5].width + 16.0f, 16.0f};
}

void UI_HandleVolumeClick(Vector2 point) {
    Rectangle sr = UI_GetVolumeSliderRect();
    if (CheckCollisionPointRec(point, sr)) {
        float rel = (point.x - sr.x) / sr.width;
        if (rel < 0) rel = 0;
        if (rel > 1) rel = 1;
        s_volume = rel;
    }
}

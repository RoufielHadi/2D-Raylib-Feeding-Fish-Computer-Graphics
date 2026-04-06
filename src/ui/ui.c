/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "ui.h"

/* ======================
Fungsi UI_GetButtonRects
=======================
Fungsi ini digunakan untuk mengambil button rects.
*/
void UI_GetButtonRects(Rectangle outRects[9]) {
    int w = GetScreenWidth();
    float margin = 18.0f;
    float top = 14.0f;
    float btnH = 72.0f;
    float gap = 12.0f;
    float leftBtnW = 148.0f;
    float resetBtnW = 154.0f;
    float utilBtnW = 118.0f;
    float backBtnW = 124.0f;

    outRects[0] = (Rectangle){margin + 0.0f * (leftBtnW + gap), top, leftBtnW, btnH};
    outRects[1] = (Rectangle){margin + 1.0f * (leftBtnW + gap), top, leftBtnW, btnH};
    outRects[2] = (Rectangle){margin + 2.0f * (leftBtnW + gap), top, leftBtnW, btnH};
    outRects[3] = (Rectangle){margin + 3.0f * (leftBtnW + gap), top, resetBtnW, btnH};

    outRects[8] = (Rectangle){(float)w - margin - backBtnW, top, backBtnW, btnH};
    outRects[7] = (Rectangle){outRects[8].x - gap - utilBtnW, top, utilBtnW, btnH};
    outRects[6] = (Rectangle){outRects[7].x - gap - utilBtnW, top, utilBtnW, btnH};
    outRects[5] = (Rectangle){outRects[6].x - gap - utilBtnW, top, utilBtnW, btnH};
    outRects[4] = (Rectangle){outRects[5].x - gap - utilBtnW, top, utilBtnW, btnH};
}

/* ======================
Fungsi UI_HitTest
=======================
Fungsi ini digunakan untuk menjalankan proses UI_HitTest.
*/
UIButtonId UI_HitTest(Vector2 point) {
    Rectangle rects[9];
    UI_GetButtonRects(rects);
    for (int i = 0; i < 9; i++) {
        if (!CheckCollisionPointRec(point, rects[i])) continue;
        switch (i) {
            case 0: return UI_BTN_ADD_GUPPY;
            case 1: return UI_BTN_ADD_CARNIVORE;
            case 2: return UI_BTN_ADD_ULTRAVORE;
            case 3: return UI_BTN_RESET;
            case 4: return UI_BTN_BUBBLE;
            case 5: return UI_BTN_AUTOSPEED;
            case 6: return UI_BTN_WIREFRAME;
            case 7: return UI_BTN_VOLUME;
            case 8: return UI_BTN_BACK;
            default: break;
        }
    }
    return UI_BTN_NONE;
}

static bool s_volumeOpen = false;
static float s_volume = 0.6f;

/* ======================
Fungsi UI_ToggleVolume
=======================
Fungsi ini digunakan untuk menjalankan proses UI_ToggleVolume.
*/
void UI_ToggleVolume(void) { s_volumeOpen = !s_volumeOpen; }

/* ======================
Fungsi UI_IsVolumeOpen
=======================
Fungsi ini digunakan untuk memeriksa volume open.
*/
bool UI_IsVolumeOpen(void) { return s_volumeOpen; }

/* ======================
Fungsi UI_GetVolume
=======================
Fungsi ini digunakan untuk mengambil volume.
*/
float UI_GetVolume(void) { return s_volume; }

/* ======================
Fungsi UI_SetVolume
=======================
Fungsi ini digunakan untuk mengatur volume.
*/
void UI_SetVolume(float v) { if (v < 0.0f) v = 0.0f; if (v > 1.0f) v = 1.0f; s_volume = v; }

/* ======================
Fungsi UI_GetVolumeSliderRect
=======================
Fungsi ini digunakan untuk mengambil volume slider rect.
*/
Rectangle UI_GetVolumeSliderRect(void) {
    Rectangle rects[9];
    UI_GetButtonRects(rects);
    return (Rectangle){rects[4].x, 92.0f, rects[8].x + rects[8].width - rects[4].x, 16.0f};
}

/* ======================
Fungsi UI_HandleVolumeClick
=======================
Fungsi ini digunakan untuk menangani volume click.
*/
void UI_HandleVolumeClick(Vector2 point) {
    Rectangle sr = UI_GetVolumeSliderRect();
    if (CheckCollisionPointRec(point, sr)) {
        float rel = (point.x - sr.x) / sr.width;
        if (rel < 0) rel = 0;
        if (rel > 1) rel = 1;
        s_volume = rel;
    }
}

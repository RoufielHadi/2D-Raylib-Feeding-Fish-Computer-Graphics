#ifndef SRC_UI_UI_H
#define SRC_UI_UI_H

#include "raylib.h"

typedef enum {
    UI_BTN_NONE = 0,
    UI_BTN_ADD_GUPPY,
    UI_BTN_ADD_CARNIVORE,
    UI_BTN_ADD_ULTRAVORE,
    UI_BTN_RESET,
    UI_BTN_VOLUME,
    UI_BTN_BACK
} UIButtonId;

void UI_GetButtonRects(Rectangle outRects[6]);
UIButtonId UI_HitTest(Vector2 point);
void UI_ToggleVolume(void);
bool UI_IsVolumeOpen(void);
float UI_GetVolume(void);
void UI_SetVolume(float v);
Rectangle UI_GetVolumeSliderRect(void);
void UI_HandleVolumeClick(Vector2 point);

#endif

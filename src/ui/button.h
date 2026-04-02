#ifndef SRC_UI_BUTTON_H
#define SRC_UI_BUTTON_H

#include "raylib.h"
#include <stdbool.h>

void DrawButton(Rectangle r, const char *label);
void UI_SetSharedFont(Font font, bool loaded);
Font UI_GetSharedFont(void);
bool UI_HasSharedFont(void);

#endif

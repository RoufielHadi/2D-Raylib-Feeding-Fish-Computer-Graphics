#ifndef APP_MENU_CONTENT_H
#define APP_MENU_CONTENT_H

#include "raylib.h"
#include <stdbool.h>

float Menu_GetHowToPlayContentHeight(void);
void Menu_DrawHowToPlayContent(Rectangle bounds, float scroll, Font font, bool useFont, float time);
float Menu_GetAboutContentHeight(Font font, bool useFont, float width);
void Menu_DrawAboutContent(Rectangle bounds, float scroll, Font font, bool useFont, Texture2D photo, bool hasPhoto, float time);

#endif

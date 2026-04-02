#ifndef APP_MENU_ASSETS_H
#define APP_MENU_ASSETS_H

#include "raylib.h"
#include <stdbool.h>

typedef struct {
    Font font;
    bool useFont;
    Texture2D authorPhoto;
    bool hasAuthorPhoto;
    Image iconImage;
    bool hasIconImage;
} MenuAssets;

void MenuAssets_Load(MenuAssets *assets);
void MenuAssets_Unload(MenuAssets *assets);

#endif

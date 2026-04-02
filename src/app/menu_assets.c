#include "menu_assets.h"

#include "ui/button.h"

void MenuAssets_Load(MenuAssets *assets) {
    assets->font = GetFontDefault();
    assets->useFont = false;
    assets->authorPhoto = (Texture2D){0};
    assets->hasAuthorPhoto = false;
    assets->iconImage = (Image){0};
    assets->hasIconImage = false;

    if (FileExists("assets/fonts/font.TTF")) {
        assets->font = LoadFontEx("assets/fonts/font.TTF", 40, 0, 0);
        assets->useFont = true;
    }

    if (FileExists("assets/Picture/Pembuat.png")) {
        assets->authorPhoto = LoadTexture("assets/Picture/Pembuat.png");
        assets->hasAuthorPhoto = true;
    }

    if (FileExists("assets/Picture/Icon.png")) {
        assets->iconImage = LoadImage("assets/Picture/Icon.png");
        assets->hasIconImage = true;
    }

    UI_SetSharedFont(assets->font, assets->useFont);
}

void MenuAssets_Unload(MenuAssets *assets) {
    if (assets->hasAuthorPhoto) {
        UnloadTexture(assets->authorPhoto);
        assets->hasAuthorPhoto = false;
    }
    if (assets->hasIconImage) {
        UnloadImage(assets->iconImage);
        assets->hasIconImage = false;
    }
    if (assets->useFont) {
        UnloadFont(assets->font);
        assets->useFont = false;
    }
}

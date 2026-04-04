/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "menu_assets.h"

#include "ui/button.h"

/* ======================
Fungsi MenuAssets_Load
=======================
Fungsi ini digunakan untuk mengelola assets load.
*/
void MenuAssets_Load(MenuAssets *assets) {
    assets->font = GetFontDefault();
    assets->useFont = false;
    assets->welcomeBackground = (Texture2D){0};
    assets->hasWelcomeBackground = false;
    assets->homeBackground = (Texture2D){0};
    assets->hasHomeBackground = false;
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

    if (FileExists("assets/Picture/Welcome.png")) {
        assets->welcomeBackground = LoadTexture("assets/Picture/Welcome.png");
        assets->hasWelcomeBackground = true;
    }

    if (FileExists("assets/Picture/MainMenu.png")) {
        assets->homeBackground = LoadTexture("assets/Picture/MainMenu.png");
        assets->hasHomeBackground = true;
    }

    if (FileExists("assets/Picture/Icon.png")) {
        assets->iconImage = LoadImage("assets/Picture/Icon.png");
        assets->hasIconImage = true;
    }

    UI_SetSharedFont(assets->font, assets->useFont);
}

/* ======================
Fungsi MenuAssets_Unload
=======================
Fungsi ini digunakan untuk mengelola assets unload.
*/
void MenuAssets_Unload(MenuAssets *assets) {
    if (assets->hasWelcomeBackground) {
        UnloadTexture(assets->welcomeBackground);
        assets->hasWelcomeBackground = false;
    }
    if (assets->hasHomeBackground) {
        UnloadTexture(assets->homeBackground);
        assets->hasHomeBackground = false;
    }
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

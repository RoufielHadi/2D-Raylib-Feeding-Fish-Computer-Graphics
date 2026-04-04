/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef APP_MENU_ASSETS_H
#define APP_MENU_ASSETS_H

#include "raylib.h"
#include <stdbool.h>

typedef struct {
    Font font;
    bool useFont;
    Texture2D welcomeBackground;
    bool hasWelcomeBackground;
    Texture2D homeBackground;
    bool hasHomeBackground;
    Texture2D authorPhoto;
    bool hasAuthorPhoto;
    Image iconImage;
    bool hasIconImage;
} MenuAssets;

/* ======================
Fungsi MenuAssets_Load
=======================
Fungsi ini digunakan untuk mengelola assets load.
*/
void MenuAssets_Load(MenuAssets *assets);

/* ======================
Fungsi MenuAssets_Unload
=======================
Fungsi ini digunakan untuk mengelola assets unload.
*/
void MenuAssets_Unload(MenuAssets *assets);

#endif

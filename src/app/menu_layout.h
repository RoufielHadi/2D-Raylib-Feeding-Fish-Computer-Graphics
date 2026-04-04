/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef APP_MENU_LAYOUT_H
#define APP_MENU_LAYOUT_H

#include "raylib.h"

typedef struct {
    Rectangle outer;
    Rectangle inner;
    Rectangle titleArea;
    Rectangle instructionBox;
    Rectangle contentBox;
    Rectangle viewport;
    Rectangle backButton;
    Rectangle scrollTrack;
} MenuPageLayout;

/* ======================
Fungsi Menu_CreateScrollableLayout
=======================
Fungsi ini digunakan untuk mengelola create scrollable layout.
*/
MenuPageLayout Menu_CreateScrollableLayout(void);

/* ======================
Fungsi Menu_CreateCenteredButton
=======================
Fungsi ini digunakan untuk mengelola create centered button.
*/
Rectangle Menu_CreateCenteredButton(float y, float width, float height);

#endif

/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef SRC_UI_UI_H
#define SRC_UI_UI_H

#include "raylib.h"

typedef enum {
    UI_BTN_NONE = 0,
    UI_BTN_ADD_GUPPY,
    UI_BTN_ADD_CARNIVORE,
    UI_BTN_ADD_ULTRAVORE,
    UI_BTN_RESET,
    UI_BTN_BUBBLE,
    UI_BTN_AUTOSPEED,
    UI_BTN_WIREFRAME,
    UI_BTN_VOLUME,
    UI_BTN_BACK
} UIButtonId;

/* ======================
Fungsi UI_GetButtonRects
=======================
Fungsi ini digunakan untuk mengambil button rects.
*/
void UI_GetButtonRects(Rectangle outRects[9]);

/* ======================
Fungsi UI_HitTest
=======================
Fungsi ini digunakan untuk menjalankan proses UI_HitTest.
*/
UIButtonId UI_HitTest(Vector2 point);

/* ======================
Fungsi UI_ToggleVolume
=======================
Fungsi ini digunakan untuk menjalankan proses UI_ToggleVolume.
*/
void UI_ToggleVolume(void);

/* ======================
Fungsi UI_IsVolumeOpen
=======================
Fungsi ini digunakan untuk memeriksa volume open.
*/
bool UI_IsVolumeOpen(void);

/* ======================
Fungsi UI_GetVolume
=======================
Fungsi ini digunakan untuk mengambil volume.
*/
float UI_GetVolume(void);

/* ======================
Fungsi UI_SetVolume
=======================
Fungsi ini digunakan untuk mengatur volume.
*/
void UI_SetVolume(float v);

/* ======================
Fungsi UI_GetVolumeSliderRect
=======================
Fungsi ini digunakan untuk mengambil volume slider rect.
*/
Rectangle UI_GetVolumeSliderRect(void);

/* ======================
Fungsi UI_HandleVolumeClick
=======================
Fungsi ini digunakan untuk menangani volume click.
*/
void UI_HandleVolumeClick(Vector2 point);

#endif

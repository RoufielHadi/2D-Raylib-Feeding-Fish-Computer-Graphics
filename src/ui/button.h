/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef SRC_UI_BUTTON_H
#define SRC_UI_BUTTON_H

#include "raylib.h"
#include <stdbool.h>

/* ======================
Fungsi DrawButton
=======================
Fungsi ini digunakan untuk menggambar button.
*/
void DrawButton(Rectangle r, const char *label);

/* ======================
Fungsi UI_SetSharedFont
=======================
Fungsi ini digunakan untuk mengatur shared font.
*/
void UI_SetSharedFont(Font font, bool loaded);

/* ======================
Fungsi UI_GetSharedFont
=======================
Fungsi ini digunakan untuk mengambil shared font.
*/
Font UI_GetSharedFont(void);

/* ======================
Fungsi UI_HasSharedFont
=======================
Fungsi ini digunakan untuk menjalankan proses UI_HasSharedFont.
*/
bool UI_HasSharedFont(void);

#endif

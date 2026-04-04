/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef APP_MENU_CONTENT_H
#define APP_MENU_CONTENT_H

#include "raylib.h"
#include <stdbool.h>

/* ======================
Fungsi Menu_SetHowToPlaySelection
=======================
Fungsi ini digunakan untuk mengelola set how to play selection.
*/
void Menu_SetHowToPlaySelection(int index);

/* ======================
Fungsi Menu_CycleHowToPlaySelection
=======================
Fungsi ini digunakan untuk mengelola cycle how to play selection.
*/
void Menu_CycleHowToPlaySelection(int delta);

/* ======================
Fungsi Menu_HandleHowToPlayClick
=======================
Fungsi ini digunakan untuk mengelola handle how to play click.
*/
void Menu_HandleHowToPlayClick(Rectangle bounds, Vector2 point);

/* ======================
Fungsi Menu_GetHowToPlayContentHeight
=======================
Fungsi ini digunakan untuk mengelola get how to play content height.
*/
float Menu_GetHowToPlayContentHeight(void);

/* ======================
Fungsi Menu_DrawHowToPlayContent
=======================
Fungsi ini digunakan untuk mengelola draw how to play content.
*/
void Menu_DrawHowToPlayContent(Rectangle bounds, float scroll, Font font, bool useFont, float time);

/* ======================
Fungsi Menu_GetAboutContentHeight
=======================
Fungsi ini digunakan untuk mengelola get about content height.
*/
float Menu_GetAboutContentHeight(Font font, bool useFont, float width);

/* ======================
Fungsi Menu_DrawAboutContent
=======================
Fungsi ini digunakan untuk mengelola draw about content.
*/
void Menu_DrawAboutContent(Rectangle bounds, float scroll, Font font, bool useFont, Texture2D photo, bool hasPhoto, float time);

#endif

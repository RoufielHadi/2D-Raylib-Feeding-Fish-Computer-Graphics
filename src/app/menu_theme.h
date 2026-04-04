/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef APP_MENU_THEME_H
#define APP_MENU_THEME_H

#include "raylib.h"
#include <stdbool.h>

typedef struct {
    Color bgTop;
    Color bgMid;
    Color bgBottom;
    Color panel;
    Color panelStrong;
    Color panelStroke;
    Color text;
    Color textSoft;
    Color accent;
    Color accentStrong;
    Color highlight;
} MenuPalette;

typedef struct {
    Rectangle bounds;
    const char *label;
} MenuButton;

/* ======================
Fungsi Menu_GetPalette
=======================
Fungsi ini digunakan untuk mengelola get palette.
*/
MenuPalette Menu_GetPalette(void);

/* ======================
Fungsi Menu_DrawBackground
=======================
Fungsi ini digunakan untuk mengelola draw background.
*/
void Menu_DrawBackground(float time);

/* ======================
Fungsi Menu_DrawBackgroundDirect
=======================
Fungsi ini digunakan untuk mengelola draw background direct.
*/
void Menu_DrawBackgroundDirect(float time);

/* ======================
Fungsi Menu_DrawShell
=======================
Fungsi ini digunakan untuk mengelola draw shell.
*/
void Menu_DrawShell(Rectangle outer, Rectangle inner);

/* ======================
Fungsi Menu_DrawPanel
=======================
Fungsi ini digunakan untuk mengelola draw panel.
*/
void Menu_DrawPanel(Rectangle rect, Color fill, Color stroke);

/* ======================
Fungsi Menu_DrawInstructionBox
=======================
Fungsi ini digunakan untuk mengelola draw instruction box.
*/
void Menu_DrawInstructionBox(Rectangle rect, Font font, bool useFont, const char *text);

/* ======================
Fungsi Menu_DrawButton
=======================
Fungsi ini digunakan untuk mengelola draw button.
*/
void Menu_DrawButton(MenuButton button, Font font, bool useFont, bool selected);

/* ======================
Fungsi Menu_ButtonPressed
=======================
Fungsi ini digunakan untuk mengelola button pressed.
*/
bool Menu_ButtonPressed(MenuButton button);

/* ======================
Fungsi Menu_DrawText
=======================
Fungsi ini digunakan untuk mengelola draw text.
*/
void Menu_DrawText(Font font, bool useFont, const char *text, Vector2 pos, float size, float spacing, Color color);

/* ======================
Fungsi Menu_MeasureText
=======================
Fungsi ini digunakan untuk mengelola measure text.
*/
Vector2 Menu_MeasureText(Font font, bool useFont, const char *text, float size, float spacing);

/* ======================
Fungsi Menu_DrawWrappedText
=======================
Fungsi ini digunakan untuk mengelola draw wrapped text.
*/
void Menu_DrawWrappedText(Font font, bool useFont, const char *text, Rectangle bounds, float size, float spacing, Color color);

/* ======================
Fungsi Menu_MeasureWrappedText
=======================
Fungsi ini digunakan untuk mengelola measure wrapped text.
*/
float Menu_MeasureWrappedText(Font font, bool useFont, const char *text, float size, float spacing, float maxWidth);

/* ======================
Fungsi Menu_DrawScrollBar
=======================
Fungsi ini digunakan untuk mengelola draw scroll bar.
*/
void Menu_DrawScrollBar(Rectangle track, float viewportHeight, float contentHeight, float scrollOffset);

#endif

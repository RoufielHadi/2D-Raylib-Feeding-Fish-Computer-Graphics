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

MenuPalette Menu_GetPalette(void);
void Menu_DrawBackground(float time);
void Menu_DrawBackgroundDirect(float time);
void Menu_DrawShell(Rectangle outer, Rectangle inner);
void Menu_DrawPanel(Rectangle rect, Color fill, Color stroke);
void Menu_DrawInstructionBox(Rectangle rect, Font font, bool useFont, const char *text);
void Menu_DrawButton(MenuButton button, Font font, bool useFont, bool selected);
bool Menu_ButtonPressed(MenuButton button);
void Menu_DrawText(Font font, bool useFont, const char *text, Vector2 pos, float size, float spacing, Color color);
Vector2 Menu_MeasureText(Font font, bool useFont, const char *text, float size, float spacing);
void Menu_DrawWrappedText(Font font, bool useFont, const char *text, Rectangle bounds, float size, float spacing, Color color);
float Menu_MeasureWrappedText(Font font, bool useFont, const char *text, float size, float spacing, float maxWidth);
void Menu_DrawScrollBar(Rectangle track, float viewportHeight, float contentHeight, float scrollOffset);

#endif

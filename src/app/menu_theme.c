/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "menu_theme.h"

#include "raylib.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

static MenuPalette s_palette = {
    {6, 39, 72, 255},
    {17, 92, 138, 255},
    {39, 154, 182, 255},
    {241, 249, 246, 232},
    {229, 240, 236, 248},
    {50, 96, 117, 255},
    {11, 36, 52, 255},
    {47, 79, 96, 255},
    {242, 178, 74, 255},
    {208, 129, 42, 255},
    {255, 245, 214, 255}
};

static RenderTexture2D s_backgroundCache = {0};
static bool s_backgroundCacheReady = false;
static int s_backgroundCacheWidth = 0;
static int s_backgroundCacheHeight = 0;

/* ======================
Fungsi DrawBackgroundLayers
=======================
Fungsi ini digunakan untuk menggambar background layers.
*/
static void DrawBackgroundLayers(int w, int h) {
    for (int y = 0; y < h; y++) {
        float t = (float)y / (float)h;
        Color line = ColorLerp(s_palette.bgTop, s_palette.bgBottom, t);
        line.g = (unsigned char)(line.g + 20.0f * sinf(t * PI));
        line.b = (unsigned char)(line.b + 20.0f * sinf(t * PI * 0.75f));
        DrawLine(0, y, w, y, line);
    }

    for (int i = 0; i < 22; i++) {
        float x = 60.0f + i * 58.0f + sinf((float)i * 0.45f) * 10.0f;
        float radius = 10.0f + (float)(i % 4) * 6.0f;
        float y = (float)h - 80.0f - fmodf(i * 30.0f, (float)h - 60.0f);
        DrawCircle((int)x, (int)y, radius, (Color){215, 245, 255, 24});
        DrawCircleLines((int)x, (int)y, radius, (Color){225, 248, 255, 55});
        DrawCircle((int)(x - radius * 0.28f), (int)(y - radius * 0.24f), radius * 0.2f, (Color){255, 255, 255, 90});
    }

    for (int i = 0; i < 5; i++) {
        float baseX = 110.0f + i * 240.0f;
        float sway = sinf((float)i) * 18.0f;
        for (int j = 0; j < 4; j++) {
            Vector2 start = {baseX + j * 12.0f, (float)h};
            Vector2 end = {baseX + j * 12.0f + sway * (0.35f + j * 0.08f), (float)h - 150.0f - j * 18.0f};
            DrawLineBezier(start, end, 6.0f - j * 0.8f, (Color){24, 112, 96, (unsigned char)(155 + j * 10)});
        }
    }

    DrawRectangleGradientV(0, h - 98, w, 98, (Color){219, 187, 124, 210}, (Color){181, 144, 86, 255});
    for (int x = 0; x < w; x += 16) {
        DrawCircle(x, h - 20 + (x % 6), 1.8f, (Color){248, 232, 187, 90});
    }
}

/* ======================
Fungsi Menu_DrawLineOfText
=======================
Fungsi ini digunakan untuk mengelola draw line of text.
*/
static void Menu_DrawLineOfText(Font font, bool useFont, const char *text, Vector2 pos, float size, float spacing, Color color) {
    if (useFont) {
        DrawTextEx(font, text, pos, size, spacing, color);
    } else {
        DrawText(text, (int)pos.x, (int)pos.y, (int)size, color);
    }
}

/* ======================
Fungsi Menu_MeasureLine
=======================
Fungsi ini digunakan untuk mengelola measure line.
*/
static Vector2 Menu_MeasureLine(Font font, bool useFont, const char *text, float size, float spacing) {
    if (useFont) {
        return MeasureTextEx(font, text, size, spacing);
    }
    return (Vector2){(float)MeasureText(text, (int)size), size};
}

/* ======================
Fungsi FlushWrappedLine
=======================
Fungsi ini digunakan untuk menjalankan proses FlushWrappedLine.
*/
static bool FlushWrappedLine(Font font, bool useFont, const char *line, float *y, Rectangle bounds,
    float size, float spacing, Color color, bool draw, bool centered) {
    float lineAdvance = size * 1.35f;
    float maxY = bounds.y + bounds.height;

    if (line[0] == '\0') {
        if (draw && *y + size > maxY) return false;
        *y += lineAdvance;
        return true;
    }

    if (draw && *y + size > maxY) return false;

    if (draw) {
        float x = bounds.x;
        if (centered) {
            float lineWidth = Menu_MeasureLine(font, useFont, line, size, spacing).x;
            x = bounds.x + (bounds.width - lineWidth) * 0.5f;
            if (x < bounds.x) x = bounds.x;
        }
        Menu_DrawLineOfText(font, useFont, line, (Vector2){x, *y}, size, spacing, color);
    }
    *y += lineAdvance;
    return true;
}

/* ======================
Fungsi WrapTextInternal
=======================
Fungsi ini digunakan untuk menjalankan proses WrapTextInternal.
*/
static float WrapTextInternal(Font font, bool useFont, const char *text, Rectangle bounds,
    float size, float spacing, Color color, bool draw, bool centered) {
    char line[1024] = {0};
    char word[256] = {0};
    int lineLen = 0;
    int wordLen = 0;
    float y = bounds.y;

    for (const char *cursor = text;; cursor++) {
        char ch = *cursor;
        bool atEnd = (ch == '\0');
        bool isSpace = (ch == ' ' || ch == '\n' || ch == '\t' || atEnd);

        if (!isSpace && wordLen < (int)sizeof(word) - 1) {
            word[wordLen++] = ch;
            word[wordLen] = '\0';
        }

        if (isSpace && wordLen > 0) {
            char candidate[1024];
            if (lineLen == 0) {
                snprintf(candidate, sizeof(candidate), "%s", word);
            } else {
                snprintf(candidate, sizeof(candidate), "%s %s", line, word);
            }

            if (Menu_MeasureLine(font, useFont, candidate, size, spacing).x > bounds.width && lineLen > 0) {
                if (!FlushWrappedLine(font, useFont, line, &y, bounds, size, spacing, color, draw, centered)) {
                    break;
                }
                snprintf(line, sizeof(line), "%s", word);
                lineLen = (int)strlen(line);
            } else {
                snprintf(line, sizeof(line), "%s", candidate);
                lineLen = (int)strlen(line);
            }

            word[0] = '\0';
            wordLen = 0;
        }

        if (ch == '\n') {
            if (!FlushWrappedLine(font, useFont, line, &y, bounds, size, spacing, color, draw, centered)) {
                break;
            }
            line[0] = '\0';
            lineLen = 0;
        }

        if (atEnd) break;
    }

    if (lineLen > 0) {
        FlushWrappedLine(font, useFont, line, &y, bounds, size, spacing, color, draw, centered);
    }

    return y - bounds.y;
}

/* ======================
Fungsi Menu_GetPalette
=======================
Fungsi ini digunakan untuk mengelola get palette.
*/
MenuPalette Menu_GetPalette(void) {
    return s_palette;
}

/* ======================
Fungsi EnsureMenuBackgroundCache
=======================
Fungsi ini digunakan untuk memastikan menu background cache.
*/
static void EnsureMenuBackgroundCache(void) {
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    if (!s_backgroundCacheReady || s_backgroundCacheWidth != w || s_backgroundCacheHeight != h) {
        if (s_backgroundCacheReady) {
            UnloadRenderTexture(s_backgroundCache);
        }
        s_backgroundCache = LoadRenderTexture(w, h);
        s_backgroundCacheReady = true;
        s_backgroundCacheWidth = w;
        s_backgroundCacheHeight = h;

        BeginTextureMode(s_backgroundCache);
        ClearBackground(BLANK);
        DrawBackgroundLayers(w, h);

        EndTextureMode();
    }
}

/* ======================
Fungsi Menu_DrawBackground
=======================
Fungsi ini digunakan untuk mengelola draw background.
*/
void Menu_DrawBackground(float time) {
    (void)time;
    EnsureMenuBackgroundCache();
    DrawTextureRec(s_backgroundCache.texture,
        (Rectangle){0.0f, 0.0f, (float)s_backgroundCache.texture.width, (float)-s_backgroundCache.texture.height},
        (Vector2){0.0f, 0.0f}, WHITE);
}

/* ======================
Fungsi Menu_DrawBackgroundDirect
=======================
Fungsi ini digunakan untuk mengelola draw background direct.
*/
void Menu_DrawBackgroundDirect(float time) {
    (void)time;
    DrawBackgroundLayers(GetScreenWidth(), GetScreenHeight());
}

/* ======================
Fungsi Menu_DrawShell
=======================
Fungsi ini digunakan untuk mengelola draw shell.
*/
void Menu_DrawShell(Rectangle outer, Rectangle inner) {
    DrawRectangleRounded(outer, 0.02f, 6, ColorAlpha(BLACK, 0.28f));
    DrawRectangleRounded(inner, 0.02f, 6, s_palette.panel);
    DrawRectangleRoundedLinesEx(inner, 0.02f, 6, 2.0f, s_palette.panelStroke);
}

/* ======================
Fungsi Menu_DrawPanel
=======================
Fungsi ini digunakan untuk mengelola draw panel.
*/
void Menu_DrawPanel(Rectangle rect, Color fill, Color stroke) {
    DrawRectangleRounded(rect, 0.02f, 6, fill);
    DrawRectangleRoundedLinesEx(rect, 0.02f, 6, 2.0f, stroke);
}

/* ======================
Fungsi Menu_DrawInstructionBox
=======================
Fungsi ini digunakan untuk mengelola draw instruction box.
*/
void Menu_DrawInstructionBox(Rectangle rect, Font font, bool useFont, const char *text) {
    DrawRectangleRounded(rect, 0.18f, 10, ColorAlpha((Color){14, 60, 87, 255}, 0.74f));
    DrawRectangleRoundedLinesEx(rect, 0.18f, 10, 1.0f, ColorAlpha(s_palette.highlight, 0.22f));
    {
        Rectangle textBounds = {rect.x + 16, rect.y + 8, rect.width - 32, rect.height - 16};
        float textHeight = WrapTextInternal(font, useFont, text, textBounds, 14.0f, 1.0f, s_palette.highlight, false, true);
        textBounds.y += (textBounds.height - textHeight) * 0.5f - 1.0f;
        WrapTextInternal(font, useFont, text, textBounds, 14.0f, 1.0f, ColorAlpha(s_palette.highlight, 0.82f), true, true);
    }
}

/* ======================
Fungsi Menu_DrawButton
=======================
Fungsi ini digunakan untuk mengelola draw button.
*/
void Menu_DrawButton(MenuButton button, Font font, bool useFont, bool selected) {
    bool hovered = CheckCollisionPointRec(GetMousePosition(), button.bounds);
    Color fill = selected || hovered ? s_palette.accent : s_palette.panelStrong;
    Color stroke = selected || hovered ? s_palette.accentStrong : s_palette.panelStroke;
    Color text = selected || hovered ? (Color){31, 27, 21, 255} : s_palette.text;
    Rectangle inner = button.bounds;

    if (hovered) {
        inner.y -= 2.0f;
    }

    DrawRectangleRounded((Rectangle){inner.x + 5, inner.y + 6, inner.width, inner.height}, 0.12f, 8, ColorAlpha(BLACK, 0.14f));
    DrawRectangleRounded(inner, 0.12f, 8, fill);
    DrawRectangleRoundedLinesEx(inner, 0.12f, 8, 2.0f, stroke);

    Vector2 labelSize = Menu_MeasureLine(font, useFont, button.label, 28.0f, 1.0f);
    Menu_DrawLineOfText(font, useFont, button.label,
        (Vector2){inner.x + (inner.width - labelSize.x) * 0.5f, inner.y + (inner.height - labelSize.y) * 0.5f - 2.0f},
        28.0f, 1.0f, text);
}

/* ======================
Fungsi Menu_ButtonPressed
=======================
Fungsi ini digunakan untuk mengelola button pressed.
*/
bool Menu_ButtonPressed(MenuButton button) {
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), button.bounds);
}

/* ======================
Fungsi Menu_DrawText
=======================
Fungsi ini digunakan untuk mengelola draw text.
*/
void Menu_DrawText(Font font, bool useFont, const char *text, Vector2 pos, float size, float spacing, Color color) {
    Menu_DrawLineOfText(font, useFont, text, pos, size, spacing, color);
}

/* ======================
Fungsi Menu_MeasureText
=======================
Fungsi ini digunakan untuk mengelola measure text.
*/
Vector2 Menu_MeasureText(Font font, bool useFont, const char *text, float size, float spacing) {
    return Menu_MeasureLine(font, useFont, text, size, spacing);
}

/* ======================
Fungsi Menu_DrawWrappedText
=======================
Fungsi ini digunakan untuk mengelola draw wrapped text.
*/
void Menu_DrawWrappedText(Font font, bool useFont, const char *text, Rectangle bounds, float size, float spacing, Color color) {
    WrapTextInternal(font, useFont, text, bounds, size, spacing, color, true, false);
}

/* ======================
Fungsi Menu_MeasureWrappedText
=======================
Fungsi ini digunakan untuk mengelola measure wrapped text.
*/
float Menu_MeasureWrappedText(Font font, bool useFont, const char *text, float size, float spacing, float maxWidth) {
    return WrapTextInternal(font, useFont, text, (Rectangle){0, 0, maxWidth, 10000}, size, spacing, s_palette.text, false, false);
}

/* ======================
Fungsi Menu_DrawScrollBar
=======================
Fungsi ini digunakan untuk mengelola draw scroll bar.
*/
void Menu_DrawScrollBar(Rectangle track, float viewportHeight, float contentHeight, float scrollOffset) {
    DrawRectangleRounded(track, 0.5f, 8, ColorAlpha((Color){31, 78, 98, 255}, 0.35f));
    DrawRectangleRoundedLinesEx(track, 0.5f, 8, 1.0f, ColorAlpha(s_palette.highlight, 0.5f));

    if (contentHeight <= viewportHeight) {
        Rectangle thumb = {track.x + 2.0f, track.y + 2.0f, track.width - 4.0f, track.height - 4.0f};
        DrawRectangleRounded(thumb, 0.5f, 8, ColorAlpha(s_palette.highlight, 0.55f));
        return;
    }

    float ratio = viewportHeight / contentHeight;
    float thumbHeight = ratio * track.height;
    if (thumbHeight < 38.0f) thumbHeight = 38.0f;
    float scrollable = contentHeight - viewportHeight;
    float progress = (scrollable <= 0.0f) ? 0.0f : scrollOffset / scrollable;
    Rectangle thumb = {
        track.x + 2.0f,
        track.y + 2.0f + (track.height - thumbHeight - 4.0f) * progress,
        track.width - 4.0f,
        thumbHeight
    };
    DrawRectangleRounded(thumb, 0.5f, 8, ColorAlpha(s_palette.highlight, 0.75f));
}

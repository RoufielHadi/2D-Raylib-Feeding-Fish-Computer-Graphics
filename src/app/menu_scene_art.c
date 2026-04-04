/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "menu_scene_art.h"

#include "raylib.h"

#include <math.h>
#include <stdbool.h>

typedef struct {
    Vector2 center;
    float scale;
    Color body;
    Color fin;
    bool facingRight;
    bool roundEye;
} ArtFish;

/* ======================
Fungsi DrawWaterBackdropBase
=======================
Fungsi ini digunakan untuk menggambar water backdrop base.
*/
static void DrawWaterBackdropBase(float time, bool framed) {
    int w = GetScreenWidth();
    int h = GetScreenHeight();

    DrawRectangleGradientV(0, 0, w, h, (Color){64, 178, 240, 255}, (Color){14, 88, 164, 255});

    for (int i = 0; i < 20; i++) {
        float y = 24.0f + i * 32.0f;
        float wave = sinf(time * 0.45f + i * 0.6f) * 10.0f;
        Color band = ColorAlpha((Color){196, 240, 255, 255}, 0.10f + (i % 3) * 0.02f);
        DrawLineBezier((Vector2){-40.0f, y + wave}, (Vector2){(float)w + 40.0f, y - wave}, 10.0f, band);
    }

    for (int i = 0; i < 8; i++) {
        float t = (float)i / 7.0f;
        float x = w * (0.18f + t * 0.64f);
        float spread = 140.0f + t * 80.0f;
        DrawTriangle(
            (Vector2){x, -10.0f},
            (Vector2){x - spread, h * 0.62f},
            (Vector2){x + spread, h * 0.62f},
            ColorAlpha(WHITE, 0.05f - t * 0.003f));
    }

    for (int i = 0; i < 30; i++) {
        float x = 70.0f + i * 46.0f + sinf(time * 0.4f + (float)i) * 8.0f;
        float r = 2.0f + (float)(i % 4) * 1.3f;
        float y = h - 90.0f - fmodf(time * (16.0f + (float)(i % 5) * 7.0f) + i * 24.0f, (float)h - 140.0f);
        DrawCircleV((Vector2){x, y}, r, ColorAlpha(WHITE, 0.22f));
        DrawCircleLines((int)x, (int)y, r, ColorAlpha((Color){210, 244, 255, 255}, 0.45f));
    }

    for (int i = 0; i < 2; i++) {
        float cx = (i == 0) ? w * 0.24f : w * 0.70f;
        float cy = h * 0.34f;
        float sx = 1.0f + i * 0.2f;
        for (int k = 0; k < 30; k++) {
            float angle = ((float)k / 30.0f) * PI * 2.0f;
            float rx = 150.0f * sx;
            float ry = 52.0f * sx;
            Vector2 p = {
                cx + cosf(angle) * rx,
                cy + sinf(angle) * ry * 0.7f
            };
            DrawEllipse((int)p.x, (int)p.y, 11, 4, ColorAlpha((Color){34, 104, 182, 255}, 0.30f));
        }
    }

    DrawRectangleGradientV(0, h - 126, w, 126, (Color){228, 207, 160, 235}, (Color){195, 159, 102, 255});
    for (int x = 0; x < w; x += 18) {
        DrawCircle(x, h - 24 + (x % 7), 2.0f, ColorAlpha((Color){247, 229, 184, 255}, 0.45f));
    }

    for (int side = 0; side < 2; side++) {
        float baseX = side == 0 ? 68.0f : (float)w - 68.0f;
        float dir = side == 0 ? 1.0f : -1.0f;
        for (int i = 0; i < 6; i++) {
            float rootX = baseX + i * 18.0f * dir;
            float rootY = (float)h - 8.0f;
            float topX = rootX + dir * (10.0f + i * 5.0f) + sinf(time * 1.2f + i * 0.8f) * 12.0f;
            float topY = h - 170.0f - i * 18.0f;
            DrawLineBezier((Vector2){rootX, rootY}, (Vector2){topX, topY}, 8.0f - i * 0.8f, (Color){52, 156, 112, 220});
        }
    }

    for (int side = 0; side < 2; side++) {
        float offsetX = side == 0 ? 40.0f : (float)w - 140.0f;
        float offsetY = h - 135.0f;
        Color coralA = side == 0 ? (Color){243, 154, 122, 255} : (Color){254, 170, 164, 255};
        Color coralB = side == 0 ? (Color){234, 112, 96, 255} : (Color){250, 139, 111, 255};
        DrawEllipse((int)(offsetX + 30), (int)(offsetY + 34), 42, 24, ColorAlpha((Color){117, 86, 155, 255}, 0.80f));
        DrawEllipse((int)(offsetX + 74), (int)(offsetY + 38), 28, 18, ColorAlpha((Color){98, 70, 139, 255}, 0.78f));
        for (int i = 0; i < 5; i++) {
            float px = offsetX + 10 + i * 18.0f;
            float py = offsetY - (float)(i % 2) * 10.0f;
            DrawEllipse((int)px, (int)py, 9, 24, coralA);
            DrawCircle((int)px, (int)(py - 22), 6, coralB);
        }
    }

    if (framed) {
        Color woodDark = (Color){119, 74, 38, 255};
        Color woodLight = (Color){160, 108, 56, 255};
        DrawRectangleGradientV(0, 0, w, 46, woodLight, woodDark);
        DrawRectangleGradientV(0, h - 46, w, 46, woodDark, woodLight);
        DrawRectangleGradientV(0, 0, 20, h, woodDark, woodLight);
        DrawRectangleGradientV(w - 20, 0, 20, h, woodLight, woodDark);
        DrawLineEx((Vector2){0.0f, 46.0f}, (Vector2){(float)w, 46.0f}, 3.0f, ColorAlpha((Color){255, 225, 160, 255}, 0.50f));
        DrawLineEx((Vector2){0.0f, (float)h - 46.0f}, (Vector2){(float)w, (float)h - 46.0f}, 3.0f, ColorAlpha((Color){72, 40, 18, 255}, 0.45f));
    }
}

/* ======================
Fungsi DrawArtFish
=======================
Fungsi ini digunakan untuk menggambar art fish.
*/
static void DrawArtFish(const ArtFish *fish) {
    float dir = fish->facingRight ? 1.0f : -1.0f;
    float s = fish->scale;
    Vector2 c = fish->center;

    DrawEllipse((int)c.x, (int)c.y, (int)(30.0f * s), (int)(18.0f * s), fish->body);
    DrawEllipse((int)(c.x + dir * 8.0f * s), (int)(c.y + 4.0f * s), (int)(16.0f * s), (int)(8.0f * s), ColorAlpha(WHITE, 0.28f));
    DrawEllipse((int)(c.x - dir * 4.0f * s), (int)(c.y - 7.0f * s), (int)(18.0f * s), (int)(6.0f * s), ColorAlpha(WHITE, 0.16f));

    DrawTriangle(
        (Vector2){c.x - dir * 20.0f * s, c.y},
        (Vector2){c.x - dir * 44.0f * s, c.y - 18.0f * s},
        (Vector2){c.x - dir * 34.0f * s, c.y - 1.0f * s},
        fish->fin);
    DrawTriangle(
        (Vector2){c.x - dir * 20.0f * s, c.y},
        (Vector2){c.x - dir * 34.0f * s, c.y + 1.0f * s},
        (Vector2){c.x - dir * 44.0f * s, c.y + 18.0f * s},
        fish->fin);

    DrawTriangle(
        (Vector2){c.x - dir * 2.0f * s, c.y - 10.0f * s},
        (Vector2){c.x + dir * 12.0f * s, c.y - 24.0f * s},
        (Vector2){c.x + dir * 22.0f * s, c.y - 8.0f * s},
        ColorAlpha(fish->fin, 0.94f));
    DrawTriangle(
        (Vector2){c.x - dir * 4.0f * s, c.y + 8.0f * s},
        (Vector2){c.x + dir * 10.0f * s, c.y + 20.0f * s},
        (Vector2){c.x + dir * 18.0f * s, c.y + 6.0f * s},
        ColorAlpha(fish->fin, 0.88f));

    DrawCircleV((Vector2){c.x + dir * 15.0f * s, c.y - 2.0f * s}, 9.5f * s, WHITE);
    DrawCircleV((Vector2){c.x + dir * 17.5f * s, c.y - 2.0f * s}, 4.8f * s, fish->roundEye ? BLACK : (Color){52, 84, 110, 255});
    DrawCircleV((Vector2){c.x + dir * 19.0f * s, c.y - 4.4f * s}, 1.8f * s, WHITE);
    DrawLineBezier((Vector2){c.x + dir * 4.0f * s, c.y + 5.0f * s}, (Vector2){c.x + dir * 22.0f * s, c.y + 6.0f * s}, 1.6f * s, ColorAlpha(BLACK, 0.12f));
}

/* ======================
Fungsi MenuArt_DrawWelcomeBackdrop
=======================
Fungsi ini digunakan untuk mengelola art draw welcome backdrop.
*/
void MenuArt_DrawWelcomeBackdrop(float time) {
    DrawWaterBackdropBase(time, false);

    ArtFish fishList[] = {
        {{180.0f, 330.0f + sinf(time * 1.4f) * 6.0f}, 1.15f, {247, 168, 52, 255}, {116, 64, 190, 255}, true, true},
        {{320.0f, 560.0f + sinf(time * 1.2f + 0.8f) * 8.0f}, 1.32f, {112, 92, 226, 255}, {255, 170, 50, 255}, true, true},
        {{980.0f, 344.0f + sinf(time * 1.3f + 0.5f) * 5.0f}, 1.22f, {247, 166, 47, 255}, {255, 137, 66, 255}, false, true},
        {{900.0f, 532.0f + sinf(time * 1.1f + 1.0f) * 8.0f}, 1.26f, {235, 54, 48, 255}, {255, 120, 82, 255}, false, true},
        {{316.0f, 742.0f + sinf(time * 1.0f + 0.6f) * 7.0f}, 1.14f, {104, 208, 74, 255}, {232, 56, 48, 255}, true, true},
        {{720.0f, 886.0f + sinf(time * 1.2f + 1.5f) * 6.0f}, 1.28f, {247, 172, 52, 255}, {236, 87, 49, 255}, false, true}
    };

    for (int i = 0; i < (int)(sizeof(fishList) / sizeof(fishList[0])); i++) {
        DrawArtFish(&fishList[i]);
    }
}

/* ======================
Fungsi MenuArt_DrawHomeBackdrop
=======================
Fungsi ini digunakan untuk mengelola art draw home backdrop.
*/
void MenuArt_DrawHomeBackdrop(float time) {
    DrawWaterBackdropBase(time, true);

    ArtFish fishList[] = {
        {{210.0f, 230.0f + sinf(time * 1.4f) * 7.0f}, 1.05f, {246, 167, 58, 255}, {255, 208, 88, 255}, true, true},
        {{410.0f, 422.0f + sinf(time * 1.2f + 0.4f) * 9.0f}, 1.18f, {78, 144, 205, 255}, {248, 150, 60, 255}, true, true},
        {{1060.0f, 252.0f + sinf(time * 1.3f + 0.8f) * 7.0f}, 0.96f, {232, 206, 138, 255}, {214, 182, 100, 255}, false, false},
        {{1120.0f, 424.0f + sinf(time * 1.1f + 1.2f) * 7.0f}, 1.08f, {242, 96, 54, 255}, {255, 182, 68, 255}, false, true},
        {{1120.0f, 606.0f + sinf(time * 1.0f + 0.5f) * 6.0f}, 0.98f, {230, 206, 136, 255}, {216, 185, 104, 255}, false, false},
        {{142.0f, 548.0f + sinf(time * 1.3f + 1.4f) * 5.0f}, 0.82f, {118, 203, 232, 255}, {255, 202, 66, 255}, true, true}
    };

    for (int i = 0; i < (int)(sizeof(fishList) / sizeof(fishList[0])); i++) {
        DrawArtFish(&fishList[i]);
    }

    DrawEllipse(260, GetScreenHeight() - 122, 56, 28, (Color){158, 122, 68, 255});
    DrawEllipse(286, GetScreenHeight() - 106, 52, 24, (Color){132, 194, 98, 255});
    DrawCircle(198, GetScreenHeight() - 84, 12, (Color){132, 180, 92, 255});
    DrawCircle(242, GetScreenHeight() - 78, 12, (Color){132, 180, 92, 255});
    DrawCircle(304, GetScreenHeight() - 80, 12, (Color){132, 180, 92, 255});
    DrawCircle(362, GetScreenHeight() - 82, 12, (Color){132, 180, 92, 255});

    {
        int turtleY = GetScreenHeight() - 102;
        DrawEllipse(332, turtleY + 12, 46, 26, (Color){165, 124, 66, 255});
        DrawEllipse(332, turtleY + 12, 40, 20, (Color){180, 142, 80, 255});
        DrawCircle(286, turtleY + 20, 18, (Color){162, 196, 106, 255});
        DrawCircle(278, turtleY + 18, 4, BLACK);
        DrawLineEx((Vector2){304.0f, (float)turtleY + 36.0f}, (Vector2){282.0f, (float)turtleY + 52.0f}, 7.0f, (Color){144, 182, 96, 255});
        DrawLineEx((Vector2){338.0f, (float)turtleY + 36.0f}, (Vector2){354.0f, (float)turtleY + 54.0f}, 7.0f, (Color){144, 182, 96, 255});
    }

    {
        float coinX = GetScreenWidth() * 0.74f;
        float coinY = GetScreenHeight() - 86.0f;
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 4 - row; col++) {
                DrawCircle((int)(coinX + col * 18.0f + row * 8.0f), (int)(coinY + row * 10.0f), 8, (Color){234, 184, 72, 255});
                DrawCircleLines((int)(coinX + col * 18.0f + row * 8.0f), (int)(coinY + row * 10.0f), 8, (Color){250, 214, 118, 255});
            }
        }
    }

    {
        float crabX = GetScreenWidth() * 0.90f;
        float crabY = (float)GetScreenHeight() - 84.0f;
        DrawCircle((int)crabX, (int)crabY, 18, (Color){240, 112, 52, 255});
        DrawCircle((int)(crabX - 24.0f), (int)(crabY + 6.0f), 6, (Color){240, 112, 52, 255});
        DrawCircle((int)(crabX + 24.0f), (int)(crabY + 6.0f), 6, (Color){240, 112, 52, 255});
        DrawLineEx((Vector2){crabX - 12.0f, crabY + 18.0f}, (Vector2){crabX - 28.0f, crabY + 30.0f}, 4.0f, (Color){240, 112, 52, 255});
        DrawLineEx((Vector2){crabX + 12.0f, crabY + 18.0f}, (Vector2){crabX + 28.0f, crabY + 30.0f}, 4.0f, (Color){240, 112, 52, 255});
    }
}

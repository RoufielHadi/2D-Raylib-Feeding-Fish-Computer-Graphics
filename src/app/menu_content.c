#include "menu_content.h"

#include "menu_theme.h"
#include "../entities/bubble.h"
#include "../entities/carnivore.h"
#include "../entities/food.h"
#include "../entities/guppy.h"
#include "../entities/ultravore.h"
#include "../render/draw_bubble.h"
#include "../render/draw_env.h"
#include "../render/draw_fish.h"
#include "../render/draw_food.h"

#include "raylib.h"

#include <math.h>

#define PRIMITIVES_NO_RAYLIB_ALIASES
#include "../utils/primitives.h"

typedef enum {
    SHOWCASE_GUPPY = 0,
    SHOWCASE_CARNIVORE,
    SHOWCASE_ULTRAVORE,
    SHOWCASE_FOOD,
    SHOWCASE_BUBBLE,
    SHOWCASE_SEAWEED,
    SHOWCASE_CORAL,
    SHOWCASE_HELMET,
    SHOWCASE_SAND,
    SHOWCASE_BACKGROUND
} ShowcaseType;

typedef struct {
    const char *label;
    const char *description;
} PartInfo;

typedef struct {
    ShowcaseType type;
    const char *name;
    const char *description;
    PartInfo parts[6];
    int partCount;
} ShowcaseInfo;

static const ShowcaseInfo kShowcases[] = {
    {SHOWCASE_GUPPY, "Ikan Guppy",
        "Ikan utama yang paling sering dipelihara pemain. Siluetnya dibuat kecil, oval, dan ringan dengan badan ellipse, perut terang, ekor kipas merah muda, serta satu sirip atas yang menyatu ke badan.",
        {{"Badan", "Ellipse oranye yang lebih lonjong untuk tubuh utama guppy."},
         {"Perut", "Ellipse terang kecil di bagian bawah depan agar badan terasa bulat."},
         {"Ekor", "Ekor kipas dua sisi dengan warna merah muda agar mirip referensi."},
         {"Sirip", "Sirip atas dan sirip bawah kecil, tanpa segitiga sisa di tengah badan."},
         {"Mata", "Circle putih, pupil hitam, dan highlight putih kecil."}}, 5},
    {SHOWCASE_CARNIVORE, "Ikan Carnivore",
        "Predator tahap menengah dengan badan bulat, punggung gelap, perut merah, garis zigzag di sisi badan, dan mulut kecil bergigi agar terlihat seperti piranha pada referensi.",
        {{"Badan", "Ellipse besar gelap dengan bentuk lebih bulat dan padat."},
         {"Perut", "Ellipse merah di bawah badan untuk kontras khas predator."},
         {"Ekor", "Ekor lebar di belakang sebagai pendorong utama renang."},
         {"Sirip", "Sirip punggung dan sirip bawah kecil untuk siluet piranha."},
         {"Mulut + Gigi", "Mulut pendek dengan gigi kecil di depan, bukan bekas gigi di badan."},
         {"Mata", "Circle kuning dengan pupil gelap untuk ekspresi galak."}}, 6},
    {SHOWCASE_ULTRAVORE, "Ikan Ultravore",
        "Predator terbesar di Feeding Fish. Bentuknya dibuat lebih memanjang dengan kepala besar, rahang terbuka, lapisan shading, dan ekor lebar. Visual ini sengaja tanpa sirip atas agar sesuai referensi terbaru.",
        {{"Badan", "Ellipse besar memanjang untuk massa tubuh utama ultravore."},
         {"Shading", "Lapisan ellipse belakang memberi volume dan kedalaman."},
         {"Ekor", "Ekor belakang besar dan lebar untuk tenaga dorong kuat."},
         {"Sirip", "Hanya sirip bawah yang dibuat tebal; tidak ada sirip atas."},
         {"Mulut + Gigi", "Rahang besar terbuka dengan gigi predator sebagai fokus utama."},
         {"Mata", "Circle merah sebagai identitas ultravore."}}, 6},
    {SHOWCASE_FOOD, "Makanan Ikan",
        "Pakan dijatuhkan pemain dengan klik kiri. Bentuknya dibuat dari dua setengah kapsul, lingkaran ujung, dan garis tengah sehingga terlihat seperti pellet dua warna.",
        {{"Kapsul Kiri", "Dua triangle membentuk bagian putih makanan."},
         {"Kapsul Kanan", "Dua triangle merah sebagai separuh kanan."},
         {"Tutup Bulat", "Circle pada kedua ujung agar membulat."},
         {"Garis Tengah", "Line tipis untuk pemisah dua warna."},
         {"Outline", "Circle line dan line tipis supaya terlihat tegas."}}, 5},
    {SHOWCASE_BUBBLE, "Bubble",
        "Elemen dekorasi yang bergerak dari bawah ke atas. Visualnya sederhana tetapi efektif: lingkaran transparan, outline, dan highlight kecil untuk efek pantulan air.",
        {{"Lingkaran", "Circle transparan sebagai gelembung utama."},
         {"Outline", "Circle line tipis untuk tepian bubble."},
         {"Highlight", "Circle kecil putih untuk pantulan cahaya."}}, 3},
    {SHOWCASE_SEAWEED, "Rumput Laut",
        "Rumput laut dibuat dari beberapa garis bezier tebal yang digerakkan sinus agar terlihat melambai. Objek ini memperkaya dasar akuarium dan memberi rasa hidup.",
        {{"Batang Dasar", "Bezier tebal dari dasar pasir menuju tengah."},
         {"Lengkung Atas", "Bezier lanjutan untuk ujung daun."},
         {"Daun Tambahan", "Beberapa helai lain dengan panjang berbeda."},
         {"Glow", "Circle transparan kecil sebagai aksen air."}}, 4},
    {SHOWCASE_CORAL, "Coral",
        "Terumbu karang di sisi kiri bawah akuarium. Disusun dari ellipse besar, circle cabang, dan panel depan sehingga tampil seperti dekorasi karang dengan detail warna kontras.",
        {{"Bonggol", "Ellipse besar ungu sebagai pondasi karang."},
         {"Lapisan", "Ellipse kedua yang lebih gelap untuk bayangan."},
         {"Cabang", "Ellipse dan circle kecil berjajar di atas."},
         {"Panel", "Rounded rectangle gelap sebagai aksen depan."}}, 4},
    {SHOWCASE_HELMET, "Helm Dekorasi",
        "Dekorasi helm di kanan bawah aquarium. Dibentuk dari lingkaran utama, visor, side pod, hatch kecil, dan baut untuk memberi kesan benda bawah laut yang tenggelam.",
        {{"Helm", "Circle utama sebagai badan helm."},
         {"Visor", "Circle tengah biru yang menjadi jendela."},
         {"Side Pod", "Dua circle samping menyerupai konektor."},
         {"Hatch", "Circle kecil di atas sebagai tutup tambahan."},
         {"Baut", "Circle kecil berjajar di bawah helm."}}, 5},
    {SHOWCASE_SAND, "Pasir Dasar",
        "Bagian dasar aquarium yang memberi landasan untuk seluruh dekorasi. Dibangun dari rectangle horizontal, strip warna bergelombang, dan titik-titik kecil seperti butiran pasir.",
        {{"Base", "Rectangle lebar berwarna pasir."},
         {"Gelombang", "Rectangle strip dengan tinggi berbeda."},
         {"Butiran", "Circle kecil transparan sebagai detail tekstur."}}, 3},
    {SHOWCASE_BACKGROUND, "Latar Air",
        "Latar belakang aquarium memakai gradient vertikal biru dan cahaya lembut di bagian atas. Komposisi ini menjaga nuansa bawah laut tanpa memakai gambar bitmap.",
        {{"Gradient", "Rangkaian line horizontal dari biru gelap ke terang."},
         {"Glow", "Ellipse transparan horizontal sebagai kilau permukaan."},
         {"Atmosfer", "Layer warna tambahan untuk kedalaman air."}}, 3}
};

static const int kShowcaseCount = (int)(sizeof(kShowcases) / sizeof(kShowcases[0]));
static const float kIntroHeight = 336.0f;
static const float kCardHeight = 418.0f;
static const float kCardGap = 24.0f;
static const Color kSectionFill = {255, 255, 255, 209};
static const Color kSectionInner = {244, 248, 250, 244};

#define DrawLine Prim_DrawLine
#define DrawLineEx Prim_DrawLineEx
#define DrawLineBezier Prim_DrawLineBezier
#define DrawTriangle Prim_DrawTriangle
#define DrawCircle Prim_DrawCircle
#define DrawCircleV Prim_DrawCircleV
#define DrawCircleLines Prim_DrawCircleLines
#define DrawEllipse Prim_DrawEllipse
#define DrawRectangle Prim_DrawRectangle
#define DrawRectangleRounded Prim_DrawRectangleRounded
#define DrawRectangleRoundedLines Prim_DrawRectangleRoundedLines
#define DrawRectangleRoundedLinesEx Prim_DrawRectangleRoundedLinesEx
#define DrawRectangleGradientV Prim_DrawRectangleGradientV

static void DrawPartPreview(Rectangle box, ShowcaseType type, int index) {
    MenuPalette palette = Menu_GetPalette();
    Color ink = (Color){29, 66, 86, 255};
    Color soft = (Color){74, 145, 171, 220};
    Vector2 center = {box.x + box.width * 0.5f, box.y + box.height * 0.46f};

    DrawRectangleRounded(box, 0.12f, 6, ColorAlpha(RAYWHITE, 0.96f));
    DrawRectangleRoundedLinesEx(box, 0.12f, 6, 1.0f, ColorAlpha(ink, 0.32f));

    if (type == SHOWCASE_GUPPY) {
        if (index == 0) DrawEllipse((int)center.x + 4, (int)center.y, 36, 19, (Color){255, 170, 60, 255});
        if (index == 1) DrawEllipse((int)center.x + 12, (int)center.y + 5, 22, 9, (Color){255, 221, 136, 255});
        if (index == 2) {
            DrawTriangle((Vector2){center.x - 2, center.y}, (Vector2){center.x - 42, center.y - 24}, (Vector2){center.x - 24, center.y - 2}, (Color){255, 81, 136, 255});
            DrawTriangle((Vector2){center.x - 2, center.y}, (Vector2){center.x - 24, center.y + 2}, (Vector2){center.x - 42, center.y + 24}, (Color){255, 81, 136, 255});
        }
        if (index == 3) {
            DrawTriangle((Vector2){center.x + 6, center.y - 7}, (Vector2){center.x + 18, center.y - 24}, (Vector2){center.x + 28, center.y - 6}, (Color){255, 81, 136, 220});
            DrawTriangle((Vector2){center.x + 2, center.y + 6}, (Vector2){center.x + 16, center.y + 20}, (Vector2){center.x + 22, center.y + 4}, (Color){255, 81, 136, 210});
        }
        if (index == 4) {
            DrawCircleV(center, 9.0f, WHITE);
            DrawCircleV((Vector2){center.x + 2.0f, center.y}, 4.0f, BLACK);
            DrawCircleV((Vector2){center.x + 4.0f, center.y - 2.0f}, 1.6f, WHITE);
        }
    } else if (type == SHOWCASE_CARNIVORE) {
        if (index == 0) DrawEllipse((int)center.x - 2, (int)center.y, 38, 24, (Color){110, 120, 130, 255});
        if (index == 1) DrawEllipse((int)center.x + 2, (int)center.y + 9, 29, 11, (Color){210, 60, 50, 255});
        if (index == 2) DrawTriangle((Vector2){center.x - 8, center.y}, (Vector2){center.x - 40, center.y - 25}, (Vector2){center.x - 37, center.y + 25}, (Color){110, 120, 130, 255});
        if (index == 3) {
            DrawTriangle((Vector2){center.x - 2, center.y - 14}, (Vector2){center.x + 10, center.y - 34}, (Vector2){center.x + 24, center.y - 8}, (Color){70, 80, 90, 255});
            DrawEllipse((int)center.x + 4, (int)center.y + 18, 12, 6, (Color){70, 80, 90, 220});
        }
        if (index == 4) {
            Vector2 zigzag[8] = {
                {center.x + 18, center.y - 2}, {center.x + 10, center.y + 8}, {center.x + 2, center.y},
                {center.x - 6, center.y + 8}, {center.x - 14, center.y + 2}, {center.x - 22, center.y + 10},
                {center.x - 30, center.y + 4}, {center.x - 36, center.y + 8}
            };
            for (int i = 0; i < 7; i++) {
                DrawLineEx(zigzag[i], zigzag[i + 1], 2.0f, (Color){28, 30, 32, 255});
            }
            for (int t = 0; t < 4; t++) {
                float x = center.x + 10 + t * 5;
                DrawTriangle((Vector2){x, center.y - 8}, (Vector2){x + 4, center.y + 2}, (Vector2){x + 8, center.y - 8}, palette.highlight);
            }
        }
        if (index == 5) {
            DrawCircleV(center, 9.0f, (Color){250, 220, 90, 255});
            DrawCircleV((Vector2){center.x + 2.0f, center.y}, 4.0f, BLACK);
            DrawCircleV((Vector2){center.x + 4.0f, center.y - 2.0f}, 1.6f, WHITE);
        }
    } else if (type == SHOWCASE_ULTRAVORE) {
        if (index == 0) DrawEllipse((int)center.x - 6, (int)center.y, 46, 24, (Color){180, 200, 210, 255});
        if (index == 1) DrawEllipse((int)center.x - 18, (int)center.y - 5, 30, 15, (Color){110, 130, 145, 220});
        if (index == 2) {
            DrawTriangle((Vector2){center.x - 12, center.y}, (Vector2){center.x - 44, center.y - 26}, (Vector2){center.x - 38, center.y + 28}, (Color){220, 140, 80, 255});
        }
        if (index == 3) {
            DrawTriangle((Vector2){center.x - 4, center.y + 10}, (Vector2){center.x + 12, center.y + 30}, (Vector2){center.x + 28, center.y + 8}, (Color){110, 130, 145, 220});
            DrawLineEx((Vector2){center.x - 12, center.y - 20}, (Vector2){center.x + 12, center.y - 16}, 2.0f, ColorAlpha((Color){110, 130, 145, 255}, 0.35f));
        }
        if (index == 4) {
            DrawTriangle((Vector2){center.x + 2, center.y - 14}, (Vector2){center.x + 30, center.y + 2}, (Vector2){center.x + 2, center.y + 16}, (Color){180, 200, 210, 255});
            for (int t = 0; t < 4; t++) {
                float x = center.x + 8 + t * 5;
                DrawTriangle((Vector2){x, center.y - 9}, (Vector2){x + 4, center.y + 2}, (Vector2){x + 8, center.y - 9}, palette.highlight);
            }
        }
        if (index == 5) {
            DrawCircleV(center, 9.0f, (Color){220, 40, 40, 255});
            DrawCircleV((Vector2){center.x + 2.0f, center.y}, 4.0f, BLACK);
            DrawCircleV((Vector2){center.x + 4.0f, center.y - 2.0f}, 1.6f, WHITE);
        }
    } else if (type == SHOWCASE_FOOD) {
        if (index == 0) DrawRectangleRounded((Rectangle){center.x - 32, center.y - 10, 34, 20}, 0.9f, 8, (Color){252, 249, 245, 255});
        if (index == 1) DrawRectangleRounded((Rectangle){center.x - 2, center.y - 10, 34, 20}, 0.9f, 8, (Color){206, 44, 44, 255});
        if (index == 2) {
            DrawCircle((int)(center.x - 24), (int)center.y, 10, (Color){252, 249, 245, 255});
            DrawCircle((int)(center.x + 24), (int)center.y, 10, (Color){206, 44, 44, 255});
        }
        if (index == 3) DrawLineEx((Vector2){center.x, center.y - 12}, (Vector2){center.x, center.y + 12}, 2.0f, soft);
        if (index == 4) DrawRectangleRoundedLines((Rectangle){center.x - 32, center.y - 10, 64, 20}, 0.9f, 8, ink);
    } else if (type == SHOWCASE_BUBBLE) {
        if (index <= 2) {
            DrawCircle((int)center.x, (int)center.y, 20, (Color){180, 220, 255, index == 0 ? 120 : 0});
            if (index == 1) DrawCircleLines((int)center.x, (int)center.y, 20, soft);
            if (index == 2) DrawCircle((int)center.x - 6, (int)center.y - 6, 6, palette.highlight);
        }
    } else if (type == SHOWCASE_SEAWEED) {
        if (index == 0) DrawLineBezier((Vector2){center.x - 10, center.y + 22}, (Vector2){center.x, center.y - 26}, 7.0f, (Color){30, 124, 90, 255});
        if (index == 1) DrawLineBezier((Vector2){center.x, center.y + 22}, (Vector2){center.x + 14, center.y - 34}, 5.0f, (Color){38, 144, 96, 255});
        if (index == 2) DrawLineBezier((Vector2){center.x + 10, center.y + 22}, (Vector2){center.x + 26, center.y - 18}, 4.0f, (Color){45, 166, 110, 255});
        if (index == 3) DrawCircle((int)(center.x + 18), (int)(center.y - 8), 6, ColorAlpha((Color){84, 196, 136, 255}, 0.35f));
    } else if (type == SHOWCASE_CORAL) {
        if (index == 0) DrawEllipse((int)center.x, (int)center.y + 8, 34, 20, (Color){93, 70, 134, 255});
        if (index == 1) DrawEllipse((int)center.x - 5, (int)center.y + 10, 28, 14, (Color){69, 51, 106, 255});
        if (index == 2) {
            for (int t = 0; t < 4; t++) {
                DrawEllipse((int)(center.x - 24 + t * 16), (int)(center.y - 16 - (t % 2) * 4), 7, 12, (Color){166, 59, 68, 255});
                DrawCircle((int)(center.x - 21 + t * 16), (int)(center.y - 24 - (t % 2) * 4), 4, (Color){214, 177, 45, 255});
            }
        }
        if (index == 3) DrawRectangleRounded((Rectangle){center.x - 30, center.y + 2, 60, 24}, 0.3f, 6, (Color){28, 32, 56, 220});
    } else if (type == SHOWCASE_HELMET) {
        if (index == 0) DrawCircle((int)center.x, (int)center.y, 24, (Color){184, 144, 84, 255});
        if (index == 1) DrawCircle((int)center.x, (int)center.y, 11, (Color){35, 59, 102, 210});
        if (index == 2) {
            DrawCircle((int)(center.x - 16), (int)center.y, 7, (Color){35, 59, 102, 210});
            DrawCircle((int)(center.x + 16), (int)center.y, 7, (Color){35, 59, 102, 210});
        }
        if (index == 3) DrawCircle((int)(center.x + 10), (int)(center.y - 18), 7, (Color){205, 174, 103, 255});
        if (index == 4) {
            for (int t = 0; t < 4; t++) DrawCircle((int)(center.x - 18 + t * 12), (int)(center.y + 22), 2, ink);
        }
    } else if (type == SHOWCASE_SAND) {
        if (index == 0) DrawRectangle((int)(box.x + 10), (int)(center.y - 6), (int)(box.width - 20), 18, (Color){210, 180, 100, 255});
        if (index == 1) {
            for (int t = 0; t < 5; t++) {
                DrawRectangle((int)(box.x + 10 + t * 18), (int)(center.y - 8 + (t % 2) * 4), 14, 22, (Color){200, 170, 90, 255});
            }
        }
        if (index == 2) {
            for (int t = 0; t < 10; t++) DrawCircle((int)(box.x + 14 + t * 11), (int)(center.y + 4 + (t % 3)), 1.6f, (Color){242, 220, 150, 170});
        }
    } else if (type == SHOWCASE_BACKGROUND) {
        if (index == 0) DrawRectangleGradientV((int)(box.x + 10), (int)(box.y + 10), (int)(box.width - 20), (int)(box.height - 20), (Color){18, 78, 148, 255}, (Color){70, 170, 218, 255});
        if (index == 1) {
            for (int t = 0; t < 3; t++) DrawEllipse((int)center.x, (int)(center.y - 18 + t * 10), 38, 6, ColorAlpha(WHITE, 0.18f));
        }
        if (index == 2) DrawRectangleGradientV((int)(box.x + 14), (int)(box.y + 14), (int)(box.width - 28), (int)(box.height - 28), ColorAlpha((Color){255, 255, 255, 25}, 0.35f), BLANK);
    }
}

static void DrawShowcasePreview(Rectangle area, ShowcaseType type, float time) {
    DrawRectangleRounded(area, 0.08f, 8, ColorAlpha(WHITE, 0.8f));
    DrawRectangleRoundedLinesEx(area, 0.08f, 8, 1.5f, ColorAlpha((Color){34, 88, 110, 255}, 0.45f));

    if (type == SHOWCASE_GUPPY) {
        Guppy guppy = {0};
        guppy.active = true;
        guppy.scale = 1.25f;
        guppy.dir = 1.0f;
        guppy.tailSwing = sinf(time * 2.6f) * 3.0f;
        guppy.finSwing = sinf(time * 3.0f) * 2.0f;
        guppy.pos = (Vector2){area.x + area.width * 0.5f, area.y + area.height * 0.56f};
        DrawGuppy(&guppy, time);
    } else if (type == SHOWCASE_CARNIVORE) {
        Carnivore carnivore = {0};
        carnivore.active = true;
        carnivore.scale = 1.2f;
        carnivore.dir = 1.0f;
        carnivore.tailSwing = sinf(time * 2.2f) * 4.0f;
        carnivore.finSwing = sinf(time * 3.0f) * 2.0f;
        carnivore.pos = (Vector2){area.x + area.width * 0.5f, area.y + area.height * 0.56f};
        DrawPiranha(&carnivore, time);
    } else if (type == SHOWCASE_ULTRAVORE) {
        Ultravore ultravore = {0};
        ultravore.active = true;
        ultravore.scale = 0.92f;
        ultravore.dir = 1.0f;
        ultravore.tailSwing = sinf(time * 2.0f) * 4.0f;
        ultravore.finSwing = sinf(time * 2.8f) * 2.0f;
        ultravore.pos = (Vector2){area.x + area.width * 0.48f, area.y + area.height * 0.58f};
        DrawUltravore(&ultravore, time);
    } else if (type == SHOWCASE_FOOD) {
        Food food = {0};
        food.active = true;
        food.pos = (Vector2){area.x + area.width * 0.5f, area.y + area.height * 0.54f};
        food.rotation = 16.0f;
        DrawFood(&food);
    } else if (type == SHOWCASE_BUBBLE) {
        Bubble bubble = {0};
        bubble.active = true;
        bubble.pos = (Vector2){area.x + area.width * 0.5f, area.y + area.height * 0.56f};
        bubble.radius = 24.0f;
        bubble.alpha = 170.0f;
        DrawBubble(&bubble);
    } else if (type == SHOWCASE_SEAWEED) {
        DrawSeaweed((Vector2){area.x + area.width * 0.34f, area.y + area.height - 8.0f}, time);
    } else if (type == SHOWCASE_CORAL) {
        DrawCoral((Vector2){area.x + area.width * 0.5f, area.y + area.height * 0.68f});
    } else if (type == SHOWCASE_HELMET) {
        DrawHelmet((Vector2){area.x + area.width * 0.5f, area.y + area.height * 0.58f});
    } else if (type == SHOWCASE_SAND) {
        DrawRectangleGradientV((int)area.x, (int)(area.y + area.height * 0.42f), (int)area.width, (int)(area.height * 0.46f),
            (Color){215, 184, 114, 255}, (Color){189, 153, 90, 255});
        for (int x = 0; x < (int)area.width; x += 18) {
            DrawRectangle((int)(area.x + x), (int)(area.y + area.height * 0.4f + (x % 26) * 0.12f), 16, (int)(area.height * 0.48f), (Color){201, 169, 98, 255});
        }
        for (int x = 0; x < (int)area.width; x += 16) {
            DrawCircle((int)(area.x + x + 6), (int)(area.y + area.height * 0.82f), 1.6f, (Color){244, 230, 182, 160});
        }
    } else if (type == SHOWCASE_BACKGROUND) {
        for (int y = 0; y < (int)area.height; y++) {
            float t = (float)y / area.height;
            Color line = ColorLerp((Color){15, 74, 148, 255}, (Color){77, 182, 225, 255}, t);
            DrawLine((int)area.x, (int)(area.y + y), (int)(area.x + area.width), (int)(area.y + y), line);
        }
        for (int i = 0; i < 4; i++) {
            DrawEllipse((int)(area.x + area.width * 0.5f), (int)(area.y + 18 + i * 18), 72, 8, ColorAlpha(WHITE, 0.16f));
        }
    }
}

#undef DrawLine
#undef DrawLineEx
#undef DrawLineBezier
#undef DrawTriangle
#undef DrawCircle
#undef DrawCircleV
#undef DrawCircleLines
#undef DrawEllipse
#undef DrawRectangle
#undef DrawRectangleRounded
#undef DrawRectangleRoundedLines
#undef DrawRectangleRoundedLinesEx
#undef DrawRectangleGradientV

static void DrawShowcaseCard(Rectangle rect, const ShowcaseInfo *info, Font font, bool useFont, float time) {
    MenuPalette palette = Menu_GetPalette();
    float previewWidth = rect.width * 0.24f;
    if (previewWidth < 240.0f) previewWidth = 240.0f;
    if (previewWidth > 300.0f) previewWidth = 300.0f;
    Rectangle preview = {rect.x + 18, rect.y + 52, previewWidth, 150};
    Rectangle descBox = {preview.x + preview.width + 20.0f, rect.y + 52, rect.x + rect.width - (preview.x + preview.width + 38.0f), 150};

    Menu_DrawPanel(rect, ColorAlpha(WHITE, 0.82f), ColorAlpha(palette.panelStroke, 0.5f));
    Menu_DrawText(font, useFont, info->name, (Vector2){rect.x + 18, rect.y + 14}, 28.0f, 1.0f, palette.text);
    DrawShowcasePreview(preview, info->type, time);
    Menu_DrawPanel(descBox, ColorAlpha((Color){239, 246, 250, 255}, 0.95f), ColorAlpha(palette.panelStroke, 0.28f));
    Menu_DrawWrappedText(font, useFont, info->description,
        (Rectangle){descBox.x + 14, descBox.y + 14, descBox.width - 28, descBox.height - 28},
        17.0f, 1.0f, palette.textSoft);

    float partTop = rect.y + 222.0f;
    int columns = (info->partCount <= 3) ? info->partCount : 3;
    int rows = (info->partCount + columns - 1) / columns;
    float rowGap = 12.0f;
    float colGap = 14.0f;
    float availableWidth = rect.width - 36.0f - colGap * (columns - 1);
    float boxWidth = availableWidth / columns;
    float boxHeight = (rect.height - (partTop - rect.y) - 22.0f - rowGap * (rows - 1)) / rows;
    for (int i = 0; i < info->partCount; i++) {
        int col = i % columns;
        int row = i / columns;
        Rectangle box = {
            rect.x + 18 + col * (boxWidth + colGap),
            partTop + row * (boxHeight + rowGap),
            boxWidth,
            boxHeight
        };
        DrawPartPreview(box, info->type, i);
        Menu_DrawText(font, useFont, info->parts[i].label, (Vector2){box.x + 8, box.y + 8}, 14.0f, 1.0f, palette.text);
        Menu_DrawWrappedText(font, useFont, info->parts[i].description,
            (Rectangle){box.x + 8, box.y + box.height - 42, box.width - 16, 34}, 10.0f, 1.0f, palette.textSoft);
    }
}

static void DrawHowToPlayBody(Rectangle bounds, Font font, bool useFont, float time) {
    MenuPalette palette = Menu_GetPalette();
    float baseY = bounds.y;

    Menu_DrawPanel((Rectangle){bounds.x + 10, baseY, bounds.width - 40, 176}, kSectionFill, ColorAlpha(palette.panelStroke, 0.42f));
    Menu_DrawText(font, useFont, "Cara Bermain", (Vector2){bounds.x + 30, baseY + 16}, 24.0f, 1.0f, palette.text);
    Menu_DrawWrappedText(font, useFont,
        "1. Pilih Play dari Home untuk masuk ke permainan.\n"
        "2. Klik area air untuk menjatuhkan makanan ikan.\n"
        "3. Gunakan tombol Tambah Guppy, Carnivore, dan Ultravore pada UI game.\n"
        "4. Guppy memakan food, Carnivore memakan Guppy, dan Ultravore memakan Carnivore.\n"
        "5. Tombol Reset Aquarium akan menghapus semua ikan, food, dan bubble.\n"
        "6. Visual Guppy kini memakai badan oval kecil dengan ekor kipas dan sirip atas halus.\n"
        "7. Carnivore ditampilkan lebih bulat seperti piranha dengan garis zigzag badan dan gigi hanya di mulut.\n"
        "8. Ultravore dibuat lebih panjang, bermulut besar, dan tidak memakai sirip atas agar sesuai referensi.\n"
        "9. Tekan Backspace atau tombol Kembali untuk pulang ke Home.",
        (Rectangle){bounds.x + 30, baseY + 50, bounds.width - 80, 136}, 14.0f, 1.0f, palette.textSoft);

    Menu_DrawPanel((Rectangle){bounds.x + 10, baseY + 194, bounds.width - 40, 122}, kSectionFill, ColorAlpha(palette.panelStroke, 0.42f));
    Menu_DrawText(font, useFont, "List Object dan Penjabaran Visualisasi", (Vector2){bounds.x + 30, baseY + 212}, 22.0f, 1.0f, palette.text);
    Menu_DrawWrappedText(font, useFont,
        "Setiap object di bawah menampilkan bentuk terbaru beserta part primitive komputer grafis yang dipakai. Deskripsi sudah disesuaikan dengan visual ikan yang sekarang tampil di project.",
        (Rectangle){bounds.x + 30, baseY + 246, bounds.width - 80, 48}, 14.0f, 1.0f, palette.textSoft);

    float y = baseY + kIntroHeight;
    for (int i = 0; i < kShowcaseCount; i++) {
        DrawShowcaseCard((Rectangle){bounds.x + 10, y, bounds.width - 40, kCardHeight}, &kShowcases[i], font, useFont, time);
        y += kCardHeight + kCardGap;
    }
}

static void DrawAboutBody(Rectangle bounds, Font font, bool useFont, Texture2D photo, bool hasPhoto) {
    MenuPalette palette = Menu_GetPalette();
    float y = bounds.y;
    Rectangle introPanel = {bounds.x + 10, y, bounds.width - 40, 206};
    Rectangle profilePanel = {bounds.x + 10, y + 194, bounds.width - 40, 242};
    Rectangle storyPanel = {bounds.x + 10, y + 436, bounds.width - 40, 232};
    float profileGap = 20.0f;
    float profileTextWidth = (profilePanel.width - 80.0f) * 0.31f;
    float campusTextWidth = (profilePanel.width - 80.0f) * 0.41f;
    float photoCardWidth = profilePanel.width - 80.0f - profileTextWidth - campusTextWidth - profileGap * 2.0f;

    Menu_DrawPanel(introPanel, kSectionFill, ColorAlpha(palette.panelStroke, 0.42f));
    Menu_DrawText(font, useFont, "Informasi Game", (Vector2){introPanel.x + 18, introPanel.y + 16}, 24.0f, 1.0f, palette.text);
    Menu_DrawWrappedText(font, useFont,
        "Game Feeding Fish terinspirasi dari konsep permainan Insaniquarium Deluxe dan dikembangkan sebagai bagian dari pemenuhan tugas besar mata kuliah Komputer Grafis untuk penilaian UTS. Implementasi ini disusun dengan menekankan pemanfaatan primitive 2D Raylib untuk membangun object aquarium, ikan, dekorasi, serta antarmuka secara terstruktur. Secara akademik, proyek ini berada di bawah bimbingan dosen pengampu M. Rizki Solahudin dan Yudhi Widiyasana sebagai bentuk penerapan materi visualisasi komputer grafis ke dalam karya interaktif mahasiswa.",
        (Rectangle){introPanel.x + 18, introPanel.y + 52, introPanel.width - 36, 136}, 16.0f, 1.0f, palette.textSoft);

    Menu_DrawPanel(profilePanel, kSectionFill, ColorAlpha(palette.panelStroke, 0.42f));
    Menu_DrawText(font, useFont, "Profil Pembuat", (Vector2){profilePanel.x + 18, profilePanel.y + 16}, 24.0f, 1.0f, palette.text);
    Rectangle photoCard = {profilePanel.x + 20, profilePanel.y + 52, photoCardWidth, 170};
    float photoBoxWidth = 178.0f;
    if (photoBoxWidth > photoCard.width - 36.0f) photoBoxWidth = photoCard.width - 36.0f;
    Rectangle photoBox = {photoCard.x + (photoCard.width - photoBoxWidth) * 0.5f, photoCard.y + 16, photoBoxWidth, 136};
    Rectangle profileTextBox = {photoCard.x + photoCard.width + profileGap, profilePanel.y + 54, profileTextWidth, 152};
    Rectangle campusTextBox = {profileTextBox.x + profileTextBox.width + profileGap, profilePanel.y + 54, campusTextWidth, 152};
    Menu_DrawPanel(photoCard, kSectionInner, ColorAlpha(palette.panelStroke, 0.30f));
    DrawRectangleRounded(photoBox, 0.08f, 8, ColorAlpha((Color){250, 252, 253, 255}, 0.98f));
    DrawRectangleRoundedLinesEx(photoBox, 0.08f, 8, 1.0f, ColorAlpha(palette.panelStroke, 0.35f));
    if (hasPhoto) {
        float scaleX = photoBox.width / (float)photo.width;
        float scaleY = photoBox.height / (float)photo.height;
        float scale = (scaleX < scaleY) ? scaleX : scaleY;
        float drawW = photo.width * scale;
        float drawH = photo.height * scale;
        Rectangle src = {0, 0, (float)photo.width, (float)photo.height};
        Rectangle dst = {
            photoBox.x + (photoBox.width - drawW) * 0.5f,
            photoBox.y + (photoBox.height - drawH) * 0.5f,
            drawW,
            drawH
        };
        DrawTexturePro(photo, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
    } else {
        Menu_DrawWrappedText(font, useFont, "Foto pembuat tidak ditemukan.", (Rectangle){photoBox.x + 12, photoBox.y + 48, photoBox.width - 24, 44}, 12.0f, 1.0f, palette.textSoft);
    }

    Menu_DrawPanel(profileTextBox, kSectionInner, ColorAlpha(palette.panelStroke, 0.24f));
    Menu_DrawPanel(campusTextBox, kSectionInner, ColorAlpha(palette.panelStroke, 0.24f));
    Menu_DrawText(font, useFont, "Nama  : Roufiel Hadi", (Vector2){profileTextBox.x + 16, profileTextBox.y + 18}, 18.0f, 1.0f, palette.textSoft);
    Menu_DrawText(font, useFont, "NIM   : 241524028", (Vector2){profileTextBox.x + 16, profileTextBox.y + 50}, 18.0f, 1.0f, palette.textSoft);
    Menu_DrawText(font, useFont, "Kelas : 2A", (Vector2){profileTextBox.x + 16, profileTextBox.y + 82}, 18.0f, 1.0f, palette.textSoft);
    Menu_DrawWrappedText(font, useFont,
        "D4 Teknik Informatika\nJurusan Teknik Komputer dan Informatika\nPoliteknik Negeri Bandung",
        (Rectangle){campusTextBox.x + 16, campusTextBox.y + 18, campusTextBox.width - 32, 108}, 16.0f, 1.0f, palette.textSoft);

    Menu_DrawPanel(storyPanel, kSectionFill, ColorAlpha(palette.panelStroke, 0.42f));
    Menu_DrawText(font, useFont, "Latar Singkat", (Vector2){storyPanel.x + 18, storyPanel.y + 16}, 24.0f, 1.0f, palette.text);
    Menu_DrawWrappedText(font, useFont,
        "Roufiel Hadi merupakan mahasiswa D4 Teknik Informatika Politeknik Negeri Bandung yang berfokus pada pengembangan perangkat lunak, visual interaktif, dan pemanfaatan teknologi untuk penyelesaian proyek akademik. Dalam pengerjaan Feeding Fish, penulis menempatkan aspek ketelitian visual, kerapian struktur program, dan kemampuan menerjemahkan konsep ke implementasi sebagai bagian dari penguatan portofolio pribadi dan pengembangan kompetensi profesional.",
        (Rectangle){storyPanel.x + 18, storyPanel.y + 54, storyPanel.width - 36, 156}, 16.0f, 1.0f, palette.textSoft);
}

float Menu_GetHowToPlayContentHeight(void) {
    return kIntroHeight + kShowcaseCount * (kCardHeight + kCardGap) + 20.0f;
}

void Menu_DrawHowToPlayContent(Rectangle bounds, float scroll, Font font, bool useFont, float time) {
    DrawHowToPlayBody((Rectangle){
        bounds.x,
        bounds.y + scroll,
        bounds.width,
        Menu_GetHowToPlayContentHeight()
    }, font, useFont, time);
}

float Menu_GetAboutContentHeight(Font font, bool useFont, float width) {
    float introHeight = Menu_MeasureWrappedText(font, useFont,
        "Game Feeding Fish terinspirasi dari konsep permainan Insaniquarium Deluxe dan dikembangkan sebagai bagian dari pemenuhan tugas besar mata kuliah Komputer Grafis untuk penilaian UTS. Implementasi ini disusun dengan menekankan pemanfaatan primitive 2D Raylib untuk membangun object aquarium, ikan, dekorasi, serta antarmuka secara terstruktur. Secara akademik, proyek ini berada di bawah bimbingan dosen pengampu M. Rizki Solahudin dan Yudhi Widiyasana sebagai bentuk penerapan materi visualisasi komputer grafis ke dalam karya interaktif mahasiswa.",
        19.0f, 1.0f, width) + 46.0f;
    float storyHeight = Menu_MeasureWrappedText(font, useFont,
        "Roufiel Hadi merupakan mahasiswa D4 Teknik Informatika Politeknik Negeri Bandung yang berfokus pada pengembangan perangkat lunak, visual interaktif, dan pemanfaatan teknologi untuk penyelesaian proyek akademik. Dalam pengerjaan Feeding Fish, penulis menempatkan aspek ketelitian visual, kerapian struktur program, dan kemampuan menerjemahkan konsep ke implementasi sebagai bagian dari penguatan portofolio pribadi dan pengembangan kompetensi profesional.",
        18.0f, 1.0f, width) + 48.0f;
    return 430.0f + introHeight + storyHeight;
}

void Menu_DrawAboutContent(Rectangle bounds, float scroll, Font font, bool useFont, Texture2D photo, bool hasPhoto, float time) {
    (void)time;
    DrawAboutBody((Rectangle){
        bounds.x,
        bounds.y + scroll,
        bounds.width,
        Menu_GetAboutContentHeight(font, useFont, bounds.width - 44.0f)
    }, font, useFont, photo, hasPhoto);
}

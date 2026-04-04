/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "menu_content.h"

#include "menu_theme.h"
#include "../entities/bubble.h"
#include "../entities/lele.h"
#include "../entities/food.h"
#include "../entities/cere.h"
#include "../entities/toman.h"
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
    {SHOWCASE_GUPPY, "Ikan Cere",
        "Ikan cere dibuat sebagai spesies terkecil di aquarium: tubuhnya ramping memanjang, kepala kecil, punggung zaitun, perut terang, garis badan halus, dan ekor tipis agar lebih dekat ke ikan cere asli.",
        {{"Badan Ramping", "Tubuh dibentuk dari rangkaian circle yang mengikuti sumbu badan agar tidak tampak oval polos."},
         {"Perut", "Lapisan body bawah dibuat lebih terang untuk memberi siluet ikan kecil yang ringan."},
         {"Ekor Tipis", "Ekor belakang dibentuk dari web segitiga dan circle ujung agar terasa menyatu."},
         {"Sirip", "Sirip atas dan sirip perut kecil dibuat dengan garis dan ray tipis."},
         {"Mata + Mulut", "Mata bulat kecil dan garis mulut pendek menjaga proporsi ikan cere."}}, 5},
    {SHOWCASE_CARNIVORE, "Ikan Lele",
        "Ikan lele tampil lebih besar dari cere, dengan kepala lebar, tubuh licin memanjang, sirip bawah panjang, dan kumis khas. Siluetnya dibuat organik agar langsung terbaca sebagai lele air tawar Indonesia.",
        {{"Kepala", "Bagian depan dibuat besar dan tumpul untuk memberi kesan kepala lele."},
         {"Tubuh", "Badan memanjang dibangun dari chain circle sehingga volume tidak lagi kaku."},
         {"Sirip", "Sirip punggung rendah dan sirip bawah panjang dibuat dengan DDA thick line."},
         {"Ekor", "Ekor belakang dibulatkan supaya terlihat lebih alami."},
         {"Kumis", "Empat sungut dibuat dari garis DDA sebagai ciri paling khas lele."},
         {"Mata", "Mata kecil pucat menyesuaikan karakter ikan dasar."}}, 6},
    {SHOWCASE_ULTRAVORE, "Ikan Toman",
        "Ikan toman dibuat paling besar dan paling dominan sebagai toman jumbo, dengan badan panjang tebal, rahang besar, sirip punggung dan anal memanjang, serta corak sisi badan yang lebih liar seperti snakehead asli.",
        {{"Kepala", "Kepala depan dibuat besar dengan rahang atas-bawah yang jelas."},
         {"Tubuh Panjang", "Volume badan dibangun memanjang dengan chain circle untuk efek snakehead."},
         {"Sirip Panjang", "Sirip punggung dan sirip anal dibentuk dari thick line agar menyambung sepanjang badan."},
         {"Ekor", "Ekor membulat lebar memberi tenaga dorong predator besar."},
         {"Corak", "Garis hangat dan strip miring dipakai sebagai aksen tubuh toman."},
         {"Mata", "Mata merah tetap dipakai agar toman besar terasa dominan."}}, 6},
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
        "Latar belakang aquarium kini memakai gradasi vertikal dan glow lembut dari fungsi bawaan raylib, sehingga transisinya terasa lebih natural tanpa bitmap.",
        {{"Gradient", "DrawRectangleGradientV dipakai untuk menyatukan warna permukaan dan kedalaman air."},
         {"Glow", "DrawCircleGradient memberi semburat cahaya lembut di area atas air."},
         {"Atmosfer", "Layer gradien tambahan dipakai untuk memberi kedalaman warna yang halus."}}, 3}
};

static const int kShowcaseCount = (int)(sizeof(kShowcases) / sizeof(kShowcases[0]));
static const Color kSectionFill = {255, 255, 255, 209};
static const Color kSectionInner = {244, 248, 250, 244};
static const float kGuideIntroHeight = 176.0f;
static const float kGuideNavHeight = 136.0f;
static const float kGuideDetailHeight = 764.0f;
static const float kGuideContentHeight = 1120.0f;
static int s_selectedShowcase = 0;

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

/* ======================
Fungsi DrawGuideFoodPellet
=======================
Fungsi ini digunakan untuk menggambar guide food pellet.
*/
static void DrawGuideFoodPellet(Vector2 center, float scale, float rotationDeg) {
    float c = cosf(rotationDeg * DEG2RAD);
    float s = sinf(rotationDeg * DEG2RAD);
    float halfW = 18.0f * scale;
    float halfH = 9.0f * scale;

    Vector2 corners[4] = {
        {-halfW, -halfH},
        { halfW, -halfH},
        { halfW,  halfH},
        {-halfW,  halfH}
    };

    for (int i = 0; i < 4; i++) {
        float x = corners[i].x;
        float y = corners[i].y;
        corners[i].x = center.x + x * c - y * s;
        corners[i].y = center.y + x * s + y * c;
    }

    Vector2 midTop = {(corners[0].x + corners[1].x) * 0.5f, (corners[0].y + corners[1].y) * 0.5f};
    Vector2 midBottom = {(corners[3].x + corners[2].x) * 0.5f, (corners[3].y + corners[2].y) * 0.5f};
    Vector2 leftCap = {(corners[0].x + corners[3].x) * 0.5f, (corners[0].y + corners[3].y) * 0.5f};
    Vector2 rightCap = {(corners[1].x + corners[2].x) * 0.5f, (corners[1].y + corners[2].y) * 0.5f};

    DrawTriangle(corners[0], midTop, midBottom, (Color){252, 249, 245, 255});
    DrawTriangle(corners[0], midBottom, corners[3], (Color){252, 249, 245, 255});
    DrawTriangle(midTop, corners[1], corners[2], (Color){206, 44, 44, 255});
    DrawTriangle(midTop, corners[2], midBottom, (Color){206, 44, 44, 255});
    DrawCircleV(leftCap, halfH, (Color){252, 249, 245, 255});
    DrawCircleV(rightCap, halfH, (Color){206, 44, 44, 255});
    DrawLineEx(midTop, midBottom, 2.0f * scale, ColorAlpha((Color){87, 94, 108, 255}, 0.80f));
    DrawLineEx(corners[0], corners[1], 1.4f * scale, ColorAlpha((Color){62, 68, 80, 255}, 0.55f));
    DrawLineEx(corners[3], corners[2], 1.4f * scale, ColorAlpha((Color){62, 68, 80, 255}, 0.55f));
}

/* ======================
Fungsi DrawGuideBubblePreview
=======================
Fungsi ini digunakan untuk menggambar guide bubble preview.
*/
static void DrawGuideBubblePreview(Vector2 center, float radius, unsigned char alpha) {
    DrawCircleV(center, radius, (Color){180, 220, 255, alpha});
    DrawCircleLines((int)center.x, (int)center.y, radius, (Color){239, 249, 255, alpha});
    DrawCircleLines((int)center.x, (int)center.y, radius - 4.0f, (Color){150, 212, 245, (unsigned char)(alpha * 0.75f)});
    DrawCircleV((Vector2){center.x - radius * 0.26f, center.y - radius * 0.24f}, radius * 0.20f, ColorAlpha(WHITE, 0.92f));
}

/* ======================
Fungsi DrawPartPreview
=======================
Fungsi ini digunakan untuk menggambar part preview.
*/
static void DrawPartPreview(Rectangle box, ShowcaseType type, int index) {
    MenuPalette palette = Menu_GetPalette();
    Color ink = (Color){29, 66, 86, 255};
    Color soft = (Color){74, 145, 171, 220};
    Vector2 center = {box.x + box.width * 0.5f, box.y + box.height * 0.46f};

    DrawRectangleRounded(box, 0.12f, 6, ColorAlpha(RAYWHITE, 0.96f));
    DrawRectangleRoundedLinesEx(box, 0.12f, 6, 1.0f, ColorAlpha(ink, 0.32f));

    if (type == SHOWCASE_GUPPY) {
        if (index == 0) {
            DrawCircle((int)(center.x - 10), (int)center.y, 10, (Color){186, 192, 150, 255});
            DrawCircle((int)center.x, (int)center.y, 9, (Color){186, 192, 150, 255});
            DrawCircle((int)(center.x + 10), (int)center.y - 1, 8, (Color){186, 192, 150, 255});
        }
        if (index == 1) {
            DrawCircle((int)(center.x + 8), (int)(center.y + 4), 6, (Color){233, 227, 183, 255});
            DrawCircle((int)(center.x - 2), (int)(center.y + 4), 5, (Color){233, 227, 183, 255});
        }
        if (index == 2) {
            DrawTriangle((Vector2){center.x - 8, center.y}, (Vector2){center.x - 34, center.y - 16}, (Vector2){center.x - 28, center.y + 2}, (Color){241, 197, 112, 255});
            DrawTriangle((Vector2){center.x - 8, center.y}, (Vector2){center.x - 28, center.y - 2}, (Vector2){center.x - 34, center.y + 16}, (Color){241, 197, 112, 255});
            DrawCircle((int)(center.x - 38), (int)center.y, 5, (Color){241, 197, 112, 255});
        }
        if (index == 3) {
            DrawTriangle((Vector2){center.x - 6, center.y - 5}, (Vector2){center.x + 8, center.y - 20}, (Vector2){center.x + 18, center.y - 4}, (Color){241, 197, 112, 220});
            DrawTriangle((Vector2){center.x - 5, center.y + 4}, (Vector2){center.x + 10, center.y + 18}, (Vector2){center.x + 16, center.y + 3}, (Color){241, 197, 112, 210});
        }
        if (index == 4) {
            DrawCircleV(center, 7.0f, WHITE);
            DrawCircleV((Vector2){center.x + 2.0f, center.y}, 3.4f, BLACK);
            DrawCircleV((Vector2){center.x + 3.0f, center.y - 2.0f}, 1.3f, WHITE);
            DrawLineEx((Vector2){center.x + 10.0f, center.y + 2.0f}, (Vector2){center.x + 18.0f, center.y + 1.0f}, 1.2f, ink);
        }
    } else if (type == SHOWCASE_CARNIVORE) {
        if (index == 0) {
            DrawCircle((int)(center.x - 12), (int)center.y, 12, (Color){76, 86, 84, 255});
            DrawCircle((int)center.x, (int)center.y, 10, (Color){76, 86, 84, 255});
            DrawCircle((int)(center.x + 12), (int)(center.y - 1), 9, (Color){76, 86, 84, 255});
            DrawCircle((int)(center.x + 24), (int)(center.y - 1), 10, (Color){76, 86, 84, 255});
        }
        if (index == 1) {
            DrawCircle((int)(center.x - 4), (int)(center.y + 5), 8, (Color){140, 151, 126, 255});
            DrawCircle((int)(center.x + 10), (int)(center.y + 5), 7, (Color){140, 151, 126, 255});
        }
        if (index == 2) {
            DrawLineEx((Vector2){center.x - 18, center.y + 8}, (Vector2){center.x - 40, center.y + 12}, 8.0f, (Color){78, 98, 72, 255});
            DrawLineEx((Vector2){center.x - 18, center.y + 8}, (Vector2){center.x - 40, center.y + 12}, 1.5f, (Color){38, 48, 49, 255});
        }
        if (index == 3) {
            DrawTriangle((Vector2){center.x - 10, center.y - 7}, (Vector2){center.x + 6, center.y - 24}, (Vector2){center.x + 16, center.y - 6}, (Color){78, 98, 72, 255});
            DrawLineEx((Vector2){center.x - 8, center.y + 11}, (Vector2){center.x - 34, center.y + 15}, 7.0f, (Color){78, 98, 72, 220});
        }
        if (index == 4) {
            DrawLineEx((Vector2){center.x + 10, center.y}, (Vector2){center.x + 30, center.y - 12}, 1.6f, ink);
            DrawLineEx((Vector2){center.x + 10, center.y}, (Vector2){center.x + 32, center.y - 2}, 1.6f, ink);
            DrawLineEx((Vector2){center.x + 10, center.y}, (Vector2){center.x + 30, center.y + 10}, 1.6f, ink);
            DrawLineEx((Vector2){center.x + 5, center.y + 4}, (Vector2){center.x + 26, center.y + 15}, 1.5f, ink);
        }
        if (index == 5) {
            DrawCircleV(center, 6.8f, (Color){235, 232, 210, 255});
            DrawCircleV((Vector2){center.x + 1.8f, center.y}, 3.1f, BLACK);
            DrawCircleV((Vector2){center.x + 3.0f, center.y - 2.0f}, 1.2f, WHITE);
        }
    } else if (type == SHOWCASE_ULTRAVORE) {
        if (index == 0) {
            DrawCircle((int)(center.x - 18), (int)center.y, 12, (Color){66, 86, 74, 255});
            DrawCircle((int)(center.x - 2), (int)center.y, 11, (Color){66, 86, 74, 255});
            DrawCircle((int)(center.x + 14), (int)(center.y - 1), 10, (Color){66, 86, 74, 255});
            DrawCircle((int)(center.x + 28), (int)(center.y - 2), 10, (Color){66, 86, 74, 255});
        }
        if (index == 1) {
            DrawCircle((int)(center.x - 4), (int)(center.y - 8), 7, (Color){34, 48, 42, 220});
            DrawCircle((int)(center.x - 18), (int)(center.y - 8), 6, (Color){34, 48, 42, 220});
        }
        if (index == 2) {
            DrawCircle((int)(center.x - 38), (int)center.y, 7, (Color){207, 88, 48, 255});
            DrawTriangle((Vector2){center.x - 18, center.y}, (Vector2){center.x - 42, center.y - 18}, (Vector2){center.x - 42, center.y + 18}, (Color){207, 88, 48, 255});
        }
        if (index == 3) {
            DrawLineEx((Vector2){center.x + 8, center.y - 14}, (Vector2){center.x - 28, center.y - 18}, 8.0f, (Color){207, 88, 48, 220});
            DrawLineEx((Vector2){center.x + 8, center.y + 12}, (Vector2){center.x - 28, center.y + 16}, 7.0f, (Color){207, 88, 48, 190});
        }
        if (index == 4) {
            for (int t = 0; t < 4; t++) {
                DrawLineEx((Vector2){center.x + 10 - t * 10.0f, center.y - 3.0f}, (Vector2){center.x - t * 10.0f, center.y + 8.0f}, 1.5f, (Color){217, 137, 76, 255});
            }
            DrawLineEx((Vector2){center.x + 6.0f, center.y - 2.0f}, (Vector2){center.x - 32.0f, center.y}, 2.0f, (Color){209, 116, 66, 255});
        }
        if (index == 5) {
            DrawCircleV(center, 7.2f, (Color){226, 74, 58, 255});
            DrawCircleV((Vector2){center.x + 2.0f, center.y}, 3.3f, BLACK);
            DrawCircleV((Vector2){center.x + 3.4f, center.y - 2.0f}, 1.3f, WHITE);
        }
    } else if (type == SHOWCASE_FOOD) {
        if (index == 0) DrawGuideFoodPellet((Vector2){center.x - 8.0f, center.y + 2.0f}, 0.80f, -8.0f);
        if (index == 1) DrawGuideFoodPellet((Vector2){center.x + 10.0f, center.y - 2.0f}, 0.80f, 8.0f);
        if (index == 2) {
            DrawCircle((int)(center.x - 22), (int)center.y, 8, (Color){252, 249, 245, 255});
            DrawCircle((int)(center.x + 22), (int)center.y, 8, (Color){206, 44, 44, 255});
        }
        if (index == 3) DrawLineEx((Vector2){center.x, center.y - 12}, (Vector2){center.x, center.y + 12}, 2.0f, soft);
        if (index == 4) DrawRectangleRoundedLines((Rectangle){center.x - 28, center.y - 10, 56, 20}, 0.9f, 8, ink);
    } else if (type == SHOWCASE_BUBBLE) {
        if (index <= 2) {
            if (index == 0) DrawGuideBubblePreview(center, 20.0f, 132);
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
        Rectangle inner = {box.x + 10.0f, box.y + 10.0f, box.width - 20.0f, box.height - 20.0f};
        DrawWaterBackgroundRect(inner);
        if (index == 0) {
            DrawRectangleGradientV((int)inner.x, (int)inner.y, (int)inner.width, (int)(inner.height * 0.22f), ColorAlpha(WHITE, 0.18f), BLANK);
        }
        if (index == 1) {
            for (int t = 0; t < 3; t++) {
                DrawRectangleGradientV((int)(inner.x + 22 + t * 26), (int)(inner.y + 8), 14, (int)(inner.height * 0.62f), ColorAlpha(WHITE, 0.10f), BLANK);
            }
        }
        if (index == 2) {
            DrawRectangleGradientV((int)inner.x, (int)(inner.y + inner.height * 0.82f), (int)inner.width, (int)(inner.height * 0.18f),
                (Color){205, 182, 118, 255}, (Color){161, 133, 84, 255});
        }
    }
}

/* ======================
Fungsi DrawMiniAquariumDecor
=======================
Fungsi ini digunakan untuk menggambar mini aquarium decor.
*/
static void DrawMiniAquariumDecor(Rectangle area) {
    Vector2 coral = {area.x + area.width * 0.16f, area.y + area.height * 0.82f};
    DrawEllipse((int)coral.x, (int)coral.y, 22, 14, (Color){109, 90, 140, 255});
    DrawEllipse((int)(coral.x - 3), (int)(coral.y + 2), 16, 10, (Color){79, 62, 112, 255});

    for (int i = 0; i < 3; i++) {
        float x = area.x + area.width * (0.72f + i * 0.05f);
        Vector2 start = {x, area.y + area.height * 0.88f};
        Vector2 end = {x + (i - 1) * 5.0f, area.y + area.height * (0.48f - i * 0.02f)};
        DrawLineBezier(start, end, 4.0f - i * 0.6f, (Color){44, 140, 102, 230});
    }
}

/* ======================
Fungsi DrawShowcasePreview
=======================
Fungsi ini digunakan untuk menggambar showcase preview.
*/
static void DrawShowcasePreview(Rectangle area, ShowcaseType type, float time) {
    (void)time;
    DrawRectangleRounded(area, 0.08f, 8, ColorAlpha(WHITE, 0.8f));
    DrawRectangleRoundedLinesEx(area, 0.08f, 8, 1.5f, ColorAlpha((Color){34, 88, 110, 255}, 0.45f));
    Rectangle scene = {area.x + 8.0f, area.y + 8.0f, area.width - 16.0f, area.height - 16.0f};
    DrawWaterBackgroundRect(scene);

    if (type == SHOWCASE_GUPPY) {
        Guppy guppy = {0};
        guppy.active = true;
        guppy.scale = 0.98f;
        guppy.dir = 1.0f;
        guppy.pos = (Vector2){scene.x + scene.width * 0.55f, scene.y + scene.height * 0.56f};
        DrawGuppyPreview(&guppy, 0.0f);
    } else if (type == SHOWCASE_CARNIVORE) {
        Carnivore carnivore = {0};
        carnivore.active = true;
        carnivore.scale = 1.18f;
        carnivore.dir = 1.0f;
        carnivore.pos = (Vector2){scene.x + scene.width * 0.52f, scene.y + scene.height * 0.58f};
        DrawPiranhaPreview(&carnivore, 0.0f);
    } else if (type == SHOWCASE_ULTRAVORE) {
        Ultravore ultravore = {0};
        ultravore.active = true;
        ultravore.scale = 1.40f;
        ultravore.dir = 1.0f;
        ultravore.pos = (Vector2){scene.x + scene.width * 0.46f, scene.y + scene.height * 0.61f};
        DrawUltravorePreview(&ultravore, 0.0f);
    } else if (type == SHOWCASE_FOOD) {
        DrawGuideFoodPellet((Vector2){scene.x + scene.width * 0.44f, scene.y + scene.height * 0.56f}, 1.25f, -12.0f);
        DrawGuideFoodPellet((Vector2){scene.x + scene.width * 0.56f, scene.y + scene.height * 0.50f}, 1.10f, 14.0f);
        DrawGuideFoodPellet((Vector2){scene.x + scene.width * 0.62f, scene.y + scene.height * 0.62f}, 0.95f, -6.0f);
    } else if (type == SHOWCASE_BUBBLE) {
        DrawGuideBubblePreview((Vector2){scene.x + scene.width * 0.42f, scene.y + scene.height * 0.60f}, 26.0f, 142);
        DrawGuideBubblePreview((Vector2){scene.x + scene.width * 0.58f, scene.y + scene.height * 0.46f}, 20.0f, 128);
        DrawGuideBubblePreview((Vector2){scene.x + scene.width * 0.68f, scene.y + scene.height * 0.68f}, 13.0f, 118);
    } else if (type == SHOWCASE_SEAWEED) {
        DrawSeaweed((Vector2){scene.x + scene.width * 0.34f, scene.y + scene.height + 6.0f}, 0.0f);
    } else if (type == SHOWCASE_CORAL) {
        DrawCoral((Vector2){scene.x + scene.width * 0.48f, scene.y + scene.height * 0.72f});
    } else if (type == SHOWCASE_HELMET) {
        DrawHelmet((Vector2){scene.x + scene.width * 0.52f, scene.y + scene.height * 0.58f});
    } else if (type == SHOWCASE_SAND) {
        DrawRectangleGradientV((int)scene.x, (int)(scene.y + scene.height * 0.42f), (int)scene.width, (int)(scene.height * 0.46f),
            (Color){215, 184, 114, 255}, (Color){189, 153, 90, 255});
        for (int x = 0; x < (int)scene.width; x += 18) {
            DrawRectangle((int)(scene.x + x), (int)(scene.y + scene.height * 0.4f + (x % 26) * 0.12f), 16, (int)(scene.height * 0.48f), (Color){201, 169, 98, 255});
        }
        for (int x = 0; x < (int)scene.width; x += 16) {
            DrawCircle((int)(scene.x + x + 6), (int)(scene.y + scene.height * 0.82f), 1.6f, (Color){244, 230, 182, 160});
        }
    } else if (type == SHOWCASE_BACKGROUND) {
        DrawMiniAquariumDecor(scene);
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

/* ======================
Fungsi DrawShowcaseCard
=======================
Fungsi ini digunakan untuk menggambar showcase card.
*/
static void DrawShowcaseCard(Rectangle rect, const ShowcaseInfo *info, Font font, bool useFont, float time) {
    MenuPalette palette = Menu_GetPalette();
    float previewWidth = rect.width * 0.35f;
    if (previewWidth < 320.0f) previewWidth = 320.0f;
    if (previewWidth > 440.0f) previewWidth = 440.0f;

    // Bagi kartu menjadi area preview visual dan area deskripsi.
    Rectangle preview = {rect.x + 20.0f, rect.y + 56.0f, previewWidth, 228.0f};
    Rectangle descBox = {preview.x + preview.width + 22.0f, rect.y + 56.0f, rect.x + rect.width - (preview.x + preview.width + 42.0f), 228.0f};

    Menu_DrawPanel(rect, ColorAlpha(WHITE, 0.82f), ColorAlpha(palette.panelStroke, 0.5f));
    Menu_DrawText(font, useFont, info->name, (Vector2){rect.x + 20.0f, rect.y + 16.0f}, 32.0f, 1.0f, palette.text);
    DrawShowcasePreview(preview, info->type, time);
    Menu_DrawPanel(descBox, ColorAlpha((Color){239, 246, 250, 255}, 0.95f), ColorAlpha(palette.panelStroke, 0.28f));
    Menu_DrawWrappedText(font, useFont, info->description,
        (Rectangle){descBox.x + 16, descBox.y + 16, descBox.width - 32, descBox.height - 32},
        19.0f, 1.0f, palette.textSoft);

    float partTop = rect.y + 302.0f;
    int columns = (info->partCount <= 2) ? info->partCount : 2;
    int rows = (info->partCount + columns - 1) / columns;
    float rowGap = 16.0f;
    float colGap = 18.0f;
    float availableWidth = rect.width - 40.0f - colGap * (columns - 1);
    float boxWidth = availableWidth / columns;
    float boxHeight = (rect.height - (partTop - rect.y) - 26.0f - rowGap * (rows - 1)) / rows;

    // Susun detail komponen dalam grid agar penjelasan tetap ringkas dan terbaca.
    for (int i = 0; i < info->partCount; i++) {
        int col = i % columns;
        int row = i / columns;
        Rectangle box = {
            rect.x + 20 + col * (boxWidth + colGap),
            partTop + row * (boxHeight + rowGap),
            boxWidth,
            boxHeight
        };
        DrawPartPreview(box, info->type, i);
        Menu_DrawText(font, useFont, info->parts[i].label, (Vector2){box.x + 10, box.y + 10}, 16.0f, 1.0f, palette.text);
        Menu_DrawWrappedText(font, useFont, info->parts[i].description,
            (Rectangle){box.x + 10, box.y + box.height - 58, box.width - 20, 48}, 12.0f, 1.0f, palette.textSoft);
    }
}

/* ======================
Fungsi GetGuideNavPanel
=======================
Fungsi ini digunakan untuk mengambil guide nav panel.
*/
static Rectangle GetGuideNavPanel(Rectangle bounds) {
    return (Rectangle){bounds.x + 10.0f, bounds.y + kGuideIntroHeight + 18.0f, bounds.width - 40.0f, kGuideNavHeight};
}

/* ======================
Fungsi GetGuideShowcaseButton
=======================
Fungsi ini digunakan untuk mengambil guide showcase button.
*/
static Rectangle GetGuideShowcaseButton(Rectangle bounds, int index) {
    Rectangle nav = GetGuideNavPanel(bounds);
    int columns = 5;
    int col = index % columns;
    int row = index / columns;
    float gap = 10.0f;
    float padX = 16.0f;
    float padY = 46.0f;
    float buttonW = (nav.width - padX * 2.0f - gap * (columns - 1)) / (float)columns;
    float buttonH = 30.0f;
    return (Rectangle){
        nav.x + padX + col * (buttonW + gap),
        nav.y + padY + row * (buttonH + 12.0f),
        buttonW,
        buttonH
    };
}

/* ======================
Fungsi DrawGuideJumpButton
=======================
Fungsi ini digunakan untuk menggambar guide jump button.
*/
static void DrawGuideJumpButton(Rectangle bounds, Font font, bool useFont, const char *label, bool selected) {
    Color fill = selected ? (Color){69, 146, 184, 255} : ColorAlpha((Color){224, 237, 245, 255}, 0.96f);
    Color stroke = selected ? (Color){24, 92, 132, 255} : ColorAlpha((Color){73, 123, 146, 255}, 0.42f);
    Color text = selected ? RAYWHITE : (Color){26, 68, 88, 255};
    DrawRectangleRounded(bounds, 0.35f, 8, fill);
    DrawRectangleRoundedLinesEx(bounds, 0.35f, 8, 1.2f, stroke);
    {
        Vector2 size = Menu_MeasureText(font, useFont, label, 12.0f, 1.0f);
        Menu_DrawText(font, useFont, label,
            (Vector2){bounds.x + (bounds.width - size.x) * 0.5f, bounds.y + (bounds.height - size.y) * 0.5f - 1.0f},
            13.0f, 1.0f, text);
    }
}

/* ======================
Fungsi DrawHowToPlayBody
=======================
Fungsi ini digunakan untuk menggambar how to play body.
*/
static void DrawHowToPlayBody(Rectangle bounds, Font font, bool useFont, float time) {
    MenuPalette palette = Menu_GetPalette();
    Rectangle intro = {bounds.x + 10.0f, bounds.y, bounds.width - 40.0f, kGuideIntroHeight};
    Rectangle nav = GetGuideNavPanel(bounds);
    Rectangle detail = {bounds.x + 10.0f, nav.y + nav.height + 20.0f, bounds.width - 40.0f, kGuideDetailHeight};

    Menu_DrawPanel(intro, kSectionFill, ColorAlpha(palette.panelStroke, 0.42f));
    Menu_DrawText(font, useFont, "Guide Overview", (Vector2){intro.x + 18.0f, intro.y + 14.0f}, 24.0f, 1.0f, palette.text);
    Menu_DrawWrappedText(font, useFont,
        "1. Choose Play from the main menu.\n"
        "2. Click inside the water to drop food, with 6 pellets active at most.\n"
        "3. Pellet fall speed stays consistent until it reaches the aquarium floor.\n"
        "4. Cere eats pellets, lele hunts cere, and toman hunts lele.\n"
        "5. Population limits stay at Cere 3, Lele 2, and Toman 1.\n"
        "6. Use Reset Aquarium to clear fish, pellets, and bubbles.\n"
        "7. Use the object buttons below to open each static visualization page.",
        (Rectangle){intro.x + 18.0f, intro.y + 48.0f, intro.width - 36.0f, intro.height - 62.0f}, 15.0f, 1.0f, palette.textSoft);

    Menu_DrawPanel(nav, kSectionFill, ColorAlpha(palette.panelStroke, 0.42f));
    Menu_DrawText(font, useFont, "Visualization Pages", (Vector2){nav.x + 18.0f, nav.y + 12.0f}, 20.0f, 1.0f, palette.text);
    Menu_DrawText(font, useFont, TextFormat("Page %d / %d", s_selectedShowcase + 1, kShowcaseCount),
        (Vector2){nav.x + nav.width - 124.0f, nav.y + 14.0f}, 15.0f, 1.0f, palette.textSoft);

    for (int i = 0; i < kShowcaseCount; i++) {
        DrawGuideJumpButton(GetGuideShowcaseButton(bounds, i), font, useFont, kShowcases[i].name, i == s_selectedShowcase);
    }

    DrawShowcaseCard(detail, &kShowcases[s_selectedShowcase], font, useFont, time);
}

/* ======================
Fungsi DrawAboutBody
=======================
Fungsi ini digunakan untuk menggambar about body.
*/
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

/* ======================
Fungsi Menu_SetHowToPlaySelection
=======================
Fungsi ini digunakan untuk mengelola set how to play selection.
*/
void Menu_SetHowToPlaySelection(int index) {
    if (index < 0) index = 0;
    if (index >= kShowcaseCount) index = kShowcaseCount - 1;
    s_selectedShowcase = index;
}

/* ======================
Fungsi Menu_CycleHowToPlaySelection
=======================
Fungsi ini digunakan untuk mengelola cycle how to play selection.
*/
void Menu_CycleHowToPlaySelection(int delta) {
    if (kShowcaseCount <= 0) return;
    s_selectedShowcase = (s_selectedShowcase + delta) % kShowcaseCount;
    if (s_selectedShowcase < 0) s_selectedShowcase += kShowcaseCount;
}

/* ======================
Fungsi Menu_HandleHowToPlayClick
=======================
Fungsi ini digunakan untuk mengelola handle how to play click.
*/
void Menu_HandleHowToPlayClick(Rectangle bounds, Vector2 point) {
    for (int i = 0; i < kShowcaseCount; i++) {
        if (CheckCollisionPointRec(point, GetGuideShowcaseButton(bounds, i))) {
            s_selectedShowcase = i;
            return;
        }
    }
}

/* ======================
Fungsi Menu_GetHowToPlayContentHeight
=======================
Fungsi ini digunakan untuk mengelola get how to play content height.
*/
float Menu_GetHowToPlayContentHeight(void) {
    return kGuideContentHeight;
}

/* ======================
Fungsi Menu_DrawHowToPlayContent
=======================
Fungsi ini digunakan untuk mengelola draw how to play content.
*/
void Menu_DrawHowToPlayContent(Rectangle bounds, float scroll, Font font, bool useFont, float time) {
    // Scroll hanya menggeser body guide di dalam viewport halaman.
    DrawHowToPlayBody((Rectangle){
        bounds.x,
        bounds.y + scroll,
        bounds.width,
        Menu_GetHowToPlayContentHeight()
    }, font, useFont, time);
}

/* ======================
Fungsi Menu_GetAboutContentHeight
=======================
Fungsi ini digunakan untuk mengelola get about content height.
*/
float Menu_GetAboutContentHeight(Font font, bool useFont, float width) {
    float introHeight = Menu_MeasureWrappedText(font, useFont,
        "Game Feeding Fish terinspirasi dari konsep permainan Insaniquarium Deluxe dan dikembangkan sebagai bagian dari pemenuhan tugas besar mata kuliah Komputer Grafis untuk penilaian UTS. Implementasi ini disusun dengan menekankan pemanfaatan primitive 2D Raylib untuk membangun object aquarium, ikan, dekorasi, serta antarmuka secara terstruktur. Secara akademik, proyek ini berada di bawah bimbingan dosen pengampu M. Rizki Solahudin dan Yudhi Widiyasana sebagai bentuk penerapan materi visualisasi komputer grafis ke dalam karya interaktif mahasiswa.",
        19.0f, 1.0f, width) + 46.0f;
    float storyHeight = Menu_MeasureWrappedText(font, useFont,
        "Roufiel Hadi merupakan mahasiswa D4 Teknik Informatika Politeknik Negeri Bandung yang berfokus pada pengembangan perangkat lunak, visual interaktif, dan pemanfaatan teknologi untuk penyelesaian proyek akademik. Dalam pengerjaan Feeding Fish, penulis menempatkan aspek ketelitian visual, kerapian struktur program, dan kemampuan menerjemahkan konsep ke implementasi sebagai bagian dari penguatan portofolio pribadi dan pengembangan kompetensi profesional.",
        18.0f, 1.0f, width) + 48.0f;
    return 430.0f + introHeight + storyHeight;
}

/* ======================
Fungsi Menu_DrawAboutContent
=======================
Fungsi ini digunakan untuk mengelola draw about content.
*/
void Menu_DrawAboutContent(Rectangle bounds, float scroll, Font font, bool useFont, Texture2D photo, bool hasPhoto, float time) {
    (void)time;
    DrawAboutBody((Rectangle){
        bounds.x,
        bounds.y + scroll,
        bounds.width,
        Menu_GetAboutContentHeight(font, useFont, bounds.width - 44.0f)
    }, font, useFont, photo, hasPhoto);
}

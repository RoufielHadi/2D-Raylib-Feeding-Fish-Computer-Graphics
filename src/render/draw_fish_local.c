/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "draw_fish.h"
#include "../utils/primitives.h"

#include <math.h>

/* ======================
Fungsi DirSign
=======================
Fungsi ini digunakan untuk menentukan sign.
*/
static float DirSign(float dir) {
    return (dir >= 0.0f) ? 1.0f : -1.0f;
}

/* ======================
Fungsi FishPoint
=======================
Fungsi ini digunakan untuk memproses point.
*/
static Vector2 FishPoint(float cx, float cy, float offsetX, float offsetY, float dir, float rot, float scale) {
    float facing = DirSign(dir);
    return (Vector2){
        cx + offsetX * scale * facing,
        cy + offsetY * scale * rot
    };
}

/* ======================
Fungsi LerpVec2
=======================
Fungsi ini digunakan untuk menginterpolasi vec2.
*/
static Vector2 LerpVec2(Vector2 a, Vector2 b, float t) {
    return (Vector2){
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t
    };
}

/* ======================
Fungsi FadeToGray
=======================
Fungsi ini digunakan untuk mengubah to gray.
*/
static Color FadeToGray(Color c, float amount) {
    Color out = c;
    unsigned char gray = (unsigned char)(0.299f * c.r + 0.587f * c.g + 0.114f * c.b);
    out.r = (unsigned char)(c.r + (gray - c.r) * amount);
    out.g = (unsigned char)(c.g + (gray - c.g) * amount);
    out.b = (unsigned char)(c.b + (gray - c.b) * amount);
    return out;
}

/* ======================
Fungsi ApplyStateTint
=======================
Fungsi ini digunakan untuk menerapkan state tint.
*/
static Color ApplyStateTint(Color base, int isHungry, int isDead) {
    if (isDead) return FadeToGray((Color){245, 245, 245, base.a}, 0.88f);
    if (isHungry) {
        Color green = (Color){102, 198, 116, base.a};
        return ColorAlphaBlend(base, green, (Color){255, 255, 255, 102});
    }
    return base;
}

/* ======================
Fungsi DeadFinColor
=======================
Fungsi ini digunakan untuk menjalankan proses DeadFinColor.
*/
static Color DeadFinColor(Color base) {
    return FadeToGray(base, 0.72f);
}

/* ======================
Fungsi DrawBodyChain
=======================
Fungsi ini digunakan untuk menggambar body chain.
*/
static void DrawBodyChain(Vector2 from, Vector2 to, float headRadius, float tailRadius, int segments, Color fill) {
    if (segments < 2) segments = 2;
    for (int i = 0; i <= segments; i++) {
        float t = (float)i / (float)segments;
        float eased = 1.0f - (1.0f - t) * (1.0f - t);
        Vector2 point = LerpVec2(from, to, t);
        float radius = headRadius + (tailRadius - headRadius) * eased;
        DrawCircleV(point, radius, fill);
    }
}

/* ======================
Fungsi DrawBodyStripe
=======================
Fungsi ini digunakan untuk menggambar body stripe.
*/
static void DrawBodyStripe(Vector2 from, Vector2 to, float thickness, Color color) {
    DrawLineEx(from, to, thickness, color);
}

/* ======================
Fungsi DrawFinWeb
=======================
Fungsi ini digunakan untuk menggambar fin web.
*/
static void DrawFinWeb(Vector2 baseA, Vector2 tip, Vector2 baseB, Color fill, Color ray, int rayCount, float thickness) {
    DrawTriangle(baseA, tip, baseB, fill);
    for (int i = 0; i <= rayCount; i++) {
        float t = (float)i / (float)((rayCount <= 0) ? 1 : rayCount);
        Vector2 root = LerpVec2(baseA, baseB, t);
        DrawLineEx(root, tip, thickness, ray);
    }
}

/* ======================
Fungsi DrawRibbonFin
=======================
Fungsi ini digunakan untuk menggambar ribbon fin.
*/
static void DrawRibbonFin(Vector2 front, Vector2 mid, Vector2 back, float facing, float frontThickness, float backThickness,
    float finLength, float verticalDir, Color fill, Color ray, int rayCount) {
    DrawLineEx(front, mid, frontThickness, fill);
    DrawLineEx(mid, back, backThickness, fill);
    DrawLineEx(front, mid, 1.0f, ColorAlpha(ray, 0.50f));
    DrawLineEx(mid, back, 1.0f, ColorAlpha(ray, 0.50f));

    for (int i = 0; i <= rayCount; i++) {
        float t = (float)i / (float)((rayCount <= 0) ? 1 : rayCount);
        Vector2 root = (t < 0.5f)
            ? LerpVec2(front, mid, t * 2.0f)
            : LerpVec2(mid, back, (t - 0.5f) * 2.0f);
        float fan = sinf(t * PI);
        Vector2 tip = {
            root.x - finLength * (0.12f + 0.10f * fan) * facing,
            root.y + verticalDir * finLength * (0.34f + 0.66f * fan)
        };
        DrawLineEx(root, tip, 0.85f, ray);
    }
}

/* ======================
Fungsi DrawForkedTail
=======================
Fungsi ini digunakan untuk menggambar forked tail.
*/
static void DrawForkedTail(Vector2 rootTop, Vector2 rootCenter, Vector2 notch, Vector2 rootBottom, Vector2 tipTop, Vector2 tipBottom,
    Color fill, Color outline, float tipRadius) {
    DrawTriangle(rootTop, rootCenter, rootBottom, fill);
    DrawCircleV(rootCenter, tipRadius * 0.78f, fill);
    DrawTriangle(rootTop, tipTop, notch, fill);
    DrawTriangle(rootBottom, notch, tipBottom, fill);
    DrawCircleV(tipTop, tipRadius, fill);
    DrawCircleV(tipBottom, tipRadius, fill);
    DrawLineEx(rootTop, tipTop, 1.1f, outline);
    DrawLineEx(notch, tipTop, 1.0f, outline);
    DrawLineEx(rootBottom, tipBottom, 1.1f, outline);
    DrawLineEx(notch, tipBottom, 1.0f, outline);
}

/* ======================
Fungsi DrawFanTail
=======================
Fungsi ini digunakan untuk menggambar fan tail.
*/
static void DrawFanTail(Vector2 rootTop, Vector2 rootCenter, Vector2 rootBottom, Vector2 tipTop, Vector2 tipMid, Vector2 tipBottom,
    Color fill, Color outline, float tipRadius) {
    DrawTriangle(rootTop, rootCenter, rootBottom, fill);
    DrawTriangle(rootTop, tipTop, tipMid, fill);
    DrawTriangle(rootTop, tipMid, rootCenter, fill);
    DrawTriangle(rootCenter, tipMid, rootBottom, fill);
    DrawTriangle(rootBottom, tipMid, tipBottom, fill);
    DrawCircleV(rootCenter, tipRadius * 0.68f, fill);
    DrawCircleV(tipTop, tipRadius * 0.55f, fill);
    DrawCircleV(tipMid, tipRadius, fill);
    DrawCircleV(tipBottom, tipRadius * 0.55f, fill);
    DrawLineEx(rootTop, tipTop, 1.0f, outline);
    DrawLineEx(rootCenter, tipMid, 1.1f, outline);
    DrawLineEx(rootBottom, tipBottom, 1.0f, outline);
}

/* ======================
Fungsi DrawEye
=======================
Fungsi ini digunakan untuk menggambar eye.
*/
static void DrawEye(Vector2 eye, float radius, float dir, Color iris, bool dead) {
    if (dead) {
        DrawCircleV(eye, radius, LIGHTGRAY);
        DrawLineEx((Vector2){eye.x - radius * 0.6f, eye.y - radius * 0.6f},
            (Vector2){eye.x + radius * 0.6f, eye.y + radius * 0.6f}, 1.2f, BLACK);
        DrawLineEx((Vector2){eye.x - radius * 0.6f, eye.y + radius * 0.6f},
            (Vector2){eye.x + radius * 0.6f, eye.y - radius * 0.6f}, 1.2f, BLACK);
        return;
    }

    DrawCircleV(eye, radius, iris);
    DrawCircleV((Vector2){eye.x + radius * 0.22f * DirSign(dir), eye.y}, radius * 0.46f, BLACK);
    DrawCircleV((Vector2){eye.x + radius * 0.34f * DirSign(dir), eye.y - radius * 0.26f}, radius * 0.17f, WHITE);
}

/* ======================
Fungsi DrawFeedRing
=======================
Fungsi ini digunakan untuk menggambar feed ring.
*/
static void DrawFeedRing(Vector2 center, float radius, float alpha) {
    DrawRing(center, radius * 0.55f, radius, 0.0f, 360.0f, 24, ColorAlpha(WHITE, alpha));
}

/* ======================
Fungsi DrawIkanCere
=======================
Fungsi ini digunakan untuk menggambar ikan cere.
*/
static void DrawIkanCere(const Guppy *g, float animTime) {
    if (!g || !g->active) return;

    float dir = DirSign(g->dir);
    float rot = (g->state == GUPPY_DEAD) ? -1.0f : 1.0f;
    float scale = g->scale;
    float sway = (g->state == GUPPY_DEAD) ? 0.0f : sinf(animTime * 4.0f + g->time) * 1.4f;
    float tailLift = (g->state == GUPPY_DEAD) ? 0.0f : g->tailSwing * 0.33f;
    float finLift = (g->state == GUPPY_DEAD) ? 0.0f : g->finSwing * 0.22f;
    float cx = g->pos.x;
    float cy = g->pos.y + sway;

    Color body = ApplyStateTint((Color){187, 191, 152, 255}, g->state == GUPPY_HUNGRY, g->state == GUPPY_DEAD);
    Color back = ApplyStateTint((Color){112, 124, 82, 255}, g->state == GUPPY_HUNGRY, g->state == GUPPY_DEAD);
    Color belly = ApplyStateTint((Color){233, 227, 183, 245}, g->state == GUPPY_HUNGRY, g->state == GUPPY_DEAD);
    Color fin = (g->state == GUPPY_DEAD) ? DeadFinColor((Color){242, 198, 114, 225}) :
        ApplyStateTint((Color){242, 198, 114, 225}, g->state == GUPPY_HUNGRY, g->state == GUPPY_DEAD);

    Vector2 head = FishPoint(cx, cy, 14.0f, -1.0f, dir, rot, scale);
    Vector2 tailRoot = FishPoint(cx, cy, -24.0f, tailLift * 0.18f, dir, rot, scale);
    Vector2 nose = FishPoint(cx, cy, 24.0f, -1.0f, dir, rot, scale);
    Vector2 eye = FishPoint(cx, cy, 17.0f, -4.5f, dir, rot, scale);
    Vector2 dorsalTip = FishPoint(cx, cy, -5.0f, -12.5f - finLift, dir, rot, scale);
    Vector2 dorsalBaseA = FishPoint(cx, cy, -12.0f, -4.8f, dir, rot, scale);
    Vector2 dorsalBaseB = FishPoint(cx, cy, -1.0f, -3.8f, dir, rot, scale);
    Vector2 bellyFinTip = FishPoint(cx, cy, -6.0f, 11.5f + finLift, dir, rot, scale);
    Vector2 bellyFinBaseA = FishPoint(cx, cy, -12.0f, 4.2f, dir, rot, scale);
    Vector2 bellyFinBaseB = FishPoint(cx, cy, -1.0f, 4.9f, dir, rot, scale);
    Vector2 tailTop = FishPoint(cx, cy, -31.0f, -8.5f - tailLift * 0.8f, dir, rot, scale);
    Vector2 tailBottom = FishPoint(cx, cy, -31.0f, 9.2f + tailLift, dir, rot, scale);
    Vector2 tailNotch = FishPoint(cx, cy, -39.0f, 1.0f + tailLift * 0.10f, dir, rot, scale);
    Vector2 tailTopTip = FishPoint(cx, cy, -46.0f, -10.8f - tailLift, dir, rot, scale);
    Vector2 tailBottomTip = FishPoint(cx, cy, -45.0f, 12.2f + tailLift, dir, rot, scale);
    Vector2 stripeA = FishPoint(cx, cy, -12.0f, -2.0f, dir, rot, scale);
    Vector2 stripeB = FishPoint(cx, cy, 13.0f, -1.0f, dir, rot, scale);

    DrawEllipse((int)cx, (int)(cy + 15.0f * scale), (int)(21.0f * scale), (int)(5.0f * scale), ColorAlpha(BLACK, 0.14f));
    DrawBodyChain(head, tailRoot, 9.5f * scale, 5.2f * scale, 10, body);
    DrawBodyChain(FishPoint(cx, cy, 10.0f, 2.0f, dir, rot, scale), FishPoint(cx, cy, -7.0f, 4.0f, dir, rot, scale),
        5.4f * scale, 4.4f * scale, 6, belly);
    DrawBodyChain(FishPoint(cx, cy, 9.0f, -5.0f, dir, rot, scale), FishPoint(cx, cy, -12.0f, -6.5f, dir, rot, scale),
        4.2f * scale, 2.2f * scale, 6, ColorAlpha(back, 0.62f));
    DrawTriangle(head, nose, FishPoint(cx, cy, 14.0f, 5.0f, dir, rot, scale), body);
    DrawFinWeb(dorsalBaseA, dorsalTip, dorsalBaseB, ColorAlpha(fin, 0.62f), ColorAlpha((Color){120, 98, 52, 255}, 0.40f), 3, 0.9f);
    DrawFinWeb(bellyFinBaseA, bellyFinTip, bellyFinBaseB, ColorAlpha(fin, 0.68f), ColorAlpha((Color){120, 98, 52, 255}, 0.44f), 3, 0.9f);
    DrawForkedTail(tailTop, tailRoot, tailNotch, tailBottom, tailTopTip, tailBottomTip, fin, ColorAlpha((Color){122, 82, 42, 255}, 0.54f), 2.8f * scale);
    DrawBodyStripe(stripeA, stripeB, 1.2f * scale, ColorAlpha((Color){68, 82, 52, 255}, 0.68f));
    DrawBodyStripe(FishPoint(cx, cy, 6.0f, -7.0f, dir, rot, scale), FishPoint(cx, cy, -14.0f, -8.0f, dir, rot, scale),
        0.8f * scale, ColorAlpha(WHITE, 0.18f));
    DrawEye(eye, 5.4f * scale, dir, WHITE, g->state == GUPPY_DEAD);
    DrawLineEx(FishPoint(cx, cy, 20.0f, 2.0f, dir, rot, scale), FishPoint(cx, cy, 25.0f, 1.5f, dir, rot, scale),
        1.0f * scale, ColorAlpha((Color){77, 68, 52, 255}, 0.56f));

    if (g->eatFlash > 0.01f) {
        DrawFeedRing(FishPoint(cx, cy, 28.0f, 0.0f, dir, rot, scale), 9.0f * scale, g->eatFlash * 0.70f);
    }
}

/* ======================
Fungsi DrawIkanLele
=======================
Fungsi ini digunakan untuk menggambar ikan lele.
*/
static void DrawIkanLele(const Carnivore *c, float animTime) {
    if (!c || !c->active) return;

    float dir = DirSign(c->dir);
    float rot = (c->state == CARNIVORE_DEAD) ? -1.0f : 1.0f;
    float scale = c->scale;
    float sway = (c->state == CARNIVORE_DEAD) ? 0.0f : sinf(animTime * 3.3f + c->time * 0.5f) * 1.6f;
    float tailLift = (c->state == CARNIVORE_DEAD) ? 0.0f : c->tailSwing * 0.28f;
    float finLift = (c->state == CARNIVORE_DEAD) ? 0.0f : c->finSwing * 0.18f;
    float cx = c->pos.x;
    float cy = c->pos.y + sway;

    Color body = ApplyStateTint((Color){76, 86, 84, 255}, c->state == CARNIVORE_HUNGRY, c->state == CARNIVORE_DEAD);
    Color top = ApplyStateTint((Color){38, 48, 49, 255}, c->state == CARNIVORE_HUNGRY, c->state == CARNIVORE_DEAD);
    Color belly = ApplyStateTint((Color){140, 151, 126, 235}, c->state == CARNIVORE_HUNGRY, c->state == CARNIVORE_DEAD);
    Color fin = (c->state == CARNIVORE_DEAD) ? DeadFinColor((Color){78, 98, 72, 228}) :
        ApplyStateTint((Color){78, 98, 72, 228}, c->state == CARNIVORE_HUNGRY, c->state == CARNIVORE_DEAD);

    Vector2 headFront = FishPoint(cx, cy, 30.0f, -1.0f, dir, rot, scale);
    Vector2 headBack = FishPoint(cx, cy, 4.0f, 0.0f, dir, rot, scale);
    Vector2 tailRoot = FishPoint(cx, cy, -44.0f, tailLift * 0.12f, dir, rot, scale);
    Vector2 tailTop = FishPoint(cx, cy, -57.0f, -8.0f - tailLift * 0.9f, dir, rot, scale);
    Vector2 tailBottom = FishPoint(cx, cy, -57.0f, 10.0f + tailLift, dir, rot, scale);
    Vector2 tailNotch = FishPoint(cx, cy, -66.0f, 2.0f + tailLift * 0.08f, dir, rot, scale);
    Vector2 tailTopTip = FishPoint(cx, cy, -73.0f, -9.5f - tailLift, dir, rot, scale);
    Vector2 tailBottomTip = FishPoint(cx, cy, -72.0f, 12.8f + tailLift, dir, rot, scale);
    Vector2 eye = FishPoint(cx, cy, 18.0f, -5.5f, dir, rot, scale);
    Vector2 dorsalTip = FishPoint(cx, cy, -3.0f, -11.5f - finLift, dir, rot, scale);
    Vector2 dorsalBaseA = FishPoint(cx, cy, -12.0f, -4.8f, dir, rot, scale);
    Vector2 dorsalBaseB = FishPoint(cx, cy, 4.0f, -4.0f, dir, rot, scale);

    DrawEllipse((int)cx, (int)(cy + 18.0f * scale), (int)(32.0f * scale), (int)(6.0f * scale), ColorAlpha(BLACK, 0.16f));
    DrawBodyChain(headBack, tailRoot, 14.4f * scale, 4.8f * scale, 18, body);
    DrawBodyChain(FishPoint(cx, cy, 10.0f, 3.0f, dir, rot, scale), FishPoint(cx, cy, -24.0f, 4.0f, dir, rot, scale),
        8.4f * scale, 3.8f * scale, 10, belly);
    DrawBodyChain(FishPoint(cx, cy, 11.0f, -7.5f, dir, rot, scale), FishPoint(cx, cy, -34.0f, -8.0f, dir, rot, scale),
        6.4f * scale, 2.0f * scale, 12, ColorAlpha(top, 0.78f));
    DrawCircleV(headFront, 12.5f * scale, body);
    DrawCircleV(FishPoint(cx, cy, 23.0f, 1.5f, dir, rot, scale), 8.2f * scale, body);
    DrawBodyStripe(FishPoint(cx, cy, 8.0f, -8.0f, dir, rot, scale), FishPoint(cx, cy, -42.0f, -7.0f, dir, rot, scale),
        1.9f * scale, ColorAlpha(top, 0.86f));
    DrawFinWeb(dorsalBaseA, dorsalTip, dorsalBaseB, ColorAlpha(fin, 0.42f), ColorAlpha((Color){26, 36, 30, 255}, 0.40f), 2, 0.8f);
    DrawRibbonFin(FishPoint(cx, cy, 1.0f, 7.8f, dir, rot, scale), FishPoint(cx, cy, -23.0f, 9.3f, dir, rot, scale),
        FishPoint(cx, cy, -52.0f, 10.6f + finLift, dir, rot, scale), dir, 6.5f * scale, 5.2f * scale,
        9.0f * scale, 1.0f, ColorAlpha(fin, 0.82f), ColorAlpha((Color){24, 34, 28, 255}, 0.36f), 6);
    DrawFinWeb(FishPoint(cx, cy, 8.0f, 6.0f, dir, rot, scale), FishPoint(cx, cy, 18.0f, 16.0f + finLift, dir, rot, scale),
        FishPoint(cx, cy, 16.0f, 4.0f, dir, rot, scale), ColorAlpha((Color){194, 132, 88, 255}, 0.78f), ColorAlpha((Color){94, 58, 34, 255}, 0.40f), 3, 0.9f);
    DrawFinWeb(FishPoint(cx, cy, 6.0f, 5.0f, dir, rot, scale), FishPoint(cx, cy, 16.0f, 15.0f + finLift, dir, rot, scale),
        FishPoint(cx, cy, 14.0f, 3.0f, dir, rot, scale), ColorAlpha((Color){194, 132, 88, 255}, 0.54f), ColorAlpha((Color){94, 58, 34, 255}, 0.32f), 2, 0.8f);
    DrawForkedTail(tailTop, tailRoot, tailNotch, tailBottom, tailTopTip, tailBottomTip,
        ColorAlpha(fin, 0.88f), ColorAlpha((Color){24, 34, 28, 255}, 0.46f), 3.4f * scale);

    DrawLineEx(FishPoint(cx, cy, 26.0f, 1.0f, dir, rot, scale), FishPoint(cx, cy, 46.0f, -12.0f, dir, rot, scale), 1.1f * scale, ColorAlpha((Color){43, 34, 22, 255}, 0.86f));
    DrawLineEx(FishPoint(cx, cy, 26.0f, 1.0f, dir, rot, scale), FishPoint(cx, cy, 49.0f, -2.0f, dir, rot, scale), 1.2f * scale, ColorAlpha((Color){43, 34, 22, 255}, 0.84f));
    DrawLineEx(FishPoint(cx, cy, 26.0f, 1.0f, dir, rot, scale), FishPoint(cx, cy, 47.0f, 10.0f, dir, rot, scale), 1.1f * scale, ColorAlpha((Color){43, 34, 22, 255}, 0.84f));
    DrawLineEx(FishPoint(cx, cy, 22.0f, 4.0f, dir, rot, scale), FishPoint(cx, cy, 37.0f, 16.0f, dir, rot, scale), 1.0f * scale, ColorAlpha((Color){43, 34, 22, 255}, 0.80f));
    DrawLineEx(FishPoint(cx, cy, 18.0f, 1.5f, dir, rot, scale), FishPoint(cx, cy, 29.0f, 2.0f, dir, rot, scale), 1.0f * scale, ColorAlpha((Color){58, 46, 28, 255}, 0.64f));
    DrawEye(eye, 4.0f * scale, dir, (Color){228, 224, 208, 255}, c->state == CARNIVORE_DEAD);

    if (c->eatFlash > 0.01f) {
        DrawFeedRing(FishPoint(cx, cy, 34.0f, 1.0f, dir, rot, scale), 10.0f * scale, c->eatFlash * 0.72f);
    }
}

/* ======================
Fungsi DrawIkanToman
=======================
Fungsi ini digunakan untuk menggambar ikan toman.
*/
static void DrawIkanToman(const Ultravore *u, float animTime) {
    if (!u || !u->active) return;

    float dir = DirSign(u->dir);
    float rot = (u->state == ULTRA_DEAD) ? -1.0f : 1.0f;
    float scale = u->scale;
    float sway = (u->state == ULTRA_DEAD) ? 0.0f : sinf(animTime * 2.8f + u->time * 0.44f) * 1.8f;
    float tailLift = (u->state == ULTRA_DEAD) ? 0.0f : u->tailSwing * 0.24f;
    float finLift = (u->state == ULTRA_DEAD) ? 0.0f : u->finSwing * 0.16f;
    float cx = u->pos.x;
    float cy = u->pos.y + sway;

    Color body = ApplyStateTint((Color){66, 86, 74, 255}, u->state == ULTRA_HUNGRY, u->state == ULTRA_DEAD);
    Color back = ApplyStateTint((Color){34, 48, 42, 255}, u->state == ULTRA_HUNGRY, u->state == ULTRA_DEAD);
    Color belly = ApplyStateTint((Color){191, 181, 134, 242}, u->state == ULTRA_HUNGRY, u->state == ULTRA_DEAD);
    Color fin = (u->state == ULTRA_DEAD) ? DeadFinColor((Color){207, 88, 48, 232}) :
        ApplyStateTint((Color){207, 88, 48, 232}, u->state == ULTRA_HUNGRY, u->state == ULTRA_DEAD);

    Vector2 head = FishPoint(cx, cy, 34.0f, -2.0f, dir, rot, scale);
    Vector2 tailRoot = FishPoint(cx, cy, -58.0f, tailLift * 0.08f, dir, rot, scale);
    Vector2 jawTop = FishPoint(cx, cy, 54.0f, -9.0f, dir, rot, scale);
    Vector2 jawBottom = FishPoint(cx, cy, 54.0f, 10.0f, dir, rot, scale);
    Vector2 nose = FishPoint(cx, cy, 70.0f, 0.0f, dir, rot, scale);
    float dorsalWave = (u->state == ULTRA_DEAD) ? 0.0f : sinf(animTime * 3.0f + u->time * 0.9f) * (4.0f * scale);
    float analWave = (u->state == ULTRA_DEAD) ? 0.0f : sinf(animTime * 3.0f + u->time * 0.9f + 0.55f) * (3.2f * scale);
    Vector2 tailTop = FishPoint(cx, cy, -73.0f, -13.0f - tailLift * 0.6f, dir, rot, scale);
    Vector2 tailBottom = FishPoint(cx, cy, -73.0f, 14.0f + tailLift, dir, rot, scale);
    Vector2 tailTopTip = FishPoint(cx, cy, -92.0f, -17.0f - tailLift, dir, rot, scale);
    Vector2 tailMidTip = FishPoint(cx, cy, -100.0f, 1.0f, dir, rot, scale);
    Vector2 tailBottomTip = FishPoint(cx, cy, -91.0f, 18.5f + tailLift, dir, rot, scale);
    Vector2 eye = FishPoint(cx, cy, 32.0f, -12.0f, dir, rot, scale);
    Vector2 dorsalFront = FishPoint(cx, cy, 8.0f, -17.0f + dorsalWave * 0.06f, dir, rot, scale);
    Vector2 dorsalMid = FishPoint(cx, cy, -28.0f, -21.0f - dorsalWave * 0.10f, dir, rot, scale);
    Vector2 dorsalBack = FishPoint(cx, cy, -66.0f, -17.0f + dorsalWave * 0.08f - finLift, dir, rot, scale);
    Vector2 analFront = FishPoint(cx, cy, 5.0f, 14.5f + analWave * 0.05f, dir, rot, scale);
    Vector2 analMid = FishPoint(cx, cy, -26.0f, 18.5f + analWave * 0.10f, dir, rot, scale);
    Vector2 analBack = FishPoint(cx, cy, -65.0f, 15.0f + analWave * 0.08f + finLift, dir, rot, scale);

    // Badan toman dibuat lebih panjang agar karakter jumbo tetap terbaca.
    DrawEllipse((int)cx, (int)(cy + 28.0f * scale), (int)(56.0f * scale), (int)(10.0f * scale), ColorAlpha(BLACK, 0.18f));
    DrawBodyChain(head, tailRoot, 20.5f * scale, 9.5f * scale, 22, body);
    DrawBodyChain(FishPoint(cx, cy, 18.0f, -10.0f, dir, rot, scale), FishPoint(cx, cy, -52.0f, -11.0f, dir, rot, scale),
        9.8f * scale, 4.0f * scale, 14, ColorAlpha(back, 0.84f));
    DrawBodyChain(FishPoint(cx, cy, 12.0f, 6.0f, dir, rot, scale), FishPoint(cx, cy, -40.0f, 10.0f, dir, rot, scale),
        10.5f * scale, 5.8f * scale, 12, ColorAlpha(belly, 0.80f));
    DrawTriangle(head, jawTop, nose, body);
    DrawTriangle(head, nose, jawBottom, belly);
    DrawCircleV(FishPoint(cx, cy, 48.0f, 0.0f, dir, rot, scale), 11.0f * scale, body);

    // Sirip pita atas dan bawah digerakkan dengan gelombang berbeda agar lebih natural.
    DrawRibbonFin(dorsalFront, dorsalMid, dorsalBack, dir, 9.5f * scale, 7.8f * scale,
        12.0f * scale, -1.0f, ColorAlpha(fin, 0.84f), ColorAlpha((Color){79, 43, 26, 255}, 0.40f), 9);
    DrawRibbonFin(analFront, analMid, analBack, dir, 8.8f * scale, 7.4f * scale,
        10.8f * scale, 1.0f, ColorAlpha(fin, 0.74f), ColorAlpha((Color){79, 43, 26, 255}, 0.34f), 8);
    DrawFinWeb(FishPoint(cx, cy, 20.0f, 5.0f, dir, rot, scale), FishPoint(cx, cy, 34.0f, 18.0f + finLift, dir, rot, scale),
        FishPoint(cx, cy, 30.0f, 3.0f, dir, rot, scale), ColorAlpha(fin, 0.74f), ColorAlpha((Color){79, 43, 26, 255}, 0.36f), 4, 1.0f);
    DrawFanTail(tailTop, tailRoot, tailBottom, tailTopTip, tailMidTip, tailBottomTip,
        ColorAlpha(fin, 0.86f), ColorAlpha((Color){79, 43, 26, 255}, 0.44f), 6.0f * scale);

    for (int i = 0; i < 6; i++) {
        float x0 = 12.0f - i * 14.0f;
        DrawLineEx(FishPoint(cx, cy, x0, -4.0f, dir, rot, scale), FishPoint(cx, cy, x0 - 15.0f, 8.0f, dir, rot, scale),
            1.7f * scale, ColorAlpha((Color){215, 228, 236, 255}, 0.60f));
    }
    DrawBodyStripe(FishPoint(cx, cy, 18.0f, -2.0f, dir, rot, scale), FishPoint(cx, cy, -54.0f, 0.5f, dir, rot, scale),
        2.6f * scale, ColorAlpha((Color){91, 104, 92, 255}, 0.62f));
    DrawBodyStripe(FishPoint(cx, cy, 14.0f, -13.0f, dir, rot, scale), FishPoint(cx, cy, -28.0f, -14.0f, dir, rot, scale),
        1.4f * scale, ColorAlpha(WHITE, 0.18f));
    DrawLineEx(FishPoint(cx, cy, 43.0f, 5.0f, dir, rot, scale), FishPoint(cx, cy, 63.0f, 7.0f, dir, rot, scale),
        1.4f * scale, ColorAlpha((Color){46, 28, 20, 255}, 0.64f));
    DrawEye(eye, 6.0f * scale, dir, (Color){196, 184, 72, 255}, u->state == ULTRA_DEAD);

    if (u->eatFlash > 0.01f) {
        DrawFeedRing(FishPoint(cx, cy, 54.0f, 1.0f, dir, rot, scale), 12.0f * scale, u->eatFlash * 0.74f);
    }
}

/* ======================
Fungsi WarmFishSpriteCaches
=======================
Fungsi ini digunakan untuk menyiapkan fish sprite caches.
*/
void WarmFishSpriteCaches(void) {
}

/* ======================
Fungsi DrawGuppyPreview
=======================
Fungsi ini digunakan untuk menggambar guppy preview.
*/
void DrawGuppyPreview(const Guppy *g, float animTime) {
    DrawIkanCere(g, animTime);
}

/* ======================
Fungsi DrawPiranhaPreview
=======================
Fungsi ini digunakan untuk menggambar piranha preview.
*/
void DrawPiranhaPreview(const Carnivore *c, float animTime) {
    DrawIkanLele(c, animTime);
}

/* ======================
Fungsi DrawUltravorePreview
=======================
Fungsi ini digunakan untuk menggambar ultravore preview.
*/
void DrawUltravorePreview(const Ultravore *u, float animTime) {
    DrawIkanToman(u, animTime);
}

/* ======================
Fungsi DrawGuppy
=======================
Fungsi ini digunakan untuk menggambar guppy.
*/
void DrawGuppy(const Guppy *g, float animTime) {
    DrawIkanCere(g, animTime);
}

/* ======================
Fungsi DrawPiranha
=======================
Fungsi ini digunakan untuk menggambar piranha.
*/
void DrawPiranha(const Carnivore *c, float animTime) {
    DrawIkanLele(c, animTime);
}

/* ======================
Fungsi DrawUltravore
=======================
Fungsi ini digunakan untuk menggambar ultravore.
*/
void DrawUltravore(const Ultravore *u, float animTime) {
    DrawIkanToman(u, animTime);
}

/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "draw_food.h"
#include "../utils/primitives.h"

#include <math.h>

static RenderTexture2D s_foodBodyCache = {0};
static RenderTexture2D s_foodShadowCache = {0};
static bool s_foodSpriteReady = false;

static const int kFoodBodyW = 40;
static const int kFoodBodyH = 24;
static const int kFoodShadowW = 40;
static const int kFoodShadowH = 20;
static const float kFoodDrawScale = 0.5f;

/* ======================
Fungsi DrawFoodBodySprite
=======================
Fungsi ini digunakan untuk menggambar food body sprite.
*/
static void DrawFoodBodySprite(Vector2 center) {
	float w = 12.0f;
	float h = 6.0f;

	Vector2 p1 = {center.x - w, center.y - h};
	Vector2 p2 = {center.x,     center.y - h};
	Vector2 p3 = {center.x,     center.y + h};
	Vector2 p4 = {center.x - w, center.y + h};

	DrawTriangle(p1, p2, p3, (Color){252, 249, 245, 255});
	DrawTriangle(p1, p3, p4, (Color){252, 249, 245, 255});

	Vector2 p5 = {center.x,     center.y - h};
	Vector2 p6 = {center.x + w, center.y - h};
	Vector2 p7 = {center.x + w, center.y + h};
	Vector2 p8 = {center.x,     center.y + h};
	Vector2 c1 = {center.x - 0.7f, center.y - h};
	Vector2 c2 = {center.x + 0.7f, center.y - h};
	Vector2 c3 = {center.x + 0.7f, center.y + h};
	Vector2 c4 = {center.x - 0.7f, center.y + h};

	DrawTriangle(p5, p6, p7, (Color){206, 44, 44, 255});
	DrawTriangle(p5, p7, p8, (Color){206, 44, 44, 255});
	DrawTriangle(c1, c2, c3, (Color){206, 44, 44, 255});
	DrawTriangle(c1, c3, c4, (Color){252, 249, 245, 255});

	DrawLineEx((Vector2){center.x, center.y - h}, (Vector2){center.x, center.y + h}, 1.2f, (Color){150, 150, 150, 220});

	Vector2 leftCircle = {center.x - w, center.y};
	Vector2 rightCircle = {center.x + w, center.y};

	DrawCircleV(leftCircle,  h, (Color){252, 249, 245, 255});
	DrawCircleV(rightCircle, h, (Color){206, 44, 44, 255});
	DrawCircleLines((int)leftCircle.x, (int)leftCircle.y, h, (Color){70, 70, 70, 210});
	DrawCircleLines((int)rightCircle.x, (int)rightCircle.y, h, (Color){70, 70, 70, 210});
	DrawLineEx(p1, p2, 1.2f, (Color){80, 80, 80, 210});
	DrawLineEx(p4, p3, 1.2f, (Color){80, 80, 80, 210});
	DrawLineEx(p5, p6, 1.2f, (Color){80, 80, 80, 210});
	DrawLineEx(p8, p7, 1.2f, (Color){80, 80, 80, 210});
}

/* ======================
Fungsi EnsureFoodSpriteCache
=======================
Fungsi ini digunakan untuk memastikan food sprite cache.
*/
static void EnsureFoodSpriteCache(void) {
	if (s_foodSpriteReady) return;

	s_foodBodyCache = LoadRenderTexture(kFoodBodyW, kFoodBodyH);
	s_foodShadowCache = LoadRenderTexture(kFoodShadowW, kFoodShadowH);

	BeginTextureMode(s_foodBodyCache);
	ClearBackground(BLANK);
	DrawFoodBodySprite((Vector2){kFoodBodyW * 0.5f, kFoodBodyH * 0.5f});
	EndTextureMode();

	BeginTextureMode(s_foodShadowCache);
	ClearBackground(BLANK);
	DrawEllipse(kFoodShadowW / 2, kFoodShadowH / 2, 15, 7, ColorAlpha(BLACK, 0.18f));
	EndTextureMode();

	s_foodSpriteReady = true;
}

/* ======================
Fungsi WarmFoodSpriteCache
=======================
Fungsi ini digunakan untuk menyiapkan food sprite cache.
*/
void WarmFoodSpriteCache(void) {
	EnsureFoodSpriteCache();
}

/* ======================
Fungsi DrawFood
=======================
Fungsi ini digunakan untuk menggambar food.
*/
void DrawFood(const Food *f) {
	if (!f || !f->active) return;

	EnsureFoodSpriteCache();

	Rectangle shadowSrc = {0.0f, 0.0f, (float)kFoodShadowW, (float)-kFoodShadowH};
	Rectangle shadowDst = {
		f->pos.x - kFoodShadowW * kFoodDrawScale * 0.5f,
		f->pos.y + 5.0f - kFoodShadowH * kFoodDrawScale * 0.5f,
		(float)kFoodShadowW * kFoodDrawScale,
		(float)kFoodShadowH * kFoodDrawScale
	};
	DrawTexturePro(s_foodShadowCache.texture, shadowSrc, shadowDst, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);

	Rectangle bodySrc = {0.0f, 0.0f, (float)kFoodBodyW, (float)-kFoodBodyH};
	Rectangle bodyDst = {
		f->pos.x,
		f->pos.y,
		(float)kFoodBodyW * kFoodDrawScale,
		(float)kFoodBodyH * kFoodDrawScale
	};
	DrawTexturePro(s_foodBodyCache.texture, bodySrc, bodyDst,
		(Vector2){kFoodBodyW * kFoodDrawScale * 0.5f, kFoodBodyH * kFoodDrawScale * 0.5f},
		f->rotation, WHITE);
}

/* ======================
Fungsi DrawAllFood
=======================
Fungsi ini digunakan untuk menggambar all food.
*/
void DrawAllFood(const Food *foods, int count) {
	if (!foods) return;
	for (int i = 0; i < count; i++) {
		if (foods[i].active) DrawFood(&foods[i]);
	}
}

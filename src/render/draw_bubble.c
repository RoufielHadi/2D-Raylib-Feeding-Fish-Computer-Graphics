/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "draw_bubble.h"
#include "../utils/primitives.h"

static RenderTexture2D s_bubbleCache = {0};
static bool s_bubbleCacheReady = false;
static const int kBubbleSpriteSize = 64;
static const float kBubbleBaseRadius = 20.0f;

/* ======================
Fungsi EnsureBubbleCache
=======================
Fungsi ini digunakan untuk memastikan bubble cache.
*/
static void EnsureBubbleCache(void) {
	if (s_bubbleCacheReady) return;

	s_bubbleCache = LoadRenderTexture(kBubbleSpriteSize, kBubbleSpriteSize);
	BeginTextureMode(s_bubbleCache);
	ClearBackground(BLANK);
	DrawCircle(kBubbleSpriteSize / 2, kBubbleSpriteSize / 2, kBubbleBaseRadius, (Color){180, 220, 255, 165});
	DrawCircleLines(kBubbleSpriteSize / 2, kBubbleSpriteSize / 2, kBubbleBaseRadius, (Color){235, 247, 255, 255});
	DrawCircleLines(kBubbleSpriteSize / 2, kBubbleSpriteSize / 2, kBubbleBaseRadius - 3.0f, (Color){170, 222, 248, 180});
	DrawCircle(kBubbleSpriteSize / 2 - 6, kBubbleSpriteSize / 2 - 6, 6, WHITE);
	EndTextureMode();
	s_bubbleCacheReady = true;
}

/* ======================
Fungsi WarmBubbleSpriteCache
=======================
Fungsi ini digunakan untuk menyiapkan bubble sprite cache.
*/
void WarmBubbleSpriteCache(void) {
	EnsureBubbleCache();
}

/* ======================
Fungsi DrawBubble
=======================
Fungsi ini digunakan untuk menggambar bubble.
*/
void DrawBubble(const Bubble *b) {
	if (!b || !b->active) return;

	EnsureBubbleCache();

	float scale = b->radius / kBubbleBaseRadius;
	Rectangle src = {0.0f, 0.0f, (float)kBubbleSpriteSize, (float)-kBubbleSpriteSize};
	Rectangle dst = {
		b->pos.x - kBubbleSpriteSize * scale * 0.5f,
		b->pos.y - kBubbleSpriteSize * scale * 0.5f,
		kBubbleSpriteSize * scale,
		kBubbleSpriteSize * scale
	};
	DrawTexturePro(s_bubbleCache.texture, src, dst, (Vector2){0.0f, 0.0f}, 0.0f, (Color){255, 255, 255, (unsigned char)b->alpha});
}

/* ======================
Fungsi DrawAllBubbles
=======================
Fungsi ini digunakan untuk menggambar all bubbles.
*/
void DrawAllBubbles(const Bubble *arr, int count) {
	if (!arr) return;
	for (int i = 0; i < count; i++) {
		if (arr[i].active) DrawBubble(&arr[i]);
	}
}

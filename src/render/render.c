/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "render.h"
#include <stddef.h>

static RenderTexture2D s_environmentCache = {0};
static bool s_environmentCacheReady = false;
static int s_environmentCacheWidth = 0;
static int s_environmentCacheHeight = 0;

/* ======================
Fungsi EnsureEnvironmentCache
=======================
Fungsi ini digunakan untuk memastikan environment cache.
*/
static void EnsureEnvironmentCache(float width, float height) {
	if (!s_environmentCacheReady || s_environmentCacheWidth != (int)width || s_environmentCacheHeight != (int)height) {
		if (s_environmentCacheReady) {
			UnloadRenderTexture(s_environmentCache);
		}

		// Cache background statis agar render per frame lebih ringan.
		s_environmentCache = LoadRenderTexture((int)width, (int)height);
		s_environmentCacheReady = true;
		s_environmentCacheWidth = (int)width;
		s_environmentCacheHeight = (int)height;

		BeginTextureMode(s_environmentCache);
		ClearBackground(BLANK);
		DrawWaterBackground();
		DrawSand();
		DrawCoral((Vector2){width * 0.12f, height - 80.0f});
		DrawHelmet((Vector2){width * 0.84f, height - 120.0f});
		EndTextureMode();
	}
}

/* ======================
Fungsi FirstActiveGuppy
=======================
Fungsi ini digunakan untuk mengambil active guppy.
*/
static const Guppy *FirstActiveGuppy(const Guppy *guppies, int count) {
	for (int i = 0; i < count; i++) {
		if (guppies[i].active) return &guppies[i];
	}
	return NULL;
}

/* ======================
Fungsi FirstActiveCarnivore
=======================
Fungsi ini digunakan untuk mengambil active carnivore.
*/
static const Carnivore *FirstActiveCarnivore(const Carnivore *carnivores, int count) {
	for (int i = 0; i < count; i++) {
		if (carnivores[i].active) return &carnivores[i];
	}
	return NULL;
}

/* ======================
Fungsi FirstActiveUltravore
=======================
Fungsi ini digunakan untuk mengambil active ultravore.
*/
static const Ultravore *FirstActiveUltravore(const Ultravore *ultravores, int count) {
	for (int i = 0; i < count; i++) {
		if (ultravores[i].active) return &ultravores[i];
	}
	return NULL;
}

/* ======================
Fungsi FishDepthKeyG
=======================
Fungsi ini digunakan untuk memproses depth key g.
*/
static float FishDepthKeyG(const Guppy *g) {
	return g->pos.y + g->depth * 40.0f;
}

/* ======================
Fungsi FishDepthKeyC
=======================
Fungsi ini digunakan untuk memproses depth key c.
*/
static float FishDepthKeyC(const Carnivore *c) {
	return c->pos.y + c->depth * 40.0f;
}

/* ======================
Fungsi FishDepthKeyU
=======================
Fungsi ini digunakan untuk memproses depth key u.
*/
static float FishDepthKeyU(const Ultravore *u) {
	return u->pos.y + u->depth * 40.0f;
}

typedef struct {
	float depthKey;
	float animTime;
	const void *fish;
	int type;
} FishDrawItem;

/* ======================
Fungsi PushFishDrawItem
=======================
Fungsi ini digunakan untuk menambahkan fish draw item.
*/
static void PushFishDrawItem(FishDrawItem *items, int *count, float depthKey, float animTime, const void *fish, int type) {
	int insertAt = *count;
	while (insertAt > 0 && items[insertAt - 1].depthKey > depthKey) {
		items[insertAt] = items[insertAt - 1];
		insertAt--;
	}
	items[insertAt].depthKey = depthKey;
	items[insertAt].animTime = animTime;
	items[insertAt].fish = fish;
	items[insertAt].type = type;
	(*count)++;
}

/* ======================
Fungsi WarmGameRenderCaches
=======================
Fungsi ini digunakan untuk menyiapkan game render caches.
*/
void WarmGameRenderCaches(void) {
	float w = (float)GetScreenWidth();
	float h = (float)GetScreenHeight();
	EnsureEnvironmentCache(w, h);
	WarmSeaweedSpriteCache();
	WarmFoodSpriteCache();
	WarmBubbleSpriteCache();
	WarmFishSpriteCaches();
	WarmHeaderUICache();
}

/* ======================
Fungsi RenderAll
=======================
Fungsi ini digunakan untuk merender all.
*/
void RenderAll(const Guppy *guppies, int guppyCount,
			   const Carnivore *carnivores, int carnivoreCount,
			   const Ultravore *ultravores, int ultravoreCount,
			   const Food *foods, int foodCount,
			   const Bubble *bubbles, int bubbleCount,
			   float time) {
	float w = (float)GetScreenWidth();
	float h = (float)GetScreenHeight();
	EnsureEnvironmentCache(w, h);

	// Gambar latar aquarium dan dekorasi statis terlebih dahulu.
	DrawTextureRec(s_environmentCache.texture,
		(Rectangle){0.0f, 0.0f, (float)s_environmentCache.texture.width, (float)-s_environmentCache.texture.height},
		(Vector2){0.0f, 0.0f}, WHITE);

	DrawSeaweed((Vector2){w * 0.24f, h}, time);
	DrawSeaweed((Vector2){w * 0.60f, h}, time + 1.0f);

	// Urutkan ikan berdasarkan kedalaman supaya layering konsisten.
	FishDrawItem drawItems[32] = {0};
	int drawCount = 0;
	for (int i = 0; i < guppyCount; i++) {
		if (!guppies[i].active) continue;
		PushFishDrawItem(drawItems, &drawCount, FishDepthKeyG(&guppies[i]), guppies[i].time + time, &guppies[i], 0);
	}
	for (int i = 0; i < carnivoreCount; i++) {
		if (!carnivores[i].active) continue;
		PushFishDrawItem(drawItems, &drawCount, FishDepthKeyC(&carnivores[i]), carnivores[i].time + time, &carnivores[i], 1);
	}
	for (int i = 0; i < ultravoreCount; i++) {
		if (!ultravores[i].active) continue;
		PushFishDrawItem(drawItems, &drawCount, FishDepthKeyU(&ultravores[i]), ultravores[i].time + time, &ultravores[i], 2);
	}

	for (int i = 0; i < drawCount; i++) {
		if (drawItems[i].type == 0) DrawGuppy((const Guppy *)drawItems[i].fish, drawItems[i].animTime);
		if (drawItems[i].type == 1) DrawPiranha((const Carnivore *)drawItems[i].fish, drawItems[i].animTime);
		if (drawItems[i].type == 2) DrawUltravore((const Ultravore *)drawItems[i].fish, drawItems[i].animTime);
	}

	// Makanan dan bubble dipertahankan di atas scene utama agar tetap terlihat.
	DrawAllFood(foods, foodCount);
	DrawAllBubbles(bubbles, bubbleCount);

	const Guppy *g0 = FirstActiveGuppy(guppies, guppyCount);
	const Carnivore *c0 = FirstActiveCarnivore(carnivores, carnivoreCount);
	const Ultravore *u0 = FirstActiveUltravore(ultravores, ultravoreCount);
	DrawUI(g0, c0, u0);
}

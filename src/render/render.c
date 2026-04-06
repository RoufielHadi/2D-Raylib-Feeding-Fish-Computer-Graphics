/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "render.h"
#include <math.h>
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

/* ======================
Fungsi WireLen
=======================
Fungsi ini digunakan untuk menghitung panjang vector.
*/
static float WireLen(Vector2 v) {
	return sqrtf(v.x * v.x + v.y * v.y);
}

/* ======================
Fungsi WireNorm
=======================
Fungsi ini digunakan untuk menormalkan vector.
*/
static Vector2 WireNorm(Vector2 v, float dir) {
	float len = WireLen(v);
	if (len <= 0.001f) return (Vector2){dir >= 0.0f ? 1.0f : -1.0f, 0.0f};
	return (Vector2){v.x / len, v.y / len};
}

/* ======================
Fungsi DrawFishWireShape
=======================
Fungsi ini digunakan untuk menggambar wireframe ikan sederhana.
*/
static void DrawFishWireShape(Vector2 pos, Vector2 vel, float width, float height, float dir, Color color) {
	Vector2 heading = WireNorm(vel, dir);
	Vector2 nose = {pos.x + heading.x * width * 0.52f, pos.y + heading.y * width * 0.52f};
	DrawEllipseLines((int)pos.x, (int)pos.y, width * 0.5f, height * 0.5f, color);
	DrawLineEx(pos, nose, 1.2f, color);
	DrawCircleV(nose, 2.0f, color);
}

/* ======================
Fungsi DrawWireframeOverlay
=======================
Fungsi ini digunakan untuk menggambar overlay wireframe.
*/
static void DrawWireframeOverlay(const Guppy *guppies, int guppyCount,
	const Carnivore *carnivores, int carnivoreCount,
	const Ultravore *ultravores, int ultravoreCount,
	const Food *foods, int foodCount,
	const Bubble *bubbles, int bubbleCount) {
	Color line = ColorAlpha((Color){226, 245, 255, 255}, 0.72f);
	DrawRectangleLinesEx((Rectangle){50.0f, 108.0f, (float)GetScreenWidth() - 100.0f, (float)GetScreenHeight() - 200.0f},
		1.4f, ColorAlpha((Color){210, 236, 255, 255}, 0.50f));

	for (int i = 0; i < guppyCount; i++) {
		if (!guppies[i].active) continue;
		DrawFishWireShape(guppies[i].pos, guppies[i].vel, 46.0f * guppies[i].scale, 20.0f * guppies[i].scale, guppies[i].dir, line);
	}
	for (int i = 0; i < carnivoreCount; i++) {
		if (!carnivores[i].active) continue;
		DrawFishWireShape(carnivores[i].pos, carnivores[i].vel, 90.0f * carnivores[i].scale, 28.0f * carnivores[i].scale, carnivores[i].dir, line);
	}
	for (int i = 0; i < ultravoreCount; i++) {
		if (!ultravores[i].active) continue;
		DrawFishWireShape(ultravores[i].pos, ultravores[i].vel, 132.0f * ultravores[i].scale, 38.0f * ultravores[i].scale, ultravores[i].dir, line);
	}
	for (int i = 0; i < foodCount; i++) {
		if (!foods[i].active) continue;
		DrawRectangleLinesEx((Rectangle){foods[i].pos.x - 4.0f, foods[i].pos.y - 3.0f, 8.0f, 6.0f}, 1.0f, ColorAlpha((Color){255, 230, 192, 255}, 0.62f));
	}
	for (int i = 0; i < bubbleCount; i++) {
		if (!bubbles[i].active) continue;
		DrawCircleLines((int)bubbles[i].pos.x, (int)bubbles[i].pos.y, bubbles[i].radius, ColorAlpha((Color){223, 247, 255, 255}, 0.60f));
	}
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
			   float time,
			   const AquariumUiOptions *options) {
	float w = (float)GetScreenWidth();
	float h = (float)GetScreenHeight();
	EnsureEnvironmentCache(w, h);

	// Gambar latar aquarium dan dekorasi statis terlebih dahulu.
	DrawTextureRec(s_environmentCache.texture,
		(Rectangle){0.0f, 0.0f, (float)s_environmentCache.texture.width, (float)-s_environmentCache.texture.height},
		(Vector2){0.0f, 0.0f}, WHITE);
	DrawWaterRefractionOverlay(time);

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
	DrawAllBubbles(bubbles, bubbleCount);
	DrawAllFood(foods, foodCount);
	if (options && options->wireframeEnabled) {
		DrawWireframeOverlay(guppies, guppyCount, carnivores, carnivoreCount, ultravores, ultravoreCount, foods, foodCount, bubbles, bubbleCount);
	}

	const Guppy *g0 = FirstActiveGuppy(guppies, guppyCount);
	const Carnivore *c0 = FirstActiveCarnivore(carnivores, carnivoreCount);
	const Ultravore *u0 = FirstActiveUltravore(ultravores, ultravoreCount);
	DrawUI(g0, c0, u0, options);
}

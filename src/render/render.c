#include "render.h"
#include <stddef.h>

static RenderTexture2D s_environmentCache = {0};
static bool s_environmentCacheReady = false;
static int s_environmentCacheWidth = 0;
static int s_environmentCacheHeight = 0;

static void EnsureEnvironmentCache(float width, float height) {
	if (!s_environmentCacheReady || s_environmentCacheWidth != (int)width || s_environmentCacheHeight != (int)height) {
		if (s_environmentCacheReady) {
			UnloadRenderTexture(s_environmentCache);
		}
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

static const Guppy *FirstActiveGuppy(const Guppy *guppies, int count) {
	for (int i = 0; i < count; i++) {
		if (guppies[i].active) return &guppies[i];
	}
	return NULL;
}

static const Carnivore *FirstActiveCarnivore(const Carnivore *carnivores, int count) {
	for (int i = 0; i < count; i++) {
		if (carnivores[i].active) return &carnivores[i];
	}
	return NULL;
}

static const Ultravore *FirstActiveUltravore(const Ultravore *ultravores, int count) {
	for (int i = 0; i < count; i++) {
		if (ultravores[i].active) return &ultravores[i];
	}
	return NULL;
}

static float FishDepthKeyG(const Guppy *g) {
	return g->pos.y + g->depth * 40.0f;
}

static float FishDepthKeyC(const Carnivore *c) {
	return c->pos.y + c->depth * 40.0f;
}

static float FishDepthKeyU(const Ultravore *u) {
	return u->pos.y + u->depth * 40.0f;
}

typedef struct {
	float depthKey;
	float animTime;
	const void *fish;
	int type;
} FishDrawItem;

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

void RenderAll(const Guppy *guppies, int guppyCount,
			   const Carnivore *carnivores, int carnivoreCount,
			   const Ultravore *ultravores, int ultravoreCount,
			   const Food *foods, int foodCount,
			   const Bubble *bubbles, int bubbleCount,
			   float time) {
	float w = (float)GetScreenWidth();
	float h = (float)GetScreenHeight();
	EnsureEnvironmentCache(w, h);

	DrawTextureRec(s_environmentCache.texture,
		(Rectangle){0.0f, 0.0f, (float)s_environmentCache.texture.width, (float)-s_environmentCache.texture.height},
		(Vector2){0.0f, 0.0f}, WHITE);

	DrawSeaweed((Vector2){w * 0.24f, h}, time);
	DrawSeaweed((Vector2){w * 0.60f, h}, time + 1.0f);

	DrawAllFood(foods, foodCount);

	DrawAllBubbles(bubbles, bubbleCount);

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

	const Guppy *g0 = FirstActiveGuppy(guppies, guppyCount);
	const Carnivore *c0 = FirstActiveCarnivore(carnivores, carnivoreCount);
	const Ultravore *u0 = FirstActiveUltravore(ultravores, ultravoreCount);
	DrawUI(g0, c0, u0);
}


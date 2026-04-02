#include "draw_ui.h"
#include "draw_fish.h"
#include "../ui/ui.h"
#include "../ui/button.h"
#include "../utils/primitives.h"

static RenderTexture2D s_headerFishCache = {0};
static bool s_headerFishCacheReady = false;
static int s_headerFishCacheWidth = 0;
static int s_headerFishCacheHeight = 0;

static void DrawUIText(const char *text, Vector2 pos, float size, Color color) {
	if (UI_HasSharedFont()) {
		DrawTextEx(UI_GetSharedFont(), text, pos, size, 1.0f, color);
	} else {
		DrawText(text, (int)pos.x, (int)pos.y, (int)size, color);
	}
}

static void DrawCenteredUIText(const char *text, Rectangle rect, float size, Color color, float offsetY) {
	if (UI_HasSharedFont()) {
		Vector2 textSize = MeasureTextEx(UI_GetSharedFont(), text, size, 1.0f);
		DrawTextEx(UI_GetSharedFont(), text,
			(Vector2){rect.x + (rect.width - textSize.x) * 0.5f, rect.y + offsetY},
			size, 1.0f, color);
	} else {
		int fontSize = (int)size;
		int textW = MeasureText(text, fontSize);
		DrawText(text, (int)(rect.x + (rect.width - textW) * 0.5f), (int)(rect.y + offsetY), fontSize, color);
	}
}

static void EnsureHeaderFishCache(void) {
	const int width = 540;
	const int height = 72;
	if (!s_headerFishCacheReady || s_headerFishCacheWidth != width || s_headerFishCacheHeight != height) {
		if (s_headerFishCacheReady) {
			UnloadRenderTexture(s_headerFishCache);
		}
		s_headerFishCache = LoadRenderTexture(width, height);
		s_headerFishCacheReady = true;
		s_headerFishCacheWidth = width;
		s_headerFishCacheHeight = height;

		BeginTextureMode(s_headerFishCache);
		ClearBackground(BLANK);

		Guppy guppy = {0};
		guppy.active = true;
		guppy.scale = 0.66f;
		guppy.dir = 1.0f;
		guppy.pos = (Vector2){92.0f, 36.0f};
		DrawGuppyPreview(&guppy, 0.0f);

		Carnivore carnivore = {0};
		carnivore.active = true;
		carnivore.scale = 0.60f;
		carnivore.dir = 1.0f;
		carnivore.pos = (Vector2){272.0f, 36.0f};
		DrawPiranhaPreview(&carnivore, 0.0f);

		Ultravore ultravore = {0};
		ultravore.active = true;
		ultravore.scale = 0.48f;
		ultravore.dir = 1.0f;
		ultravore.pos = (Vector2){452.0f, 36.0f};
		DrawUltravorePreview(&ultravore, 0.0f);

		EndTextureMode();
	}
}

void WarmHeaderUICache(void) {
	EnsureHeaderFishCache();
}

static void DrawFishSpawnCard(Rectangle r, int kind, const char *label) {
	Vector2 mp = GetMousePosition();
	bool hover = CheckCollisionPointRec(mp, r);
	Color outer = hover ? (Color){154, 112, 67, 255} : (Color){122, 84, 50, 255};
	Color inner = hover ? (Color){241, 228, 206, 248} : (Color){230, 214, 191, 240};
	Color border = hover ? (Color){255, 236, 201, 255} : (Color){96, 62, 34, 255};
	Color labelColor = (Color){42, 27, 15, 255};
	Rectangle innerShell = {r.x + 4.0f, r.y + 4.0f, r.width - 8.0f, r.height - 8.0f};
	Rectangle labelBand = {r.x + 8.0f, r.y + r.height - 23.0f, r.width - 16.0f, 15.0f};

	DrawRectangleRounded(r, 0.18f, 10, outer);
	DrawRectangleRounded(innerShell, 0.18f, 10, inner);
	DrawRectangleRoundedLinesEx(r, 0.18f, 10, 2.0f, border);

	Rectangle preview = {r.x + 10.0f, r.y + 8.0f, r.width - 20.0f, 48.0f};
	DrawRectangleRounded(preview, 0.18f, 8, ColorAlpha(RAYWHITE, hover ? 0.40f : 0.28f));
	EnsureHeaderFishCache();
	Rectangle src = {(float)(kind * 180), 0.0f, 180.0f, -(float)s_headerFishCacheHeight};
	Rectangle dst = {preview.x, preview.y, preview.width, preview.height};
	DrawTexturePro(s_headerFishCache.texture, src, dst, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);

	DrawRectangleRounded(labelBand, 0.45f, 8, ColorAlpha((Color){255, 248, 236, 255}, 0.88f));
	DrawCenteredUIText(label, r, 14.0f, labelColor, 58.0f);
}

static void DrawActionCard(Rectangle r, const char *title, const char *subtitle, Color fill, Color accent, Color textColor) {
	Vector2 mp = GetMousePosition();
	bool hover = CheckCollisionPointRec(mp, r);
	Color outer = hover ? ColorAlpha(fill, 1.0f) : ColorAlpha(fill, 0.92f);
	Color glow = hover ? ColorAlpha(accent, 0.38f) : ColorAlpha(accent, 0.24f);
	Rectangle titleBand = {r.x + 10.0f, r.y + 12.0f, r.width - 20.0f, 26.0f};

	DrawRectangleRounded(r, 0.18f, 10, outer);
	DrawRectangleRounded((Rectangle){r.x + 5.0f, r.y + 5.0f, r.width - 10.0f, r.height - 10.0f}, 0.16f, 10, glow);
	DrawRectangleRoundedLinesEx(r, 0.18f, 10, 2.0f, ColorAlpha(BLACK, 0.28f));
	DrawRectangleRounded(titleBand, 0.35f, 8, ColorAlpha(BLACK, 0.12f));
	DrawCenteredUIText(title, r, 17.0f, textColor, 14.0f);
	DrawCenteredUIText(subtitle, r, 11.0f, ColorAlpha(textColor, 0.88f), 41.0f);
}

void DrawUI(const Guppy *g, const Carnivore *c, const Ultravore *u) {
	(void)g;
	(void)c;
	(void)u;
	int screenWidth = GetScreenWidth();
	DrawRectangle(0, 0, screenWidth, 102, (Color){86, 58, 40, 255});
	DrawRectangleGradientV(0, 0, screenWidth, 102, (Color){122, 85, 60, 124}, (Color){58, 37, 24, 0});
	DrawLine(0, 101, screenWidth, 101, (Color){37, 22, 14, 255});

	Rectangle rects[6];
	UI_GetButtonRects(rects);
	Rectangle leftGroup = {rects[0].x - 8.0f, 8.0f, rects[3].x + rects[3].width - rects[0].x + 16.0f, 84.0f};
	Rectangle rightGroup = {rects[4].x - 8.0f, 8.0f, rects[5].x + rects[5].width - rects[4].x + 16.0f, 84.0f};
	Rectangle centerInfo = {leftGroup.x + leftGroup.width + 18.0f, 14.0f, rightGroup.x - (leftGroup.x + leftGroup.width) - 36.0f, 70.0f};

	DrawRectangleRounded(leftGroup, 0.14f, 10, ColorAlpha((Color){63, 41, 28, 255}, 0.34f));
	DrawRectangleRounded(rightGroup, 0.14f, 10, ColorAlpha((Color){63, 41, 28, 255}, 0.34f));
	DrawRectangleRounded(centerInfo, 0.16f, 10, ColorAlpha((Color){245, 235, 220, 255}, 0.14f));
	DrawRectangleRoundedLinesEx(centerInfo, 0.16f, 10, 1.0f, ColorAlpha((Color){255, 231, 197, 255}, 0.18f));
	DrawUIText("KONTROL AQUARIUM", (Vector2){centerInfo.x + 18.0f, centerInfo.y + 14.0f}, 18.0f, (Color){249, 239, 219, 255});
	DrawUIText("Klik kartu untuk tambah ikan, klik area air untuk memberi makan.", (Vector2){centerInfo.x + 18.0f, centerInfo.y + 39.0f}, 12.0f, ColorAlpha((Color){248, 240, 227, 255}, 0.90f));

	DrawFishSpawnCard(rects[0], 0, "Guppy");
	DrawFishSpawnCard(rects[1], 1, "Carnivore");
	DrawFishSpawnCard(rects[2], 2, "Ultravore");
	DrawActionCard(rects[3], "RESET", "Aquarium", (Color){171, 60, 46, 255}, (Color){255, 210, 180, 255}, (Color){255, 247, 238, 255});
	DrawActionCard(rects[4], "ATUR", "Volume", (Color){67, 100, 131, 255}, (Color){179, 223, 255, 255}, (Color){246, 250, 255, 255});
	DrawActionCard(rects[5], "KEMBALI", "Home", (Color){62, 124, 96, 255}, (Color){204, 255, 219, 255}, (Color){245, 255, 247, 255});

	if (UI_IsVolumeOpen()) {
		Rectangle sr = UI_GetVolumeSliderRect();
		DrawRectangleRounded((Rectangle){sr.x - 14, sr.y - 28, sr.width + 28, 54}, 0.25f, 8, (Color){32, 20, 14, 220});
		DrawRectangleRounded(sr, 0.5f, 8, (Color){84, 84, 84, 240});
		DrawRectangleRounded((Rectangle){sr.x, sr.y, sr.width * UI_GetVolume(), sr.height}, 0.5f, 8, (Color){213, 159, 76, 255});
		float vol = UI_GetVolume();
		int handleX = (int)(sr.x + vol * sr.width);
		DrawCircle(handleX, (int)(sr.y + sr.height / 2.0f), 9.0f, (Color){250, 244, 237, 255});
		DrawCircleLines(handleX, (int)(sr.y + sr.height / 2.0f), 9.0f, (Color){86, 63, 44, 255});
		DrawUIText(TextFormat("Volume %d%%", (int)(vol * 100.0f)), (Vector2){sr.x, sr.y - 19.0f}, 14.0f, RAYWHITE);
	}
}

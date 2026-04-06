/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "draw_ui.h"
#include "draw_fish.h"
#include "../ui/ui.h"
#include "../ui/button.h"
#include "../utils/primitives.h"

#include <math.h>

static RenderTexture2D s_headerFishCache = {0};
static bool s_headerFishCacheReady = false;
static int s_headerFishCacheWidth = 0;
static int s_headerFishCacheHeight = 0;

/* ======================
Fungsi DrawUIText
=======================
Fungsi ini digunakan untuk menggambar uitext.
*/
static void DrawUIText(const char *text, Vector2 pos, float size, Color color) {
	if (UI_HasSharedFont()) {
		DrawTextEx(UI_GetSharedFont(), text, pos, size, 1.0f, color);
	} else {
		DrawText(text, (int)pos.x, (int)pos.y, (int)size, color);
	}
}

/* ======================
Fungsi DrawCenteredUIText
=======================
Fungsi ini digunakan untuk menggambar centered uitext.
*/
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

/* ======================
Fungsi DrawRightAlignedUIText
=======================
Fungsi ini digunakan untuk menggambar teks rata kanan.
*/
static void DrawRightAlignedUIText(const char *text, float rightX, float y, float size, Color color) {
	if (UI_HasSharedFont()) {
		Vector2 textSize = MeasureTextEx(UI_GetSharedFont(), text, size, 1.0f);
		DrawTextEx(UI_GetSharedFont(), text, (Vector2){rightX - textSize.x, y}, size, 1.0f, color);
	} else {
		int fontSize = (int)size;
		int textW = MeasureText(text, fontSize);
		DrawText(text, (int)(rightX - textW), (int)y, fontSize, color);
	}
}

/* ======================
Fungsi EnsureHeaderFishCache
=======================
Fungsi ini digunakan untuk memastikan header fish cache.
*/
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
		guppy.scale = 0.47f;
		guppy.dir = 1.0f;
	guppy.pos = (Vector2){86.0f, 40.0f};
	DrawGuppyPreview(&guppy, 0.0f);

		Carnivore carnivore = {0};
		carnivore.active = true;
		carnivore.scale = 0.73f;
		carnivore.dir = 1.0f;
	carnivore.pos = (Vector2){265.0f, 40.0f};
	DrawPiranhaPreview(&carnivore, 0.0f);

		Ultravore ultravore = {0};
		ultravore.active = true;
		ultravore.scale = 0.86f;
		ultravore.dir = 1.0f;
	ultravore.pos = (Vector2){444.0f, 42.0f};
	DrawUltravorePreview(&ultravore, 0.0f);

		EndTextureMode();
	}
}

/* ======================
Fungsi WarmHeaderUICache
=======================
Fungsi ini digunakan untuk menyiapkan header uicache.
*/
void WarmHeaderUICache(void) {
	EnsureHeaderFishCache();
}

/* ======================
Fungsi DrawFishSpawnCard
=======================
Fungsi ini digunakan untuk menggambar fish spawn card.
*/
static void DrawFishSpawnCard(Rectangle r, int kind, const char *label) {
	Vector2 mp = GetMousePosition();
	bool hover = CheckCollisionPointRec(mp, r);
	Color outer = hover ? (Color){154, 112, 67, 255} : (Color){122, 84, 50, 255};
	Color inner = hover ? (Color){241, 228, 206, 248} : (Color){230, 214, 191, 240};
	Color border = hover ? (Color){255, 236, 201, 255} : (Color){96, 62, 34, 255};
	Color labelColor = (Color){42, 27, 15, 255};
	Rectangle innerShell = {r.x + 4.0f, r.y + 4.0f, r.width - 8.0f, r.height - 8.0f};
	Rectangle labelBand = {r.x + 8.0f, r.y + r.height - 25.0f, r.width - 16.0f, 17.0f};

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
	DrawCenteredUIText(label, r, 13.0f, labelColor, 57.0f);
}

/* ======================
Fungsi DrawActionCard
=======================
Fungsi ini digunakan untuk menggambar action card.
*/
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

/* ======================
Fungsi Clamp01
=======================
Fungsi ini digunakan untuk membatasi nilai 0..1.
*/
static float Clamp01(float value) {
	if (value < 0.0f) return 0.0f;
	if (value > 1.0f) return 1.0f;
	return value;
}

/* ======================
Fungsi BubbleModeLabel
=======================
Fungsi ini digunakan untuk mengambil label bubble mode.
*/
static const char *BubbleModeLabel(BubbleControlMode mode) {
	if (mode == BUBBLE_CTRL_CALM) return "Calm";
	if (mode == BUBBLE_CTRL_TURBO) return "Turbo";
	return "Normal";
}

/* ======================
Fungsi HungerStateLabel
=======================
Fungsi ini digunakan untuk mengambil label keadaan lapar ikan.
*/
static const char *HungerStateLabel(bool dead, float ratio) {
	if (dead) return "Dead";
	if (ratio < 0.35f) return "Critical";
	if (ratio < 0.65f) return "Hungry";
	return "Stable";
}

/* ======================
Fungsi DrawFishStatusTile
=======================
Fungsi ini digunakan untuk menggambar kartu status ikan.
*/
static void DrawFishStatusTile(Rectangle r, const char *label, bool active, bool dead, float hunger, float hungerMax, Color accent) {
	Color shell = ColorAlpha((Color){22, 33, 46, 255}, 0.84f);
	Color border = ColorAlpha((Color){255, 236, 201, 255}, 0.20f);
	float ratio = active ? Clamp01(1.0f - hunger / hungerMax) : 0.0f;
	float remain = active ? fmaxf(0.0f, hungerMax - hunger) : 0.0f;
	Color fill = (dead || ratio < 0.28f) ? (Color){210, 86, 74, 255} : accent;
	Color stateTint = dead ? (Color){255, 214, 208, 255} :
		(ratio < 0.35f ? (Color){255, 206, 160, 255} :
		(ratio < 0.65f ? (Color){255, 232, 152, 255} : (Color){207, 255, 216, 255}));

	DrawRectangleRounded(r, 0.20f, 10, shell);
	DrawRectangleRoundedLinesEx(r, 0.20f, 10, 1.0f, border);
	DrawRectangleRounded((Rectangle){r.x + 7.0f, r.y + 5.0f, r.width - 14.0f, 11.0f}, 0.6f, 10, ColorAlpha(accent, 0.16f));
	DrawUIText(label, (Vector2){r.x + 10.0f, r.y + 5.0f}, 9.0f, (Color){248, 242, 229, 255});

	if (!active) {
		DrawRightAlignedUIText("Not added", r.x + r.width - 10.0f, r.y + 6.0f, 9.0f, ColorAlpha((Color){250, 239, 219, 255}, 0.76f));
		Rectangle idleTrack = {r.x + 12.0f, r.y + 24.0f, r.width - 24.0f, 10.0f};
		DrawRectangleRounded(idleTrack, 0.7f, 10, ColorAlpha(WHITE, 0.08f));
		return;
	}

	DrawRightAlignedUIText(HungerStateLabel(dead, ratio), r.x + r.width - 12.0f, r.y + 6.0f, 9.0f, stateTint);

	Rectangle track = {r.x + 12.0f, r.y + 24.0f, r.width - 24.0f, 10.0f};
	DrawRectangleRounded(track, 0.7f, 10, ColorAlpha(BLACK, 0.32f));
	DrawRectangleRounded((Rectangle){track.x, track.y, track.width * ratio, track.height}, 0.7f, 10, fill);
	DrawRectangleRoundedLinesEx(track, 0.7f, 10, 0.8f, ColorAlpha(WHITE, 0.12f));
	DrawCenteredUIText(dead ? "Time left 0.0s" : TextFormat("Time left %.1fs", remain),
		(Rectangle){r.x, r.y + 38.0f, r.width, 12.0f}, 9.0f, ColorAlpha((Color){250, 239, 219, 255}, 0.94f), 0.0f);
}

/* ======================
Fungsi DrawUI
=======================
Fungsi ini digunakan untuk menggambar ui.
*/
void DrawUI(const Guppy *g, const Carnivore *c, const Ultravore *u, const AquariumUiOptions *options) {
	int screenWidth = GetScreenWidth();
	DrawRectangle(0, 0, screenWidth, 102, (Color){86, 58, 40, 255});
	DrawRectangleGradientV(0, 0, screenWidth, 102, (Color){122, 85, 60, 124}, (Color){58, 37, 24, 0});
	DrawLine(0, 101, screenWidth, 101, (Color){37, 22, 14, 255});

	Rectangle rects[9];
	UI_GetButtonRects(rects);
	Rectangle leftGroup = {rects[0].x - 8.0f, 8.0f, rects[3].x + rects[3].width - rects[0].x + 16.0f, 84.0f};
	Rectangle rightGroup = {rects[4].x - 8.0f, 8.0f, rects[8].x + rects[8].width - rects[4].x + 16.0f, 84.0f};
	Rectangle centerInfo = {leftGroup.x + leftGroup.width + 18.0f, 18.0f, rightGroup.x - (leftGroup.x + leftGroup.width) - 36.0f, 58.0f};
	Rectangle infoTop = {centerInfo.x + 12.0f, centerInfo.y + 8.0f, centerInfo.width - 24.0f, 18.0f};
	Rectangle statusPanel = {screenWidth - 306.0f, 118.0f, 278.0f, 200.0f};
	Rectangle statusTitle = {statusPanel.x + 16.0f, statusPanel.y + 14.0f, statusPanel.width - 32.0f, 20.0f};
	Rectangle statusA = {statusPanel.x + 14.0f, statusPanel.y + 46.0f, statusPanel.width - 28.0f, 46.0f};
	Rectangle statusB = {statusPanel.x + 14.0f, statusPanel.y + 98.0f, statusPanel.width - 28.0f, 46.0f};
	Rectangle statusC = {statusPanel.x + 14.0f, statusPanel.y + 150.0f, statusPanel.width - 28.0f, 46.0f};
	const AquariumUiOptions fallbackOptions = {false, false, BUBBLE_CTRL_NORMAL, 1.0f};
	const AquariumUiOptions *uiOptions = options ? options : &fallbackOptions;

	DrawRectangleRounded(leftGroup, 0.14f, 10, ColorAlpha((Color){63, 41, 28, 255}, 0.34f));
	DrawRectangleRounded(rightGroup, 0.14f, 10, ColorAlpha((Color){63, 41, 28, 255}, 0.34f));
	DrawRectangleRounded(centerInfo, 0.16f, 10, ColorAlpha((Color){8, 20, 32, 255}, 0.52f));
	DrawRectangleRoundedLinesEx(centerInfo, 0.16f, 10, 1.0f, ColorAlpha((Color){255, 231, 197, 255}, 0.22f));
	DrawUIText("AQUARIUM CONTROLS", (Vector2){infoTop.x, infoTop.y}, 14.0f, (Color){249, 239, 219, 255});
	DrawUIText(TextFormat("Speed x%.2f  |  Bubble %s  |  Drop food with click",
		uiOptions->simSpeed, BubbleModeLabel(uiOptions->bubbleMode)),
		(Vector2){infoTop.x, infoTop.y + 14.0f}, 10.0f, ColorAlpha((Color){248, 240, 227, 255}, 0.84f));
	DrawUIText("Keys: A speed  W wire  B bubble", (Vector2){infoTop.x, infoTop.y + 28.0f}, 10.0f, ColorAlpha((Color){248, 240, 227, 255}, 0.72f));

	DrawRectangleRounded(statusPanel, 0.14f, 10, ColorAlpha((Color){10, 23, 38, 255}, 0.86f));
	DrawRectangleRoundedLinesEx(statusPanel, 0.14f, 10, 1.2f, ColorAlpha((Color){209, 228, 243, 255}, 0.24f));
	DrawRectangleGradientV((int)statusPanel.x, (int)statusPanel.y, (int)statusPanel.width, (int)statusPanel.height,
		ColorAlpha((Color){88, 122, 148, 255}, 0.08f), ColorAlpha((Color){16, 31, 44, 255}, 0.00f));
	DrawUIText("FISH STATUS", (Vector2){statusTitle.x, statusTitle.y}, 17.0f, (Color){244, 248, 252, 255});
	DrawUIText("Hunger timer before death", (Vector2){statusTitle.x, statusTitle.y + 18.0f}, 10.0f, ColorAlpha((Color){244, 248, 252, 255}, 0.76f));

	DrawFishStatusTile(statusA, "Cere", g && g->active, g && g->state == GUPPY_DEAD, g ? g->hunger : 0.0f, 10.0f, (Color){164, 201, 117, 255});
	DrawFishStatusTile(statusB, "Lele", c && c->active, c && c->state == CARNIVORE_DEAD, c ? c->hunger : 0.0f, 20.0f, (Color){138, 176, 129, 255});
	DrawFishStatusTile(statusC, "Toman", u && u->active, u && u->state == ULTRA_DEAD, u ? u->hunger : 0.0f, 30.0f, (Color){219, 116, 78, 255});

	DrawFishSpawnCard(rects[0], 0, "Cere");
	DrawFishSpawnCard(rects[1], 1, "Lele");
	DrawFishSpawnCard(rects[2], 2, "Toman");
	DrawActionCard(rects[3], "RESET", "Clear tank", (Color){171, 60, 46, 255}, (Color){255, 210, 180, 255}, (Color){255, 247, 238, 255});
	DrawActionCard(rects[4], "BUBBLE", BubbleModeLabel(uiOptions->bubbleMode),
		(Color){72, 118, 156, 255}, (Color){205, 238, 255, 255}, (Color){246, 250, 255, 255});
	DrawActionCard(rects[5], "SPEED", uiOptions->autoSpeedEnabled ? "Auto" : "Manual",
		uiOptions->autoSpeedEnabled ? (Color){73, 132, 92, 255} : (Color){112, 90, 70, 255},
		uiOptions->autoSpeedEnabled ? (Color){217, 255, 226, 255} : (Color){240, 219, 194, 255},
		(Color){250, 252, 246, 255});
	DrawActionCard(rects[6], "WIRE", uiOptions->wireframeEnabled ? "On" : "Off",
		uiOptions->wireframeEnabled ? (Color){92, 118, 173, 255} : (Color){92, 86, 104, 255},
		uiOptions->wireframeEnabled ? (Color){212, 235, 255, 255} : (Color){218, 211, 227, 255},
		(Color){250, 251, 255, 255});
	DrawActionCard(rects[7], "VOLUME", "Audio", (Color){67, 100, 131, 255}, (Color){179, 223, 255, 255}, (Color){246, 250, 255, 255});
	DrawActionCard(rects[8], "BACK", "Menu", (Color){62, 124, 96, 255}, (Color){204, 255, 219, 255}, (Color){245, 255, 247, 255});

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

/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "draw_env.h"
#include "../utils/primitives.h"

#include <math.h>

enum {
	kSeaweedFrameCount = 32,
	kSeaweedSpriteW = 104,
	kSeaweedSpriteH = 132,
	kSeaweedAnchorX = 28,
	kSeaweedAnchorY = 124
};

static RenderTexture2D s_seaweedFrames[kSeaweedFrameCount] = {0};
static unsigned char s_seaweedFrameReady[kSeaweedFrameCount] = {0};

/* ======================
Fungsi DrawSeaweedAccentBubble
=======================
Fungsi ini digunakan untuk menggambar bubble aksen di sekitar seaweed.
*/
static void DrawSeaweedAccentBubble(Vector2 center, float radius, float alpha) {
	DrawCircleV(center, radius, ColorAlpha((Color){180, 220, 255, 255}, alpha * 0.55f));
	DrawCircleLines((int)center.x, (int)center.y, radius, ColorAlpha((Color){239, 249, 255, 255}, alpha));
	DrawCircleLines((int)center.x, (int)center.y, radius - 2.0f, ColorAlpha((Color){150, 212, 245, 255}, alpha * 0.72f));
	DrawCircleV((Vector2){center.x - radius * 0.24f, center.y - radius * 0.20f}, radius * 0.18f,
		ColorAlpha(WHITE, alpha * 0.92f));
}

/* ======================
Fungsi DrawSeaweedFrond
=======================
Fungsi ini digunakan untuk menggambar satu helai rumput laut.
*/
static void DrawSeaweedFrond(Vector2 root, float height, float thickness, float time,
	float phase, float lean, float tipLift, bool withLeaf,
	Color baseColor, Color midColor, Color tipColor) {
	float sway = sinf(time * 1.7f + phase) * (10.0f + thickness * 1.35f)
		+ cosf(time * 1.15f + phase * 0.7f) * 3.4f;
	Vector2 a = root;
	Vector2 b = {root.x + sway * 0.24f + lean * 0.14f, root.y - height * 0.28f};
	Vector2 c = {root.x + sway * 0.56f + lean * 0.46f, root.y - height * 0.60f};
	Vector2 d = {root.x + sway + lean, root.y - height - tipLift};

	DrawLineBezier(a, b, thickness, baseColor);
	DrawLineBezier(b, c, fmaxf(1.4f, thickness - 0.9f), midColor);
	DrawLineBezier(c, d, fmaxf(1.1f, thickness - 1.8f), tipColor);
	DrawCircleV(c, fmaxf(2.0f, thickness * 0.70f), ColorAlpha((Color){84, 196, 136, 255}, 0.28f));

	if (!withLeaf) return;

	Vector2 leafA = {root.x + sway * 0.34f + lean * 0.22f, root.y - height * 0.46f};
	Vector2 leafB = {leafA.x + sway * 0.18f + 8.0f, leafA.y - height * 0.14f};
	DrawLineBezier(leafA, leafB, fmaxf(1.0f, thickness - 2.1f), ColorAlpha(tipColor, 0.82f));
}

/* ======================
Fungsi DrawSeaweedGenerated
=======================
Fungsi ini digunakan untuk menggambar seaweed generated.
*/
static void DrawSeaweedGenerated(Vector2 pos, float time) {
	for (int i = 0; i < 5; i++) {
		DrawSeaweedFrond(
			(Vector2){pos.x + i * 12.0f, pos.y},
			88.0f + (float)(i % 2) * 14.0f,
			5.0f - i * 0.35f,
			time,
			i * 0.8f,
			-4.0f + i * 1.4f,
			(float)((i + 1) % 2) * 4.0f,
			(i % 2) == 0,
			(Color){30, 124, 90, 255},
			(Color){38, 144, 96, 255},
			(Color){45, 166, 110, 255});
	}
}

/* ======================
Fungsi SeaweedFrameIndex
=======================
Fungsi ini digunakan untuk menjalankan proses SeaweedFrameIndex.
*/
static int SeaweedFrameIndex(float time) {
	float phase = time * 1.7f;
	float normalized = fmodf(phase, 2.0f * PI);
	if (normalized < 0.0f) normalized += 2.0f * PI;
	int frame = (int)floorf((normalized / (2.0f * PI)) * kSeaweedFrameCount);
	if (frame < 0) frame = 0;
	if (frame >= kSeaweedFrameCount) frame = kSeaweedFrameCount - 1;
	return frame;
}

/* ======================
Fungsi EnsureSeaweedFrame
=======================
Fungsi ini digunakan untuk memastikan seaweed frame.
*/
static void EnsureSeaweedFrame(int frame) {
	if (s_seaweedFrameReady[frame]) return;

	s_seaweedFrames[frame] = LoadRenderTexture(kSeaweedSpriteW, kSeaweedSpriteH);
	BeginTextureMode(s_seaweedFrames[frame]);
	ClearBackground(BLANK);
	DrawSeaweedGenerated((Vector2){(float)kSeaweedAnchorX, (float)kSeaweedAnchorY},
		((float)frame / (float)kSeaweedFrameCount) * ((2.0f * PI) / 1.7f));
	EndTextureMode();
	s_seaweedFrameReady[frame] = 1;
}

/* ======================
Fungsi WarmSeaweedSpriteCache
=======================
Fungsi ini digunakan untuk menyiapkan seaweed sprite cache.
*/
void WarmSeaweedSpriteCache(void) {
	for (int frame = 0; frame < kSeaweedFrameCount; frame++) {
		EnsureSeaweedFrame(frame);
	}
}

/* ======================
Fungsi DrawWaterBackgroundRect
=======================
Fungsi ini digunakan untuk menggambar water background rect.
*/
void DrawWaterBackgroundRect(Rectangle area) {
	DrawRectangleGradientV((int)area.x, (int)area.y, (int)area.width, (int)area.height,
		(Color){96, 188, 226, 255}, (Color){11, 70, 128, 255});
	DrawRectangleGradientV((int)area.x, (int)(area.y + area.height * 0.28f), (int)area.width, (int)(area.height * 0.72f),
		ColorAlpha((Color){74, 159, 214, 255}, 0.34f), ColorAlpha((Color){6, 36, 82, 255}, 0.72f));
	DrawRectangleGradientV((int)area.x, (int)area.y, (int)area.width, (int)(area.height * 0.18f),
		ColorAlpha(WHITE, 0.16f), BLANK);
	DrawCircleGradient((int)(area.x + area.width * 0.18f), (int)(area.y + area.height * 0.12f), area.width * 0.22f,
		ColorAlpha((Color){235, 249, 255, 255}, 0.34f), BLANK);
	DrawCircleGradient((int)(area.x + area.width * 0.78f), (int)(area.y + area.height * 0.18f), area.width * 0.28f,
		ColorAlpha((Color){176, 231, 255, 255}, 0.22f), BLANK);
	DrawCircleGradient((int)(area.x + area.width * 0.50f), (int)(area.y + area.height * 0.78f), area.width * 0.42f,
		ColorAlpha((Color){6, 35, 84, 255}, 0.42f), BLANK);

	for (int i = 0; i < 5; i++) {
		float beamX = area.x + area.width * (0.10f + i * 0.18f);
		float beamW = area.width * 0.09f;
		DrawRectangleGradientV((int)beamX, (int)(area.y + area.height * 0.10f), (int)beamW, (int)(area.height * 0.70f),
			ColorAlpha(WHITE, 0.10f), BLANK);
	}

	DrawRectangleGradientV((int)area.x, (int)(area.y + area.height * 0.87f), (int)area.width, (int)(area.height * 0.13f),
		(Color){204, 181, 118, 255}, (Color){158, 130, 82, 255});
	for (int i = 0; i < (int)area.width; i += 18) {
		DrawCircle((int)(area.x + i + 6), (int)(area.y + area.height - 12 + (i % 7) * 0.2f), 1.5f, ColorAlpha((Color){241, 223, 174, 255}, 0.46f));
	}

	DrawRectangleGradientH((int)area.x, (int)area.y, 10, (int)area.height, ColorAlpha((Color){6, 34, 70, 255}, 0.24f), BLANK);
	DrawRectangleGradientH((int)(area.x + area.width - 10), (int)area.y, 10, (int)area.height, BLANK, ColorAlpha((Color){6, 34, 70, 255}, 0.24f));
	DrawRectangleGradientV((int)area.x, (int)area.y, (int)area.width, 8, ColorAlpha(WHITE, 0.30f), BLANK);
}

/* ======================
Fungsi DrawWaterBackground
=======================
Fungsi ini digunakan untuk menggambar water background.
*/
void DrawWaterBackground(void) {
	DrawWaterBackgroundRect((Rectangle){0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()});
}

/* ======================
Fungsi DrawSand
=======================
Fungsi ini digunakan untuk menggambar sand.
*/
void DrawSand(void) {
	int w = GetScreenWidth();
	int baseY = GetScreenHeight() - 80;
	DrawRectangle(0, baseY, w, 80, (Color){210,180,100,255});
	for (int i = 0; i < w; i += 20) {
		float offset = sinf(i * 0.05f) * 6.0f;
		DrawRectangle(i, baseY + (int)offset, 20, 80 - (int)offset, (Color){200,170,90,255});
	}
	for (int i = 0; i < w; i += 14) {
		DrawCircle(i, baseY + 8 + (i % 9), 1.2f, (Color){242, 220, 150, 120});
	}
}

/* ======================
Fungsi DrawSeaweed
=======================
Fungsi ini digunakan untuk menggambar seaweed.
*/
void DrawSeaweed(Vector2 pos, float time) {
	int frame = SeaweedFrameIndex(time);
	EnsureSeaweedFrame(frame);
	DrawTextureRec(s_seaweedFrames[frame].texture,
		(Rectangle){0.0f, 0.0f, (float)kSeaweedSpriteW, (float)-kSeaweedSpriteH},
		(Vector2){pos.x - kSeaweedAnchorX, pos.y - kSeaweedAnchorY},
		WHITE);
}

/* ======================
Fungsi DrawSeaweedClusterRect
=======================
Fungsi ini digunakan untuk menggambar cluster rumput laut bergaya game
di dalam area tertentu.
*/
void DrawSeaweedClusterRect(Rectangle area, float time, int variant, bool withBubbles, bool drawSand) {
	float sandHeight = area.height * 0.18f;
	float baseY = area.y + area.height - 4.0f;
	int frondCount = 6 + (variant & 1);
	float rootMinX = area.x + 18.0f;
	float rootMaxX = area.x + area.width - 20.0f;

	if (drawSand) {
		DrawRectangleGradientV((int)area.x, (int)(area.y + area.height - sandHeight), (int)area.width, (int)sandHeight,
			(Color){204, 181, 118, 220}, (Color){162, 135, 86, 220});
	}

	for (int i = 0; i < frondCount; i++) {
		float lane = (frondCount <= 1) ? 0.5f : (float)i / (float)(frondCount - 1);
		float rootX = rootMinX + lane * (rootMaxX - rootMinX);
		float height = area.height * (0.44f + (float)((i + variant) % 3) * 0.09f);
		float thickness = 4.8f - lane * 1.6f;
		float lean = -6.0f + lane * 12.0f + sinf(variant * 0.7f + i * 0.45f) * 3.0f;
		unsigned char tint = (unsigned char)(i * 5);

		DrawSeaweedFrond(
			(Vector2){rootX, baseY},
			height,
			thickness,
			time,
			i * 0.72f + variant * 0.55f,
			lean,
			(float)((i + variant) % 2) * (area.height * 0.03f),
			((i + variant) % 2) == 0,
			(Color){30, (unsigned char)(124 + tint), (unsigned char)(90 + tint / 2), 255},
			(Color){38, (unsigned char)(144 + tint), (unsigned char)(96 + tint / 2), 255},
			(Color){45, (unsigned char)(166 + tint), (unsigned char)(110 + tint / 2), 255});
	}

	if (!withBubbles) return;

	for (int i = 0; i < 5; i++) {
		float phase = time * 0.6f + i * 0.85f + variant;
		Vector2 bubblePos = {
			area.x + area.width * (0.18f + (float)(i % 3) * 0.23f) + sinf(phase) * 6.0f,
			area.y + area.height * (0.66f - i * 0.10f) + cosf(phase * 1.1f) * 4.0f
		};
		DrawSeaweedAccentBubble(bubblePos, 4.0f + (float)(i % 2) * 2.5f, 0.38f + i * 0.08f);
	}
}

/* ======================
Fungsi DrawWaterRefractionOverlay
=======================
Fungsi ini digunakan untuk menggambar refraksi cahaya air.
*/
void DrawWaterRefractionOverlay(float time) {
	float w = (float)GetScreenWidth();
	float h = (float)GetScreenHeight() - 86.0f;

	for (int i = 0; i < 10; i++) {
		float phase = time * (0.22f + i * 0.015f) + i * 0.9f;
		float y = 136.0f + i * 64.0f + sinf(phase) * 18.0f;
		float x = w * (0.10f + i * 0.085f) + cosf(phase * 0.8f) * 40.0f;
		float length = 120.0f + sinf(phase * 1.3f) * 34.0f;
		DrawLineEx((Vector2){x - length * 0.5f, y}, (Vector2){x + length * 0.5f, y + sinf(phase * 1.7f) * 5.0f},
			1.2f, ColorAlpha((Color){241, 250, 255, 255}, 0.08f));
	}

	for (int i = 0; i < 20; i++) {
		float phase = time * 0.16f + i * 1.2f;
		float x = w * (0.06f + (float)(i % 10) * 0.09f) + sinf(phase * 1.1f) * 14.0f;
		float y = 150.0f + (float)(i / 10) * (h * 0.35f) + cosf(phase * 0.9f) * 20.0f;
		DrawCircleV((Vector2){x, y}, 1.2f + 0.4f * sinf(phase), ColorAlpha((Color){229, 243, 255, 255}, 0.10f));
	}
}

/* ======================
Fungsi DrawCoral
=======================
Fungsi ini digunakan untuk menggambar coral.
*/
void DrawCoral(Vector2 pos) {
	DrawEllipse((int)pos.x, (int)pos.y, 84, 58, (Color){93, 70, 134, 255});
	DrawEllipse((int)(pos.x - 5), (int)(pos.y + 4), 66, 42, (Color){69, 51, 106, 255});
	DrawCircle((int)(pos.x - 20), (int)pos.y, 26, (Color){41, 33, 64, 255});
	for (int i = 0; i < 8; i++) {
		float ox = (float)(-72 + i * 18);
		float oy = (float)(i % 3) * 6.0f;
		DrawEllipse((int)(pos.x + ox), (int)(pos.y - 34 - oy), 9, 14, (Color){166, 59, 68, 255});
		DrawCircle((int)(pos.x + ox + 3), (int)(pos.y - 44 - oy), 5, (Color){214, 177, 45, 255});
	}
	DrawRectangleRounded((Rectangle){pos.x - 52, pos.y - 14, 102, 48}, 0.35f, 8, (Color){28, 32, 56, 205});
	DrawRectangleRoundedLines((Rectangle){pos.x - 52, pos.y - 14, 102, 48}, 0.35f, 8, (Color){74, 60, 108, 220});
}

/* ======================
Fungsi DrawHelmet
=======================
Fungsi ini digunakan untuk menggambar helmet.
*/
void DrawHelmet(Vector2 pos) {
	DrawCircle((int)pos.x, (int)pos.y, 56, (Color){184, 144, 84, 255});
	DrawCircle((int)(pos.x - 10), (int)(pos.y - 10), 44, (Color){212, 178, 114, 45});
	DrawCircleLines((int)pos.x, (int)pos.y, 56, (Color){124, 92, 40, 255});
	DrawCircle((int)pos.x, (int)pos.y, 24, (Color){35, 59, 102, 210});
	DrawCircle((int)pos.x, (int)pos.y, 18, (Color){109, 167, 226, 60});
	DrawCircleLines((int)pos.x, (int)pos.y, 24, (Color){233, 197, 117, 255});
	DrawCircle((int)(pos.x - 30), (int)pos.y, 12, (Color){35, 59, 102, 210});
	DrawCircle((int)(pos.x + 30), (int)pos.y, 12, (Color){35, 59, 102, 210});
	DrawCircleLines((int)(pos.x - 30), (int)pos.y, 12, (Color){233, 197, 117, 255});
	DrawCircleLines((int)(pos.x + 30), (int)pos.y, 12, (Color){233, 197, 117, 255});
	DrawCircle((int)(pos.x + 18), (int)(pos.y - 34), 14, (Color){205, 174, 103, 255});
	DrawCircleLines((int)(pos.x + 18), (int)(pos.y - 34), 14, (Color){138, 108, 54, 255});
	for (int i = 0; i < 6; i++) {
		DrawCircle((int)(pos.x - 40 + i * 16), (int)(pos.y + 52), 2.0f, (Color){136, 103, 51, 255});
	}
}

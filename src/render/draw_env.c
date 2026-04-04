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
Fungsi DrawSeaweedGenerated
=======================
Fungsi ini digunakan untuk menggambar seaweed generated.
*/
static void DrawSeaweedGenerated(Vector2 pos, float time) {
	for (int i = 0; i < 5; i++) {
		float sway = sinf(time * 1.7f + i * 0.8f) * (10.0f + i * 1.5f);
		Vector2 a = { pos.x + i * 12.0f, pos.y };
		Vector2 b = { pos.x + i * 12.0f + sway * 0.35f, pos.y - 28 };
		Vector2 c = { pos.x + i * 10.0f + sway * 0.7f, pos.y - 58 };
		Vector2 d = { pos.x + i * 8.0f + sway, pos.y - (88 + (i % 2) * 14) };
		DrawLineBezier(a, b, 5.0f, (Color){30, 124, 90, 255});
		DrawLineBezier(b, c, 4.0f, (Color){38, 144, 96, 255});
		DrawLineBezier(c, d, 3.0f, (Color){45, 166, 110, 255});
		DrawCircleV(c, 3.0f, (Color){84, 196, 136, 70});
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

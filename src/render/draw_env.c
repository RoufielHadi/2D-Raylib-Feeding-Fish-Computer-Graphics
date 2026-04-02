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

static int SeaweedFrameIndex(float time) {
	float phase = time * 1.7f;
	float normalized = fmodf(phase, 2.0f * PI);
	if (normalized < 0.0f) normalized += 2.0f * PI;
	int frame = (int)floorf((normalized / (2.0f * PI)) * kSeaweedFrameCount);
	if (frame < 0) frame = 0;
	if (frame >= kSeaweedFrameCount) frame = kSeaweedFrameCount - 1;
	return frame;
}

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

void WarmSeaweedSpriteCache(void) {
	for (int frame = 0; frame < kSeaweedFrameCount; frame++) {
		EnsureSeaweedFrame(frame);
	}
}

void DrawWaterBackground(void) {
	int w = GetScreenWidth();
	int h = GetScreenHeight();
	for (int y = 0; y < h; y++) {
		float t = (float)y / (float)h;
		float glow = sinf(t * PI) * 35.0f;
		Color col = (Color){(unsigned char)(18 + 24 * t), (unsigned char)(78 + 70 * t + glow * 0.2f), (unsigned char)(148 + 78 * t + glow), 255};
		DrawLine(0, y, w, y, col);
	}
	for (int i = 0; i < 18; i++) {
		float y = 30.0f + i * 22.0f;
		DrawEllipse((int)(w * 0.48f + sinf(i) * 120.0f), (int)y, 140, 10, (Color){255, 255, 255, (unsigned char)(14 + (i % 3) * 6)});
	}
}

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

void DrawSeaweed(Vector2 pos, float time) {
	int frame = SeaweedFrameIndex(time);
	EnsureSeaweedFrame(frame);
	DrawTextureRec(s_seaweedFrames[frame].texture,
		(Rectangle){0.0f, 0.0f, (float)kSeaweedSpriteW, (float)-kSeaweedSpriteH},
		(Vector2){pos.x - kSeaweedAnchorX, pos.y - kSeaweedAnchorY},
		WHITE);
}

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

void DrawEnvironment(float time) {
	DrawSand();
	DrawCoral((Vector2){GetScreenWidth() * 0.12f, GetScreenHeight() - 80.0f});
	DrawSeaweed((Vector2){GetScreenWidth() * 0.24f, GetScreenHeight()}, time);
	DrawSeaweed((Vector2){GetScreenWidth() * 0.60f, GetScreenHeight()}, time + 1.0f);
	DrawHelmet((Vector2){GetScreenWidth() * 0.84f, GetScreenHeight() - 120.0f});
}

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

/* ======================
Fungsi DrawBubble
=======================
Fungsi ini digunakan untuk menggambar bubble.
*/
void DrawBubble(const Bubble *b) {
	if (!b || !b->active) return;

	unsigned char alpha = (unsigned char)((b->alpha < 0.0f) ? 0.0f : ((b->alpha > 255.0f) ? 255.0f : b->alpha));
	float r = b->radius;
	Color shell = (Color){198, 228, 255, (unsigned char)(alpha * 0.58f)};
	Color outer = (Color){238, 248, 255, alpha};
	Color inner = (Color){171, 223, 249, (unsigned char)(alpha * 0.72f)};

	DrawCircleV(b->pos, r, shell);
	DrawCircleLines((int)b->pos.x, (int)b->pos.y, r, outer);
	if (r > 3.0f) {
		DrawCircleLines((int)b->pos.x, (int)b->pos.y, r - 2.0f, inner);
	}
	DrawCircleV((Vector2){b->pos.x - r * 0.28f, b->pos.y - r * 0.26f}, r * 0.24f, (Color){255, 255, 255, (unsigned char)(alpha * 0.92f)});
	DrawCircleV((Vector2){b->pos.x + r * 0.12f, b->pos.y + r * 0.10f}, r * 0.10f, (Color){220, 238, 255, (unsigned char)(alpha * 0.42f)});
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

/* ======================
Fungsi WarmBubbleSpriteCache
=======================
Fungsi ini digunakan untuk menyiapkan bubble sprite cache.
*/
void WarmBubbleSpriteCache(void) {
}

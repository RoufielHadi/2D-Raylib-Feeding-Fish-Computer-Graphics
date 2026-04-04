/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "button.h"
#include "../utils/primitives.h"

static Font s_uiFont;
static bool s_fontLoaded = false;

/* ======================
Fungsi UI_SetSharedFont
=======================
Fungsi ini digunakan untuk mengatur shared font.
*/
void UI_SetSharedFont(Font font, bool loaded) {
	s_uiFont = font;
	s_fontLoaded = loaded;
}

/* ======================
Fungsi UI_GetSharedFont
=======================
Fungsi ini digunakan untuk mengambil shared font.
*/
Font UI_GetSharedFont(void) {
	return s_uiFont;
}

/* ======================
Fungsi UI_HasSharedFont
=======================
Fungsi ini digunakan untuk menjalankan proses UI_HasSharedFont.
*/
bool UI_HasSharedFont(void) {
	return s_fontLoaded;
}

/* ======================
Fungsi DrawButton
=======================
Fungsi ini digunakan untuk menggambar button.
*/
void DrawButton(Rectangle r, const char *label) {
	Vector2 mp = GetMousePosition();
	bool hover = CheckCollisionPointRec(mp, r);
	float inset = hover ? 3.0f : 0.0f;
	Rectangle inner = (Rectangle){r.x + inset, r.y + inset, r.width - inset * 2.0f, r.height - inset * 2.0f};
	DrawRectangleRounded(inner, 0.28f, 8, hover ? (Color){146, 105, 67, 255} : (Color){123, 88, 58, 255});
	DrawRectangleRoundedLines(inner, 0.28f, 8, hover ? (Color){96, 64, 38, 255} : (Color){69, 46, 27, 255});

	if (UI_HasSharedFont()) {
		float fontSize = hover ? 17.0f : 16.0f;
		Vector2 size = MeasureTextEx(UI_GetSharedFont(), label, fontSize, 1.0f);
		DrawTextEx(UI_GetSharedFont(), label,
			(Vector2){inner.x + (inner.width - size.x) / 2.0f, inner.y + (inner.height - size.y) / 2.0f - 1.0f},
			fontSize, 1.0f, (Color){252, 243, 230, 255});
	} else {
		int fontSize = hover ? 15 : 14;
		int txtW = MeasureText(label, fontSize);
		DrawText(label, (int)(inner.x + (inner.width - txtW)/2), (int)(inner.y + (inner.height - fontSize)/2), fontSize, (Color){252, 243, 230, 255});
	}
}

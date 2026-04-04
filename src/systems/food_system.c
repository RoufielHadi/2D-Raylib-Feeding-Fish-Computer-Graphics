/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "food_system.h"
#include "raylib.h"
#include <math.h>

/* ======================
Fungsi UpdateFoodSystem
=======================
Fungsi ini digunakan untuk memperbarui food system.
*/
void UpdateFoodSystem(Food *foods, int foodCount, float dt) {
	if (!foods) return;

	// Kecepatan pelet dibuat konstan hingga mencapai dasar aquarium.
	const float fallSpeed = 30.0f;
	const float groundY = GetScreenHeight() - 80.0f;

	for (int i = 0; i < foodCount; i++) {
		Food *f = &foods[i];
		if (!f->active) continue;

		f->time += dt;
		f->vel.y = fallSpeed;
		f->pos.x += f->vel.x * dt;
		f->pos.y += f->vel.y * dt;

		if (f->pos.y >= groundY) {
			f->pos.y = groundY;
			f->vel = (Vector2){0, 0};
			f->groundedTime += dt;

			// Hapus pelet yang terlalu lama diam agar aquarium tidak penuh.
			if (f->groundedTime >= 3.0f) {
				f->active = false;
				f->state = FOOD_EATEN;
				continue;
			}
		} else {
			f->groundedTime = 0.0f;
		}

		f->rotation += sinf(f->time * 6.0f) * 60.0f * dt;
	}
}

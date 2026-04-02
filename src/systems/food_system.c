#include "food_system.h"
#include "raylib.h"
#include <math.h>

void UpdateFoodSystem(Food *foods, int foodCount, float dt) {
	if (!foods) return;
	const float gravity = 120.0f;
	const float groundY = GetScreenHeight() - 80.0f;

	for (int i = 0; i < foodCount; i++) {
		Food *f = &foods[i];
		if (!f->active) continue;

		f->time += dt;
		f->vel.y += gravity * dt;
		f->pos.x += f->vel.x * dt;
		f->pos.y += f->vel.y * dt;

		if (f->pos.y >= groundY) {
			f->pos.y = groundY;
			f->vel = (Vector2){0, 0};
			f->groundedTime += dt;
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

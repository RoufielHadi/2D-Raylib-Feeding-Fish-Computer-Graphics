#include "food.h"

void InitFood(Food *f, Vector2 pos) {
    f->pos = pos;
    f->vel = (Vector2){0, 0};
    f->time = 0;
    f->rotation = 0;
    f->groundedTime = 0;
    f->active = true;
    f->state = FOOD_ACTIVE;
}

bool IsFoodActive(Food f) {
    return f.active && f.state == FOOD_ACTIVE;
}

void MarkFoodEaten(Food *f) {
    f->active = false;
    f->state = FOOD_EATEN;
}

void ResetFood(Food *f) {
    f->vel = (Vector2){0,0};
    f->time = 0;
    f->rotation = 0;
    f->groundedTime = 0;
    f->active = true;
    f->state = FOOD_ACTIVE;
}

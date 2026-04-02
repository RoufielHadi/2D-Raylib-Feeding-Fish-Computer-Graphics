#ifndef DRAW_FOOD_H
#define DRAW_FOOD_H

#include "raylib.h"
#include "../entities/food.h"

void DrawFood(const Food *f);
void DrawAllFood(const Food *foods, int count);
void WarmFoodSpriteCache(void);

#endif


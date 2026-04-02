#ifndef FOOD_H
#define FOOD_H

#include "raylib.h"
#include <stdbool.h>

typedef enum {
    FOOD_ACTIVE,
    FOOD_EATEN
} FoodState;

typedef struct {
    Vector2 pos;
    Vector2 vel;

    float time;
    float rotation;
    float groundedTime;

    bool active;
    FoodState state;
} Food;

void InitFood(Food *f, Vector2 pos);
bool IsFoodActive(Food f);
void MarkFoodEaten(Food *f);
void ResetFood(Food *f);

#endif

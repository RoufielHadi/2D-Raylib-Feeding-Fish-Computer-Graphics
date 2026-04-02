#ifndef COLLISION_H
#define COLLISION_H

#include "raylib.h"
#include <stdbool.h>

float DistanceBetween(Vector2 a, Vector2 b);
bool WithinDistance(Vector2 a, Vector2 b, float thresh);

#endif

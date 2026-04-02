#ifndef DRAW_ENV_H
#define DRAW_ENV_H

#include "raylib.h"

void DrawWaterBackground(void);
void DrawSand(void);
void DrawSeaweed(Vector2 pos, float time);
void DrawCoral(Vector2 pos);
void DrawHelmet(Vector2 pos);
void DrawEnvironment(float time);
void WarmSeaweedSpriteCache(void);

#endif


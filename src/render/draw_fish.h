#ifndef DRAW_FISH_H
#define DRAW_FISH_H

#include "raylib.h"
#include "../entities/guppy.h"
#include "../entities/carnivore.h"
#include "../entities/ultravore.h"

void DrawGuppy(const Guppy *g, float animTime);
void DrawPiranha(const Carnivore *c, float animTime);
void DrawUltravore(const Ultravore *u, float animTime);
void DrawGuppyPreview(const Guppy *g, float animTime);
void DrawPiranhaPreview(const Carnivore *c, float animTime);
void DrawUltravorePreview(const Ultravore *u, float animTime);
void WarmFishSpriteCaches(void);

#endif

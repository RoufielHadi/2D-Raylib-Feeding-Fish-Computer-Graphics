#ifndef DRAW_UI_H
#define DRAW_UI_H

#include "raylib.h"
#include "../entities/guppy.h"
#include "../entities/carnivore.h"
#include "../entities/ultravore.h"

void DrawUI(const Guppy *g, const Carnivore *c, const Ultravore *u);
void WarmHeaderUICache(void);

#endif

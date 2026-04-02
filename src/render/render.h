#ifndef RENDER_H
#define RENDER_H

#include "raylib.h"
#include "draw_env.h"
#include "draw_food.h"
#include "draw_bubble.h"
#include "draw_fish.h"
#include "draw_ui.h"

void RenderAll(const Guppy *guppies, int guppyCount,
               const Carnivore *carnivores, int carnivoreCount,
               const Ultravore *ultravores, int ultravoreCount,
               const Food *foods, int foodCount,
               const Bubble *bubbles, int bubbleCount,
               float time);
void WarmGameRenderCaches(void);

#endif

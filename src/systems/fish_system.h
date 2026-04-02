#ifndef FISH_SYSTEM_H
#define FISH_SYSTEM_H

#include "../entities/guppy.h"
#include "../entities/carnivore.h"
#include "../entities/ultravore.h"
#include "../entities/food.h"

void UpdateGuppies(Guppy *guppies, int guppyCount, Food *foods, int foodCount, float dt);
void UpdateCarnivores(Carnivore *carnivores, int carnivoreCount, Guppy *guppies, int guppyCount, float dt);
void UpdateUltravoids(Ultravore *ultravoids, int ultravoreCount, Carnivore *carnivores, int carnivoreCount, float dt);

#endif

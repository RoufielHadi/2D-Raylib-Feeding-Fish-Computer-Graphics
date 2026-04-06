/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef RENDER_H
#define RENDER_H

#include "raylib.h"
#include "draw_env.h"
#include "draw_food.h"
#include "draw_bubble.h"
#include "draw_fish.h"
#include "draw_ui.h"

/* ======================
Fungsi RenderAll
=======================
Fungsi ini digunakan untuk merender all.
*/
void RenderAll(const Guppy *guppies, int guppyCount,
               const Carnivore *carnivores, int carnivoreCount,
               const Ultravore *ultravores, int ultravoreCount,
               const Food *foods, int foodCount,
               const Bubble *bubbles, int bubbleCount,
               float time,
               const AquariumUiOptions *options);

/* ======================
Fungsi WarmGameRenderCaches
=======================
Fungsi ini digunakan untuk menyiapkan game render caches.
*/
void WarmGameRenderCaches(void);

#endif

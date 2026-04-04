/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef DRAW_FOOD_H
#define DRAW_FOOD_H

#include "raylib.h"
#include "../entities/food.h"

/* ======================
Fungsi DrawFood
=======================
Fungsi ini digunakan untuk menggambar food.
*/
void DrawFood(const Food *f);

/* ======================
Fungsi DrawAllFood
=======================
Fungsi ini digunakan untuk menggambar all food.
*/
void DrawAllFood(const Food *foods, int count);

/* ======================
Fungsi WarmFoodSpriteCache
=======================
Fungsi ini digunakan untuk menyiapkan food sprite cache.
*/
void WarmFoodSpriteCache(void);

#endif

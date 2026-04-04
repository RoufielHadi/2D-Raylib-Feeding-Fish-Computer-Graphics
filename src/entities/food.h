/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

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

/* ======================
Fungsi InitFood
=======================
Fungsi ini digunakan untuk menginisialisasi food.
*/
void InitFood(Food *f, Vector2 pos);

/* ======================
Fungsi IsFoodActive
=======================
Fungsi ini digunakan untuk memeriksa food active.
*/
bool IsFoodActive(Food f);

/* ======================
Fungsi MarkFoodEaten
=======================
Fungsi ini digunakan untuk menjalankan proses MarkFoodEaten.
*/
void MarkFoodEaten(Food *f);

/* ======================
Fungsi ResetFood
=======================
Fungsi ini digunakan untuk mengatur ulang food.
*/
void ResetFood(Food *f);

#endif

/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "food.h"

/* ======================
Fungsi InitFood
=======================
Fungsi ini digunakan untuk menginisialisasi food.
*/
void InitFood(Food *f, Vector2 pos) {
    f->pos = pos;
    f->vel = (Vector2){0, 0};
    f->time = 0;
    f->rotation = 0;
    f->groundedTime = 0;
    f->active = true;
    f->state = FOOD_ACTIVE;
}

/* ======================
Fungsi IsFoodActive
=======================
Fungsi ini digunakan untuk memeriksa food active.
*/
bool IsFoodActive(Food f) {
    return f.active && f.state == FOOD_ACTIVE;
}

/* ======================
Fungsi MarkFoodEaten
=======================
Fungsi ini digunakan untuk menjalankan proses MarkFoodEaten.
*/
void MarkFoodEaten(Food *f) {
    f->active = false;
    f->state = FOOD_EATEN;
}

/* ======================
Fungsi ResetFood
=======================
Fungsi ini digunakan untuk mengatur ulang food.
*/
void ResetFood(Food *f) {
    f->vel = (Vector2){0,0};
    f->time = 0;
    f->rotation = 0;
    f->groundedTime = 0;
    f->active = true;
    f->state = FOOD_ACTIVE;
}

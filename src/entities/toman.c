/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "toman.h"

/* ======================
Fungsi InitUltravore
=======================
Fungsi ini digunakan untuk menginisialisasi ultravore.
*/
void InitUltravore(Ultravore *u, Vector2 pos) {
    u->pos = pos;
    u->vel = (Vector2){64, 0};
    u->wanderTarget = pos;
    u->time = 0;
    u->dir = 1;
    u->tailSwing = 0.0f;
    u->wanderTimer = 0.0f;
    u->finSwing = 0.0f;
    u->scale = 1.50f + (float)GetRandomValue(0, 18) / 100.0f;
    u->depth = 0.84f + (float)GetRandomValue(0, 18) / 100.0f;
    u->colorShift = (float)GetRandomValue(-6, 8);
    u->eatFlash = 0.0f;
    u->deathTimer = 0.0f;
    u->hunger = 0;
    u->flipAngle = 0;
    u->active = true;
    u->state = ULTRA_NORMAL;
}

/* ======================
Fungsi IsUltravoreAlive
=======================
Fungsi ini digunakan untuk memeriksa ultravore alive.
*/
bool IsUltravoreAlive(Ultravore u) {
    return u.state != ULTRA_DEAD;
}

/* ======================
Fungsi IsUltravoreHungry
=======================
Fungsi ini digunakan untuk memeriksa ultravore hungry.
*/
bool IsUltravoreHungry(Ultravore u) {
    return (u.active && u.state != ULTRA_DEAD && u.hunger > 0.7f);
}

/* ======================
Fungsi ResetUltravore
=======================
Fungsi ini digunakan untuk mengatur ulang ultravore.
*/
void ResetUltravore(Ultravore *u) {
    u->vel = (Vector2){64, 0};
    u->wanderTarget = u->pos;
    u->time = 0;
    u->dir = 1;
    u->tailSwing = 0.0f;
    u->wanderTimer = 0.0f;
    u->finSwing = 0.0f;
    u->eatFlash = 0.0f;
    u->deathTimer = 0.0f;
    u->hunger = 0;
    u->flipAngle = 0;
    u->state = ULTRA_NORMAL;
    u->active = true;
}

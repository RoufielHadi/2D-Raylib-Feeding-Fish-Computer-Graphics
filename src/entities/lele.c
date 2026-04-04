/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "lele.h"

/* ======================
Fungsi InitCarnivore
=======================
Fungsi ini digunakan untuk menginisialisasi carnivore.
*/
void InitCarnivore(Carnivore *c, Vector2 pos) {
    c->pos = pos;
    c->vel = (Vector2){76, 0};
    c->wanderTarget = pos;
    c->time = 0;
    c->dir = 1;
    c->tailSwing = 0.0f;
    c->wanderTimer = 0.0f;
    c->finSwing = 0.0f;
    c->scale = 1.00f + (float)GetRandomValue(0, 10) / 100.0f;
    c->depth = 0.75f + (float)GetRandomValue(0, 24) / 100.0f;
    c->colorShift = (float)GetRandomValue(-8, 10);
    c->eatFlash = 0.0f;
    c->deathTimer = 0.0f;
    c->hunger = 0;
    c->flipAngle = 0;
    c->active = true;
    c->state = CARNIVORE_NORMAL;
}

/* ======================
Fungsi IsCarnivoreAlive
=======================
Fungsi ini digunakan untuk memeriksa carnivore alive.
*/
bool IsCarnivoreAlive(Carnivore c) {
    return c.state != CARNIVORE_DEAD;
}

/* ======================
Fungsi IsCarnivoreHungry
=======================
Fungsi ini digunakan untuk memeriksa carnivore hungry.
*/
bool IsCarnivoreHungry(Carnivore c) {
    return (c.active && c.state != CARNIVORE_DEAD && c.hunger > 0.6f);
}

/* ======================
Fungsi ResetCarnivore
=======================
Fungsi ini digunakan untuk mengatur ulang carnivore.
*/
void ResetCarnivore(Carnivore *c) {
    c->vel = (Vector2){76, 0};
    c->wanderTarget = c->pos;
    c->time = 0;
    c->dir = 1;
    c->tailSwing = 0.0f;
    c->wanderTimer = 0.0f;
    c->finSwing = 0.0f;
    c->eatFlash = 0.0f;
    c->deathTimer = 0.0f;
    c->hunger = 0;
    c->flipAngle = 0;
    c->state = CARNIVORE_NORMAL;
    c->active = true;
}

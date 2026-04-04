/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef FISH_SYSTEM_H
#define FISH_SYSTEM_H

#include "../entities/cere.h"
#include "../entities/lele.h"
#include "../entities/toman.h"
#include "../entities/food.h"

/* ======================
Fungsi UpdateGuppies
=======================
Fungsi ini digunakan untuk memperbarui guppies.
*/
void UpdateGuppies(Guppy *guppies, int guppyCount, Food *foods, int foodCount, float dt);

/* ======================
Fungsi UpdateCarnivores
=======================
Fungsi ini digunakan untuk memperbarui carnivores.
*/
void UpdateCarnivores(Carnivore *carnivores, int carnivoreCount, Guppy *guppies, int guppyCount, float dt);

/* ======================
Fungsi UpdateUltravoids
=======================
Fungsi ini digunakan untuk memperbarui ultravoids.
*/
void UpdateUltravoids(Ultravore *ultravoids, int ultravoreCount, Carnivore *carnivores, int carnivoreCount, float dt);

#endif

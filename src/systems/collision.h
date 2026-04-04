/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef COLLISION_H
#define COLLISION_H

#include "raylib.h"
#include <stdbool.h>

/* ======================
Fungsi DistanceBetween
=======================
Fungsi ini digunakan untuk menjalankan proses DistanceBetween.
*/
float DistanceBetween(Vector2 a, Vector2 b);

/* ======================
Fungsi WithinDistance
=======================
Fungsi ini digunakan untuk menjalankan proses WithinDistance.
*/
bool WithinDistance(Vector2 a, Vector2 b, float thresh);

#endif

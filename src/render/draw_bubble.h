/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef DRAW_BUBBLE_H
#define DRAW_BUBBLE_H

#include "raylib.h"
#include "../entities/bubble.h"

/* ======================
Fungsi DrawBubble
=======================
Fungsi ini digunakan untuk menggambar bubble.
*/
void DrawBubble(const Bubble *b);

/* ======================
Fungsi DrawAllBubbles
=======================
Fungsi ini digunakan untuk menggambar all bubbles.
*/
void DrawAllBubbles(const Bubble *arr, int count);

/* ======================
Fungsi WarmBubbleSpriteCache
=======================
Fungsi ini digunakan untuk menyiapkan bubble sprite cache.
*/
void WarmBubbleSpriteCache(void);

#endif

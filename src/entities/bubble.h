/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef BUBBLE_H
#define BUBBLE_H

#include "raylib.h"
#include <stdbool.h>

typedef enum {
    BUBBLE_ACTIVE,
    BUBBLE_POPPED
} BubbleState;

typedef struct {
    Vector2 pos;
    Vector2 vel;

    float radius;
    float time;

    float alpha;
    bool active;
    BubbleState state;

} Bubble;

/* ======================
Fungsi InitBubble
=======================
Fungsi ini digunakan untuk menginisialisasi bubble.
*/
void InitBubble(Bubble *b, Vector2 pos);

/* ======================
Fungsi IsBubbleActive
=======================
Fungsi ini digunakan untuk memeriksa bubble active.
*/
bool IsBubbleActive(Bubble b);

/* ======================
Fungsi PopBubble
=======================
Fungsi ini digunakan untuk menjalankan proses PopBubble.
*/
void PopBubble(Bubble *b);

/* ======================
Fungsi ResetBubble
=======================
Fungsi ini digunakan untuk mengatur ulang bubble.
*/
void ResetBubble(Bubble *b);

#endif

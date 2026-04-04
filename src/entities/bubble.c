/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "bubble.h"

/* ======================
Fungsi InitBubble
=======================
Fungsi ini digunakan untuk menginisialisasi bubble.
*/
void InitBubble(Bubble *b, Vector2 pos) {
    b->pos = pos;
    b->vel = (Vector2){0, (float)(-30 - GetRandomValue(0,30))};
    b->radius = (float)(GetRandomValue(4, 12));
    b->time = 0;
    b->alpha = (unsigned char)GetRandomValue(140, 240);
    b->active = true;
    b->state = BUBBLE_ACTIVE;
}

/* ======================
Fungsi IsBubbleActive
=======================
Fungsi ini digunakan untuk memeriksa bubble active.
*/
bool IsBubbleActive(Bubble b) {
    return b.active && b.state == BUBBLE_ACTIVE;
}

/* ======================
Fungsi PopBubble
=======================
Fungsi ini digunakan untuk menjalankan proses PopBubble.
*/
void PopBubble(Bubble *b) {
    b->active = false;
    b->state = BUBBLE_POPPED;
}

/* ======================
Fungsi ResetBubble
=======================
Fungsi ini digunakan untuk mengatur ulang bubble.
*/
void ResetBubble(Bubble *b) {
    b->time = 0;
    b->alpha = 180;
    b->active = true;
    b->state = BUBBLE_ACTIVE;
}

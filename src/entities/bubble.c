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
    float radius = (float)(GetRandomValue(4, 12));
    float normalized = (radius - 4.0f) / 8.0f;
    float speedScale = 1.55f - normalized * 0.78f;

    b->pos = pos;
    b->vel = (Vector2){0, -(28.0f + (float)GetRandomValue(0, 22)) * speedScale};
    b->radius = radius;
    b->time = 0;
    b->driftAmplitude = 6.0f + (1.0f - normalized) * 8.0f;
    b->driftFrequency = 1.4f + (float)GetRandomValue(0, 80) / 100.0f;
    b->speedScale = speedScale;
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
    b->driftAmplitude = 8.0f;
    b->driftFrequency = 1.7f;
    b->speedScale = 1.0f;
    b->alpha = 180;
    b->active = true;
    b->state = BUBBLE_ACTIVE;
}

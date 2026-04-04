/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "math_utils.h"

#include <math.h>

/* ======================
Fungsi Math_RotatePoint
=======================
Fungsi ini digunakan untuk menjalankan proses Math_RotatePoint.
*/
Vector2 Math_RotatePoint(Vector2 point, Vector2 center, float angleDeg) {
    float rad = angleDeg * DEG2RAD;
    float sinAngle = sinf(rad);
    float cosAngle = cosf(rad);
    float x = point.x - center.x;
    float y = point.y - center.y;

    return (Vector2){
        center.x + x * cosAngle - y * sinAngle,
        center.y + x * sinAngle + y * cosAngle
    };
}

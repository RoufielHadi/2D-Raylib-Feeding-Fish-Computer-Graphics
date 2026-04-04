/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#include "raylib.h"

/* ======================
Fungsi Input_IsLeftClick
=======================
Fungsi ini digunakan untuk memeriksa left click.
*/
bool Input_IsLeftClick(void) {
	return IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

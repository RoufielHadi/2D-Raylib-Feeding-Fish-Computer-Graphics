/*
Author: Roufiel Hadi
NIM: 241524028
Kelas: 1A
Prodi: Sarjana Terapan Teknik Informatika
Jurusan: Teknik Komputer dan Informatika
Politeknik Negeri Bandung
*/

#ifndef APP_GAME_SESSION_H
#define APP_GAME_SESSION_H

#include "raylib.h"
#include <stdbool.h>

typedef enum {
    GAME_SESSION_RETURN_HOME = 0,
    GAME_SESSION_EXIT_APP
} GameSessionResult;

/* ======================
Fungsi RunGameSession
=======================
Fungsi ini digunakan untuk menjalankan proses RunGameSession.
*/
GameSessionResult RunGameSession(Font uiFont, bool hasCustomFont);

#endif

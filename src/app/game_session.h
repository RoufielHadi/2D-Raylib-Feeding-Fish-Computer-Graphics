#ifndef APP_GAME_SESSION_H
#define APP_GAME_SESSION_H

#include "raylib.h"
#include <stdbool.h>

typedef enum {
    GAME_SESSION_RETURN_HOME = 0,
    GAME_SESSION_EXIT_APP
} GameSessionResult;

GameSessionResult RunGameSession(Font uiFont, bool hasCustomFont);

#endif

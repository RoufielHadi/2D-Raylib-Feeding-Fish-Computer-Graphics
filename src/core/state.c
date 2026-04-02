#include "raylib.h"

int Game_GetSeed(void) {
	return (int) (GetTime() * 1000.0);
}

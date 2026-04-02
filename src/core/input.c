#include "raylib.h"

bool Input_IsLeftClick(void) {
	return IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

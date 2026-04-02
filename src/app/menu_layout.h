#ifndef APP_MENU_LAYOUT_H
#define APP_MENU_LAYOUT_H

#include "raylib.h"

typedef struct {
    Rectangle outer;
    Rectangle inner;
    Rectangle titleArea;
    Rectangle instructionBox;
    Rectangle contentBox;
    Rectangle viewport;
    Rectangle backButton;
    Rectangle scrollTrack;
} MenuPageLayout;

MenuPageLayout Menu_CreateScrollableLayout(void);
Rectangle Menu_CreateCenteredButton(float y, float width, float height);

#endif

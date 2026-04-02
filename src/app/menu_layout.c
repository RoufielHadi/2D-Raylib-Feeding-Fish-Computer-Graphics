#include "menu_layout.h"

#include "raylib.h"

MenuPageLayout Menu_CreateScrollableLayout(void) {
    float screenW = (float)GetScreenWidth();
    float screenH = (float)GetScreenHeight();
    float outerMarginX = screenW * 0.035f;
    float outerMarginY = screenH * 0.045f;
    float outerWidth = screenW - outerMarginX * 2.0f;
    float outerHeight = screenH - outerMarginY * 2.0f;
    float innerPadX = screenW * 0.022f;
    float innerPadY = screenH * 0.024f;

    MenuPageLayout layout;
    layout.outer = (Rectangle){outerMarginX, outerMarginY, outerWidth, outerHeight};
    layout.inner = (Rectangle){
        layout.outer.x + innerPadX,
        layout.outer.y + innerPadY,
        layout.outer.width - innerPadX * 2.0f,
        layout.outer.height - innerPadY * 2.0f
    };

    float titleTop = layout.inner.y + screenH * 0.018f;
    float sideInset = screenW * 0.022f;
    float bottomBandHeight = screenH * 0.108f;
    float backWidth = screenW * 0.086f;
    float backHeight = screenH * 0.054f;
    float contentTop = titleTop + backHeight + screenH * 0.032f;
    float contentHeight = layout.inner.y + layout.inner.height - contentTop - bottomBandHeight;

    layout.titleArea = (Rectangle){
        layout.inner.x + sideInset,
        titleTop,
        screenW * 0.34f,
        screenH * 0.07f
    };
    layout.backButton = (Rectangle){
        layout.inner.x + layout.inner.width - sideInset - backWidth,
        titleTop + 2.0f,
        backWidth,
        backHeight
    };
    layout.contentBox = (Rectangle){
        layout.inner.x + 12.0f,
        contentTop,
        layout.inner.width - 24.0f,
        contentHeight
    };
    layout.viewport = (Rectangle){
        layout.contentBox.x + 22.0f,
        layout.contentBox.y + 18.0f,
        layout.contentBox.width - 72.0f,
        layout.contentBox.height - 24.0f
    };
    layout.scrollTrack = (Rectangle){
        layout.contentBox.x + layout.contentBox.width - 18.0f,
        layout.viewport.y,
        12.0f,
        layout.viewport.height
    };

    float instructionWidth = screenW * 0.46f;
    if (instructionWidth < 520.0f) instructionWidth = 520.0f;
    if (instructionWidth > layout.inner.width - 120.0f) instructionWidth = layout.inner.width - 120.0f;
    layout.instructionBox = (Rectangle){
        layout.inner.x + (layout.inner.width - instructionWidth) * 0.5f,
        layout.inner.y + layout.inner.height - bottomBandHeight + 18.0f,
        instructionWidth,
        44.0f
    };
    return layout;
}

Rectangle Menu_CreateCenteredButton(float y, float width, float height) {
    return (Rectangle){
        (GetScreenWidth() - width) * 0.5f,
        y,
        width,
        height
    };
}

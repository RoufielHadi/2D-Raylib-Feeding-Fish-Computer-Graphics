#include "app.h"

#include "game_session.h"
#include "menu_assets.h"
#include "menu_content.h"
#include "menu_layout.h"
#include "menu_scene_art.h"
#include "menu_theme.h"

#include "raylib.h"

#include <math.h>

typedef enum {
    SCREEN_WELCOME = 0,
    SCREEN_HOME,
    SCREEN_HOW_TO_PLAY,
    SCREEN_ABOUT,
    SCREEN_EXIT
} AppScreen;

typedef struct {
    MenuAssets assets;
    AppScreen screen;
    int homeSelection;
    float howToScroll;
    float aboutScroll;
} AppState;

static const int kScreenWidth = 1536;
static const int kScreenHeight = 1024;

typedef struct {
    RenderTexture2D texture;
    bool ready;
    int width;
    int height;
} PageChromeCache;

static PageChromeCache s_howToChromeCache = {0};
static PageChromeCache s_aboutChromeCache = {0};

static void DrawSubtleHint(const AppState *app, Rectangle box, const char *text, Color fill, Color stroke, Color textColor) {
    DrawRectangleRounded(box, 0.35f, 10, fill);
    DrawRectangleRoundedLinesEx(box, 0.35f, 10, 1.0f, stroke);
    Vector2 textSize = Menu_MeasureText(app->assets.font, app->assets.useFont, text, 16.0f, 1.0f);
    Menu_DrawText(app->assets.font, app->assets.useFont, text,
        (Vector2){box.x + (box.width - textSize.x) * 0.5f, box.y + (box.height - textSize.y) * 0.5f - 1.0f},
        16.0f, 1.0f, textColor);
}

static void DrawImageMenuButton(const AppState *app, MenuButton button, bool selected) {
    bool hovered = CheckCollisionPointRec(GetMousePosition(), button.bounds);
    Rectangle outer = button.bounds;
    Rectangle shadow = {outer.x + 4.0f, outer.y + 6.0f, outer.width, outer.height};
    Rectangle inner = {outer.x + 8.0f, outer.y + 8.0f, outer.width - 16.0f, outer.height - 16.0f};
    Color fill = selected ? (Color){186, 122, 60, 245} : (hovered ? (Color){164, 105, 49, 236} : (Color){136, 85, 38, 228});
    Color fillSoft = selected ? (Color){225, 182, 116, 185} : (hovered ? (Color){212, 159, 94, 170} : (Color){186, 129, 73, 150});
    Color border = selected ? (Color){255, 234, 192, 255} : (hovered ? (Color){245, 218, 168, 245} : (Color){108, 62, 28, 235});
    Color textColor = selected ? (Color){255, 249, 234, 255} : (Color){250, 240, 217, 255};

    DrawRectangleRounded(shadow, 0.36f, 18, ColorAlpha(BLACK, hovered || selected ? 0.24f : 0.18f));
    DrawRectangleRounded(outer, 0.36f, 18, fill);
    DrawRectangleRounded(inner, 0.36f, 18, fillSoft);
    DrawRectangleRoundedLinesEx(outer, 0.36f, 18, 2.4f, border);

    Vector2 labelSize = Menu_MeasureText(app->assets.font, app->assets.useFont, button.label, 26.0f, 1.0f);
    Menu_DrawText(app->assets.font, app->assets.useFont, button.label,
        (Vector2){outer.x + (outer.width - labelSize.x) * 0.5f, outer.y + (outer.height - labelSize.y) * 0.5f - 1.0f},
        26.0f, 1.0f, textColor);
}

static void ClampScroll(float *value, float viewportHeight, float contentHeight) {
    float minScroll = (contentHeight > viewportHeight) ? (viewportHeight - contentHeight) : 0.0f;
    if (*value < minScroll) *value = minScroll;
    if (*value > 0.0f) *value = 0.0f;
}

static float UpdateScroll(float current, float viewportHeight, float contentHeight) {
    float wheel = GetMouseWheelMove();
    current += wheel * 48.0f;
    if (IsKeyDown(KEY_DOWN)) current -= 4.0f;
    if (IsKeyDown(KEY_UP)) current += 4.0f;
    if (IsKeyPressed(KEY_PAGE_DOWN)) current -= viewportHeight * 0.82f;
    if (IsKeyPressed(KEY_PAGE_UP)) current += viewportHeight * 0.82f;
    ClampScroll(&current, viewportHeight, contentHeight);
    return current;
}

static void DrawTitle(const AppState *app, const char *title, Vector2 pos, float size) {
    MenuPalette palette = Menu_GetPalette();
    Menu_DrawText(app->assets.font, app->assets.useFont, title, pos, size, 1.0f, palette.text);
}

static void DrawHeroTitle(const AppState *app, const char *title, float y, float size) {
    Vector2 textSize = Menu_MeasureText(app->assets.font, app->assets.useFont, title, size, 1.0f);
    Vector2 pos = {(GetScreenWidth() - textSize.x) * 0.5f, y};

    for (int sx = -3; sx <= 3; sx++) {
        for (int sy = -3; sy <= 3; sy++) {
            if (sx == 0 && sy == 0) continue;
            Menu_DrawText(app->assets.font, app->assets.useFont, title,
                (Vector2){pos.x + (float)sx, pos.y + (float)sy},
                size, 1.0f, ColorAlpha((Color){14, 42, 92, 255}, 0.62f));
        }
    }

    Menu_DrawText(app->assets.font, app->assets.useFont, title, pos, size, 1.0f, (Color){214, 246, 255, 255});
    Menu_DrawText(app->assets.font, app->assets.useFont, title,
        (Vector2){pos.x + 1.5f, pos.y + 3.5f},
        size, 1.0f, ColorAlpha((Color){24, 86, 198, 255}, 0.36f));
}

static Rectangle GetHomeButtonBounds(int index) {
    const float buttonWidth = 398.0f;
    const float buttonHeight = 86.0f;
    const float startY = 310.0f;
    const float gap = 64.0f;

    return (Rectangle){
        (GetScreenWidth() - buttonWidth) * 0.5f,
        startY + index * (buttonHeight + gap),
        buttonWidth,
        buttonHeight
    };
}

static void DrawWelcomeScreen(const AppState *app, float time) {
    MenuArt_DrawWelcomeBackdrop(time);

    Rectangle glassOuter = {72.0f, 40.0f, (float)GetScreenWidth() - 144.0f, (float)GetScreenHeight() - 132.0f};
    Rectangle glassInner = {glassOuter.x + 30.0f, glassOuter.y + 26.0f, glassOuter.width - 60.0f, glassOuter.height - 52.0f};
    Rectangle enterButton = {(GetScreenWidth() - 360.0f) * 0.5f, GetScreenHeight() - 174.0f, 360.0f, 70.0f};

    DrawRectangleRounded(glassOuter, 0.035f, 14, ColorAlpha((Color){4, 26, 48, 255}, 0.18f));
    DrawRectangleRounded(glassInner, 0.035f, 14, ColorAlpha((Color){255, 255, 255, 255}, 0.05f));

    DrawHeroTitle(app, "Feeding Fish", 246.0f, 102.0f);
    Menu_DrawWrappedText(app->assets.font, app->assets.useFont,
        "Kelola aquarium digital dan pelajari bagaimana object komputer grafis membentuk seluruh isi game.",
        (Rectangle){(GetScreenWidth() - 760.0f) * 0.5f, 364.0f, 760.0f, 74.0f},
        22.0f, 1.0f, ColorAlpha((Color){235, 247, 255, 255}, 0.92f));

    Menu_DrawButton((MenuButton){enterButton, "Press Enter"}, app->assets.font, app->assets.useFont, true);
    Menu_DrawInstructionBox((Rectangle){(GetScreenWidth() - 572.0f) * 0.5f, GetScreenHeight() - 86.0f, 572.0f, 40.0f},
        app->assets.font, app->assets.useFont, "Tekan Enter untuk masuk ke Home.");

    DrawSubtleHint(app, (Rectangle){32.0f, 30.0f, 190.0f, 48.0f}, "POLBAN",
        ColorAlpha((Color){4, 36, 58, 255}, 0.25f),
        ColorAlpha((Color){180, 232, 245, 255}, 0.18f),
        ColorAlpha((Color){38, 46, 138, 255}, 0.95f));

    Menu_DrawText(app->assets.font, app->assets.useFont, "Created By Roufiel Hadi",
        (Vector2){GetScreenWidth() - 284.0f, GetScreenHeight() - 44.0f}, 16.0f, 1.0f, ColorAlpha(RAYWHITE, 0.94f));
}

static void DrawHomeScreen(const AppState *app, float time) {
    MenuButton buttons[4] = {
        {GetHomeButtonBounds(0), "Play"},
        {GetHomeButtonBounds(1), "How To Play"},
        {GetHomeButtonBounds(2), "About"},
        {GetHomeButtonBounds(3), "Exit"}
    };

    MenuArt_DrawHomeBackdrop(time);
    DrawHeroTitle(app, "Feeding Fish", 84.0f, 74.0f);

    for (int i = 0; i < 4; i++) {
        DrawImageMenuButton(app, buttons[i], app->homeSelection == i);
    }

    DrawSubtleHint(app,
        (Rectangle){(GetScreenWidth() - 620.0f) * 0.5f, (float)GetScreenHeight() - 46.0f, 620.0f, 32.0f},
        "Panah atas/bawah atau mouse untuk memilih. Enter untuk masuk.",
        ColorAlpha((Color){5, 36, 52, 255}, 0.20f),
        ColorAlpha((Color){165, 225, 238, 255}, 0.08f),
        ColorAlpha((Color){240, 247, 255, 255}, 0.78f));
}

static void EnsureScrollableChromeCache(PageChromeCache *cache, const AppState *app, MenuPageLayout layout, const char *title, const char *instruction) {
    int width = GetScreenWidth();
    int height = GetScreenHeight();
    if (!cache->ready || cache->width != width || cache->height != height) {
        if (cache->ready) {
            UnloadRenderTexture(cache->texture);
        }
        cache->texture = LoadRenderTexture(width, height);
        cache->ready = true;
        cache->width = width;
        cache->height = height;

        BeginTextureMode(cache->texture);
        ClearBackground(BLANK);
        Menu_DrawBackgroundDirect(0.0f);
        Menu_DrawShell(layout.outer, layout.inner);
        DrawTitle(app, title, (Vector2){layout.titleArea.x, layout.titleArea.y + 8.0f}, 50.0f);
        Menu_DrawPanel(layout.contentBox, ColorAlpha(WHITE, 0.84f), ColorAlpha(Menu_GetPalette().panelStroke, 0.6f));
        Menu_DrawInstructionBox(layout.instructionBox, app->assets.font, app->assets.useFont, instruction);
        EndTextureMode();
    }
}

static void DrawCachedScrollablePage(const AppState *app, PageChromeCache *cache, MenuPageLayout layout, const char *title, const char *instruction) {
    EnsureScrollableChromeCache(cache, app, layout, title, instruction);
    DrawTextureRec(cache->texture.texture,
        (Rectangle){0.0f, 0.0f, (float)cache->texture.texture.width, (float)-cache->texture.texture.height},
        (Vector2){0.0f, 0.0f}, WHITE);
    Menu_DrawButton((MenuButton){layout.backButton, "Back"}, app->assets.font, app->assets.useFont, false);
}

static bool UpdateWelcomeScreen(void) {
    Rectangle hintBox = {
        (GetScreenWidth() - 312.0f) * 0.5f,
        (float)GetScreenHeight() - 72.0f,
        312.0f,
        42.0f
    };
    return IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || Menu_ButtonPressed((MenuButton){hintBox, ""});
}

static AppScreen UpdateHomeScreen(AppState *app) {
    MenuButton buttons[4] = {
        {GetHomeButtonBounds(0), "Play"},
        {GetHomeButtonBounds(1), "How To Play"},
        {GetHomeButtonBounds(2), "About"},
        {GetHomeButtonBounds(3), "Exit"}
    };

    if (IsKeyPressed(KEY_DOWN)) app->homeSelection = (app->homeSelection + 1) % 4;
    if (IsKeyPressed(KEY_UP)) app->homeSelection = (app->homeSelection + 3) % 4;
    if (IsKeyPressed(KEY_BACKSPACE)) return SCREEN_WELCOME;

    for (int i = 0; i < 4; i++) {
        if (Menu_ButtonPressed(buttons[i])) app->homeSelection = i;
    }

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER) || Menu_ButtonPressed(buttons[app->homeSelection])) {
        if (app->homeSelection == 0) {
            GameSessionResult result = RunGameSession(app->assets.font, app->assets.useFont);
            return (result == GAME_SESSION_EXIT_APP) ? SCREEN_EXIT : SCREEN_HOME;
        }
        if (app->homeSelection == 1) return SCREEN_HOW_TO_PLAY;
        if (app->homeSelection == 2) return SCREEN_ABOUT;
        return SCREEN_EXIT;
    }

    return SCREEN_HOME;
}

static AppScreen UpdateScrollScreen(float *scroll, Rectangle viewport, float contentHeight, Rectangle backButton, AppScreen current) {
    *scroll = UpdateScroll(*scroll, viewport.height, contentHeight);
    if (IsKeyPressed(KEY_BACKSPACE)) return SCREEN_HOME;
    if (Menu_ButtonPressed((MenuButton){backButton, "Back"})) return SCREEN_HOME;
    return current;
}

int App_Run(void) {
    AppState app = {0};
    MenuPageLayout page = {0};

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(kScreenWidth, kScreenHeight, "Feeding Fish");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    page = Menu_CreateScrollableLayout();

    MenuAssets_Load(&app.assets);
    if (app.assets.hasIconImage) {
        SetWindowIcon(app.assets.iconImage);
    }
    app.screen = SCREEN_WELCOME;

    while (app.screen != SCREEN_EXIT && !WindowShouldClose()) {
        float time = (float)GetTime();
        page = Menu_CreateScrollableLayout();

        if (app.screen == SCREEN_WELCOME && UpdateWelcomeScreen()) {
            app.screen = SCREEN_HOME;
        } else if (app.screen == SCREEN_HOME) {
            app.screen = UpdateHomeScreen(&app);
        } else if (app.screen == SCREEN_HOW_TO_PLAY) {
            app.screen = UpdateScrollScreen(&app.howToScroll, page.viewport, Menu_GetHowToPlayContentHeight(), page.backButton, SCREEN_HOW_TO_PLAY);
        } else if (app.screen == SCREEN_ABOUT) {
            app.screen = UpdateScrollScreen(&app.aboutScroll, page.viewport,
                Menu_GetAboutContentHeight(app.assets.font, app.assets.useFont, page.viewport.width - 44.0f),
                page.backButton, SCREEN_ABOUT);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (app.screen == SCREEN_WELCOME) {
            DrawWelcomeScreen(&app, time);
        } else if (app.screen == SCREEN_HOME) {
            DrawHomeScreen(&app, time);
        } else if (app.screen == SCREEN_HOW_TO_PLAY) {
            DrawCachedScrollablePage(&app, &s_howToChromeCache, page, "HOW TO PLAY", "Backspace atau tombol Back untuk kembali. Scroll dengan mouse atau tombol panah.");
            BeginScissorMode((int)page.viewport.x, (int)page.viewport.y, (int)page.viewport.width, (int)page.viewport.height);
            Menu_DrawHowToPlayContent(page.viewport, app.howToScroll, app.assets.font, app.assets.useFont, time);
            EndScissorMode();
            Menu_DrawScrollBar(page.scrollTrack, page.viewport.height, Menu_GetHowToPlayContentHeight(), -app.howToScroll);
        } else if (app.screen == SCREEN_ABOUT) {
            DrawCachedScrollablePage(&app, &s_aboutChromeCache, page, "ABOUT", "Backspace atau tombol Back untuk kembali ke Home.");
            BeginScissorMode((int)page.viewport.x, (int)page.viewport.y, (int)page.viewport.width, (int)page.viewport.height);
            Menu_DrawAboutContent(page.viewport, app.aboutScroll, app.assets.font, app.assets.useFont,
                app.assets.authorPhoto, app.assets.hasAuthorPhoto, time);
            EndScissorMode();
            Menu_DrawScrollBar(page.scrollTrack, page.viewport.height,
                Menu_GetAboutContentHeight(app.assets.font, app.assets.useFont, page.viewport.width - 44.0f), -app.aboutScroll);
        }

        EndDrawing();
    }

    MenuAssets_Unload(&app.assets);
    CloseWindow();
    return 0;
}

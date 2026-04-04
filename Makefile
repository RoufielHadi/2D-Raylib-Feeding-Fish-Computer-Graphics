
CC := gcc
WINDRES := windres
CFLAGS := -O2 -Wall -std=c11 -I./src -IC:/msys64/mingw64/include
LDFLAGS := -LC:/msys64/mingw64/lib -lraylib -lopengl32 -lgdi32 -lwinmm

SRCS := \
	src/main.c \
	src/app/app.c src/app/game_session.c src/app/menu_theme.c src/app/menu_content.c src/app/menu_assets.c src/app/menu_layout.c src/app/menu_scene_art.c \
	src/core/game.c src/core/input.c src/core/state.c \
	src/utils/math_utils.c src/utils/primitives.c src/utils/bresenham.c src/utils/dda.c src/utils/midcircle.c src/utils/bresellipse.c src/utils/midellips.c \
	src/entities/cere.c src/entities/lele.c src/entities/toman.c src/entities/food.c src/entities/bubble.c \
	src/ui/ui.c src/ui/button.c \
	src/systems/collision.c src/systems/food_system.c src/systems/fish_system.c \
	src/render/draw_env.c src/render/draw_bubble.c src/render/draw_food.c src/render/draw_fish_local.c src/render/draw_ui.c src/render/render.c

OBJS := $(SRCS:.c=.o)
RESOBJ := app.res.o

TARGET := aquarium.exe

default: all

all: $(TARGET)

$(TARGET): $(OBJS) $(RESOBJ)
	$(CC) $(CFLAGS) $(OBJS) $(RESOBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(RESOBJ): app.rc assets/Picture/Icon.ico
	$(WINDRES) app.rc -O coff -o $(RESOBJ)

clean:
	-del /Q $(OBJS) $(RESOBJ) $(TARGET) 2>nul

.PHONY: default all clean


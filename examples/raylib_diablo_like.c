// @BAKE gcc $@ -o $*.out -I../ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
#include <raylib.h>
#include "dyrect.h"

rect_t hotbar;
rect_t healt;
rect_t manna;
rect_t inventory;
rect_t inventory_inner;
rect_t inventory_slot;

#define LOG_RECT(rect) \
    TraceLog(LOG_INFO, "%f %f %f %f", rect.x, rect.y, rect.width, rect.height)

void do_resize(void) {
    hotbar = inner_down(get_screen_rect(), scaley(get_screen_rect(), 0.17f));
    healt = inner_down(get_screen_rect(), scale(get_unit_rect(), hotbar.height * 1.2f));
    manna = inner_right(hotbar, healt);
    inventory = reachy(hotbar, inner_right(get_screen_rect(), scalex(get_screen_rect(), 0.5f)));
    inventory_inner = buoyance(inventory, scale(inventory, 0.8f));
    inventory_slot = inner_left(inventory_inner, inner_up(inventory_inner, scale(get_unit_rect(), inventory_inner.width / 9)));
}

signed main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(600, 600, "");

    do_resize();

    while (!WindowShouldClose()) {
        if (IsWindowResized()) {
            do_resize();
        }

      BeginDrawing();
        DrawRectangleRec(get_screen_rect(), RAYWHITE);
        DrawRectangleRec(inventory, BLACK);
        DrawRectangleRec(inventory_inner, GRAY);

        for (int i = 0; i < 6; i++) {
            for (int h = 0; h < 9; h++) {
                DrawRectangleRec(
                    rope_step(rail_step(inventory_slot, h), i),
                    (Color) {
                        (unsigned char)(0 + ((i + h) * 10)),
                        (unsigned char)(0 + ((i + h) * 10)),
                        (unsigned char)(0 + ((i + h) * 10)),
                        255,
                    }
                );
            }
        }

        DrawRectangleRec(hotbar, GRAY);
        DrawRectangleRec(healt, RED);
        DrawRectangleRec(manna, BLUE);
      EndDrawing();
    }

    CloseWindow();

    return 0;
}

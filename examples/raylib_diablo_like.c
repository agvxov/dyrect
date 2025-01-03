// @BAKE gcc $@ -o $*.out -I../ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
#include <raylib.h>
#include "dyrect.h"

rect_t hotbar;
rect_t healt;
rect_t manna;
rect_t inventory;
rect_t inventory_inner;
rect_t inventory_slot;

void do_resize(void) {
    hotbar = ride(after(get_screen_rect(), 1), scaley(get_screen_rect(), 0.17f));
    healt = ride(after(get_screen_rect(), 1), scale(get_unit_rect(), hotbar.height * 1.2f));
    manna = paper(hotbar, healt);
    inventory = stretchy(hotbar, paper(get_screen_rect(), scalex(get_screen_rect(), 0.5f)));
    inventory_inner = buoyance(inventory, scale(inventory, 0.8f));
    inventory_slot = rock(inventory_inner, hang(inventory_inner, scale(get_unit_rect(), inventory_inner.width / 9)));
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
                    after(next(inventory_slot, h), i),
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

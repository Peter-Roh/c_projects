#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

// --------------------------------------------------
// defines
// --------------------------------------------------
#define SQUARE_SIZE 20

// --------------------------------------------------
// types and structures
// --------------------------------------------------



// --------------------------------------------------
// global variables
// --------------------------------------------------
static const int SCREEN_WIDTH = 442;
static const int SCREEN_HEIGHT = 450;

static bool b_game_over = false;
static bool b_begin_game = false;
static int g_level = 1;

// --------------------------------------------------
// counters
// --------------------------------------------------


void draw_init_page(void) {
    BeginDrawing();
    ClearBackground(WHITE);

    Vector2 offset;
    Vector2 triangle1;
    Vector2 triangle2;
    Vector2 triangle3;

    offset.x = 2;
    offset.y = 2;

    for(int i = 0; i < 5; ++i) {
        for(int j = 0; j < 20; ++j) {
            if((i == 0 && j == 0) || (i == 1 && j >= 0 && j <= 2)) {
                DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, ORANGE);
            } else if((i == 3 || i == 4) && (j >=0 && j <=1)) {
                DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, GOLD);
            } else if((i == 0 && j >= 18) || (j == 19 && i >= 0 && i <= 2)) {
                DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, DARKGREEN);
            } else if(i == 4 && j >= 15 && j <= 18) {
                DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, PINK);
            } else if((i == 0 && j >=15 && j <= 16) || (i == 1 && j >=16 && j <= 17)) {
                DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, MAROON);
            } else if((i >= 1 && i <= 3 && j == 5) || (i == 2 && j == 6)) {
                DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, SKYBLUE);
            } else if (i == 2 && j == 8) {
                DrawText("TETRIS", offset.x, offset.y, 20, DARKBLUE);
            } else if(!(i == 2 && ((j >= 7 && j <= 12) || j == 15)) && !(i == 4 && j == 2) && !((i == 1 && (j == 7 || j == 10 || j == 11)) ||(i == 3 && (j == 8 || j == 11)))) {
                DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, GRAY);
            }
            offset.x += (SQUARE_SIZE + 2);
        }
        offset.x = 2;
        offset.y += (SQUARE_SIZE + 2);
    }

    offset.x = 60;
    offset.y += 50;

    struct timespec specific_time;
    int time_seed;
    clock_gettime(CLOCK_REALTIME, &specific_time);
    time_seed = floor(specific_time.tv_nsec / 1.0e8);

    if(time_seed % 2 == 0) {
        DrawText("Please press Enter to start...", offset.x, offset.y, 20, BLACK);
    } else {
        DrawText("Please press Enter to start...", offset.x, offset.y, 20, GRAY);
    }

    triangle1.x = offset.x + 30;
    triangle1.y = offset.y + 50;
    triangle2.x = triangle1.x - 10;
    triangle2.y = triangle1.y + 20;
    triangle3.x = triangle1.x + 10;
    triangle3.y = triangle1.y + 20;
    offset.x += 130;
    offset.y += 45;

    DrawTriangle(triangle1, triangle2, triangle3, BLACK);
    DrawText(": Spin", offset.x, offset.y, 20, BLACK);

    triangle1.x -= 40;
    triangle1.y += 40;
    triangle2.x -= 10;
    triangle2.y += 30;
    triangle3.x = triangle2.x;
    triangle3.y += 10;
    offset.y += 30;

    DrawTriangle(triangle1, triangle2, triangle3, BLACK);

    triangle1.x += 60;
    triangle1.y -= 10;
    triangle2.x += 40;
    triangle3.x += 60;
    triangle3.y += 10;

    DrawTriangle(triangle1, triangle2, triangle3, BLACK);
    DrawText(": Left / Right", offset.x, offset.y, 20, BLACK);

    triangle1.x -= 30;
    triangle1.y += 30;
    triangle2.x -= 20;
    triangle2.y += 30;
    triangle3.x -= 30;
    triangle3.y += 20;
    offset.y += 30;

    DrawTriangle(triangle1, triangle2, triangle3, BLACK);
    DrawText(": Soft Drop", offset.x, offset.y, 20, BLACK);

    offset.y += 40;
    DrawText(": Hard Drop", offset.x, offset.y, 20, BLACK);

    offset.y += 30;
    DrawText(": Pause", offset.x, offset.y, 20, BLACK);

    offset.y += 30;
    DrawText(": Quit", offset.x, offset.y, 20, BLACK);

    offset.x -= 130;
    offset.y -= 60;
    DrawText("SPACE", offset.x, offset.y, 20, BLACK);

    offset.y += 30;
    DrawText("P", offset.x, offset.y, 20, BLACK);

    offset.y += 30;
    DrawText("ESC", offset.x, offset.y, 20, BLACK);

    EndDrawing();
}

// when pressed ENTER key, game begins
void check_game_start(void) {
    if(IsKeyPressed(KEY_ENTER)) {
        b_begin_game = true;
    }
}

// initialize game variables
void init_game(void) {
}

void update_draw_frame(void) {
    BeginDrawing();
    ClearBackground(WHITE);
    EndDrawing();
}

// unload game variables
void unload_game(void) {
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "tetris");
    init_game();

    // main game loop
    while (!WindowShouldClose()) {
        if(!b_begin_game) {
            draw_init_page();
            check_game_start();
        } else {
            update_draw_frame();
        }
    }

    unload_game();
    CloseWindow();

    return 0;
}

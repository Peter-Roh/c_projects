#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

// --------------------------------------------------
// defines
// --------------------------------------------------
#define SQUARE_SIZE 20
#define GRID_X_SIZE 12
#define GRID_Y_SIZE 21

// --------------------------------------------------
// types and structures
// --------------------------------------------------
typedef enum grid_square {
    EMPTY,
    MOVING,
    FULL,
    BLOCK,
    FADING
} grid_square_t;

// --------------------------------------------------
// global variables
// --------------------------------------------------
static const int SCREEN_WIDTH = 442;
static const int SCREEN_HEIGHT = 450;

static bool b_game_over = false;
static bool b_begin_game = false;
static bool b_pause = false;

static int g_level = 1;
static int g_speed = 30;

static grid_square_t grid[GRID_X_SIZE][GRID_Y_SIZE];
static grid_square_t piece[4][4];
static grid_square_t incomingPiece[4][4];

static int piece_position_x = 0;
static int piece_position_y = 0;

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
    time_seed = floor(specific_time.tv_nsec / 1.0e6);

    if(time_seed < 500) {
        DrawText("Please press Enter to start...", offset.x, offset.y, 20, BLACK);
    } else {
        DrawText("Please press Enter to start...", offset.x, offset.y, 20, WHITE);
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
    int i;
    int j;

    b_pause = false;
    g_level = 1;
    g_speed = 30;

    for(i = 0; i < GRID_X_SIZE; ++i) {
        for(int j = 0; j < GRID_Y_SIZE; ++j) {
            if((j == GRID_Y_SIZE - 1) || (i == 0) || (i == GRID_X_SIZE - 1)) {
                grid[i][j] = BLOCK;
            } else {
                grid[i][j] = EMPTY;
            }
        }
    }

    for(i = 0; i < 4; ++i) {
        for(j = 0; j < 4; ++j) {
            incomingPiece[i][j] = EMPTY;
        }
    }
}

void draw_map(void) {
    BeginDrawing();
    ClearBackground(WHITE);

    int i;
    int j;
    Vector2 offset;
    offset.x = 22;
    offset.y = 12;

    int controller_x = offset.x;
    int controller_y = offset.y;

    for(j = 0; j < GRID_Y_SIZE; ++j) {
        for(i = 0; i < GRID_X_SIZE; ++i) {
            if(grid[i][j] == EMPTY) {
                DrawLine(offset.x, offset.y, offset.x + SQUARE_SIZE, offset.y, LIGHTGRAY);
                DrawLine(offset.x, offset.y, offset.x, offset.y + SQUARE_SIZE, LIGHTGRAY);
                DrawLine(offset.x + SQUARE_SIZE, offset.y, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, LIGHTGRAY);
                DrawLine(offset.x, offset.y + SQUARE_SIZE, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, LIGHTGRAY);
            } else if(grid[i][j] == BLOCK) {
                DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, LIGHTGRAY);
            }

            offset.x += SQUARE_SIZE;
        }
        offset.x = controller_x;
        offset.y += SQUARE_SIZE;
    }

    offset.x += (SQUARE_SIZE * (GRID_X_SIZE + 3));
    offset.y = controller_y + (SQUARE_SIZE);
    controller_x = offset.x;
    controller_y = offset.y;

    DrawText("INCOMING:", offset.x, offset.y - 20, 10, GRAY);
    for(j = 0; j < 4; ++j) {
        for(i = 0; i < 4; ++i) {
            if(incomingPiece[i][j] == EMPTY) {
                DrawLine(offset.x, offset.y, offset.x + SQUARE_SIZE, offset.y, LIGHTGRAY);
                DrawLine(offset.x, offset.y, offset.x, offset.y + SQUARE_SIZE, LIGHTGRAY);
                DrawLine(offset.x + SQUARE_SIZE, offset.y, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, LIGHTGRAY);
                DrawLine(offset.x, offset.y + SQUARE_SIZE, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, LIGHTGRAY);
            } else if(incomingPiece[i][j] == MOVING) {
                DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, GRAY);
            }

            offset.x += SQUARE_SIZE;
        }
        offset.x = controller_x;
        offset.y += SQUARE_SIZE;
    }

    offset.y += SQUARE_SIZE;
    DrawText(TextFormat("Level: %02d", g_level), offset.x, offset.y, 12, GRAY);

    if(b_pause) {
        DrawText("GAME PAUSED", (SCREEN_WIDTH - MeasureText("GAME_PAUSED", 40)) / 2, SCREEN_HEIGHT / 2 - 40 , 40, GRAY);
    }

    EndDrawing();
}

void update_draw_frame(void) {
    if(!b_game_over) {
        if(IsKeyPressed(KEY_P)) {
            b_pause = !b_pause;
        }
    }
}

void create_piece() {
}

void get_random_piece() {
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
            draw_map();
        }
    }

    unload_game();
    CloseWindow();

    return 0;
}

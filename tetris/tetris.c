#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "gamedata.h"
#include "tetris.h"

static void draw_init_page(void) {
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

    offset.x = 70;
    offset.y += 30;

    struct timespec specific_time;
    clock_gettime(CLOCK_REALTIME, &specific_time);
    const int time_seed = floor(specific_time.tv_nsec / 1.0e6);

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
    offset.y += 35;

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
    offset.y += 35;

    DrawTriangle(triangle1, triangle2, triangle3, BLACK);
    DrawText(": Soft Drop", offset.x, offset.y, 20, BLACK);

    offset.y += 40;
    DrawText(": Hard Drop", offset.x, offset.y, 20, BLACK);

    offset.y += 30;
    DrawText(": Hold", offset.x, offset.y, 20, BLACK);

    offset.y += 30;
    DrawText(": Pause", offset.x, offset.y, 20, BLACK);

    offset.y += 30;
    DrawText(": Quit", offset.x, offset.y, 20, BLACK);

    offset.x -= 130;
    offset.y -= 90;
    DrawText("SPACE", offset.x, offset.y, 20, BLACK);

    offset.y += 30;
    DrawText("Shift", offset.x, offset.y, 20, BLACK);

    offset.y += 30;
    DrawText("P", offset.x, offset.y, 20, BLACK);

    offset.y += 30;
    DrawText("ESC", offset.x, offset.y, 20, BLACK);

    EndDrawing();
}

// when pressed ENTER key, game begins
static void check_game_start(game_state_t* game_state) {
    if(IsKeyPressed(KEY_ENTER)) {
        set_begin_game(game_state , true);
    }
}

// initialize game variables
static void init_game(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE], grid_square_t incoming_piece[4][4], grid_square_t hold_piece[4][4], grid_square_t piece[4][4], game_state_t* game_state, counter_t* counter) {
    reset_game_state(game_state);
    reset_counter(counter);

    for(int i = 0; i < GRID_Y_SIZE; ++i) {
        for(int j = 0; j < GRID_X_SIZE; ++j) {
            if((i == GRID_Y_SIZE - 1) || (j == 0) || (j == GRID_X_SIZE - 1)) {
                grid[i][j] = BLOCK;
            } else {
                grid[i][j] = EMPTY;
            }
        }
    }

    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            incoming_piece[i][j] = EMPTY;
            hold_piece[i][j] = EMPTY;
            piece[i][j] = EMPTY;
        }
    }
}

static void draw_map(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE], grid_square_t incoming_piece[4][4], grid_square_t hold_piece[4][4], game_state_t* game_state, Color* current_piece_color, Color* incoming_piece_color, Color* hold_piece_color) {
    BeginDrawing();
    ClearBackground(WHITE);

    if(!game_state->b_game_over) {
        Color square_color;
        Vector2 offset;
        offset.x = 22;
        offset.y = 12;
        int controller_x = offset.x;
        int controller_y = offset.y;

        for(int i = 0; i < GRID_Y_SIZE; ++i) {
            for(int j = 0; j < GRID_X_SIZE; ++j) {
                if(grid[i][j] == EMPTY) {
                    DrawLine(offset.x, offset.y, offset.x + SQUARE_SIZE, offset.y, LIGHTGRAY);
                    DrawLine(offset.x, offset.y, offset.x, offset.y + SQUARE_SIZE, LIGHTGRAY);
                    DrawLine(offset.x + SQUARE_SIZE, offset.y, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, LIGHTGRAY);
                    DrawLine(offset.x, offset.y + SQUARE_SIZE, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, LIGHTGRAY);
                } else if(grid[i][j] == BLOCK) {
                    DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, GRAY);
                } else if(grid[i][j] == MOVING) {
                    DrawLine(offset.x, offset.y, offset.x + SQUARE_SIZE, offset.y, LIGHTGRAY);
                    DrawLine(offset.x, offset.y, offset.x, offset.y + SQUARE_SIZE, LIGHTGRAY);
                    DrawLine(offset.x + SQUARE_SIZE, offset.y, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, LIGHTGRAY);
                    DrawLine(offset.x, offset.y + SQUARE_SIZE, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, LIGHTGRAY);
                    DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, *current_piece_color);
                } else if(grid[i][j] >= FULL) {
                    square_color = get_piece_color(grid[i][j] - 5);
                    DrawLine(offset.x, offset.y, offset.x + SQUARE_SIZE, offset.y, LIGHTGRAY);
                    DrawLine(offset.x, offset.y, offset.x, offset.y + SQUARE_SIZE, LIGHTGRAY);
                    DrawLine(offset.x + SQUARE_SIZE, offset.y, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, LIGHTGRAY);
                    DrawLine(offset.x, offset.y + SQUARE_SIZE, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, LIGHTGRAY);
                    DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, square_color);
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
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                if(incoming_piece[i][j] == EMPTY) {
                    DrawLine(offset.x, offset.y, offset.x + SQUARE_SIZE, offset.y, LIGHTGRAY);
                    DrawLine(offset.x, offset.y, offset.x, offset.y + SQUARE_SIZE, LIGHTGRAY);
                    DrawLine(offset.x + SQUARE_SIZE, offset.y, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, LIGHTGRAY);
                    DrawLine(offset.x, offset.y + SQUARE_SIZE, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, LIGHTGRAY);
                } else if(incoming_piece[i][j] == MOVING) {
                    DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, *incoming_piece_color);
                }
                offset.x += SQUARE_SIZE;
            }
            offset.x = controller_x;
            offset.y += SQUARE_SIZE;
        }

        offset.y += (SQUARE_SIZE * 2);
        controller_y = offset.y;

        DrawText("HOLD:", offset.x, offset.y - 20, 10, GRAY);
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                if(hold_piece[i][j] == EMPTY) {
                    DrawLine(offset.x, offset.y, offset.x + SQUARE_SIZE, offset.y, LIGHTGRAY);
                    DrawLine(offset.x, offset.y, offset.x, offset.y + SQUARE_SIZE, LIGHTGRAY);
                    DrawLine(offset.x + SQUARE_SIZE, offset.y, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, LIGHTGRAY);
                    DrawLine(offset.x, offset.y + SQUARE_SIZE, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, LIGHTGRAY);
                } else if(hold_piece[i][j] == MOVING) {
                    DrawRectangle(offset.x, offset.y, SQUARE_SIZE, SQUARE_SIZE, *hold_piece_color);
                }
                offset.x += SQUARE_SIZE;
            }
            offset.x = controller_x;
            offset.y += SQUARE_SIZE;
        }

        offset.y += SQUARE_SIZE;
        DrawText(TextFormat("Level: %02d", game_state->g_level), offset.x, offset.y, 12, GRAY);

        if(game_state->b_pause) {
            DrawText("GAME PAUSED", (SCREEN_WIDTH - MeasureText("GAME_PAUSED", 40)) / 2, SCREEN_HEIGHT / 2 - 40 , 40, GRAY);
        }
    } else {
        // game over 문구
    }

    EndDrawing();
}

static void update_draw_frame(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE], grid_square_t incoming_piece[4][4], grid_square_t piece[4][4], game_state_t* game_state, counter_t* counter, Color* current_piece_color, Color* incoming_piece_color) {
    if(IsKeyPressed(KEY_P)) {
        set_pause(game_state, !game_state->b_pause);
    }

    if(!game_state->b_pause) {
        if(!game_state->b_line_to_delete) {
            if(!game_state->b_piece_active) {
                set_piece_active(game_state, create_piece(grid, incoming_piece, piece, game_state, current_piece_color, incoming_piece_color));
            } else {
                increment_gravity_movement_counter(counter);
                increment_lateral_movement_counter(counter);
                increment_turn_movement_counter(counter);

                if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) {
                    set_lateral_movement_counter(counter, LATERAL_SPEED);
                }

                if(IsKeyPressed(KEY_UP)) {
                    set_turn_movement_counter(counter, TURNING_SPEED);
                }

                if(counter->gravity_movement_counter >= game_state->g_speed) {
                    check_detection(grid, game_state);
                    resolve_falling_movement(grid, game_state);
                    check_completion(grid, game_state);
                    set_gravity_movement_counter(counter, 0);
                }

                if(counter->lateral_movement_counter >= LATERAL_SPEED) {
                    if(!resolve_lateral_movement(grid, game_state)) {
                        set_lateral_movement_counter(counter, 0);
                    }
                }

                if(counter->turn_movement_counter >= TURNING_SPEED) {
                    if(resolve_turn_movement(grid, piece, game_state)) {
                        set_turn_movement_counter(counter, 0);
                    }
                }
            }
        } else {
            // delete line
        }
    }
}

static void save_color(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE], const int piece_num) {
    for(int i = GRID_Y_SIZE - 2; i >= 0 ; --i) {
        for(int j = 1; j < GRID_X_SIZE - 1; ++j) {
            if(grid[i][j] == FULL) {
                grid[i][j] = piece_num + 5;
            }
        }
    }
}

static void check_detection(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE], game_state_t* game_state) {
    for(int i = GRID_Y_SIZE - 2; i >= 0 ; --i) {
        for(int j = 1; j < GRID_X_SIZE - 1; ++j) {
            if((grid[i][j] == MOVING) && ((grid[i + 1][j] >= FULL) || (grid[i + 1][j] == BLOCK))) {
                game_state->b_detection = true;
            }
        }
    }
}

static void resolve_falling_movement(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE], game_state_t* game_state) {
    if(game_state->b_detection) { // finish moving piece
        for(int i = GRID_Y_SIZE - 2; i >= 0 ; --i) {
            for(int j = 1; j < GRID_X_SIZE - 1; ++j) {
                if(grid[i][j] == MOVING) {
                    grid[i][j] = FULL;
                    set_detection(game_state, false);
                    set_piece_active(game_state, false);
                }
            }
        }
        save_color(grid, game_state->finished_piece_num);
    } else { // move piece down
        for(int i = GRID_Y_SIZE - 2; i >= 0 ; --i) {
            for(int j = 1; j < GRID_X_SIZE - 1; ++j) {
                if(grid[i][j] == MOVING) {
                    grid[i + 1][j] = MOVING;
                    grid[i][j] = EMPTY;
                }
            }
        }
        increment_piece_position_y(game_state);
    }
}

static bool resolve_lateral_movement(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE], game_state_t* game_state) {
    bool collision = false;

    if(IsKeyDown(KEY_LEFT)) {
        for(int i = GRID_Y_SIZE - 2; i >= 0 ; --i) {
            for(int j = 1; j < GRID_X_SIZE - 1; ++j) {
                if(grid[i][j] == MOVING) {
                    if(i == 1 || grid[i][j - 1] >= FULL || grid[i][j - 1] == BLOCK) {
                        collision = true;
                    }
                }
            }
        }

        if(!collision) {
            for(int i = GRID_Y_SIZE - 2; i >= 0 ; --i) {
                for(int j = 1; j < GRID_X_SIZE - 1; ++j) {
                    if(grid[i][j] == MOVING) {
                        grid[i][j - 1] = MOVING;
                        grid[i][j] = EMPTY;
                    }
                }
            }

            decrement_piece_position_x(game_state);
        }
    } else if(IsKeyDown(KEY_RIGHT)) {
        for(int i = GRID_Y_SIZE - 2; i >= 0 ; --i) {
            for(int j = 1; j < GRID_X_SIZE - 1; ++j) {
                if(grid[i][j] == MOVING) {
                    if(i == GRID_X_SIZE - 1 || grid[i][j + 1] >= FULL || grid[i][j + 1] == BLOCK) {
                        collision = true;
                    }
                }
            }
        }

        if(!collision) {
            for(int i = GRID_Y_SIZE - 2; i >= 0 ; --i) {
                for(int j = GRID_X_SIZE - 2; j >= 1; --j) {
                    if(grid[i][j] == MOVING) {
                        grid[i][j + 1] = MOVING;
                        grid[i][j] = EMPTY;
                    }
                }
            }

            increment_piece_position_x(game_state);
        }
    }

    return collision;
}

static bool resolve_turn_movement(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE], grid_square_t piece[4][4], game_state_t* game_state) {
    if(IsKeyDown(KEY_UP)) {
        grid_square_t temp;
        bool checker = false;

        if(grid[game_state->piece_position_y][game_state->piece_position_x + 3] == MOVING &&
            (grid[game_state->piece_position_y][game_state->piece_position_x] != EMPTY &&
            grid[game_state->piece_position_y][game_state->piece_position_x] != MOVING)) {
            checker = true;
        }

        if(grid[game_state->piece_position_y + 3][game_state->piece_position_x + 3] == MOVING &&
            grid[game_state->piece_position_y][game_state->piece_position_x + 3] != EMPTY &&
            grid[game_state->piece_position_y][game_state->piece_position_x + 3] != MOVING) {
            checker = true;
        }

        if(grid[game_state->piece_position_y + 3][game_state->piece_position_x] == MOVING &&
            grid[game_state->piece_position_y + 3][game_state->piece_position_x + 3] != EMPTY &&
            grid[game_state->piece_position_y + 3][game_state->piece_position_x + 3] != MOVING) {
            checker = true;
        }

        if(grid[game_state->piece_position_y][game_state->piece_position_x] == MOVING &&
            grid[game_state->piece_position_y + 3][game_state->piece_position_x] != EMPTY &&
            grid[game_state->piece_position_y + 3][game_state->piece_position_x] != MOVING) {
            checker = true;
        }

        if(grid[game_state->piece_position_y][game_state->piece_position_x + 1] == MOVING &&
            grid[game_state->piece_position_y + 2][game_state->piece_position_x] != EMPTY &&
            grid[game_state->piece_position_y + 2][game_state->piece_position_x] != MOVING) {
            checker = true;
        }

        if(grid[game_state->piece_position_y + 1][game_state->piece_position_x + 3] == MOVING &&
            grid[game_state->piece_position_y][game_state->piece_position_x + 1] != EMPTY &&
            grid[game_state->piece_position_y][game_state->piece_position_x + 1] != MOVING) {
            checker = true;
        }

        if(grid[game_state->piece_position_y + 3][game_state->piece_position_x + 2] == MOVING &&
            grid[game_state->piece_position_y + 1][game_state->piece_position_x + 3] != EMPTY &&
            grid[game_state->piece_position_y + 1][game_state->piece_position_x + 3] != MOVING) {
            checker = true;
        }

        if(grid[game_state->piece_position_y + 2][game_state->piece_position_x] == MOVING &&
            grid[game_state->piece_position_y + 3][game_state->piece_position_x + 2] != EMPTY &&
            grid[game_state->piece_position_y + 3][game_state->piece_position_x + 2] != MOVING) {
            checker = true;
        }

        if(grid[game_state->piece_position_y][game_state->piece_position_x + 2] == MOVING &&
            grid[game_state->piece_position_y + 1][game_state->piece_position_x] != EMPTY &&
            grid[game_state->piece_position_y + 1][game_state->piece_position_x] != MOVING) {
            checker = true;
        }

        if(grid[game_state->piece_position_y + 2][game_state->piece_position_x + 3] == MOVING &&
            grid[game_state->piece_position_y][game_state->piece_position_x + 2] != EMPTY &&
            grid[game_state->piece_position_y][game_state->piece_position_x + 2] != MOVING) {
            checker = true;
        }

        if(grid[game_state->piece_position_y + 3][game_state->piece_position_x + 1] == MOVING &&
            grid[game_state->piece_position_y + 2][game_state->piece_position_x + 3] != EMPTY &&
            grid[game_state->piece_position_y + 2][game_state->piece_position_x + 3] != MOVING) {
            checker = true;
        }

        if(grid[game_state->piece_position_y + 1][game_state->piece_position_x] == MOVING &&
            grid[game_state->piece_position_y + 3][game_state->piece_position_x + 1] != EMPTY &&
            grid[game_state->piece_position_y + 3][game_state->piece_position_x + 1] != MOVING) {
            checker = true;
        }

        if(grid[game_state->piece_position_y + 1][game_state->piece_position_x + 1] == MOVING &&
            grid[game_state->piece_position_y + 2][game_state->piece_position_x + 1] != EMPTY &&
            grid[game_state->piece_position_y + 2][game_state->piece_position_x + 1] != MOVING) {
            checker = true;
        }

        if(grid[game_state->piece_position_y + 1][game_state->piece_position_x + 2] == MOVING &&
            grid[game_state->piece_position_y + 1][game_state->piece_position_x + 1] != EMPTY &&
            grid[game_state->piece_position_y + 1][game_state->piece_position_x + 1] != MOVING) {
            checker = true;
        }

        if(grid[game_state->piece_position_y + 2][game_state->piece_position_x + 2] == MOVING &&
            grid[game_state->piece_position_y + 1][game_state->piece_position_x + 2] != EMPTY &&
            grid[game_state->piece_position_y + 1][game_state->piece_position_x + 2] != MOVING) {
            checker = true;
        }

        if(grid[game_state->piece_position_y + 1][game_state->piece_position_x + 2] == MOVING &&
            grid[game_state->piece_position_y + 2][game_state->piece_position_x + 2] != EMPTY &&
            grid[game_state->piece_position_y + 2][game_state->piece_position_x + 2] != MOVING) {
            checker = true;
        }

        if(!checker) {
            temp = piece[0][0];
            piece[0][0] = piece[0][3];
            piece[0][3] = piece[3][3];
            piece[3][3] = piece[3][0];
            piece[3][0] = temp;

            temp = piece[0][1];
            piece[0][1] = piece[1][3];
            piece[1][3] = piece[3][2];
            piece[3][2] = piece[2][0];
            piece[2][0] = temp;

            temp = piece[0][2];
            piece[0][2] = piece[2][3];
            piece[2][3] = piece[3][1];
            piece[3][1] = piece[1][0];
            piece[1][0] = temp;

            temp = piece[1][1];
            piece[1][1] = piece[1][2];
            piece[1][2] = piece[2][2];
            piece[2][2] = piece[2][1];
            piece[2][1] = temp;
        }

        for(int i = GRID_Y_SIZE - 2; i >= 0 ; --i) {
            for(int j = 1; j < GRID_X_SIZE - 1; ++j) {
                if(grid[i][j] == MOVING) {
                    grid[i][j] = EMPTY;
                }
            }
        }

        for(int i = game_state->piece_position_y; i < game_state->piece_position_y + 4; ++i) {
            for(int j = game_state->piece_position_x; j < game_state->piece_position_x + 4; ++j) {
                if(piece[i - game_state->piece_position_y][j - game_state->piece_position_x] == MOVING) {
                    grid[i][j] = MOVING;
                }
            }
        }

        return true;
    }

    return false;
}

static void check_completion(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE], game_state_t* game_state) {
    int calculator;

    for(int i = GRID_Y_SIZE - 2; i >= 0 ; --i) {
        calculator = 0;
        for(int j = 1; j < GRID_X_SIZE - 1; ++j) {
            if(grid[i][j] >= FULL) {
                ++calculator;
            }

            if(calculator == GRID_Y_SIZE - 2) {
                set_line_to_delete(game_state, true);
                calculator = 0;

                for(int z = 1; z < GRID_X_SIZE - 1; ++z) {
                    grid[z][i] = FADING;
                }
            }
        }
    }
}

static bool create_piece(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE], grid_square_t incoming_piece[4][4], grid_square_t piece[4][4], game_state_t* game_state, Color* current_piece_color, Color* incoming_piece_color) {
    int piece_num;
    set_piece_position_x(game_state, (int)((GRID_X_SIZE - 4) / 2));
    set_piece_position_y(game_state, 0);

    if(game_state->b_begin_play) { // first block creation
        piece_num = get_random_piece(incoming_piece);
        set_current_piece_num(game_state, piece_num);
        set_begin_play(game_state, false);
    }

    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            piece[i][j] = incoming_piece[i][j];
        }
    }

    *current_piece_color = get_piece_color(game_state->current_piece_num);
    set_finished_piece_num(game_state, game_state->current_piece_num);

    // assign next random piece
    piece_num = get_random_piece(incoming_piece);
    set_current_piece_num(game_state, piece_num);
    *incoming_piece_color = get_piece_color(piece_num);

    for(int i = 0; i < 4; ++i) {
        for(int j = game_state->piece_position_x; j < game_state->piece_position_x + 4; ++j) {
            if(piece[i][j - game_state->piece_position_x] == MOVING) {
                grid[i][j] = MOVING;
            }
        }
    }

    return true;
}

// generate block randomly
static int get_random_piece(grid_square_t incoming_piece[4][4]) {
    const int random = GetRandomValue(0, 6);
    for(int i = 0; i < 4; ++i) { // clean array
        for(int j = 0; j < 4; ++j) {
            incoming_piece[i][j] = EMPTY;
        }
    }

    switch(random) {
        case 0: // cube
            incoming_piece[1][1] = MOVING;
            incoming_piece[1][2] = MOVING;
            incoming_piece[2][1] = MOVING;
            incoming_piece[2][2] = MOVING;
            break;
        case 1: // L
            incoming_piece[0][1] = MOVING;
            incoming_piece[1][1] = MOVING;
            incoming_piece[2][1] = MOVING;
            incoming_piece[2][2] = MOVING;
            break;
        case 2: // J
            incoming_piece[0][2] = MOVING;
            incoming_piece[1][2] = MOVING;
            incoming_piece[2][2] = MOVING;
            incoming_piece[2][1] = MOVING;
            break;
        case 3: // I
            incoming_piece[0][1] = MOVING;
            incoming_piece[1][1] = MOVING;
            incoming_piece[2][1] = MOVING;
            incoming_piece[3][1] = MOVING;
            break;
        case 4: // T
            incoming_piece[0][1] = MOVING;
            incoming_piece[1][1] = MOVING;
            incoming_piece[2][1] = MOVING;
            incoming_piece[1][2] = MOVING;
            break;
        case 5: // S
            incoming_piece[0][1] = MOVING;
            incoming_piece[1][1] = MOVING;
            incoming_piece[1][2] = MOVING;
            incoming_piece[2][2] = MOVING;
            break;
        case 6: // Z
            incoming_piece[0][2] = MOVING;
            incoming_piece[1][1] = MOVING;
            incoming_piece[1][2] = MOVING;
            incoming_piece[2][1] = MOVING;
            break;
    }

    return random;
}

// assign a certain color for a certain shape
static Color get_piece_color(const int num) {
    Color piece_color;

    switch(num) {
        case 0: // cube
            piece_color = GOLD;
            break;
        case 1: // L
            piece_color = DARKGREEN;
            break;
        case 2: // J
            piece_color = ORANGE;
            break;
        case 3: // I
            piece_color = PINK;
            break;
        case 4: // T
            piece_color = SKYBLUE;
            break;
        case 5: // S
            piece_color = VIOLET;
            break;
        case 6: // Z
            piece_color = MAROON;
            break;
    }

    return piece_color;
}

// unload game variables
static void unload_game(void) {
}

int main(void) {
    game_state_t game_state;
    counter_t counter;
    grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE]; // tetris map
    grid_square_t incoming_piece[4][4]; // next block
    grid_square_t hold_piece[4][4]; // hold block
    grid_square_t piece[4][4]; // generated block
    Color current_piece_color;
    Color incoming_piece_color;
    Color hold_piece_color;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "tetris");
    init_game(grid, incoming_piece, hold_piece, piece, &game_state, &counter);
    SetTargetFPS(60);

    // main game loop
    while (!WindowShouldClose()) {
        if(!game_state.b_begin_game) {
            draw_init_page();
            check_game_start(&game_state);
        } else {
            if(!game_state.b_game_over) {
                update_draw_frame(grid, incoming_piece, piece, &game_state, &counter, &current_piece_color, &incoming_piece_color);
            } else { // game over
                if(IsKeyPressed(KEY_ENTER)) { // restart
                    init_game(grid, incoming_piece, hold_piece, piece, &game_state, &counter);
                    set_game_over(&game_state, false);
                }
            }
            draw_map(grid, incoming_piece, hold_piece, &game_state, &current_piece_color, &incoming_piece_color, &hold_piece_color);
        }
    }

    unload_game();
    CloseWindow();

    return 0;
}

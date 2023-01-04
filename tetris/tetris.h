#ifndef TETRIS_H
#define TETRIS_H

#include <raylib.h>
#include "gamedata.h"

// --------------------------------------------------
// types and constants
// --------------------------------------------------

typedef enum grid_square {
    EMPTY,
    MOVING,
    BLOCK,
    FADING,
    FULL,
    CUBE_BLOCK,
    L_BLOCK,
    J_BLOCK,
    I_BLOCK,
    T_BLOCK,
    S_BLOCK,
    Z_BLOCK,
} grid_square_t;

enum {
    SQUARE_SIZE = 20,
    GRID_X_SIZE = 12,
    GRID_Y_SIZE = 21,
    SCREEN_WIDTH = 442,
    SCREEN_HEIGHT = 450,
    LATERAL_SPEED = 15,
    TURNING_SPEED = 12,
    FAST_FALL_AWAIT_COUNTER = 30,
    FADING_TIME = 33
};

static void draw_init_page(void);
static void check_game_start(game_state_t* game_state);
static void init_game(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE], grid_square_t incoming_piece[4][4], grid_square_t hold_piece[4][4], grid_square_t piece[4][4], game_state_t* game_state, counter_t* counter);
static void draw_map(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE], grid_square_t incoming_piece[4][4], grid_square_t hold_piece[4][4], game_state_t* game_state, Color* current_piece_color, Color* incoming_piece_color, Color* hold_piece_color, counter_t* counter);
static void update_draw_frame(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE], grid_square_t incoming_piece[4][4], grid_square_t piece[4][4], game_state_t* game_state, counter_t* counter, Color* current_piece_color, Color* incoming_piece_color);
static void save_color(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE], const int piece_num);
static void check_detection(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE], game_state_t* game_state);
static void resolve_falling_movement(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE], game_state_t* game_state);
static bool resolve_lateral_movement(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE], game_state_t* game_state);
static bool resolve_turn_movement(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE], grid_square_t piece[4][4], game_state_t* game_state);
static int delete_complete_lines(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE]);
static void check_completion(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE], game_state_t* game_state);
static bool create_piece(grid_square_t grid[GRID_Y_SIZE][GRID_X_SIZE], grid_square_t incoming_piece[4][4], grid_square_t piece[4][4], game_state_t* game_state, Color* current_piece_color, Color* incoming_piece_color);
static int get_random_piece(grid_square_t incoming_piece[4][4]);
static Color get_piece_color(const int num);

#endif /* TETRIS_H */

#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <stdbool.h>

typedef struct game_state_t {
    bool b_game_over;
    bool b_begin_game; // 시작 화면에서 게임 화면으로 넘어가기 위해 사용
    bool b_begin_play; // only true at first. used for the first block creation
    bool b_pause; // p 누르면 게임 일시정지
    bool b_piece_active; // 현재 블록이 이동 중인가
    bool b_detection; // 낙하 충돌 감지
    bool b_line_to_delete;
    int g_level;
    int g_speed;
    int piece_position_x;
    int piece_position_y;
    int current_piece_num; // 현재 블록
    int finished_piece_num; // 이동 끝난 블록
} game_state_t;

typedef struct counter_t {
    int fast_fall_movement_counter;
    int gravity_movement_counter;
    int lateral_movement_counter;
    int turn_movement_counter;
} counter_t;

// reset data

void reset_game_state(game_state_t* self);
void reset_counter(counter_t* self);

// game_state_t functions

void set_game_over(game_state_t* self, bool val);
void set_begin_game(game_state_t* self, bool val);
void set_begin_play(game_state_t* self, bool val);
void set_pause(game_state_t* self, bool val);
void set_piece_active(game_state_t* self, bool val);
void set_detection(game_state_t* self, bool val);
void set_line_to_delete(game_state_t* self, bool val);
void set_level(game_state_t* self, int val);
void set_speed(game_state_t* self, int val);
void set_piece_position_x(game_state_t* self, int val);
void increment_piece_position_x(game_state_t* self);
void decrement_piece_position_x(game_state_t* self);
void set_piece_position_y(game_state_t* self, int val);
void increment_piece_position_y(game_state_t* self);
void decrement_piece_position_y(game_state_t* self);
void set_current_piece_num(game_state_t* self, int val);
void set_finished_piece_num(game_state_t* self, int val);

// counter_t functions

void increment_fast_fall_movement_counter(counter_t* self);
void decrement_fast_fall_movement_counter(counter_t* self);
void increment_gravity_movement_counter(counter_t* self);
void decrement_gravity_movement_counter(counter_t* self);
void increment_lateral_movement_counter(counter_t* self);
void decrement_lateral_movement_counter(counter_t* self);
void increment_turn_movement_counter(counter_t* self);
void decrement_turn_movement_counter(counter_t* self);
void set_fast_fall_movement_counter(counter_t* self, int val);
void set_gravity_movement_counter(counter_t* self, int val);
void set_lateral_movement_counter(counter_t* self, int val);
void set_turn_movement_counter(counter_t* self, int val);

#endif /* GAMEDATA_H */

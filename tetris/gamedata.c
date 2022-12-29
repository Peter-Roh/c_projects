#include "gamedata.h"

// --------------------------------------------------
// reset data
// --------------------------------------------------

void reset_game_state(game_state_t* self) {
    self->b_game_over = false;
    self->b_begin_game = false;
    self->b_begin_play = true;
    self->b_pause = false;
    self->b_piece_active = false;
    self->b_detection = false;
    self->b_line_to_delete = false;
    self->g_level = 1;
    self->g_speed = 30;
    self->piece_position_x = 0;
    self->piece_position_y = 0;
    self->current_piece_num = -1;
    self->finished_piece_num = -1;
}

void reset_counter(counter_t* self) {
    self->gravity_movement_counter = 0;
    self->lateral_movement_counter = 0;
    self->turn_movement_counter = 0;
}

// --------------------------------------------------
// game_state_t functions
// --------------------------------------------------

void set_game_over(game_state_t* self, bool val) {
    self->b_game_over = val;
}

void set_begin_game(game_state_t* self, bool val) {
    self->b_begin_game = val;
}

void set_begin_play(game_state_t* self, bool val) {
    self->b_begin_play = val;
}

void set_pause(game_state_t* self, bool val) {
    self->b_pause = val;
}

void set_piece_active(game_state_t* self, bool val) {
    self->b_piece_active = val;
}

void set_detection(game_state_t* self, bool val) {
    self->b_detection = val;
}

void set_line_to_delete(game_state_t* self, bool val) {
    self->b_line_to_delete = val;
}

void set_level(game_state_t* self, int val) {
    self->g_level = val;
}

void set_speed(game_state_t* self, int val) {
    self->g_speed = val;
}

void set_piece_position_x(game_state_t* self, int val) {
    self->piece_position_x = val;
}

void increment_piece_position_x(game_state_t* self) {
    ++(self->piece_position_x);
}

void decrement_piece_position_x(game_state_t* self) {
    --(self->piece_position_x);
}

void set_piece_position_y(game_state_t* self, int val) {
    self->piece_position_y = val;
}

void increment_piece_position_y(game_state_t* self) {
    ++(self->piece_position_y);
}

void decrement_piece_position_y(game_state_t* self) {
    --(self->piece_position_y);
}

void set_current_piece_num(game_state_t* self, int val) {
    self->current_piece_num = val;
}

void set_finished_piece_num(game_state_t* self, int val) {
    self->finished_piece_num = val;
}

// --------------------------------------------------
// counter_t functions
// --------------------------------------------------

void increment_gravity_movement_counter(counter_t* self) {
    ++(self->gravity_movement_counter);
}

void decrement_gravity_movement_counter(counter_t* self) {
    --(self->gravity_movement_counter);
}

void increment_lateral_movement_counter(counter_t* self) {
    ++(self->lateral_movement_counter);
}

void decrement_lateral_movement_counter(counter_t* self) {
    --(self->lateral_movement_counter);
}

void increment_turn_movement_counter(counter_t* self) {
    ++(self->turn_movement_counter);
}

void decrement_turn_movement_counter(counter_t* self) {
    --(self->turn_movement_counter);
}

void set_gravity_movement_counter(counter_t* self, int val) {
    self->gravity_movement_counter = val;
};

void set_lateral_movement_counter(counter_t* self, int val) {
    self->lateral_movement_counter = val;
};

void set_turn_movement_counter(counter_t* self, int val) {
    self->turn_movement_counter = val;
};

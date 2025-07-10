#pragma once

struct Rhythm {
    // song info
    float bpm;
    float loop_length;
    // song setting
    // float beat_per_loop;
    float step_factor;
};

struct Timer {
    // time
    int current_time_milli;
    int delta_time_milli;
    float delta_time; // seconds
    
    // rhythm
    const Rhythm *rhythm;
    // calculated
    // float beat; // crotchet
    float rhythm_step; // minimal step
    float rhythm_step_inv;
    int rhythm_start_time_milli; // milliseconds
    float rhythm_position;
};

const Timer *timer_get_timer();
float timer_get_delta_time();
int timer_get_current_time_milli();
float timer_get_rhythm_position();
float timer_get_rhythm_step();
float timer_get_rhythm_step_inv();
void timer_init(int current_time_milli);
void timer_init_rhythm(const Rhythm* rhythm);
void timer_update(int current_time_milli, int delta_time_milli);

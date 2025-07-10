#include "timer.h"
#include <memory.h>

static Timer g_timer;

const Timer* timer_get_timer() {
    return &g_timer;
}

float timer_get_delta_time() {
    return g_timer.delta_time;
}

int timer_get_current_time_milli() {
    return g_timer.current_time_milli;
}

float timer_get_rhythm_position() {
    return g_timer.rhythm_position;
}

float timer_get_rhythm_step() {
    return g_timer.rhythm_step;
}

float timer_get_rhythm_step_inv() {
    return g_timer.rhythm_step_inv;
}

void timer_init(int current_time_milli) {
    memset(&g_timer, 0, sizeof(Timer));
    g_timer.current_time_milli = current_time_milli;
}

void timer_init_rhythm(const Rhythm* rhythm) {
    g_timer.rhythm = rhythm;
    float crotchet = 60.0f / rhythm->bpm;
    g_timer.rhythm_step = crotchet * rhythm->step_factor; // beats per crotchet
    // avoid divide zero
    g_timer.rhythm_step_inv = g_timer.rhythm_step > 1e-4 ? 1.0f / g_timer.rhythm_step : 1.0f;

    // rhythm timer
    g_timer.rhythm_start_time_milli = g_timer.current_time_milli; // TODO: check if this is the case
    g_timer.rhythm_position = 0.0f;
}

void timer_update(int current_time_milli, int delta_time_milli) {
    // update time
    g_timer.current_time_milli = current_time_milli;
    g_timer.delta_time_milli = delta_time_milli;

    g_timer.delta_time = delta_time_milli / 1000.0f;

    // update rhythm position
    if (g_timer.rhythm) {
        float rhythm_time_pos = g_timer.current_time_milli - g_timer.rhythm_start_time_milli;
        rhythm_time_pos = rhythm_time_pos > g_timer.rhythm->loop_length ?
            rhythm_time_pos - g_timer.rhythm->loop_length : rhythm_time_pos;
        g_timer.rhythm_position = rhythm_time_pos / 1000.0f * g_timer.rhythm_step_inv; // avoid divide zero
    }
}


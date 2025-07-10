#include "audio.h"
#include "platform.h"
#include "timer.h"
#include <memory.h>

//Audio g_audio;
// handles
static int g_handles_bgm[AudioBgm_Num];
static Rhythm g_bgm_infos[AudioBgm_Num];

// sounds
static int g_handles_sound[AudioSound_Num];
static bool g_sound_playing[AudioSound_Num];
// status
static int g_bgm_current = 0; // 0 for error so never happening

void audio_init() {
    // load all audios here
    g_handles_bgm[AudioBgm_Game] = platform_opensound((char *)"assets/audios/MesmerizingGalaxyLoop.mp3");
    g_bgm_infos[AudioBgm_Game] = Rhythm{
        124,// bpm
        92970, // song length
        1
    };
    g_handles_bgm[AudioBgm_Title] = platform_opensound((char*)"assets/audios/GalacticRap.mp3");
    g_bgm_infos[AudioBgm_Title] = Rhythm{
        120,// bpm
        142054, // song length
        1
    };

    g_handles_sound[AudioSound_ItemGain] = platform_opensound((char *)"assets/audios/se_itemget_009.mp3");
    g_handles_sound[AudioSound_ItemGainBad] = platform_opensound((char*)"assets/audios/maou_se_system39.mp3");
    g_handles_sound[AudioSound_Select] = platform_opensound((char*)"assets/audios/maou_se_system41.mp3");
    g_handles_sound[AudioSound_Jump] = platform_opensound((char*)"assets/audios/se_jump1.mp3");
    g_handles_sound[AudioSound_Fall] = platform_opensound((char*)"assets/audios/se_select_006.mp3");
    g_handles_sound[AudioSound_Spot] = platform_opensound((char*)"assets/audios/maou_se_system46.mp3");
    g_handles_sound[AudioSound_ItemSpawn] = platform_opensound((char*)"assets/audios/se_powerup_006.mp3");
    memset(g_sound_playing, 0, sizeof(bool) * AudioSound_Num);
    // TODO: error handling
}

void audio_destroy() {
    for (int i = 0; i < AudioBgm_Num; ++i) {
        platform_closesound(g_handles_bgm[i]);
    }
    for (int i = 0; i < AudioSound_Num; ++i) {
        platform_closesound(g_handles_sound[i]);
    }
}

void audio_play_bgm(AudioBgm bgm) {
    if (bgm >= 0 && bgm < AudioBgm_Num) {
        // valid
        if (g_bgm_current) { // stop current playing bgm
            // TODO: allow multiple??
            platform_stopsound(g_bgm_current);
        }
        if (g_handles_bgm[bgm]) {
            // play sound if audio file opened correctly
            platform_playsound(g_handles_bgm[bgm], 1); // 1 for repeat
        }
        // set rhythm with predefined song info, even if the file does not exist
        timer_init_rhythm(&g_bgm_infos[bgm]);
        g_bgm_current = g_handles_bgm[bgm];
    }
}

void audio_play_sound(AudioSound sound) {
    if (sound >= 0 && sound < AudioSound_Num && g_handles_sound[sound]) {
        // valid
        if (g_sound_playing[sound]) {
            platform_stopsound(g_handles_sound[sound]);
        }
        platform_playsound(g_handles_sound[sound], 0);
    }
}
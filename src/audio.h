#pragma once

enum AudioBgm {
    // begin
    AudioBgm_Game,
    AudioBgm_Title,
    // end
    AudioBgm_Num // number of bgms
};

enum AudioSound {
    AudioSound_ItemGain,
    AudioSound_ItemGainBad,
    AudioSound_Select,
    AudioSound_Jump,
    AudioSound_Fall,
    AudioSound_Spot,
    AudioSound_ItemSpawn,
    // end
    AudioSound_Num
};

//struct Audio {
//    int handles_bgm[AudioBgm_Num];
//    int bgm_current;
//};

//Audio* audio_get_audio();
void audio_init();
void audio_destroy();
void audio_play_bgm(AudioBgm bgm);
void audio_play_sound(AudioSound sound);
#define MINIAUDIO_IMPLEMENTATION
#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"
#include "AudioManager.h"


void AudioManager::Init() {
    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = ma_format_f32;
    config.playback.channels = 2;
    config.sampleRate = 44100;
    config.dataCallback = data_callback;
    config.pUserData = this;

    ma_device_init(NULL, &config, &device);
    ma_device_start(&device);

}
void AudioManager::Terminate() {
	
}

void AudioManager::StartSound(const char* path) {
    Sound s;
    ma_decoder_init_file(path, NULL, &s.decoder);
    s.active = true;
    sounds.push_back(s);
}

void AudioManager::data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    float* out = (float*)pOutput;
    memset(out, 0, frameCount * sizeof(float) * 2); // stereo

    AudioManager* audio = (AudioManager*)pDevice->pUserData;

    for (auto s = audio->sounds.begin(); s != audio->sounds.end();) {
        if (!s->active) continue;

        float temp[4096 * 2]; // temp buffer
        ma_uint64 framesRead = frameCount;
        ma_decoder_read_pcm_frames(&s->decoder, temp, framesRead, NULL);

        for (ma_uint32 i = 0; i < framesRead * 2; ++i) {
            float sample = out[i] + temp[i];
            if (sample > 1.0f) sample = 1.0f;
            if (sample < -1.0f) sample = -1.0f;
            out[i] = sample;
        }

        if (framesRead < frameCount) {
            s->active = false;
            ma_decoder_uninit(&s->decoder);
            s = audio->sounds.erase(s);
        }
        else s++;
    }
}
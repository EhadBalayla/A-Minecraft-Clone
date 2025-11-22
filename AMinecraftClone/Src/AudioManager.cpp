#define MINIAUDIO_IMPLEMENTATION
#include "AudioManager.h"
#include <iostream>


void AudioManager::Init() {
	ma_result result = ma_engine_init(NULL, &soundEngine);
	if (result != MA_SUCCESS) {
		std::cerr << "FAILED TO INITIALIZE AUDIO" << std::endl;
	}

}
void AudioManager::Terminate() {
	ma_engine_uninit(&soundEngine);
}

void AudioManager::StartSound(const char* path) {
	ma_engine_play_sound(&soundEngine, path, NULL);
}
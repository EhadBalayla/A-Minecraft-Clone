#pragma once
#include "miniaudio.h"
#include <vector>

struct Sound {
	ma_decoder decoder;
	bool active = false;
};

class AudioManager
{
public:
	void Init();
	void Terminate();

	void StartSound(const char* path); //for playing regular action sounds
private:
	ma_device device;
	std::vector<Sound> sounds;

	static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
};


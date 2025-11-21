#pragma once
#include "miniaudio.h"

class AudioManager
{
public:
	void Init();
	void Terminate();

	void StartSound(const char* path); //for playing regular action sounds
private:
	ma_engine soundEngine;
};


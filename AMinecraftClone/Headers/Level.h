#pragma once
#include "WorldManager.h"
#include "SkyManager.h"


class Level
{
public:
	Level();
	~Level();

	void RenderLevel();
	void LevelUpdate(float DeltaTime);

	//getters
	WorldManager& GetWorld();
	SkyManager& GetSky();
private:
	WorldManager worldManager;
	SkyManager skyManager;
};


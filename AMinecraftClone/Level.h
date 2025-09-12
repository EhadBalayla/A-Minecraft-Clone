#pragma once
#include "WorldManager.h"


class Level
{
public:
	Level();
	~Level();

	void RenderLevel();
	void LevelUpdate(float DeltaTime);

	//getters
	WorldManager& GetWorld();
private:
	WorldManager worldManager;
};


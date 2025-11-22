#include "Level.h"



Level::Level() {
	worldManager.owningLevel = this;
}

Level::~Level() {

}


void Level::RenderLevel() {
	//render sky
	skyManager.RenderSky();
	skyManager.RenderClouds();

	//render the map
	worldManager.RenderWorld();
}
void Level::LevelUpdate(float DeltaTime) {
	worldManager.WorldUpdate(DeltaTime);
}



WorldManager& Level::GetWorld() {
	return worldManager;
}
SkyManager& Level::GetSky() {
	return skyManager;
}
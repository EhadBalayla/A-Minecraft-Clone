#include "Level.h"



Level::Level() {
	worldManager.owningLevel = this;
}

Level::~Level() {

}


void Level::RenderLevel() {
	worldManager.RenderWorld();
}
void Level::LevelUpdate(float DeltaTime) {
	worldManager.WorldUpdate(DeltaTime);
}



WorldManager& Level::GetWorld() {
	return worldManager;
}
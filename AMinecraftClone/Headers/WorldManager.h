#pragma once

#include "ChunkGenerator.h"
#include "ChunkProvider.h"

class Level;
class WorldManager
{
public:
	bool IsSnowWorld = false;
	Level* owningLevel;

	WorldManager();
	~WorldManager();

	void ChunksStart(int CenterX, int CenterZ); //performs world startup operations for chunks
	void UpdateChunks(int CenterX, int CenterZ, uint8_t LOD); //basically called for when the player switches a chunk
	void WorldUpdate(float DeltaTime);
	void RenderWorld();

	//block helpers
	BlockType getBlockAt(int x, int y, int z);
	void setBlockAt(int x, int y, int z, BlockType type);
	int getHeightValue(int x, int z);
	bool IsSolidBlock(int x, int y, int z);
	void PlaceBlock(int x, int y, int z, BlockType type);
	void BreakBlock(int x, int y, int z);

	ChunkGenerator& GetChunkGenerator();
	ChunkProvider& GetChunkProvider();
private:
	ChunkGenerator chunkGenerator; //the world generator itself
	ChunkProvider chunkProvider; //the actual chunks cacher that owns the chunks
};

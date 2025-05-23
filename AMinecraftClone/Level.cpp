#include "Level.h"
#include "Game.h"
#include "CombinedNoise.h"
#include "OctaveNoise.h"
#include <cmath>
#include <iostream>
#include <algorithm>



Level::Level() {

	noiseGen1 = new NoiseGeneratorOctave(16);
	noiseGen2 = new NoiseGeneratorOctave(16);
	noiseGen3 = new NoiseGeneratorOctave(8);
	noiseGen4 = new NoiseGeneratorOctave(4);
	noiseGen5 = new NoiseGeneratorOctave(4);
	noiseGen6 = new NoiseGeneratorOctave(5);

	//NoiseGeneratorOctave(3);
	//NoiseGeneratorOctave(3);
	//NoiseGeneratorOctave(3);
}

Level::~Level() {
	delete noiseGen1;
	delete noiseGen2;
	delete noiseGen3;
	delete noiseGen4;
	delete noiseGen5;
	delete noiseGen6;
}

void Level::RenderLevel() {
	for (auto& pair : chunks) {
		glm::vec3 relativePos = glm::dvec3(pair.second->ChunkX * 16, 0, pair.second->ChunkZ * 16) - Game::player.GetPosition();
		Game::e_DefaultShader.setMat4("model", glm::translate(glm::mat4(1.0), relativePos));
		pair.second->RenderChunk();
	}
}

void Level::UpdateChunks(int ChunkX, int ChunkZ) { //this function updates render distance chunks, the parameters are the central chunk to use as the center
	
	//remove chunks outside of render distance
	for (auto& pair : chunks) {
		Chunk* chunkToCompare = getChunkAt(ChunkX, ChunkZ);
		if(chunkToCompare) {
			if (pair.second->DistanceFromChunk(getChunkAt(ChunkX, ChunkZ)) > Game::RenderDistance) {
				chunkRemovalQueue.push_back(pair.first);
			}
		}
		//this is in case the player used a teleport and teleported to an unloaded chunk where we can't compare render distances
		else chunkRemovalQueue.push_back(pair.first);
	}
	DeleteRemovalQueuedChunks();

	//generate new chunks that aren't in render distance.
	for (int x = -Game::RenderDistance; x <= Game::RenderDistance; x++) {
		for (int z = -Game::RenderDistance; z <= Game::RenderDistance; z++) {
			if (chunks.find(glm::ivec2(ChunkX + x, ChunkZ + z)) == chunks.end()) {
				LoadNewChunk(ChunkX + x, ChunkZ + z);
			}
		}
	}

	// generates the mesh for the chunks after finished generating loading all chunks
	for (auto& pair : chunks) {
		pair.second->GenerateMesh();
	}

}

void Level::LoadNewChunk(int ChunkX, int ChunkZ) { //loads a new chunk at given ChunkX and ChunkZ
	Chunk* chunk = new Chunk();
	chunk->ChunkX = ChunkX;
	chunk->ChunkZ = ChunkZ;
	chunk->owningLevel = this;
	chunk->GenerateChunk(noiseGen1, noiseGen2, noiseGen3, noiseGen4, noiseGen5, noiseGen6, rand);
	chunks[glm::ivec2(ChunkX, ChunkZ)] = chunk;
}

void Level::RemoveChunk(int ChunkX, int ChunkZ) { //removes a chunk at given ChunkX and ChunkZ
	auto it = chunks.find(glm::ivec2(ChunkX, ChunkZ));
	if (it != chunks.end()) { //basically if the chunk is found
		delete it->second;
		chunks.erase(it);
	}
}

bool Level::IsValidChunk(int ChunkX, int ChunkZ) { //coordinates are in chunk space
	return (chunks.find(glm::ivec2(ChunkX, ChunkZ)) != chunks.end()); //basically return true if this chunk is loaded (keyword: LOADED!!!)
}

AABB Level::getBlockHitbox(int x, int y, int z) { //coordinates are in world space
	return getBlockAt(x, y, z)->blockHitbox.MovedTo(glm::ivec3(x, y, z));
}

Chunk* Level::getChunkAt(int ChunkX, int ChunkZ) { //these coords are in chunk space
	auto it = chunks.find(glm::ivec2(ChunkX, ChunkZ));
	if (it != chunks.end()) //basically if it found any
		return it->second;
	return nullptr;
}

Block* Level::getBlockAt(int x, int y, int z) { //coordinates are in world space
	Chunk* chunk = getChunkAt(floor((float)x / 16.0f), floor((float)z / 16.0f));
	if (chunk)
		return &chunk->m_Blocks[(x % 16 + 16) % 16][y][(z % 16 + 16) % 16];
	return nullptr;
}

bool Level::IsSolidBlock(int x, int y, int z) { //coordinates are in world space
	Block* block = getBlockAt(x, y, z);
	if(block)
		return block->getType() != BlockType::Air && block->data.visibility == Opaque;
	return false;
}

void Level::PlaceBlock(int x, int y, int z, BlockType type) { //coordinates are in world space
	if (!IsSolidBlock(x, y, z)) {
		getBlockAt(x, y, z)->setType(type);
		getChunkAt(floor((float)x / 16.0f), floor((float)z / 16.0f))->GenerateMesh();
	}
}

void Level::DeleteRemovalQueuedChunks() {
	for (auto& n : chunkRemovalQueue) {
		delete chunks[n];
		chunks.erase(n);
	}
	chunkRemovalQueue.clear();


}
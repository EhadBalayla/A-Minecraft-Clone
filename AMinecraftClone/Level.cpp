#include "Level.h"
#include "Game.h"
#include "CombinedNoise.h"
#include "OctaveNoise.h"
#include <cmath>
#include <iostream>
#include <algorithm>



Level::Level() {

	//initialize noise for first world generation
	noiseGen1 = new NoiseGeneratorOctave(16);
	noiseGen2 = new NoiseGeneratorOctave(16);
	noiseGen3 = new NoiseGeneratorOctave(8);
	noiseGen4 = new NoiseGeneratorOctave(4);
	noiseGen5 = new NoiseGeneratorOctave(4);
	noiseGen6 = new NoiseGeneratorOctave(5);
	NoiseGeneratorOctave n1(3);
	NoiseGeneratorOctave n2(3);
	NoiseGeneratorOctave n3(3);


	//initialize noise for second world generation
	noiseGen1_2 = new NoiseGeneratorOctave2(rand, 16);
	noiseGen2_2 = new NoiseGeneratorOctave2(rand, 16);
	noiseGen3_2 = new NoiseGeneratorOctave2(rand, 8);
	mobSpawnerNoise_2 = new NoiseGeneratorOctave2(rand, 5);
}

Level::~Level() {
	delete noiseGen1;
	delete noiseGen2;
	delete noiseGen3;
	delete noiseGen4;
	delete noiseGen5;
	delete noiseGen6;

	delete noiseGen1_2;
	delete noiseGen2_2;
	delete noiseGen3_2;
	delete mobSpawnerNoise_2;
}

void Level::LevelUpdate(float DeltaTime) {
	//generate chunks
	if (!chunkGenQueue.empty()) {
		chunkGenTimer += DeltaTime;
		float dynamicDelay = chunkGenDelay + (chunkGenQueue.size() - 1) * 0.1f;
		if (chunkGenTimer >= dynamicDelay) {
			chunkGenTimer = 0;
			glm::ivec2 chunkPos = chunkGenQueue.front();
			chunkGenQueue.pop();
			LoadNewChunk(chunkPos.x, chunkPos.y);

			GenerateChunkMeshes();
		}
	}
}
float squaredDistance(const glm::vec2& p1, const glm::vec2& p2) {
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;
	return dx * dx + dy * dy;
}
void Level::RenderLevel() {
	//render opaque and also collect those with water
	std::vector<Chunk*> waterChunks;
	for (auto& pair : chunks) {
		if(pair.second->RenderReady) {
			pair.second->RenderOpaqueAndPlants();
		}
		if(pair.second->HasWater)
			waterChunks.push_back(pair.second);
	}

	//sorts chunks with water in their order
	std::sort(waterChunks.begin(), waterChunks.end(), [&](Chunk* a, Chunk* b) {
		float distA = squaredDistance(glm::vec3(a->ChunkX + 8, 0, a->ChunkZ + 8), Game::player.GetPosition());
		float distB = squaredDistance(glm::vec3(b->ChunkX + 8, 0, b->ChunkZ + 8), Game::player.GetPosition());
		return distA > distB; // Furthest first
		});


	//rendering features for the waters
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	for (auto& pair : chunks) {
		if (pair.second->RenderReady) {
			pair.second->RenderWater();
		}
	}
}

void Level::UpdateChunks(int ChunkX, int ChunkZ) { //this function updates render distance chunks, the parameters are the central chunk to use as the center

	//chunks removal
	for (auto it = chunks.begin(); it != chunks.end(); ) {
		const glm::ivec2& pos = it->first;

		int dx = std::abs(pos.x - ChunkX);
		int dz = std::abs(pos.y - ChunkZ);

		if (dx > Game::RenderDistance || dz > Game::RenderDistance) {
			chunkMeshGenQueue.erase(std::remove(chunkMeshGenQueue.begin(), chunkMeshGenQueue.end(), pos), chunkMeshGenQueue.end());
			delete it->second;
			it = chunks.erase(it);
		}
		else {
			++it;
		}
	}

	//generate new chunks that aren't in render distance.
	for (int r = 0; r <= Game::RenderDistance; ++r) {
		for (int dx = -r; dx <= r; ++dx) {
			for (int dz = -r; dz <= r; ++dz) {
				if (std::abs(dx) != r && std::abs(dz) != r) continue; // Only edges of the square
				if (chunks.find(glm::ivec2(ChunkX + dx, ChunkZ + dz)) == chunks.end()) {
					chunkGenQueue.push(glm::ivec2(ChunkX + dx, ChunkZ + dz));
				}
			}
		}
	}
}

void Level::LoadNewChunk(int ChunkX, int ChunkZ) { //loads a new chunk at given ChunkX and ChunkZ
	Chunk* chunk = new Chunk();
	chunk->ChunkX = ChunkX;
	chunk->ChunkZ = ChunkZ;
	chunk->owningLevel = this;

	//the commented "GenerateChunk" functions are for different terrain generations, i wanna keep all the terrain generations from older versions of Minecraft
	//if you are interested you can check em out by uncommenting another option and commenting the rest
	// 
	//chunk->GenerateChunk(noiseGen1, noiseGen2, noiseGen3, noiseGen4, noiseGen5, noiseGen6, rand);
	chunk->GenerateChunk2(noiseGen1_2, noiseGen2_2, noiseGen3_2, rand, mobSpawnerNoise_2);
	chunkMeshGenQueue.push_back(glm::ivec2(ChunkX, ChunkZ));
	chunks[glm::ivec2(ChunkX, ChunkZ)] = chunk;
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

bool Level::IsChunkNeighboorsGood(int ChunkX, int ChunkZ) {
	if (chunks.find(glm::ivec2(ChunkX, ChunkZ)) == chunks.end())
		return false;

	int centerChunkX = static_cast<int>(std::floor(Game::player.GetPosition().x / 16.0));
	int centerChunkZ = static_cast<int>(std::floor(Game::player.GetPosition().z / 16.0));

	bool IsPositiveX = (ChunkX + 1 > centerChunkX + Game::RenderDistance) || IsValidChunk(ChunkX + 1, ChunkZ); //positive X of render distance
	bool IsNegativeX = (ChunkX - 1 < centerChunkX - Game::RenderDistance) || IsValidChunk(ChunkX - 1, ChunkZ); //negative X of render distance
	bool IsPositiveZ = (ChunkZ + 1 > centerChunkZ + Game::RenderDistance) || IsValidChunk(ChunkX, ChunkZ + 1); //positive Z of render distanec
	bool IsNegativeZ = (ChunkZ - 1 < centerChunkZ - Game::RenderDistance) || IsValidChunk(ChunkX, ChunkZ - 1); //negative Z of render distance


	return IsPositiveX && IsNegativeX && IsPositiveZ && IsNegativeZ;
}

void Level::GenerateChunkMeshes() {
	if (!chunkMeshGenQueue.empty()) {
		for (auto it = chunkMeshGenQueue.begin(); it != chunkMeshGenQueue.end(); ) {
			glm::ivec2 chunkPos = *it;
			if (IsChunkNeighboorsGood(chunkPos.x, chunkPos.y)) {
				chunks[chunkPos]->GenerateMesh();
				chunks[chunkPos]->RenderReady = true;
				it = chunkMeshGenQueue.erase(it);
			}
			else ++it;
		}
	}
}
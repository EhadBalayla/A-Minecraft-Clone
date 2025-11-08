#pragma once
#include "Chunk.h"

#include <unordered_map>


namespace std {
	template<>
	struct hash<glm::ivec2> {
		std::size_t operator()(const glm::ivec2& v) const {
			std::size_t h1 = std::hash<int>()(v.x);
			std::size_t h2 = std::hash<int>()(v.y);
			return h1 ^ (h2 << 1); // combine hashes
		}
	};
}

class WorldManager;
class ChunkProvider
{
public:
	ChunkProvider(WorldManager* world);
	WorldManager* owningWorld;

	Chunk* ProvideChunk(int ChunkX, int ChunkZ);
	std::unordered_map<glm::ivec2, Chunk*>& GetAllChunks();
private:
	std::unordered_map<glm::ivec2, Chunk*> chunks;

	bool IsValidChunk(int ChunkX, int ChunkZ);
	Chunk* LoadNewChunk(int ChunkX, int ChunkZ);

	void populate(int ChunkX, int ChunkZ);
};


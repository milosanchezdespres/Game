#pragma once


#include "../core/core.h"

namespace iso
{
    struct Tile { int index = 0; };

    struct Chunk
    {
        static constexpr int SIZE = 32;
        std::vector<std::vector<Tile>> layers;

        Chunk(int layer_count = 1) { layers.resize(layer_count, std::vector<Tile>(SIZE * SIZE)); }

        Tile& at(int layer, int i, int j) { return layers[layer][j * SIZE + i]; }
    };

    struct ChunkCoord
    {
        int x, y;
        bool operator==(const ChunkCoord& o) const { return x == o.x && y == o.y; }
    };
}

namespace std
{
    template <>
    struct hash<iso::ChunkCoord>
    {
        size_t operator()(const iso::ChunkCoord& k) const { return std::hash<int>()(k.x) ^ (std::hash<int>()(k.y) << 1); }
    };
}

namespace iso
{
    struct Camera
    {
        float x = 0;
        float y = 0;
        float zoom = 1.0f;
    };

    struct World
    {
        std::unordered_map<ChunkCoord, Chunk> chunks;

        void add(int layer, int i, int j, int tile_index)
        {
            ChunkCoord coord = {i / Chunk::SIZE, j / Chunk::SIZE};
            int local_i = i % Chunk::SIZE;
            int local_j = j % Chunk::SIZE;
            if (local_i < 0) { coord.x -= 1; local_i += Chunk::SIZE; }
            if (local_j < 0) { coord.y -= 1; local_j += Chunk::SIZE; }
            Chunk& chunk = chunks[coord];
            if (chunk.layers.size() <= layer) chunk.layers.resize(layer + 1, std::vector<Tile>(Chunk::SIZE * Chunk::SIZE));
            chunk.at(layer, local_i, local_j).index = tile_index;
        }

        Tile* get(int layer, int i, int j)
        {
            ChunkCoord coord = {i / Chunk::SIZE, j / Chunk::SIZE};
            int local_i = i % Chunk::SIZE;
            int local_j = j % Chunk::SIZE;
            if (local_i < 0) { coord.x -= 1; local_i += Chunk::SIZE; }
            if (local_j < 0) { coord.y -= 1; local_j += Chunk::SIZE; }
            auto it = chunks.find(coord);
            if (it == chunks.end()) return nullptr;
            Chunk& chunk = it->second;
            if (layer >= chunk.layers.size()) return nullptr;
            return &chunk.at(layer, local_i, local_j);
        }

        std::vector<Tile*> get_chunk_tiles(const ChunkCoord& coord, int layer)
        {
            std::vector<Tile*> result;
            auto it = chunks.find(coord);
            if (it == chunks.end()) return result;
            Chunk& chunk = it->second;
            if (layer >= chunk.layers.size()) return result;
            for (auto& tile : chunk.layers[layer]) result.push_back(&tile);
            return result;
        }
    };
}

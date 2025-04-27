#pragma once
// 寻路:这个项目的寻路比较简单，地图中的每个格子都存在方向

#include "tile.h"

#include <SDL.h>

class Route
{
public:
    typedef std::vector<SDL_Point> IdxList;
    Route() = default;
    Route(const TileMap& tileMap, const SDL_Point& idx_origin)
    {
        size_t width_map = tileMap[0].size();
        size_t height_map = tileMap.size();
        SDL_Point idx_next = idx_origin;
        while (true)
        {
            if (idx_next.x >= width_map || idx_next.y >= height_map)
                break;
            if (Check_Duplicate_Idx(idx_next)) // 避免环卡死
                break;
            idx_List.push_back(idx_next);

            bool is_has_next_dir = true;
            const Tile& tile = tileMap[idx_next.y][idx_next.x];

            if (tile.special_flag == 0) break; // 已到达终点

            switch (tile.direction)
            {
            case Tile::Direction::Up:
                idx_next.y --;
                break;
            case Tile::Direction::Down:
                idx_next.y ++;
                break;
            case Tile::Direction::Left:
                idx_next.x --;
                break;
            case Tile::Direction::Right:
                idx_next.x ++;
                break;
            default:
                is_has_next_dir = false;
                break;
            };
            if (!is_has_next_dir)
                break;
        }

    };
    ~Route() {};
    
    const IdxList& Get_Idx_List() const { return idx_List; };

private:
    IdxList idx_List; // 路径

    bool Check_Duplicate_Idx(SDL_Point& idx) // 检查是否重复
    {
        for (const SDL_Point& idx : idx_List)
        {
            if (idx.x == idx.x && idx.y == idx.y)
                return true;
        }
        return false;
    };
};
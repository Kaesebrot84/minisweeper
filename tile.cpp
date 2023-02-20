#include "tile.h"

namespace minis
{
    Tile::Tile(Vector2 position, Vector2 grid_position,
               float size, int font_size,
               Texture2D *tile_texture,
               Texture2D *flag_texture,
               Texture2D *mine_texture,
               Texture2D *cross_texture)
        : position(position),
          grid_position(grid_position),
          size(size),
          font_size(font_size),
          tile_texture(tile_texture),
          flag_texture(flag_texture),
          mine_texture(mine_texture),
          cross_texture(cross_texture) {}

    void Tile::Update() {}

    void Tile::Draw(bool game_over)
    {
        if (concealed == true && flag == true)
        {
            DrawTexture(*tile_texture, position.x, position.y, WHITE);
            DrawTexture(*flag_texture, position.x, position.y, WHITE);
        }
        else if (game_over && flag && !is_mine)
        {
            DrawTexture(*mine_texture, position.x, position.y, WHITE);
            DrawTexture(*cross_texture, position.x, position.y, WHITE);
        }
        else if (concealed == true)
            DrawTexture(*tile_texture, position.x, position.y, WHITE);
        else if (is_mine)
        {
            if (!triggered)
            {
                DrawTexture(*mine_texture, position.x, position.y, WHITE);
            }
            else
            {
                DrawRectangle(position.x, position.y, size, size, RED);
                DrawTexture(*mine_texture, position.x, position.y, WHITE);
            }
        }
        else if (num_neighbor_mines > 0)
            DrawText(std::to_string(num_neighbor_mines).c_str(),
                     10 + position.x,
                     5 + position.y,
                     font_size,
                     NumberColor(num_neighbor_mines));
    }
}
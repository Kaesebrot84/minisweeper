#ifndef TILE_H
#define TILE_H

#include "raylib.h"
#include <string>

namespace minis
{

    /**
     * @brief Returns a color code based on a number (amount of adjacent mines i. e.).
     *
     * @param number A number (amount of adjacent mines i. e.).
     * @return Color Color code based on the number passed.
     */
    inline static Color NumberColor(int number)
    {
        switch (number)
        {
        case 1:
            return BLUE;
        case 2:
            return GREEN;
        case 3:
            return RED;
        default:
            return DARKBLUE;
        }
    }

    class Tile
    {
    private:
        Vector2 position;
        Vector2 grid_position;
        bool is_mine = false;
        bool concealed = true;
        int num_neighbor_mines = 0;
        bool flag = false;
        bool triggered = false;
        float size;
        int font_size;

        Texture2D *tile_texture;
        Texture2D *flag_texture;
        Texture2D *mine_texture;
        Texture2D *cross_texture;

    public:
        inline void SetMine(bool value) { is_mine = value; }
        inline void SetNumberNeighborMines(int value) { num_neighbor_mines = value; }
        inline int GetNumberNeighborMines() { return num_neighbor_mines; }
        inline bool IsMine() { return is_mine; }
        inline bool Flagged() { return flag; }
        inline void Trigger() { triggered = true; }
        inline void Reveal() { concealed = false; }
        inline bool Concealed() { return concealed; }
        inline int PosX() { return position.x; }
        inline int PosY() { return position.y; }
        inline int GridPosX() { return grid_position.x; }
        inline int GridPosY() { return grid_position.y; }
        inline bool ToggleFlag()
        {
            flag = !flag;
            return flag;
        }
        inline bool CheckCollision(Vector2 &mouse_point)
        {
            return CheckCollisionPointRec(mouse_point,
                                          (Rectangle){position.x, position.y,
                                                      size, size});
        }

        /**
         * @brief Construct a new Tile object
         *
         * @param position Tile position.
         * @param grid_position Tile position on the grid (row, column position).
         * @param size Pixel size of the tile.
         * @param font_size Font size of the text in the tile.
         * @param tile_texture Tile texture.
         * @param flag_texture Flag texture.
         * @param mine_texture Mine texture.
         * @param cross_texture Cross texture
         */
        Tile(Vector2 position, Vector2 grid_position,
             float size, int font_size,
             Texture2D *tile_texture,
             Texture2D *flag_texture,
             Texture2D *mine_texture,
             Texture2D *cross_texture);

        /**
         * @brief Update tile logic.
         *
         */
        void Update();

        /**
         * @brief Draws the tile and its content.
         *
         */
        void Draw(bool game_over);
    };
}
#endif
#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <iostream>
#include <functional>
#include "tile.h"
#include "settings.h"

namespace minis
{
    class Field
    {
    public:
        /**
         * @brief Construct a new Field object
         *
         * @param position Field's screen position
         * @param settings - Game/Field settings
         */
        Field(Vector2 position, GameSettings settings);

        /**
         * @brief Destroy the Field object
         *
         */
        ~Field();

        /**
         * @brief Draw the field with all its content
         *
         */
        void Draw();
        Tile *GetTile(int row, int col);
        void RevealGrid();
        void FloodFill(Tile *tile);
        bool WinningConditionMet();
        bool GameOver();
        void HandleLeftMouse(Vector2 *mouse_point, std::function<void()> sound_callback);
        void HandleRightMouse(Vector2 *mouse_point, std::function<void()> sound_callback);
        Vector2 Position();

        inline const GameSettings *GetGameSettings()
        {
            return &settings;
        }

        /**
         * @brief Returns the count of mines hidden in the field
         *
         * @return int Number of mines in the field
         */
        inline int MineCount()
        {
            return settings.mines;
        }

        /**
         * @brief Returns the number of currently set flags on the field.
         *
         * @return int Current number of set flags
         */
        inline int FlagCount()
        {
            return flag_count;
        }

        /**
         * @brief Returns the number of rows in the field.
         *
         * @return int Number of rows in the field
         */
        inline int Rows()
        {
            return settings.rows;
        }

        /**
         * @brief Returns the number of columns in the field
         *
         * @return int Number of colimns in the field
         */
        inline int Columns()
        {
            return settings.columns;
        }

        inline int TileSize()
        {
            return settings.tile_size;
        }

    private:
        std::vector<std::vector<Tile>> grid;
        Vector2 grid_position;
        bool game_over = false;
        int tiles_open_count = 0;

        /**
         * @brief Checks if the row and column indices are valid (lie within the bound) for this field.
         *
         * @param row Target row index
         * @param col Target column index
         * @return true If row and column are within the field bounds
         * @return false If row or column are not within field bounds
         */
        bool IsTileValid(int row, int col);
        bool MineInTile(int row_pos, int col_pos);
        int GetNeighborMineCount(int own_row_pos, int own_col_pos);
        GameSettings settings;

        int flag_count = 0;

        Texture2D tile_texture;
        Texture2D flag_texture;
        Texture2D mine_texture;
        Texture2D cross_texture;
    };
}

#endif
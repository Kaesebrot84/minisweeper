#include "field.h"
#include "defines.h"

namespace minis
{
    /**
     * @brief Create a field object.
     *
     * @param position Upper left point the field will be drawn to.
     * @param settings Field settings.
     */
    Field::Field(Vector2 position, GameSettings settings)
        : grid_position(position), settings(settings)
    {
        if (settings.columns < 1 || settings.rows < 1)
            throw("Unable to create a field with less than 1 column or row.");

        // Load Textures
        if (settings.tile_size == 31)
        {
            tile_texture = LoadTexture("assets/tile_31x31.png");
            flag_texture = LoadTexture("assets/flag_31x31.png");
            cross_texture = LoadTexture("assets/cross_31x31.png");
            mine_texture = LoadTexture("assets/mine_31x31.png");
        }
        else if (settings.tile_size == 51)
        {
            tile_texture = LoadTexture("assets/tile_51x51.png");
            flag_texture = LoadTexture("assets/flag_51x51.png");
            cross_texture = LoadTexture("assets/cross_51x51.png");
            mine_texture = LoadTexture("assets/mine_51x51.png");
        }

        for (int row = 0; row < settings.rows; row++)
        {
            std::vector<Tile> tile_row;
            for (int col = 0; col < settings.columns; col++)
            {
                Tile tile = Tile(
                    Vector2{col * settings.tile_size + grid_position.x,
                            row * settings.tile_size + grid_position.y},
                    Vector2{(float)row, (float)col},
                    settings.tile_size, settings.font_size,
                    &tile_texture,
                    &flag_texture,
                    &mine_texture,
                    &cross_texture);

                tile_row.push_back(tile);
            }
            grid.push_back(tile_row);
        }

        // Populate mines
        int num_mines_placed = 0;

        while (num_mines_placed < settings.mines)
        {
            int row = GetRandomValue(0, settings.rows - 1);
            int col = GetRandomValue(0, settings.columns - 1);

            if (!MineInTile(row, col))
            {
                grid.at(row).at(col).SetMine(true);
                num_mines_placed++;
            }
        }

        // Assign tile numbers
        for (int row = 0; row < settings.rows; row++)
        {
            for (int col = 0; col < settings.columns; col++)
            {
                grid.at(row).at(col).SetNumberNeighborMines(GetNeighborMineCount(row, col));
            }
        }
    }

    Field::~Field()
    {
        UnloadTexture(tile_texture);
        UnloadTexture(mine_texture);
        UnloadTexture(flag_texture);
        UnloadTexture(cross_texture);
    }

    /**
     * @brief Draws the field grid and tiles.
     *
     */
    void Field::Draw()
    {
        // Draw vertical lines
        for (int i = 0; i <= Columns(); i++)
        {
            DrawLineV((Vector2){(float)(settings.tile_size * i) + grid_position.x, grid_position.y},
                      (Vector2){(float)(settings.tile_size * i) + grid_position.x, (float)settings.tile_size * Rows() + grid_position.y},
                      LIGHTGRAY);
        }

        // Draw horizontal lines
        for (int j = 0; j <= Rows(); j++)
        {
            DrawLineV((Vector2){grid_position.x, (float)(settings.tile_size * j) + grid_position.y},
                      (Vector2){(float)settings.tile_size * Columns() + grid_position.x, (float)(settings.tile_size * j) + grid_position.y},
                      LIGHTGRAY);
        }

        // Draw tiles, mines and flags
        for (int row = 0; row < Rows(); row++)
        {
            for (int col = 0; col < Columns(); col++)
            {
                Tile *tile = GetTile(row, col);
                tile->Draw(game_over);
            }
        }
    }

    /**
     * @brief Return a pointer to a tile based on a given row and column positions.
     * Returns `NULL` if invalid positions are passed.
     *
     * @param row Row index of the tile
     * @param col Column index of the tile
     * @return Tile* Tile on the given position or NULL if position is invalid
     */
    Tile *Field::GetTile(int row, int col)
    {
        if (!IsTileValid(row, col))
            return NULL;
        return &grid.at(row).at(col);
    }

    void Field::RevealGrid()
    {
        for (auto &row : grid)
        {
            for (auto &tile : row)
            {
                if (tile.IsMine() || (!tile.IsMine() && tile.Flagged()))
                    tile.Reveal();
            }
        }
    }

    /**
     * @brief Checks if the winning condition was met.
     *
     * @return true Player won the game.
     * @return false Player did not (yet) win the game.
     */
    bool Field::WinningConditionMet()
    {
        return (Rows() * Columns()) - tiles_open_count == settings.mines;
    }

    /**
     * @brief Performs the flood fill algorithm on a certain tile in the grid.
     *
     * @param tile Target tile around which the flood fill algorithm will be performed.
     */
    void Field::FloodFill(Tile *tile)
    {
        if (tile == NULL || tile->Flagged() || !tile->Concealed())
            return;
        if (tile->GetNumberNeighborMines() > 0)
        {
            tile->Reveal();
            tiles_open_count++;
            return;
        }

        tile->Reveal();
        tiles_open_count++;

        FloodFill(GetTile(tile->GridPosX() - 1, tile->GridPosY()));     // N
        FloodFill(GetTile(tile->GridPosX() - 1, tile->GridPosY() + 1)); // NE
        FloodFill(GetTile(tile->GridPosX(), tile->GridPosY() + 1));     // E
        FloodFill(GetTile(tile->GridPosX() + 1, tile->GridPosY() + 1)); // SE
        FloodFill(GetTile(tile->GridPosX() + 1, tile->GridPosY()));     // S
        FloodFill(GetTile(tile->GridPosX() + 1, tile->GridPosY() - 1)); // SW
        FloodFill(GetTile(tile->GridPosX(), tile->GridPosY() - 1));     // W
        FloodFill(GetTile(tile->GridPosX() - 1, tile->GridPosY() - 1)); // NW
    }

    /**
     * @brief Sanity check for a certain position on the grid.
     *
     * @param row Row position
     * @param col Column position
     * @return true If row and column are valid positions on the grid.
     * @return false Row and column are not valid position on the grid.
     */
    bool Field::IsTileValid(int row, int col)
    {
        return (!(row < 0) && !(row > grid.size() - 1)) && (!(col < 0) && !(col > grid.at(0).size() - 1));
    }

    /**
     * @brief Check if there is a mine in the tile on a given position.
     *
     * @param row_pos Row position of the target tile.
     * @param col_pos Column position of the target tile.
     * @return true There is a mine in the target tile.
     * @return false There is no mine in the target tile.
     */
    bool Field::MineInTile(int row_pos, int col_pos)
    {
        if (!IsTileValid(row_pos, col_pos))
            return false;
        return grid.at(row_pos).at(col_pos).IsMine();
    }

    /**
     * @brief Returns the number of adjacent tiles containing a mine.
     *
     * @param own_row_pos Row position of the target tile.
     * @param own_col_pos Column position of the target tile.
     * @return int Number of adhjacent tiles containing a mine.
     */
    int Field::GetNeighborMineCount(int own_row_pos, int own_col_pos)
    {
        int num_mines = 0;
        // N
        if (MineInTile(own_row_pos - 1, own_col_pos))
            num_mines++;

        // NE
        if (MineInTile(own_row_pos - 1, own_col_pos + 1))
            num_mines++;

        // E
        if (MineInTile(own_row_pos, own_col_pos + 1))
            num_mines++;

        // SE
        if (MineInTile(own_row_pos + 1, own_col_pos + 1))
            num_mines++;

        // S
        if (MineInTile(own_row_pos + 1, own_col_pos))
            num_mines++;

        // SW
        if (MineInTile(own_row_pos + 1, own_col_pos - 1))
            num_mines++;

        // W
        if (MineInTile(own_row_pos, own_col_pos - 1))
            num_mines++;

        // NW
        if (MineInTile(own_row_pos - 1, own_col_pos - 1))
            num_mines++;

        return num_mines;
    }

    /**
     * @brief Handles a right click event by toggling flags on valid tiles.
     *
     * @param mouse_point Mouse position when the right click event occured.
     * @param sound_callback Callback function which plays the click sound.
     */
    void Field::HandleRightMouse(Vector2 *mouse_point, std::function<void()> sound_callback)
    {
        bool hit = false;

        for (int row = 0; row < Rows(); row++)
        {
            for (int col = 0; col < Columns(); col++)
            {
                if (CheckCollisionPointRec(*mouse_point, (Rectangle){col * (float)settings.tile_size + grid_position.x,
                                                                     row * (float)settings.tile_size + grid_position.y,
                                                                     settings.tile_size, settings.tile_size}))
                {
                    Tile *tile = GetTile(row, col);
                    if (tile->Concealed())
                    {
                        if (tile->ToggleFlag())
                            flag_count++;
                        else
                            flag_count--;
                    }

                    sound_callback();
                    hit = true;
                    break;
                }
            }

            if (hit)
                break;
        }
    }

    /**
     * @brief Determines if the game over state has been reached.
     *
     * @return true Game over.
     * @return false Game is not over.
     */
    bool Field::GameOver()
    {
        return game_over;
    }

    /**
     * @brief Returns the current (upper, left) field position
     *
     * @return Vector2 Upper left field position.
     */
    Vector2 Field::Position()
    {
        return grid_position;
    }

    /**
     * @brief Handles a left click event by revealing closed tiles.
     * 
     * @param mouse_point Mouse position when the right click event occured.
     * @param sound_callback Callback function which plays the click sound.
     */
    void Field::HandleLeftMouse(Vector2 *mouse_point, std::function<void()> sound_callback)
    {
        if (WinningConditionMet() || game_over)
            return;

        bool hit = false;

        for (int row = 0; row < Rows(); row++)
        {
            for (int col = 0; col < Columns(); col++)
            {
                Tile *tile = GetTile(row, col);
                if (tile->CheckCollision(*mouse_point))
                {
                    if (tile->IsMine() && !tile->Flagged())
                    {
                        tile->Reveal();
                        tile->Trigger();
                        game_over = true;
                        RevealGrid();
                        sound_callback();
                        break;
                    }

                    if (tile->Concealed() && !tile->Flagged())
                    {
                        if (tile->GetNumberNeighborMines() < 1)
                        {
                            FloodFill(GetTile(row, col));
                        }
                        else
                        {
                            tile->Reveal();
                            tiles_open_count++;
                        }
                        sound_callback();
                    }
                    if (WinningConditionMet())
                    {
                        sound_callback();
                        RevealGrid();
                    }

                    hit = true;
                    break;
                }
            }

            if (hit)
                break;
        }
    }
}
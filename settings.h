#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <raylib.h>
#include "defines.h"

#define TILE_SIZE_SMALL 31
#define TILE_SIZE_BIG 51

namespace minis
{
    const std::string level_txt = "Beginner (8 x 8, 10);"
                                  "Beginner (9 x 9, 10);"
                                  "Beginner (10 x 10, 10);"
                                  "Intermediate (13 x 15, 40);"
                                  "Intermediate (16 x 16, 40);"
                                  "Expert (16 x 30, 99);"
                                  "Expert (30 x 16, 99)";

    enum DifficultyLevel
    {
        BEGINNER_1 = 0,
        BEGINNER_2,
        BEGINNER_3,
        INTERMEDIATE_1,
        INTERMEDIATE_2,
        EXPERT_1,
        EXPERT_2,
    };

    struct GameSettings
    {
        int rows;
        int columns;
        int mines;
        int tile_size;
        int font_size;
    };

    /**
     * @brief Returns the GameSettings based on the difficulty level.
     * 
     * @param level Desired difficulty level.
     * @return GameSettings GameSettings based on the difficulty level.
     */
    inline GameSettings GetSettings(const DifficultyLevel level)
    {
        switch (level)
        {
        case BEGINNER_1:
            return GameSettings{8, 8, 10, TILE_SIZE_BIG, 45};
        case BEGINNER_2:
            return GameSettings{9, 9, 10, TILE_SIZE_BIG, 45};
        case BEGINNER_3:
            return GameSettings{10, 10, 10, TILE_SIZE_BIG, 45};
        case INTERMEDIATE_1:
            return GameSettings{13, 15, 40, TILE_SIZE_BIG, 45};
        case INTERMEDIATE_2:
            return GameSettings{16, 16, 40, TILE_SIZE_BIG, 45};
        case EXPERT_1:
            return GameSettings{16, 30, 99, TILE_SIZE_SMALL, 25};
        case EXPERT_2:
            return GameSettings{30, 16, 99, TILE_SIZE_SMALL, 25};

        default:
            return GameSettings{8, 8, 10, TILE_SIZE_BIG};
        }
    }

    /**
     * @brief Calculates the window size according to the amount of rows, columns and the cell size.
     * 
     * @param settings GameSetting containing the field setup information
     * @return Vector2 Window width and height based on the field size.
     */
    inline Vector2 GetWindowSize(const GameSettings *settings)
    {
        float width = settings->tile_size * settings->columns;
        float height = settings->tile_size * settings->rows + HEADER_HEIGHT;
        return Vector2{width, height};
    }

}

#endif
#ifndef GAME_H
#define GAME_H

#include <vector>
#include <chrono>

#include "raylib.h"
#include "tile.h"
#include "field.h"
#include "digital_display.h"
#include "settings.h"

#define SQUARE_SIZE 31

namespace minis
{
    enum State
    {
        Play,
        ModeSelect,
    };

    class Game
    {
    private:
        Field *field;

        Vector2 mouse_point = Vector2{0.0f, 0.0f};
        std::chrono::_V2::steady_clock::time_point timer_start;
        int time_passed = 0;
        DigitalDisplay *timer;
        DigitalDisplay *mine_counter;
        bool show_info = false;
        int button_position_x;
        Sound click_sound = LoadSound("assets/click.wav");
        State state = State::Play;
        bool sound_on = true;

        /**
         * @brief Get the Button Icon object based on the current game state `state`
         *
         * @return int raygui icon identifier
         */
        int GetButtonIcon();

        /**
         * @brief Recalculates variables which are used for UI rendering based on the current game settings
         *
         */
        void RecalculateUI();

        /**
         * @brief Draws the game header
         * 
         */
        void DrawHeader();

        /**
         * @brief Draws the field selection menu 
         * 
         */
        void DrawMenu();

        void PlayClickSoundCallback();

    public:
        /**
         * @brief Construct a new Game object
         *
         * @param settings GameSettings for the current game
         */
        Game(GameSettings settings);

        /**
         * @brief Destroy the Game object
         *
         */
        ~Game();

        /**
         * @brief Update game logic
         *
         */
        void Update();

        /**
         * @brief Draw game content
         *
         */
        void Draw();
    };
}

#endif
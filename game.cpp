#include "game.h"
#include "defines.h"
#define RAYGUI_IMPLEMENTATION
#include "third_party/raygui.h"

namespace minis
{
    /**
     * @brief Creates a Game object according to the game settings.
     *
     * @param settings Settings for the game object.
     */
    Game::Game(GameSettings settings)
    {
        field = new Field(Vector2{0.0f, HEADER_HEIGHT}, settings);
        timer_start = std::chrono::steady_clock::now();

        button_position_x = settings.tile_size * field->Columns() / 2 - BUTTON_SIZE / 2;
        timer = new DigitalDisplay(
            Rectangle{(float)button_position_x + SQUARE_SIZE + DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y, DISPLAY_WIDTH, BUTTON_SIZE});
        mine_counter = new DigitalDisplay(
            Rectangle{(float)button_position_x - DISPLAY_WIDTH - DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y, DISPLAY_WIDTH, BUTTON_SIZE});
        SetSoundVolume(click_sound, 0.9f);
        GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
    }

    Game::~Game()
    {
        field->~Field();
        delete (timer);
        delete (mine_counter);
        StopSound(click_sound);
        UnloadSound(click_sound);
        CloseAudioDevice();
    }

    /**
     * @brief Updates game logic and handles user input.
     *
     */
    void Game::Update()
    {
        if (show_info || state == State::ModeSelect)
            return;

        if (state == State::Play)
        {
            timer->Update();
            mine_counter->Update();
            mouse_point = GetMousePosition();
            if (!field->GameOver() && !field->WinningConditionMet())
            {
                std::chrono::duration<double> elapsed_seconds = std::chrono::steady_clock::now() - timer_start;
                // Timer stops counting after reaching 10000 seconds
                time_passed = (int)(elapsed_seconds.count()) < 10000 ? (int)(elapsed_seconds.count()) : 9999;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    field->HandleLeftMouse(&mouse_point, std::bind(&Game::PlayClickSoundCallback, this));
                }
                else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
                {
                    field->HandleRightMouse(&mouse_point, std::bind(&Game::PlayClickSoundCallback, this));
                }
            }
        }
    }

    /**
     * @brief Plays click sound.
     *
     */
    void Game::PlayClickSoundCallback()
    {
        if (sound_on)
            PlaySound(click_sound);
    }

    /**
     * @brief Draws anything related to the game like the game field and ui.
     *
     */
    void Game::Draw()
    {
        if (state == State::Play)
        {
            field->Draw();
        }
        else if (state == State::ModeSelect)
        {
            DrawMenu();
        }

        DrawHeader();
    }

    /**
     * @brief Draws the header containing: Game state button, the timer, mine counter, info button and the sound toggle button.
     *
     */
    void Game::DrawHeader()
    {
        std::string time_str = std::to_string(time_passed);
        time_str.insert(0, 4 - time_str.length(), '0');

        int icon = GetButtonIcon();

        // Draw game state icon
        if (GuiButton((Rectangle){(float)button_position_x, 5, BUTTON_SIZE, BUTTON_SIZE}, GuiIconText(icon, "")) && !show_info)
        {
            state = State::ModeSelect;
            if (sound_on)
                PlaySound(click_sound);
        }

        // Draw timer
        timer->Draw(&time_str[0]);
        std::string mine_count = std::to_string(field->MineCount() - field->FlagCount());

        // Draw mine counter
        mine_counter->Draw(&mine_count[0]);

        // Draw Info button
        if (GuiButton(Rectangle{BUTTON_OFFSET_X, BUTTON_OFFSET_Y, BUTTON_SIZE, BUTTON_SIZE}, GuiIconText(ICON_HELP, "")))
        {
            show_info = true;
            if (sound_on)
                PlaySound(click_sound);
        }

        // Draw sound toggle button
        if (sound_on)
            GuiSetState(STATE_PRESSED);

        if (GuiButton(Rectangle{(float)field->Columns() * field->TileSize() - BUTTON_SIZE - BUTTON_OFFSET_X, BUTTON_OFFSET_Y, BUTTON_SIZE, BUTTON_SIZE}, GuiIconText(ICON_AUDIO, "")))
        {
            if (!sound_on)
                PlaySound(click_sound);
            sound_on = !sound_on;
        }

        GuiSetState(STATE_NORMAL);

        // Show info dialog if info button was pressed.
        if (show_info)
        {
            Vector2 win_size = GetWindowSize(field->GetGameSettings());
            int result = GuiMessageBox(
                Rectangle{INFO_DIALOG_OFFSET, HEADER_HEIGHT + INFO_DIALOG_OFFSET, win_size.x - INFO_DIALOG_OFFSET * 2, win_size.y - (INFO_DIALOG_OFFSET * 2) - HEADER_HEIGHT},
                GuiIconText(ICON_INFO, "Info"),
                "Created by: Kasebrot84\n\nCredits:\nraylib: https://www.raylib.com/\nraygui: https://github.com/raysan5/raygui",
                "Close");
            if ((result == 0) || (result == 1))
            {
                show_info = false;
                if (sound_on)
                    PlaySound(click_sound);
            }
        }
    }

    /**
     * @brief Draws the difficulty selection menu.
     *
     */
    void Game::DrawMenu()
    {
        int combo_x_pos = ((field->Columns() * field->TileSize()) / 2) - (COMBOBOX_WIDTH / 2);
        int combo_y_pos = ((field->Rows() * field->TileSize()) / 4) - (COMBOBOX_HEIGHT / 2);

        static int combobox_active = 0;

        // Draw combobox for difficulty selection
        combobox_active = GuiComboBox(
            Rectangle{(float)combo_x_pos, (float)combo_y_pos, (float)COMBOBOX_WIDTH, (float)COMBOBOX_HEIGHT},
            level_txt.c_str(), combobox_active);

        GameSettings settings = GetSettings((DifficultyLevel)combobox_active);

        std::string rowscols_info_txt = "Rows x columns: " + std::to_string(settings.rows) + " x " + std::to_string(settings.columns);
        std::string mines_info_txt = "Mines: " + std::to_string(settings.mines);

        float top_text_y_pos = (float)combo_y_pos + COMBOBOX_HEIGHT + HEADER_HEIGHT;
        // Draw Info about number of rows/columns and the number of mines.
        DrawText(rowscols_info_txt.c_str(), (float)combo_x_pos, top_text_y_pos, MENU_FONT_SIZE, GRAY);
        DrawText(mines_info_txt.c_str(), (float)combo_x_pos, top_text_y_pos + HEADER_HEIGHT, MENU_FONT_SIZE, GRAY);

        // Draw Start button.
        int button_y_pos = top_text_y_pos + START_BUTTON_OFFSET_Y;
        if (GuiButton(Rectangle{(float)combo_x_pos, (float)button_y_pos, (float)COMBOBOX_WIDTH, (float)COMBOBOX_HEIGHT}, "Start") && !show_info)
        {
            Vector2 win_size = GetWindowSize(&settings);
            SetWindowSize(win_size.x, win_size.y);
            field = new Field(Vector2{0.0f, HEADER_HEIGHT}, settings);
            timer_start = std::chrono::steady_clock::now();
            RecalculateUI();
            state = State::Play;
            if (sound_on)
                PlaySound(click_sound);
        }
    }

    /**
     * @brief Returns an icon code based on the current game state.
     *
     * @return int Icon code according to the current game state.
     */
    int Game::GetButtonIcon()
    {
        if (field->GameOver())
        {
            return ICON_DEMON;
        }
        else if (field->WinningConditionMet())
        {
            return ICON_STAR;
        }

        return ICON_HEART;
    }

    /**
     * @brief Recalculates the position of some ui elements after the windows size has changed according to game settings.
     *
     */
    void Game::RecalculateUI()
    {
        button_position_x = field->TileSize() * field->Columns() / 2 - BUTTON_SIZE / 2;
        timer->SetPosition(Vector2{(float)button_position_x + SQUARE_SIZE + DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y});
        mine_counter->SetPosition(Vector2{(float)button_position_x - DISPLAY_WIDTH - DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y});
    }
}
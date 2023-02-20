#include "digital_display.h"

namespace minis
{
    DigitalDisplay::DigitalDisplay(Rectangle bounds) : background(bounds)
    {
        this->inner_background = Rectangle{
            bounds.x + inner_offset, bounds.y + inner_offset,
            bounds.width - inner_offset * 2, bounds.height - inner_offset * 2};
    }

    void DigitalDisplay::Update() {}

    void DigitalDisplay::Draw(char *text)
    {
        DrawRectangle(background.x, background.y, background.width, background.height, DARKGRAY);
        DrawRectangle(inner_background.x, inner_background.y, inner_background.width, inner_background.height, BLACK);
        DrawText(text, inner_background.x + 2, inner_background.y + 2, 35, GREEN);
    }

    void DigitalDisplay::SetPosition(Vector2 position)
    {
        background.x = position.x;
        background.y = position.y;
        this->inner_background = Rectangle{
            background.x + inner_offset, background.y + inner_offset,
            background.width - inner_offset * 2, background.height - inner_offset * 2};
    }

}
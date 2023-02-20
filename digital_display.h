#ifndef DIGITIAL_DISPLAY_H
#define DIGITIAL_DISPLAY_H

#include "raylib.h"

namespace minis
{
    class DigitalDisplay
    {
    public:
        DigitalDisplay(Rectangle bounds);
        void Draw(char *text);
        void Update();
        void SetPosition(Vector2 position);

    private:
        Rectangle background;
        Rectangle inner_background;
        int inner_offset = 2;
    };
}

#endif
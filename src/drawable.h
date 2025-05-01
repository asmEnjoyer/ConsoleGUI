#pragma once
#include <sstream>

class Drawable
{
public:
    struct Rect
    {
        int x;
        int y;
        int width;
        int height;
    };

public:
    virtual void draw(std::stringstream &, Rect &rect) = 0;
};

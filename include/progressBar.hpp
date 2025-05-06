#pragma once
#include "drawable.hpp"

class ProgressBar : public Drawable
{
private:
    int *_pValue;

public:
    ProgressBar(int &value);
    void draw(std::stringstream &_buffer, Drawable::Rect &rect);
};

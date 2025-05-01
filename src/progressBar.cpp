#include "progressBar.h"

ProgressBar::ProgressBar(int &value)
{
    _pValue = &value;
}

void ProgressBar::draw(std::stringstream &_buffer, Drawable::Rect &rect)
{
    _buffer << "\e[38;5;2m";
    _buffer << "\e[" << rect.height + rect.y - 1 << ";" << rect.x << "H[";
    _buffer << "\e[" << rect.width + rect.x - 1 << "G]";
    _buffer << "\e[" << rect.height + rect.y - 1 << ";" << rect.x << "H[";
    int barLength = (rect.width - 2) * (std::min(*_pValue, 100));
    barLength /= 100;
    for (int i = 0; i < barLength; i++)
        _buffer << "=";

    _buffer << "\e[48;5;7m";
    if (*_pValue < 50)
        _buffer << "\e[38;5;1m";
    else if (*_pValue < 90)
        _buffer << "\e[38;5;3m";
    else
        _buffer << "\e[38;5;10m";

    int centerPoint = rect.width;
    if (*_pValue > 9)
        centerPoint -= 2;
    if (std::min(*_pValue, 100) == 100)
        centerPoint -= 2;
    centerPoint /= 2;
    _buffer << "\e[" << centerPoint + rect.x << "G";
    if (std::min(*_pValue, 100) == 100)
        _buffer << "DONE!";
    else
        _buffer << *_pValue << "%";

    _buffer << "\e[0m";
    rect.height--;
}
#include "variableList.h"

void VariableList::draw(std::stringstream &_buffer, Drawable::Rect &rect)
{
    std::stringstream list;
    std::string value;
    int maxWidth = 0, lastMaxWidth;
    int height = 0;

    for (auto a : _integers)
    {
        lastMaxWidth = maxWidth;
        if (height + 2 > rect.height)
            break;
        if (a.first.length() > maxWidth)
            maxWidth = a.first.length();
        value = std::to_string(*a.second);
        if (value.length() > maxWidth)
            maxWidth = value.length();
        if (maxWidth > rect.width)
        {
            maxWidth = lastMaxWidth;
            continue;
        }
        list << "#" << a.first << "\e[1B\e[" << a.first.length() + 1 << "D";
        list << "#" << value << "\e[1B\e[" << value.length() + 1 << "D";
        height += 2;
    }
    for (; height < rect.height; height++)
    {
        list << "#\e[1B\e[1D";
    }
    _buffer << "\e[" << rect.y << ";" << rect.x + rect.width - maxWidth - 1 << "H";
    _buffer << list.str();
    rect.width -= maxWidth + 1;
}

void VariableList::add(const std::string &name, int &variable)
{
    _integers.insert({name, &variable});
}

void VariableList::add(const std::string &name, float &variable)
{
    _floats.insert({name, &variable});
}

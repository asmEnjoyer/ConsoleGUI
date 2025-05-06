#pragma once
#include "drawable.hpp"

#include <unordered_map>
#include <string>

class VariableList : public Drawable
{
private:
    std::unordered_map<std::string, int *> _integers;
    std::unordered_map<std::string, float *> _floats;

public:
    void draw(std::stringstream &_buffer, Drawable::Rect &rect);
    void add(const std::string &name, int &variable);
    void add(const std::string &name, float &variable);
};

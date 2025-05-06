#pragma once
#include "drawable.hpp"

#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <thread>
#include <mutex>

#include <stdio.h>
#include <ctype.h>

#include <sstream>
#include <vector>

#define CTRL_KEY(k) ((k) & 0x1f)

inline class console
{
private:
    struct Drawable::Rect _screen;

    struct termios _orig_termios;
    std::stringstream _buffer;
    bool _shouldClose = false;

    std::mutex _readMutex;
    // std::queue<std::string> logs;

public:
    std::vector<Drawable *>
        _drawable;
    console();
    ~console();
    void editorDrawRows();
    void clear();
    void enableRawMode();
    void disableRawMode();
    void printBuffer();
    void shouldClose();
    void log(std::string s);

private:
    void mainLoop();
    void inputLoop();
    void getTermios();
    void save();
    void load();
    void close();
    void getCursorPos(int &line, int &column);
} Console;

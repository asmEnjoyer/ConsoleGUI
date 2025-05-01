#pragma once

#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <thread>

#include <stdio.h>
#include <ctype.h>

#include <sstream>

#define CTRL_KEY(k) ((k) & 0x1f)

inline class console
{
private:
    struct settings
    {
        bool progressBarVisible = false;
        int *pProgressBarValue;
    } _settings;

    struct cursor
    {
        int line;
        int column;
    } _cursor;

    struct window
    {
        int width;
        int height;
    } _window;

    struct termios _orig_termios;
    std::stringstream _buffer;
    bool _shouldClose = false;

public:
    console();
    ~console();
    int getWindowSize(int *rows, int *cols);
    void editorDrawRows();
    void clear();
    void enableRawMode();
    void disableRawMode();
    void printBuffer();
    void shouldClose();
    void displayProgress(int *progress);
    void hideProgress();

private:
    void mainLoop();
    void getTermios();
    void save();
    void load();
    void close();
    void getCursorPos(int &line, int &column);
    void drawProgressBar();
} Console;

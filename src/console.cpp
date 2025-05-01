#include "console.hpp"

void console::mainLoop()
{
    getTermios();
    enableRawMode();

    save();
    clear();
    editorDrawRows();
    printBuffer();
    while (!_shouldClose)
    {
        char c = '\0';
        read(STDIN_FILENO, &c, 1);
        if (c == CTRL_KEY('q'))
            shouldClose();
        clear();
        editorDrawRows();
        if (_settings.progressBarVisible)
            drawProgressBar();
        printBuffer();
    }
    close();
}

void console::getTermios()
{
    tcgetattr(STDIN_FILENO, &_orig_termios);
}

void console::save()
{
    _buffer << "\e[?69l";
    _buffer << "\e[s";
    _buffer << "\e[?47h";
    _buffer << "\e[?25l";
    _buffer << "\e[=14h";
}

void console::load()
{
    _buffer << "\e[?25h";
    _buffer << "\e[?47l";
    _buffer << "\e[H";
    _buffer << "\e[u\r";
}

void console::close()
{
    disableRawMode();
    printBuffer();
    load();
    printBuffer();
    exit(0);
}

void console::getCursorPos(int &line, int &column)
{
    line = 0;
    column = 0;
    write(STDOUT_FILENO, "\e[6n", 4);
    char ch;
    bool isLine = true;
    while (ch != 'R')
    {
        read(0, &ch, 1);
        if (ch == ';')
            isLine = false;
        if (!isdigit(ch))
            continue;
        if (isLine)
        {
            line *= 10;
            line += (ch & 0xf);
        }
        else
        {
            column *= 10;
            column += (ch & 0xf);
        }
    }
}

void console::drawProgressBar()
{
    _buffer << "\e[" << _window.height - 1 << ";" << 2 << "H[";
    int barLength = (_window.width - 3) * (*_settings.pProgressBarValue);
    barLength /= 100;
    for (int i = 0; i < barLength; i++)
        _buffer << "=";
    int centerPoint = _window.width;
    if (*_settings.pProgressBarValue > 9)
        centerPoint--;
    if (*_settings.pProgressBarValue == 100)
        centerPoint--;
    centerPoint /= 2;
    _buffer << "\e[" << centerPoint << "G";
    if (*_settings.pProgressBarValue == 100)
        _buffer << "DONE!";
    else
        _buffer << *_settings.pProgressBarValue << "%";

    _buffer << "\e[" << _window.width - 1 << "G]";
}

void console::printBuffer()
{
    write(STDOUT_FILENO, _buffer.str().c_str(), _buffer.str().length());
    _buffer.str("");
}

void console::shouldClose()
{
    _shouldClose = true;
}

void console::displayProgress(int *progress)
{
    _settings.progressBarVisible = true;
    _settings.pProgressBarValue = progress;
}

void console::hideProgress()
{
    _settings.progressBarVisible = false;
}

console::console()
{
    _cursor.line = 0;
    _cursor.column = 0;
    std::thread thread(&console::mainLoop, this);
    thread.detach();
}

int console::getWindowSize(int *rows, int *cols)
{
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
        return -1;
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
}

void console::editorDrawRows()
{
    write(STDOUT_FILENO, "\e[999C\e[999B", 12);
    getCursorPos(_window.height, _window.width);
    _buffer << "\e[H";
    for (int y = 0; y < _window.height; y++)
    {
        _buffer << "#";
        if (y == 0 || y == _window.height - 1)
            for (int x = 1; x < _window.width; x++)
                _buffer << "#";
        else
            _buffer << "\e[" << _window.width << "G#";
        if (y < _window.height - 1)
            _buffer
                << "\r\n";
    }
}

void console::clear()
{
    _buffer << "\e[H";
    _buffer << "\e[2J";
}

void console::enableRawMode()
{
    struct termios raw = _orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void console::disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &_orig_termios);
}

console::~console()
{
    close();
}

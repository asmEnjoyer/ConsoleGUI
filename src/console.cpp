#include "console.hpp"

void console::mainLoop()
{

    while (!_shouldClose)
    {
        clear();
        editorDrawRows();
        for (auto d : _drawable)
        {
            d->draw(_buffer, _screen);
        }
        printBuffer();
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(10ms);
        }
    }
    close();
}

void console::inputLoop()
{
    char c = '\0';
    while (!_shouldClose)
    {
        _readMutex.lock();
        read(STDIN_FILENO, &c, 1);
        _readMutex.unlock();
        if (c == CTRL_KEY('q'))
            shouldClose();
    }
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
    // exit(0);
}

void console::getCursorPos(int &line, int &column)
{
    _readMutex.lock();
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
    _readMutex.unlock();
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

void console::log(std::string s)
{
    // logs.push(s);
}

console::console()
{
    getTermios();
    enableRawMode();

    save();
    std::thread mainThread(&console::mainLoop, this);
    std::thread inputThread(&console::inputLoop, this);
    mainThread.detach();
    inputThread.detach();
}

void console::editorDrawRows()
{
    write(STDOUT_FILENO, "\e[999C\e[999B", 12);
    getCursorPos(_screen.height, _screen.width);
    _screen.x = 1;
    _screen.y = 1;
    _buffer << "\e[H";
    for (int y = 0; y < _screen.height; y++)
    {
        _buffer << "#";
        if (y == 0 || y == _screen.height - 1)
            for (int x = 1; x < _screen.width; x++)
                _buffer << "#";
        else
            _buffer << "\e[" << _screen.width << "G#";
        if (y < _screen.height - 1)
            _buffer
                << "\r\n";
    }
    _screen.x += 1;
    _screen.y += 1;
    _screen.width -= 2;
    _screen.height -= 2;
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
    raw.c_cc[VTIME] = 0;
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

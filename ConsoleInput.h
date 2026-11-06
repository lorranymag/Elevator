#ifndef CONSOLE_INPUT_H
#define CONSOLE_INPUT_H

#include <string>
#ifndef _WIN32
#include <termios.h>
#endif


class ConsoleInput {
public:
    enum class Result { Waiting, Line, End };
    ConsoleInput();
    ~ConsoleInput();
    void setHidden(bool hide);
    Result poll(std::string& line);
    const std::string& getPendingText() const;
    bool isInteractive() const;

private:
    std::string pending;
    bool interactive;
    bool hidden = false;
#ifndef _WIN32
    termios visibleSettings{};
#endif
    bool skipLineFeed;
};

#endif

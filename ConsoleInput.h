#ifndef CONSOLE_INPUT_H
#define CONSOLE_INPUT_H

#include <string>

// Small platform adapter 
class ConsoleInput {
public:
    enum class Result { Waiting, Line, End };
    ConsoleInput();
    Result poll(std::string& line);
    const std::string& getPendingText() const;
    bool isInteractive() const;

private:
    std::string pending;
    bool interactive;
    bool skipLineFeed;
};

#endif

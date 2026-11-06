#include "ConsoleInput.h"
#include <cstdio>
#include <iostream>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <conio.h>
#include <io.h>
#else
#include <cerrno>
#include <sys/select.h>
#include <unistd.h>
#endif


/**
The elevators and the menu run in the same loop. 
A regular getline() would stop that loop until the user presses Enter,
so we put the platform specific keyboard checks in a separate class. 
If there’s no complete command yet, the loop keeps updating the elevators.
Likely out of scope for this assignment, but the code is written to be cross-platform and to handle both interactive and piped input.
**/


ConsoleInput::ConsoleInput() : interactive(false), skipLineFeed(false) {
#ifdef _WIN32
    interactive = _isatty(_fileno(stdin)) != 0;
#else
    interactive = isatty(STDIN_FILENO) != 0;
#endif
}

ConsoleInput::~ConsoleInput() { setHidden(false); }

void ConsoleInput::setHidden(bool hide) {
    if (!interactive || hide == hidden) { return; }
#ifndef _WIN32
    if (hide) {
        if (tcgetattr(STDIN_FILENO, &visibleSettings) != 0) { return; }
        termios settings = visibleSettings;
        settings.c_lflag &= ~ECHO;
        if (tcsetattr(STDIN_FILENO, TCSANOW, &settings) != 0) { return; }
    }
    else if (tcsetattr(STDIN_FILENO, TCSANOW, &visibleSettings) != 0) {
        return;
    }
#endif
    hidden = hide;
}

bool ConsoleInput::isInteractive() const { return interactive; }
const std::string& ConsoleInput::getPendingText() const { return pending; }

ConsoleInput::Result ConsoleInput::poll(std::string& line) {
    while (true) {
        char ch;
#ifdef _WIN32
        if (interactive) {
            if (!_kbhit()) { return Result::Waiting; }
            int key = _getch();
            if (key == 0 || key == 224) { _getch(); continue; } // Arrow/function key.
            if (key == 3 || key == 26) { return Result::End; } // Ctrl+C / Ctrl+Z.
            ch = static_cast<char>(key);
        }
        else {
            HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
            if (GetFileType(handle) == FILE_TYPE_PIPE) {
                DWORD available = 0;
                if (!PeekNamedPipe(handle, nullptr, 0, nullptr, &available, nullptr)) {
                    if (pending.empty()) { return Result::End; }
                    line = pending; pending.clear(); return Result::Line;
                }
                if (available == 0) { return Result::Waiting; }
            }
            if (_read(_fileno(stdin), &ch, 1) != 1) {
                if (pending.empty()) { return Result::End; }
                line = pending; pending.clear(); return Result::Line;
            }
        }
#else
        fd_set ready;
        FD_ZERO(&ready);
        FD_SET(STDIN_FILENO, &ready);
        timeval timeout = { 0, 0 };
        int count = select(STDIN_FILENO + 1, &ready, nullptr, nullptr, &timeout);
        if (count < 0) { return errno == EINTR ? Result::Waiting : Result::End; }
        if (count == 0) { return Result::Waiting; }
        if (read(STDIN_FILENO, &ch, 1) != 1) {
            if (pending.empty()) { return Result::End; }
            line = pending; pending.clear(); return Result::Line;
        }
#endif
        if (ch == '\n' && skipLineFeed) { skipLineFeed = false; continue; }
        skipLineFeed = false;
        if (ch == '\r' || ch == '\n') {
            skipLineFeed = ch == '\r';
#ifdef _WIN32
            if (interactive) { std::cout << '\n'; }
#else
            if (hidden) { std::cout << '\n'; }
#endif
            line = pending;
            pending.clear();
            return Result::Line;
        }
        if (ch == '\b' || ch == 127) {
            if (!pending.empty()) {
                pending.pop_back();
#ifdef _WIN32
                if (interactive && !hidden) { std::cout << "\b \b" << std::flush; }
#endif
            }
        }
        else if (ch >= ' ' || ch == '\t') {
            pending += ch;
#ifdef _WIN32
            if (interactive && !hidden) { std::cout << ch << std::flush; }
#endif
        }
    }
}

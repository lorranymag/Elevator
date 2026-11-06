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

/**
References:
https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/kbhit?view=msvc-170
https://gist.github.com/sunmeat/a875fdcc90ca3c63d3d1e1de8bac1921
https://man7.org/linux/man-pages/man2/select.2.html
**/




// constructor for ConsoleInput class, initializes variables to default values
ConsoleInput::ConsoleInput() : interactive(false), skipLineFeed(false) {
#ifdef _WIN32
    interactive = _isatty(_fileno(stdin)) != 0;
#else
    interactive = isatty(STDIN_FILENO) != 0;
#endif
}

// destructor for ConsoleInput class, restores terminal settings if necessary
ConsoleInput::~ConsoleInput() { setHidden(false); }


// helper function to set the terminal input to hidden or visible, which is used when entering a PIN
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


// helper function to check if the input is interactive (i.e., from a terminal) or not, which is used to determine how to handle input and output
bool ConsoleInput::isInteractive() const { return interactive; }

// helper function to get the current pending input text, which is used to display the input as the user types
const std::string& ConsoleInput::getPendingText() const { return pending; }



// helper function to poll for input, which is called in the main loop of the interface to check for user input without blocking elevator updates
ConsoleInput::Result ConsoleInput::poll(std::string& line) {
    while (true) {
        char ch;
#ifdef _WIN32
		// Windows-specific code to check for keyboard input without blocking
        if (interactive) {
            if (!_kbhit()) { return Result::Waiting; }
            int key = _getch();
            if (key == 0 || key == 224) { _getch(); continue; } // Arrow/function key.
            if (key == 3 || key == 26) { return Result::End; } // Ctrl+C / Ctrl+Z.
            ch = static_cast<char>(key);
        }
		// Non-interactive input (piped or redirected) is handled differently, using PeekNamedPipe to check for available input without blocking.
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
		// Non-Windows platforms use select() to check for available input without blocking, and read() to get the input character.
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

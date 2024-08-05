// SETTINGS ARE HERE

#define LINE_1         U"Hello World"
#define LINE_2         U"This is some rainbow text"
#define LINE_3         U"Теперь с поддержкой Unicode и Linux!"

#define SPACE          ' ' // Only one character!!! (Default is ' ', everything else slows down performance)
#define CYCLE_SPEED    950 // From 1 to 1000, default is 950
#define DIRECTION      1 // 1 = from left to right, 0 = from right to left
#define CHANGE_AFTER   2 // Change color after N character(s), default is 2

// END SETTINGS



#define ALTERNATE_BUF "\033[?1049h"
#define DEFAULT_BUF "\033[?1049l"
#define CLEAR "\033[2J\033[1;1H"
#define PREV_LINE "\033[F"
#define HIDE_CURSOR "\033[?25l"
#define SHOW_CURSOR "\033[?25h"
#define RESET_COLOR "\033[0m"
#define RED "\033[91m"
#define ORANGE "\033[33m"
#define YELLOW "\033[93m"
#define GREEN "\033[92m"
#define BLUE "\033[36m"
#define INDIGO "\033[94m"
#define VIOLET "\033[95m"

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <signal.h>
#include "lib/utf8.h"
#if defined(_WIN32)
#include <windows.h>
#elif defined(__unix__) || defined(__linux__)
#include <sys/ioctl.h>
#include <unistd.h>
#endif

std::string char_to_utf8(char32_t ch)
{
    std::u32string utf32ch {ch};
    std::string utf8ch;
    utf8::unchecked::utf32to8(utf32ch.begin(), utf32ch.end(), std::back_inserter(utf8ch));
    return utf8ch;
}

std::u32string centered(std::u32string str, int width, int max_len)
{
    std::u32string space_1((width - str.length()) / 2, SPACE);
    str.insert(0, space_1);
    int pad = (width - max_len) / 2 + max_len - str.length();
    if (pad > 0) {
        std::u32string space_2(pad, SPACE);
        str.append(space_2);
    }
    return str;
}

void printLine(std::u32string line, const char** colors, int frame, int direction = 0)
{
    int color;
    size_t utf8_chlen;
    for (int chara = 0; chara < line.length(); chara ++) {
        if (line[chara] != U' ') {
            if (CHANGE_AFTER > 0) {
                if (direction == 0) color = ((frame + chara) / CHANGE_AFTER) % 6;
                else color = ((frame + line.length() - 1 - chara) / CHANGE_AFTER) % 6;
            }
            else color = frame % 6;

            std::cout << colors[color] << char_to_utf8(line[chara]);
        }
        else std::cout << RESET_COLOR << " ";
    }
}

void getWindowSize(int &columns, int &rows)
{
    #if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    #elif defined(__unix__) || defined(__linux__)
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    columns = w.ws_col;
    rows = w.ws_row;
    #else
    columns = 120;
    rows = 30;
    #endif
}

void sigintHandler(int sig_num) 
{
    std::cout << RESET_COLOR << CLEAR << DEFAULT_BUF << SHOW_CURSOR;
    std::exit(0);
}

int main()
{
    #if defined(_WIN32)
    // Enable escape sequence processing in cmd.exe
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    // Change windows console encoding to UTF-8
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    #endif

    // Change CTRL+C behavior to exit gracefully
    signal(SIGINT, sigintHandler);
    // Enable alternate screen buffer and clear the screen
    std::cout << ALTERNATE_BUF << CLEAR;
    // Define color array
    const char* colors[6] = {RED, ORANGE, YELLOW, GREEN, INDIGO, VIOLET};
    // Get window width and height
    int w, h, w_prev = 0, h_prev = 0;
    // Hide the blinking cursor
    std::cout << HIDE_CURSOR;
    // Create lines array
    std::u32string lines[] {
        std::u32string(LINE_1),
        std::u32string(LINE_2),
        std::u32string(LINE_3)
    };
    std::u32string centered_lines[3];

    unsigned int frame = 0;

    while (true) {
        getWindowSize(w, h);
        if (w != w_prev) {
            try {
                int max_len = 0;
                for (int i = 0; i < 3; i++)
                    if (lines[i].length() > max_len) max_len = lines[i].length();
                for (int i = 0; i < 3; i++)
                    centered_lines[i] = centered(lines[i], w, max_len);
                std::cout << CLEAR;
                for (int row = 0; row < (h - 3) / 2; row++)
                    std::cout << "\n";
            }
            catch (std::length_error) {
                std::cout << RESET_COLOR << CLEAR << "Window too small" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                h_prev = 0;
                continue;
            }
            w_prev = w;
        }
        if (h != h_prev) {
            if (h <= 3) {
                std::cout << RESET_COLOR << CLEAR << "Window too small" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            // Move to the center (vertically)
            std::cout << CLEAR;
            for (int row = 0; row < (h - 3) / 2; row++)
                std::cout << "\n";
            h_prev = h;
        }

        for (std::u32string line : centered_lines) {
            printLine(line, colors, frame, int(DIRECTION));
            std::cout << "\n";
        }

        for (std::u32string line : centered_lines) std::cout << PREV_LINE;
        frame++;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 - int(CYCLE_SPEED)));
    }
}

// SETTINGS ARE HERE

#define LINE_1         "Hello World!"
#define LINE_2         "This is some rainbow text"
#define LINE_3         "I <3 C++"

#define SPACE          " " // Only one character!!! (Default is " ", everything else slows down performance)
#define CYCLE_SPEED    950 // From 1 to 1000, default is 950
#define DIRECTION      1 // 1 = from left to right, 0 = from right to left
#define CHANGE_AFTER   2 // Change color after N character(s), default is 2
#define DIMENSIONS     120, 30 // Default values for Windows CMD: 120, 30 (you can look in your shell settings)

// END SETTINGS



#include <iostream>
#include <string>
#include <windows.h> // Only for sleep command

using namespace std;

string centered(string str, int width) {
    string space_1((width - str.size()) / 2, SPACE[0]);
    str.insert(0, space_1);
    string space_2(width - str.size(), SPACE[0]);
    str.append(space_2);
    return str;
}

void print_line(string line, const string* colors, const string reset_color, int frame, int direction = 0) {
    int color;
    for (int chara = 0; chara < line.size(); chara++) {
        if (line[chara] != ' ') {
            if (CHANGE_AFTER > 0) {
                if (direction == 0) color = ((frame + chara) / CHANGE_AFTER) % 7;
                else color = ((frame + line.size() - 1 - chara) / CHANGE_AFTER) % 7;
            }
            else color = frame % 7;

            cout << colors[color] + line[chara];
        }
        else cout << reset_color + " ";
    }
}

int main()
{
    // Multi-language support:
    setlocale(LC_ALL, "");
    // Clear the console
    system("cls");
    // Initialize color support in Windows Command Prompt:
    system("color");
    // Sequence to reset color:
    const string reset_color = "\033[0m";
    // Colors: 0 = red, 1 = orange, 2 = yellow, 3 = green, 4 = blue, 5 = indigo, 6 = violet:
    const string colors[7] = { "\033[91m", "\033[33m", "\033[93m", "\033[92m", "\033[36m", "\033[94m", "\033[95m" };
    // Sequence to move to previous line:
    const string up = "\033[F";
    // Console window width and height:
    const unsigned short int dimensions[] = { DIMENSIONS };
    // Hide the blinking cursor:
    cout << "\033[?25l";
    // Move to the center (vertically):
    for (int row = 0; row < (dimensions[1] - 3) / 2; row++) cout << "\n";

    string lines[] = {
        centered(string(LINE_1), dimensions[0]),
        centered(string(LINE_2), dimensions[0]),
        centered(string(LINE_3), dimensions[0]),
    };

    unsigned long long int frame = 0;

    while (true) {
        for (string line : lines) {
            print_line(line, colors, reset_color, frame, int(DIRECTION));
            cout << "\n";
        }

        for (string line : lines) cout << up;
        frame++;
        Sleep(1000 - int(CYCLE_SPEED));
    }

    // The code below is never executed due to the infinite while loop.
    cout << reset_color << "\033[?25h"; // The last one unhides the cursor
    return 0;
}

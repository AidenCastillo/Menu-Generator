#include "menu.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <ctype.h>

// Displays the menu on the terminal and incharge of highlighting the selected item.
void display_menu(MenuItem* items, int selected) {
    // TIOCGWINSZ: Get window size
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        // Handle error if ioctl fails
        fprintf(stderr, "ioctl failed (%d): %s\n", errno, strerror(errno));
        return;
    }
    printf("\033[H"); // Move cursor to top-left
    // Creates buffer to hold menu content. Prints the entire screen at once instead of line by line to reduce flickering.
    char buffer[4096];
    snprintf(buffer, sizeof(buffer), "%*s\n", (int)(w.ws_col * 0.5), "Menu");
    snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "%*s\n", (int)(w.ws_col * 0.8), "Use Arrow Keys to navigate, Enter to select, or number keys to jump to an item");
    for (int i = 0; i < w.ws_col; i++) {
        snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "-");
    }
    for (int i = 0; i < 100; i++) {
        if (items[i].name == NULL) {
            break;
        }
        // centered in the terminal
        snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "%*s%d. %s%s\n", (int)(w.ws_col * 0.4), " ", i + 1, (i == selected) ? "> " : "  ", items[i].name);
    }
    printf("%s", buffer);
}
// Generate menu based on the provided MenuItem array pointer. Will handle user input and call appropriate function pointers in item structs. 
void generate_menu(MenuItem* items) {
    int selected = 0;
    printf("\033[?1049h"); // Enter alternate screen
    // User input handling
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    display_menu(items, selected);
    // Input loop
    while (1) {
        char ch = getchar();
        if (ch == '\033') { // Escape sequence
            // When user uses arrow keys.
            getchar(); // skip the [
            switch(getchar()) {
                case 'A': // Up arrow
                    if (selected > 0) selected--;
                    break;
                case 'B': // Down arrow
                    if (items[selected + 1].name != NULL) selected++;
                    break;
            }
        } else if (ch == '\n') { // Enter key
            // Execute the selected menu item
            printf("\033[?1049l"); // Exit alternate screen
            // To fix terminal state. If not done terminal wont display user input into command line.
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            items[selected].action(items[selected].action_arg);
            return;
        } else if (isdigit(ch)) {
            // If user presses a numbe key, execute the corresponding menu item if it exists.
            int num = ch - '0';
            if (items[num - 1].name != NULL) {
                printf("\033[?1049l"); // Exit alternate screen
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                items[num - 1].action(items[num - 1].action_arg);
                return;
            }
        }

        display_menu(items, selected);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\033[?1049l"); // Exit alternate screen
    return;
}

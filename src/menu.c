#include "menu.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


void generate_menu(MenuItem* items) {
    // 1. Enter alternate screen
    printf("\033[?1049h\033[H"); 
    // printf("Generating menu...\n");
    // TIOCGWINSZ: Get window size
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        // Handle error if ioctl fails
        fprintf(stderr, "ioctl failed (%d): %s\n", errno, strerror(errno));
        return EXIT_FAILURE;
    }

    for (int i = 0; i < w.ws_col; i++) {
        printf("-");
    }

    

    for (int i = 0; i < 100; i++) {
        if (items[i].name == NULL) {
            break;
        }
        printf("%*s: ID=%d, Name=%s\n", (int)(w.ws_col * 0.3), "Menu Item", items[i].id, items[i].name);
    }
    char choice[10];
    printf("Enter your choice: ");
    fgets(choice, sizeof(choice), stdin);
    int choice_id = atoi(choice);
    printf("\033[?1049l"); 
    items[choice_id - 1].action(items[choice_id - 1].action_arg);
    // 2. Exit alternate screen (restores previous content)
    return;
}

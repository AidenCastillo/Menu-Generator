#include "actions.h"
#include "menu.h"
#include "parser.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

void go_back_to_main_menu(UNUSED MenuItem* item) {
    // This function can be used as the action for the "Exit" item in submenus to return to the main menu.
    // printf("Returning to main menu...\n");

    generate_menu(main_menu);

    return;
}

// Example ACTION type function, lists files and folders in current directory.
int list_files_action(UNUSED MenuItem* item) {
    DIR* d;
    struct dirent* dir;
    d = opendir(".");
    if (d) {
        printf("Files in current directory:\n");
        while ((dir = readdir(d)) != NULL) {
            printf("  %s\n", dir->d_name);
        }
        closedir(d);
    } else {
        perror("opendir");
    }
    return 0;
}

// Function for menu items with ACTION type. Searches for the specified action arg and execuutes the corresponding function.
void action_function(MenuItem* item) {
    int return_code = 0;
    printf("Action executed with argument: %s\n", item->action_arg);

    if (strcmp(item->action_arg, "list_files") == 0) {
        return_code = list_files_action(item);
    } else if (strcmp(item->action_arg, "main_menu") == 0) {
        go_back_to_main_menu(item);
    } else if (strcmp(item->action_arg, "exit") == 0) {
        printf("Exiting program...\n");
        return;
    } else {
        printf("Unknown action argument: %s\n", item->action_arg);
    }
    printf("Action returned code: %d\n", return_code);
    return;
}

// Function for menu items with SHELL type. Executes the shell command specified in arg and prints the output.
void shell_command_function(MenuItem* item) {
    printf("Shell command executed with argument: %s\n", item->action_arg);
    int return_code = system(item->action_arg);
    printf("Shell command returned code: %d\n", return_code);
    return;
}

// Function for menu items with EXEC type. Executes the specified executable with the given argument and prints the output.
void executable_function(MenuItem* item) {
    printf("Executable run with argument: %s\n", item->action_arg);
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "./%s", item->action_arg);
    int return_code = system(cmd);
    printf("Executable returned code: %d\n", return_code);
    return;
}

void submenu_action(MenuItem* item) {
    // This function can be used to generate a submenu when a menu item with a submenu is selected.
    // just call generate_menu with the submenu items.
    // printf("Entering submenu...\n");
    if (item->submenu == NULL) {
        printf("No submenu items found.\n");
        return;
    }

    generate_menu(item->submenu);
    return;
}

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

    // parse for $VAR in cmd and replace with environment variable value.
    char* var_start = strstr(cmd, "$");

    while (var_start) {
        char* var_end = var_start + 1;
        // stops when reaches a non-alphanumeric character like space or end of string.
        while (*var_end && (isalnum(*var_end) || *var_end == '_')) {
            var_end++;
        }
        char var_name[256];
        strncpy(var_name, var_start + 1, var_end - var_start - 1);
        var_name[var_end - var_start - 1] = '\0';
        char* var_value = getenv(var_name);
        if (var_value) {
            char new_cmd[256];
            snprintf(new_cmd, sizeof(new_cmd), "%.*s%s%s", (int)(var_start - cmd), cmd, var_value, var_end);
            strncpy(cmd, new_cmd, sizeof(cmd));
        } else {
            printf("Environment variable '%s' not found. Leaving as is.\n", var_name);
        }
        var_start = strstr(var_start + 1, "$");
    }
    printf("Final command to execute: %s\n", cmd);
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

void toggle_boolean_value(MenuItem* item) {
    // uses system() to toggle the environment variable specified in item->action_arg between "1" and "0".
    char* var_name = item->action_arg;
    char* current_value = getenv(var_name);
    if (current_value == NULL) {
        printf("Environment variable '%s' not found. Setting to '1'.\n", var_name);
        setenv(var_name, "1", 1);
    } else if (strcmp(current_value, "0") == 0) {
        printf("Toggling '%s' from '0' to '1'.\n", var_name);
        setenv(var_name, "1", 1);
    } else {
        printf("Toggling '%s' from '%s' to '0'.\n", var_name, current_value);
        setenv(var_name, "0", 1);
    }
    printf("Current value of '%s' is now: %s\n", var_name, getenv(var_name));
    generate_menu(main_menu);
    return;
}

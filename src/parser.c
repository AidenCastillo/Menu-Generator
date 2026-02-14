#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "menu.h"
#include "actions.h"

MenuItem* main_menu; // Global pointer to main menu for easy access in action functions.

// Remove leading whitespace from a string
char* ltrim(char* s) {
    while (*s == ' ' || *s == '\t') s++;
    return s;
}

// Seperates a line from template file into its components. Returns MenuItem struct.
MenuItem parseItem(char* line) {
    MenuItem item = {0};
    char type[16];
    char name[256];
    char arg[256];
    int id;

    // Parse the line to extract the item type, name, and argument
    sscanf(line, "ITEM %d \"%[^\"]\" %s \"%[^\"]\"", &id, name, type, arg);
    
    item.id = id;
    item.name = malloc(strlen(name) + 1);
    item.action_arg = malloc(strlen(arg) + 1);
    if (item.name == NULL || item.action_arg == NULL) {
        perror("Failed to allocate memory for menu item");
        exit(1);
    }
    strcpy(item.name, name);
    strcpy(item.action_arg, arg);

    if (strcmp(type, "ACTION") == 0) {
        item.action = action_function;
    } else if (strcmp(type, "SHELL") == 0) {
        item.action = shell_command_function;
    } else if (strcmp(type, "EXEC") == 0) {
        item.action = executable_function;
    } else {
        item.action = NULL;
    }

    if (item.action != NULL) {
        // printf("Parsed item: ID=%d, Name=%s, Type=%s, Arg=%s\n", item.id, item.name, type, item.action_arg);
    } else {
        // printf("Parsed item with unknown type: ID=%d, Name=%s, Type=%s, Arg=%s\n", item.id, item.name, type, item.action_arg);
    }
    
    return item;
}

// Parse will turn the menu-template.cfg into internal data structures. The expected format of the file is:
// `ITEM 1 "List Files" ACTION list_files`
// `ITEM 2 "Show Date" SHELL "date"`
// `ITEM 3 "Check Disk Usage" SHELL "df -h"`
// `ITEM 4 "Run Analysis" EXEC run_analysis`
// `ITEM 5 "Exit" ACTION exit`
// MENU 6 "Utilities" {
//     ITEM 1 "System Info" SHELL "uname -a"
//     ITEM 2 "Memory Usage" SHELL "free -h"
//     ITEM 3 "Network Status" SHELL "ifconfig"
// }
MenuItem* parse_menu_template(char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("fopen");
        return NULL;
    }

    MenuItem* items = calloc(100, sizeof(MenuItem));
    int count = 0;

    char line[256];
    MenuItem* current_menu = NULL;

    while (fgets(line, sizeof(line), file)) {

        char* trimmed_line = ltrim(line);
    
        if (strncmp(trimmed_line, "ITEM", 4) == 0) {
            MenuItem item = parseItem(trimmed_line);

            if (current_menu) {
                current_menu->submenu[current_menu->submenu_count++] = item;
                // printf("Added '%s' to submenu '%s'\n", item.name, current_menu->name);
            } else {
                items[count++] = item;
                // printf("Added '%s' to main menu\n", item.name);
            }
        }

        else if (strncmp(trimmed_line, "MENU", 4) == 0) {
            MenuItem menu = {0};
            char name[256];

            sscanf(trimmed_line, "MENU %d \"%[^\"]\"", &menu.id, name);
            menu.name = malloc(strlen(name) + 1);
            strcpy(menu.name, name);
            menu.action = submenu_action;
            menu.submenu = malloc(10 * sizeof(MenuItem));
            if (menu.submenu == NULL) {
                perror("Failed to allocate memory for submenu");
                exit(1);
            }
            menu.submenu_count = 0;

            items[count] = menu;
            current_menu = &items[count];
            current_menu->is_submenu = 1;
            // printf("Created submenu '%s'\n", current_menu->name);
            count++;

        }

        else if (strncmp(trimmed_line, "}", 1) == 0) {
            // printf("Closed submenu\n");
            // add exit item to submenu with ACTION "Exit" and action function that just returns to main menu.
            if (current_menu) {
                MenuItem exit_item = {0};
                exit_item.id = current_menu->submenu_count + 1;
                exit_item.name = malloc(strlen("return to main menu") + 1);
                strcpy(exit_item.name, "Return to Main Menu");
                exit_item.action = action_function; // Just call the function to go back to main menu.
                exit_item.action_arg = malloc(strlen("main_menu") + 1);
                strcpy(exit_item.action_arg, "main_menu");  
                current_menu->submenu[current_menu->submenu_count++] = exit_item;
                // printf("Added 'Exit' to submenu '%s'\n", current_menu->name);
            }
            current_menu = NULL;
        }
    }

    fclose(file);
    return items;
}

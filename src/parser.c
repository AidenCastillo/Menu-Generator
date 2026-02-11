#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "menu.h"

// Function for menu items with ACTION type
void action_function(char* arg) {
    
    printf("Action executed with argument: %s\n", arg);
}

// Function for menu items with SHELL type. Executes the shell command specified in arg and prints the output.
void shell_command_function(char* arg) {
    printf("Shell command executed with argument: %s\n", arg);
    int return_code = system(arg);
    printf("Shell command returned code: %d\n", return_code);
}

// Function for menu items with EXEC type. Executes the specified executable with the given argument and prints the output.
void executable_function(char* arg) {
    printf("Executable run with argument: %s\n", arg);
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "./%s", arg);
    int return_code = system(cmd);
    printf("Executable returned code: %d\n", return_code);
}

// Parse will turn the menu-template.cfg into internal data structures. The expected format of the file is:
// `ITEM 1 "List Files" ACTION list_files`
// `ITEM 2 "Show Date" SHELL "date"`
// `ITEM 3 "Check Disk Usage" SHELL "df -h"`
// `ITEM 4 "Run Analysis" EXEC run_analysis`
// `ITEM 5 "Exit" ACTION exit`
MenuItem* parse_menu_template(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open menu template file");
        return NULL;
    }
    MenuItem* items = malloc(100 * sizeof(MenuItem));
    if (items == NULL) {
        perror("Failed to allocate memory for menu items");
        fclose(file);
        return NULL;
    }
    for (int j = 0; j < 100; j++) {
        items[j].name = NULL;
        items[j].action = NULL;
        items[j].action_arg = NULL;
    }
    int count = 0;
    char line[256];
    char temp_name[256];
    // Reads file line by line and creates MenuItem structs based on the content.
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "ITEM", 4) == 0) {
            MenuItem item;
            char action_name[50];
            char action_type[20];
            sscanf(line, "ITEM %d \"%[^\"]\" %s \"%[^\"]\"", &item.id, temp_name, action_type, action_name);
            item.name = malloc(strlen(temp_name) + 1);
            item.action_arg = malloc(strlen(action_name) + 1);
            if (item.name == NULL || item.action_arg == NULL) {
                perror("Failed to allocate memory for item name or action argument");
                for (int k = 0; k < count; k++) {
                    free(items[k].name);
                    free(items[k].action_arg);
                }
                free(items);
                fclose(file);
                return NULL;
            }
            strcpy(item.name, temp_name);
            strcpy(item.action_arg, action_name);
            
            // Assign the function pointer based on the action type (ACTION, SHELL, EXEC)
            if (action_type[0] == 'A') {
                item.action = action_function;
            } else if (action_type[0] == 'S') {
                item.action = shell_command_function;
            } else if (action_type[0] == 'E') {
                item.action = executable_function;
            } else {
                item.action = NULL;
            }
            // Add item to the list
            items[count++] = item;
        }
    }
    
    fclose(file);
    return items;
}

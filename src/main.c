#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "menu.h"


int main() {
    main_menu = parse_menu_template("menu-template.cfg");
    if (main_menu == NULL) {
        printf("Failed to parse menu template.\n");
        return 1;
    }
        // list each submenu in 4th menu item for testing
    // MenuItem* list = &main_menu[4];
    // printf("Submenu items for menu item 4:\n");
    // for (int i = 0; i < list->submenu_count; i++) {
    //     printf("  %d. %s (action_arg: %s)\n", i + 1, list->submenu[i].name, list->submenu[i].action_arg);
    // }
    // return 0;
    // generate_menu(items[4].submenu);
    // for (int i = 0; i < 100; i++) {
    //     if (items[i].name == NULL) {
    //         break;
    //     }
    //     printf("Menu Item %d: %s (action_arg: %s)\n", items[i].id, items[i].name, items[i].action_arg);
    // }
    generate_menu(main_menu);
    
    // Tear down
    for (int i = 0; i < 100; i++) {
        if (main_menu[i].name == NULL) {
            break;
        }
        if (main_menu[i].submenu_count > 0 && main_menu[i].submenu != NULL) {
            for (int j = 0; j < main_menu[i].submenu_count; j++) {
                free(main_menu[i].submenu[j].name);
                free(main_menu[i].submenu[j].action_arg);
            }
            free(main_menu[i].submenu);
        }
        free(main_menu[i].name);
        if (main_menu[i].action_arg != NULL) {
            free(main_menu[i].action_arg);
        }
    }
    free(main_menu);
    // printf("properly freed memory and exiting program.\n");
    return 0;
}

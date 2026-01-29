#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "menu.h"


int main() {
    MenuItem* items = parse_menu_template("menu-template.cfg");
    if (items == NULL) {
        printf("Failed to parse menu template.\n");
        return 1;
    }
    
    generate_menu(items);
    
    // Tear down
    for (int i = 0; i < 100; i++) {
        if (items[i].name == NULL) {
            break;
        }
        free(items[i].name);
        free(items[i].action_arg);
    }
    free(items);
    return 0;
}

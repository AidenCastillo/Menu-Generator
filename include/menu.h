#ifndef MENU_H
#define MENU_H

typedef struct MenuItem {
    int id;
    char* name;
    void (*action)(struct MenuItem*); // Function pointer for the action to execute when this item is selected
    char* action_arg;
    struct MenuItem* submenu; // Pointer to submenu items (if this item is a menu)
    int submenu_count; // Number of items in the submenu
    int is_submenu; // Flag to indicate if this item is a submenu (1 for submenu, 0 for regular item)
} MenuItem;

void generate_menu(MenuItem* items);

// pointer to main menu


#endif // MENU_H

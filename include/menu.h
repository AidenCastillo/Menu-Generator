#ifndef MENU_H
#define MENU_H

typedef struct MenuItem {
    int id;
    char* name;
    void (*action)(char*);
    char* action_arg;
} MenuItem;

void generate_menu(MenuItem* items);

#endif // MENU_H

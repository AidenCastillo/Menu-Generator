#ifndef PARSER_H
#define PARSER_H

#include <string.h>
#include "menu.h"


void submenu_action(MenuItem* submenu_items);

MenuItem* parse_menu_template(char* filename);

extern MenuItem* main_menu;

#endif // PARSER_H

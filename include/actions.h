#ifndef ACTIONS_H
#define ACTIONS_H

#include "menu.h"


void go_back_to_main_menu(struct MenuItem* item);
void action_function(struct MenuItem* item);    
void shell_command_function(struct MenuItem* item);
void executable_function(struct MenuItem* item);
void submenu_action(struct MenuItem* item);
void toggle_boolean_value(struct MenuItem* item);

#endif // ACTIONS_H

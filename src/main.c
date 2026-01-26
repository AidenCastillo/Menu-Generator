#include <stdio.h>
#include <dirent.h>
#include <string.h>

// Search for menu-template.cfg in the home directory or current directory
void search_config_file(const char* filename) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, filename) == 0) {
            printf("Found %s in current directory.\n", filename);
            closedir(dir);
            return;
        }
    }
    closedir(dir);
    printf("%s not found in current directory.\n", filename);
}


int main() {
    printf("Searching for configuration file...\n");
    search_config_file("menu-template.cfg");

    

    return 0;
}

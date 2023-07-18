#include <stdio.h>
#include "../ini.c"

int main() {
    FILE* file;
    ini_t ini;
    char filename[] = "test1.ini";
    int result;

    ini_init(&ini);

    if(!(file = fopen(filename, "r"))) {
        fprintf(stderr, "Failed to open the file %s\n", filename);
    }

    if((result = ini_parse(file, &ini)) != 0) {
        fprintf(stderr, "Failed to read the file %s\n", filename);
    }
    
    return 0;
}
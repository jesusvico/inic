# C .INI parser

## Usage

### Read .ini files

```c
#include <stdio.h>
#include "../ini.c"

int main() {
    FILE* file;
    char filename[] = "example.ini";
    ini_t ini;
    int result;
    char* value;
    
    // Initialize ini 
    ini_init(&ini);

    // Get the file descriptor
    if(!(file = fopen(filename, "r"))) {
        fprintf(stderr, "Failed to open the file %s\n", filename);
    }

    // Parse the .ini file
    if((result = ini_parse(file, &ini)) != 0) {
        fprintf(stderr, "Failed to parse the file %s\n", filename);
        return result;
    }

    // Get a value from a section and a name
    value = ini_get(&ini, "Section", "name");
    printf("%s", value);
    
    return 0;
}
```

### Write .ini file

```c
#include <stdio.h>
#include "../ini.c"

int main() {
    FILE* file;
    char filename[] = "example.ini";
    ini_t ini;
    int result;
    
    // Initialize ini 
    ini_init(&ini);

    // Get the file descriptor
    if(!(file = fopen(filename, "w"))) {
        fprintf(stderr, "Failed to open the file %s\n", filename);
    }

    // Add values to ini
    ini_add(&ini, "Section", "name", "value");
    ini_add(&ini, "Section", "name2", "value2");
    ini_add(&ini, "Section2", "name", "value");

    // Write the ini file
    if((result = ini_write(file, &ini)) != 0) {
        fprintf(stderr, "Failed to write the file %s\n", filename);
    }
    
    return 0;
}
```

## Building and install

You can just include the `ini.c` and `ini.h` file in your project.

Another option is executing `make install` to install the header and library files into `/usr/local/{include,lib}`.

You can also install them in a specific path by executing `make install prefix=/a/file/path`.

## Testing

You can execute the test by running `make check`.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "ini.h"


void ini_init(ini_t* ini) {
    // The initial number of sections is 0
    ini->n_sections = 0;
    ini->sections = (ini_section_t*) malloc(0);
}


void ini_write(FILE* file, ini_t* ini) {
    // For every section
    for(size_t i = 0; i < ini->n_sections; i++) {
        fprintf(file, "[%s]\n", ini->sections[i].title);
        // For every value
        for(size_t j = 0; j < ini->sections[i].n_values; j++) {
            fprintf(file, "%s = %s", ini->sections[i].values[j].key, ini->sections[i].values[j].value);
            if(i + 1 < ini->n_sections || j + 1 < ini->sections[i].n_values)
                fprintf(file, "\n");
        }
        if(i + 1 < ini->n_sections)
                fprintf(file, "\n");
    }
}


ini_section_t* ini_get_section(ini_t* ini, char* section_title) {
    // Search if the section exists
    for(size_t i = 0; i < ini->n_sections; i++) {
        if(strcmp(ini->sections[i].title, section_title) == 0) {
            // If exists return it
            return &(ini->sections[i]);
        }
    }

    // If section does not exist create it
    ini_section_t section;
    section.n_values = 0;
    section.title = malloc(strlen(section_title) + 1);
    strcpy(section.title, section_title);
    
    ini->n_sections++;
    ini->sections = realloc(ini->sections, sizeof(ini_section_t) * ini->n_sections);
    ini->sections[ini->n_sections - 1] = section;
    ini->sections[ini->n_sections - 1].values = malloc(0); // Get space for the values

    // Return the new section, that is the last one
    return &(ini->sections[ini->n_sections - 1]);
}


ini_value_t* ini_get_value(ini_section_t* section, char* key_string) {
    // Search if the key exists
    for(size_t i = 0; i < section->n_values; i++) {
        if(strcmp(section->values[i].key, key_string) == 0) {
            // If exists return it
            return &(section->values[i]);
        }
    }

    // If value does not exist, create it
    ini_value_t value;
    value.key = malloc(strlen(key_string) + 1);
    strcpy(value.key, key_string);

    section->n_values++;
    section->values = realloc(section->values, sizeof(ini_value_t) * section->n_values);
    section->values[section->n_values - 1] = value;
    section->values[section->n_values - 1].value = malloc(0); // Get space for the value

    // Return the new value, that is the last one
    return &(section->values[section->n_values - 1]);
}


void ini_set_value(ini_value_t* value, char* value_string) {
    value->value = realloc(value->value, strlen(value_string) + 1); // Reassign memory for the value
    strcpy(value->value, value_string); // Copy the value
}


void ini_add(ini_t* ini, char* section_title, char* key_string, char* value_string) {
    // Get the section
    ini_section_t* section = ini_get_section(ini, section_title);

    // Get the value
    ini_value_t* value = ini_get_value(section, key_string);

    // Set the value
    ini_set_value(value, value_string);
}


char* ini_get(ini_t* ini, char* section_title, char* key_string) {
    // First search the section
    for(size_t i = 0; i < ini->n_sections; i++) {
        if(strcmp(section_title, ini->sections[i].title) == 0) {
            for(size_t j = 0; j < ini->sections[i].n_values; j++) {
                if(strcmp(key_string, ini->sections[i].values[j].key) == 0) {
                    return ini->sections[i].values[j].value;
                }
            }
        }
    }
    // If there is no match return NULL
    return NULL;
}


void trim(char * s) {
    char * p = s;
    int l = strlen(p);

    while(isspace(p[l - 1])) p[--l] = 0;
    while(* p && isspace(* p)) ++p, --l;

    memmove(s, p, l + 1);
}


int ini_parse(FILE* file, ini_t* ini) {
    char c;

    size_t chunk_size = 100;
    char* section = malloc(chunk_size);
    char* name = malloc(chunk_size);
    char* value = malloc(chunk_size);
    int pos = 0;

    bool reading_section = false;
    bool reading_name = false;
    bool reading_value = false;

    while((c = fgetc(file)) != EOF) {
        switch(c) {
            // Elements cannot span multiple lines
            case '\n':
                // It is not valid to have a newline while reading a section
                if(reading_section == true) return -1;
                // You cannot have a name without a value
                if(reading_name == true) return -1;
                // If we where reading a value the value finished
                if(reading_value) {
                    reading_value = false;
                    // Now that you have a new value execute the handler
                    trim(value);
                    ini_add(ini, section, name, value);
                }
                continue;
                break;
            case '[':
                // If we are not reading a name we are reading a section
                if(!reading_name) {
                    reading_section = true;
                    // Resize the allocated memory
                    section = realloc(section, chunk_size);
                    pos = 0;
                    continue;
                }
                break;
            case ']':
                // If we were reading a section we finish reading it 
                if(reading_section) reading_section = false;
                continue;
                break;
            case '=':
                // If we were reading a name start reading a value
                if(reading_name) {
                    reading_name = false;
                    trim(name);
                    reading_value = true;
                    // Resize the allocated memory
                    value = realloc(value, chunk_size);
                    pos = 0;
                    continue;
                }
                break;
            default:
                // We start reading a name
                if(!reading_section && !reading_name && !reading_value){
                    reading_name = true;
                    // Resize the allocated memory
                    name = realloc(name, chunk_size);
                    pos = 0;
                }
        }

        // Store section characters
        if(reading_section) {
            // Copy the character
            section[pos] = c;
            // Realloc if we do not have enough space
            if((pos + 1) % chunk_size == 0) section = realloc(section, (pos + 1) + chunk_size);
            // Add end of string
            section[pos + 1] = '\0';
            pos++;
        } else if (reading_name) {
            // Copy the character
            name[pos] = c;
            // Realloc if we do not have enough space
            if((pos + 1) % chunk_size == 0) name = realloc(name, (pos + 1) + chunk_size);
            // Add end of string
            name[pos + 1] = '\0';
            pos++;
        } else if (reading_value) {
            // Copy the character
            value[pos] = c;
            // Realloc if we do not have enough space
            if((pos + 1) % chunk_size == 0) value = realloc(value, (pos + 1) + chunk_size);
            // Add end of string
            value[pos + 1] = '\0';
            pos++;
        }
    }

    if(reading_value) {
        // Now that you have a new value execute the handler
        reading_value = false;
        trim(value);
        ini_add(ini, section, name, value);
    }

    return 0;
}
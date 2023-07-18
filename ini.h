typedef struct {
    char* key;
    char* value;
} ini_value_t;

typedef struct {
    char* title;
    size_t n_values;
    ini_value_t* values;
} ini_section_t;

typedef struct {
    size_t n_sections;
    ini_section_t* sections;
} ini_t;


/**
 * @brief Initialialized the struct ini_t.
 * 
 * @param ini Struct to be initialized. 
 */
void ini_init(ini_t* ini);


/**
 * @brief Writes an ini_t struct using a file descriptor.
 * 
 * @param file File descriptor used to write the .ini file.
 * @param ini Struct that is written.
 */
void ini_write(FILE* file, ini_t* ini);


/**
 * @brief Adds content to the ini_t struct.
 * 
 * @param ini Struct where the content is added.
 * @param section_title Section title.
 * @param key_string Key of the content.
 * @param value_string Value of the content.
 */
void ini_add(ini_t* ini, char* section_title, char* key_string, char* value_string);


/**
 * @brief Returns the value of the specified section and key.
 * 
 * @param ini Struct where the data is stored.
 * @param section_title Section title.
 * @param key_string Key of the content.
 * @return char* 
 */
char* ini_get(ini_t* ini, char* section_title, char* key_string);

/**
 * @brief 
 * 
 * @param file 
 * @param handler 
 * @param data 
 * @return int 
 */
int ini_parse(FILE* file, ini_t* ini);



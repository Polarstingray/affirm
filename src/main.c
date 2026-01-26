#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // for seeding rand
#include <unistd.h>
#include <sys/stat.h>


#define CACHE_SIZE 100
#define LIMIT 20


int readfile(const char *filepath, char *lines[], int max_lines) {
    FILE *file = fopen(filepath, "r");
    if (!file) return -1;

    size_t capacity = 256;
    size_t len = 0;
    char *buffer = malloc(capacity);
    if (!buffer) {
        fclose(file);
        return -1;
    }

    int count = 0;
    int c; 
    // Initialize all elements of lines to NULL
    for (int i = 0; i < max_lines; i++) {
        lines[i] = NULL;
    }

    // read in lines char by char
    while ((c = fgetc(file)) != EOF) {
        if (count >= max_lines) {
            len = 0;
            break;
        };
        if (len + 1 >= capacity) {
            capacity *= 2;
            char *tmp = realloc(buffer, capacity);
            if (!tmp) {
                free(buffer);
                fclose(file);
                return -1;
            }
            buffer = tmp;
        }
        
        if (c == '\n') {
            buffer[len] = '\0';
            if (len > 0) {
                lines[count++] = strdup(buffer);
            }
            len = 0;
        } else { // reading chars into buffer
            buffer[len++] = (char)c; 
        }
    }

    if (len > 0 && count < max_lines) {
        buffer[len] = '\0';
        lines[count++] = strdup(buffer);
    }

    free(buffer);
    fclose(file);
    return count;
}


void free_memory(char *lines[], int count) {
    for (int i = 0; i < count; i++) {
        free(lines[i]);
    }
}

void cache(const char* filepath, const char* line) {
    FILE *file= fopen(filepath, "a");
    if (file == NULL) {
        printf("Error openening cache file\n");
        return;
    }

    fprintf(file, "%s\n", line);
    fclose(file);
}

void clear_cache(const char* filepath) {
    printf("Clearing cache...\n");
    FILE *file = fopen(filepath, "w");
    fclose(file);
}

int cache_entry(char* tokens, char* cached_id) {
    if (cached_id == NULL) return -1; // Ensure cached_id is not NULL

    char* endptr = NULL;
    tokens = strtok(NULL, ":");
    if (tokens == NULL) return -1; // Ensure tokens is not NULL

    int cached_choice = strtol(tokens, &endptr, 10);
    return cached_choice;
}


int main(int argc, char* argv[]) {
    char *home = getenv("HOME");

    if (home == NULL) {
        printf("Failed to get HOME env var\n");
        return 1;
    }
    const char* AFFIRM_DATA_DIR = malloc(strlen(home) + strlen("/.local/share/affirm") + 1);
    if (AFFIRM_DATA_DIR == NULL) {
        printf("Failed to allocate memory for AFFIRM_DATA_DIR\n");
        return 1;
    }
    sprintf((char*)AFFIRM_DATA_DIR, "%s/.local/share/affirm", home);

    // argument check
    if (argc < 2) {
        printf("Usage: %s <ID>\n", argv[0]);
        return 1;
    }
    char* ID = argv[1];

    // construct file path
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s/%s.txt", AFFIRM_DATA_DIR, ID);
    // read file lines into array
    char *lines[2048];
    int count = readfile(filepath, lines, 2048);
    if (count == 0) {
        printf("nothing found in %s\n", filepath);
        return 1;
    }
    if (count < 0) {
        printf("Failed to read file\n");
        return 1;
    }

    // random selection
    srand(time(NULL)); // must seed rand for different results each run
    int choice = rand() % count; // random index from 0 to count-1
    
    // read cache file

    char cachedir[256];
    char cachefile[256];
    snprintf(cachedir, sizeof(cachedir), "%s/.cache/affirm", home);
    snprintf(cachefile, sizeof(cachefile), "%s/recent.txt", cachedir);
    // snprintf(filepath, sizeof(filepath), "%s/recent.txt", cachedir);
    if (access(cachedir, F_OK) == -1) {
        // cache directory does not exist, create it
        printf("Creating cache directory...\n");
        mkdir(cachedir, 0700);
    
    }

    char *cache_lines[1024] = {0}; // Initialize cache_lines to NULL
    int cache_count = readfile(cachefile, cache_lines, 1024);
    if (cache_count < 0) {
        FILE *file = fopen(cachefile, "w");
        if (file) fclose(file); // Ensure file is successfully created
        cache_count = 0;
    }

    int attempts = 0;
    int last_try = 0;
    int found_in_cache = 0;
    do {
        attempts++;
        if (attempts >= LIMIT) {
            clear_cache(cachefile);
            last_try = 1;
        }
        if (cache_count >= CACHE_SIZE) {
            clear_cache(cachefile);
            cache_count = 0;
        } else {
            found_in_cache = 0;
            for (int i = 0; i < cache_count; i++) {
                if (cache_lines[i] == NULL || strlen(cache_lines[i]) == 0) continue; // Skip NULL or empty entries

                char line[256];
                strncpy(line, cache_lines[i], sizeof(line) - 1); 
                line[sizeof(line) - 1] = '\0';

                // Validate line format before using strtok
                if (strchr(line, ':') == NULL) continue; // Skip if ':' is not found

                char* tokens = strtok(line, ":");
                char* cached_id = tokens;

                int cached_choice = cache_entry(tokens, cached_id);
                if (cached_choice < 0) continue; // invalid cache entry, skip
                if (strcmp(ID, cached_id) == 0 && choice == cached_choice) {
                    found_in_cache = 1;
                    choice = rand() % count;
                    break;
                }
            }
        }
    } while (found_in_cache != 0 && last_try == 0);
    char str[256]; 
    printf("%s\n", lines[choice]);
    snprintf(str, sizeof(str), "%s:%d", ID, choice);
    cache(cachefile, str);

    // free allocated memory
    free_memory(lines, count);
    free_memory(cache_lines, cache_count);

    return 0;
}


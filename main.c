#include <stdio.h>
#include <stdlib.h> // for general environment functions
#include <string.h>
#include <time.h>

#define CACHE_SIZE 4
#define LIMIT 20

int readfile(const char* filepath, char * lines[]) {
    /*
    Reads lines from a file into an array of strings.
    Returns number of lines read, or -1 on error.
    */

    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        return -1;
    }

    // using buffer to read from file
    // char *lines[1024];
    int count = 0;
    char buffer[256];   

    // read line from stream (file) into buffer
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0; // replace newlines
        lines[count] = strdup(buffer); // allocate heap MEMORY (essentially malloc)
        
        count++;
    }
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
    FILE *file = fopen(filepath, "w");
    fclose(file);
}

int cache_entry(char* tokens, char* cached_id) {
    // char* line = cache_lines[i];
    // char* tokens = strtok(line, ":");

    char* endptr = NULL;
    // cached_id = tokens;
    if (cached_id == NULL) return -1;
    tokens = strtok(NULL, ":");
    if (tokens == NULL) return -1;
    int cached_choice = strtol(tokens, &endptr, 10);
    return cached_choice;
}


int main(int argc, char* argv[]) {
    // argument check
    if (argc < 2) {
        printf("Usage: %s <ID>\n", argv[0]);
        return 1;
    }
    char* ID = argv[1];

    // construct file path
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "affirmations/%s.txt", ID);
    // read file lines into array
    char *lines[1024];
    int count = readfile(filepath, lines);
    if (count < 0) {
        printf("Failed to read file\n");
        return 1;
    }

    // random selection
    srand(time(NULL)); // must seed rand for different results each run
    int choice = rand() % count; // random index from 0 to count-1
    
    // read cache file
    snprintf(filepath, sizeof(filepath), "cache/recent.txt");
    char *cache_lines[1024];
    int cache_count = readfile(filepath, cache_lines);
    if (cache_count < 0) {
        FILE *file = fopen(filepath, "w");
        fclose(file);
        cache_count = 0;
    }

    int found_in_cache = 0;
    int attempts = 0;
    while (found_in_cache == 0 && attempts < LIMIT) {
        attempts++;
        if (cache_count >= CACHE_SIZE) { // clear cache
            clear_cache(filepath);
            cache_count = 0;
        } else { // check cache for recently used lines
            for (int i = 0; i < cache_count; i++) {
                found_in_cache = 0;
                char line[24]; // = cache_lines[i];
                strcpy(line, cache_lines[i]);
                char* tokens = strtok(line, ":");
                char* cached_id = tokens;

                int cached_choice = cache_entry(tokens, cached_id);
                if (strcmp(ID, cached_id) == 0 && choice == cached_choice) { // found in cache, pick new
                    found_in_cache = 1;
                    choice = rand() % count;
                    break;
                } 
            } // not found in cache, proceed
        }
    }

    char str[24]; 
    printf("%s\n", lines[choice]);
    snprintf(str, sizeof(str), "%s:%d", ID, choice);
    cache(filepath, str);

    // free allocated memory
    free_memory(lines, count);
    free_memory(cache_lines, cache_count);

    return 0;
}


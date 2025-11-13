#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VARS 100
#define MAX_LINE 256

typedef struct {
    char name[50];
    int value;
} Variable;

Variable variables[MAX_VARS];
int varCount = 0;

// Find variable index, -1 if not found
int findVar(const char *name) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(variables[i].name, name) == 0) return i;
    }
    return -1;
}

// Add or update variable
void setVar(const char *name, int value) {
    int idx = findVar(name);
    if (idx != -1) {
        variables[idx].value = value;
    } else {
        strcpy(variables[varCount].name, name);
        variables[varCount].value = value;
        varCount++;
    }
}

// Execute a CK file
void executeCKFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("[CK] File not found: %s\n", filename);
        return;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;

        if (strncmp(line, "let ", 4) == 0) {
            char var[50];
            int value;
            sscanf(line + 4, "%s = %d", var, &value);
            setVar(var, value);
        } else if (strncmp(line, "print ", 6) == 0) {
            char var[50];
            sscanf(line + 6, "%s", var);
            int idx = findVar(var);
            if (idx != -1) {
                printf("%d\n", variables[idx].value);
            } else {
                printf("%s\n", var);
            }
        }
    }

    fclose(file);
}

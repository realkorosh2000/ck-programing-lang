#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256
#define MAX_VARS 100

typedef struct {
    char name[50];
    int value;
} Variable;

Variable variables[MAX_VARS];
int varCount = 0;

// CK variable functions
int findVar(const char *name) {
    for (int i = 0; i < varCount; i++)
        if (strcmp(variables[i].name, name) == 0) return i;
    return -1;
}

void setVar(const char *name, int value) {
    int idx = findVar(name);
    if (idx != -1) variables[idx].value = value;
    else {
        strcpy(variables[varCount].name, name);
        variables[varCount].value = value;
        varCount++;
    }
}

// Execute a single CK line
void executeCKLine(char *line) {
    if (strncmp(line, "let ", 4) == 0) {
        char var[50];
        int value;
        sscanf(line + 4, "%s = %d", var, &value);
        setVar(var, value);
    } else if (strncmp(line, "print ", 6) == 0) {
        char var[50];
        sscanf(line + 6, "%s", var);
        int idx = findVar(var);
        if (idx != -1) printf("%d\n", variables[idx].value);
        else printf("%s\n", var);
    }
}

// Execute a CK file
void executeCKFile(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { printf("[CK] File not found: %s\n", filename); return; }
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;
        executeCKLine(line);
    }
    fclose(f);
}

// CKS style functions
void setFont(const char *font) { printf("[CKS] Font set to: %s\n", font); }
void setColor(const char *color) { printf("[CKS] Color set to: %s\n", color); }

// Execute a CKS file
void executeCKSFile(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return; // style file is optional
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;
        if (strncmp(line, "set.font(", 9) == 0) {
            char font[50];
            sscanf(line, "set.font(\"%[^\"]\")", font);
            setFont(font);
        } else if (strncmp(line, "set.color(", 10) == 0) {
            char color[50];
            sscanf(line, "set.color(\"%[^\"]\")", color);
            setColor(color);
        }
    }
    fclose(f);
}

// Main shell
int main(int argc, char *argv[])
{
    char *ckFile;

    if (argc == 2)
    {
        ckFile = argv[1]; // file double-clicked
    }
    else if (argc == 3 && strcmp(argv[1], "-run") == 0)
    {
        ckFile = argv[2]; // from command line
    }
    else
    {
        printf("Usage: double-click a .ck file or use cck -run <file>\n");
        return 1;
    }

    // Automatically detect CKS file
    char cksFile[256];
    strcpy(cksFile, ckFile);
    char *dot = strrchr(cksFile, '.');
    if (dot)
        strcpy(dot, ".cks");

    executeCKSFile(cksFile); // Apply styles
    executeCKFile(ckFile);   // Run CK program

    printf("Press Enter to exit...");
    getchar();
    return 0;
}

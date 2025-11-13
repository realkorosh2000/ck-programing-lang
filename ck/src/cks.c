#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declaration for CK function
void executeCKFile(const char *filename);

// Simple style functions
void setFont(const char *font)
{
    printf("[CKS] Font set to: %s\n", font);
}

void setColor(const char *color)
{
    printf("[CKS] Color set to: %s\n", color);
}

// Handle CK import
void importCK(const char *filename)
{
    printf("[CKS] Importing CK file: %s\n", filename);
    executeCKFile(filename);
}

// Execute CKS file
void executeCKSFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("[CKS] File not found: %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        // Remove newline
        line[strcspn(line, "\n")] = 0;

        if (strncmp(line, "set.font(", 9) == 0)
        {
            char font[50];
            sscanf(line, "set.font(\"%[^\"]\")", font);
            setFont(font);
        }
        else if (strncmp(line, "set.color(", 10) == 0)
        {
            char color[50];
            sscanf(line, "set.color(\"%[^\"]\")", color);
            setColor(color);
        }
        else if (strncmp(line, "std.import(", 11) == 0)
        {
            char fileImport[100];
            sscanf(line, "std.import(\"%[^\"]\")", fileImport);
            importCK(fileImport);
        }
    }

    fclose(file);
}

int main()
{
    printf("[SK] Running CKS file...\n");
    executeCKSFile("example.cks");

    printf("Press Enter to exit...");
    getchar(); // wait for user
    return 0;
}

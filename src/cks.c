#include <stdio.h>
#include <string.h>
#include "ck.h"
#include "cks.h"

void setFont(const char *font)
{
    printf("[CKS] Font set to: %s\n", font);
}

void setColor(const char *color)
{
    printf("[CKS] Color set to: %s\n", color);
}

void importCK(const char *filename)
{
    printf("[CKS] Importing CK file: %s\n", filename);
    executeCKFile(filename);
}

void executeCKSFile(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
        return;
    char line[256];
    while (fgets(line, sizeof(line), f))
    {
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
    fclose(f);
}

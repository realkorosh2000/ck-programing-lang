#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ck.h"
#include "cks.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: cck file.ck\n");
        return 1;
    }

    // Automatically detect CKS file
    char cksFile[256];
    strcpy(cksFile, argv[1]);
    char *dot = strrchr(cksFile, '.');
    if (dot)
        strcpy(dot, ".cks");

    executeCKSFile(cksFile); // Apply styles
    executeCKFile(argv[1]);  // Run CK program

    return 0;
}

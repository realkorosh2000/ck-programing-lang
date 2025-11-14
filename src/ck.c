#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ck.h"

// Global variables
Variable variables[MAX_VARS];
int varCount = 0;

Function functions[MAX_FUNCS];
int funcCount = 0;

// --- Variable functions ---
int findVar(const char *name)
{
    for (int i = 0; i < varCount; i++)
        if (strcmp(variables[i].name, name) == 0)
            return i;
    return -1;
}

void setVar(const char *name, int value)
{
    int idx = findVar(name);
    if (idx != -1)
        variables[idx].value = value;
    else
    {
        strcpy(variables[varCount].name, name);
        variables[varCount].value = value;
        varCount++;
    }
}

int getVar(const char *name)
{
    int idx = findVar(name);
    if (idx != -1)
        return variables[idx].value;
    printf("[CK] Variable '%s' not found!\n", name);
    return 0;
}

// --- Print function ---
void ckPrint(const char *text)
{
    int i = 0;
    while (text[i])
    {
        if (text[i] == '/' && text[i + 1] == '!')
        {
            putchar('!');
            i += 2;
        }
        else if (text[i] == '/' && text[i + 1] == 'n')
        {
            putchar('\n');
            i += 2;
        }
        else
        {
            putchar(text[i]);
            i++;
        }
    }
    printf("\n");
}

// --- Execute function ---
int executeFunction(const char *name, int args[], int argCount)
{
    int fIdx = -1;
    for (int i = 0; i < funcCount; i++)
    {
        if (strcmp(functions[i].name, name) == 0)
        {
            fIdx = i;
            break;
        }
    }
    if (fIdx == -1)
    {
        printf("[CK] Function '%s' not found\n", name);
        return 0;
    }

    // Set parameters
    for (int i = 0; i < functions[fIdx].paramCount; i++)
    {
        setVar(functions[fIdx].params[i], args[i]);
    }

    // Execute code
    for (int i = 0; i < functions[fIdx].codeLineCount; i++)
    {
        char *line = functions[fIdx].code[i];
        if (strncmp(line, "return ", 7) == 0)
        {
            int val;
            sscanf(line + 7, "%d", &val);
            return val;
        }
        else if (strncmp(line, "print ", 6) == 0)
        {
            char buf[200];
            sscanf(line + 6, " %[^\n]", buf);
            ckPrint(buf);
        }
        else if (strchr(line, '='))
        {
            char var[50];
            int val;
            sscanf(line, "%s = %d", var, &val);
            setVar(var, val);
        }
    }
    return 0;
}

// --- Parse CK file ---
void executeCKFile(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        printf("[CK] File not found: %s\n", filename);
        return;
    }

    char line[MAX_LINE];
    int inFunction = 0;
    Function currentFunc;

    while (fgets(line, sizeof(line), f))
    {
        line[strcspn(line, "\n")] = 0;

        // Skip comments
        if (strncmp(line, "!", 1) == 0)
            continue;
        if (strncmp(line, "!!", 2) == 0)
        {
            while (fgets(line, sizeof(line), f) && strncmp(line, "!!", 2) != 0)
                ;
            continue;
        }

        // Function start
        if (strncmp(line, "fun ", 4) == 0)
        {
            inFunction = 1;
            memset(&currentFunc, 0, sizeof(Function));
            char params[100];
            sscanf(line, "fun %[^-]-> (%[^)]):", currentFunc.name, params);
            char *tok = strtok(params, ",");
            while (tok)
            {
                while (*tok == ' ')
                    tok++;
                strcpy(currentFunc.params[currentFunc.paramCount], tok);
                currentFunc.paramCount++;
                tok = strtok(NULL, ",");
            }
            currentFunc.codeLineCount = 0;
            continue;
        }

        if (inFunction)
        {
            if (strlen(line) == 0 || line[0] != ' ')
            {
                functions[funcCount++] = currentFunc;
                inFunction = 0;
            }
            else
            {
                char *ptr = line;
                while (*ptr == ' ')
                    ptr++;
                strcpy(currentFunc.code[currentFunc.codeLineCount++], ptr);
            }
        }

        // Global code
        if (!inFunction)
        {
            if (strncmp(line, "print ", 6) == 0)
            {
                char buf[200];
                sscanf(line + 6, " %[^\n]", buf);
                ckPrint(buf);
            }
            else if (strchr(line, '='))
            {
                char var[50];
                int val;
                sscanf(line, "%s = %d", var, &val);
                setVar(var, val);
            }
            else if (strchr(line, '('))
            {
                char fname[50];
                int a1, a2;
                int n = sscanf(line, "%[^'('](%d,%d)", fname, &a1, &a2);
                if (n > 0)
                {
                    int args[2] = {0, 0};
                    if (n >= 2)
                        args[0] = a1;
                    if (n == 3)
                        args[1] = a2;
                    executeFunction(fname, args, n - 1);
                }
            }
        }
    }
    if (inFunction)
        functions[funcCount++] = currentFunc;
    fclose(f);
}

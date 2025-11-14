#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ck.h"

// Global variables
Variable variables[MAX_VARS];
int varCount = 0;

Function functions[MAX_FUNCS];
int funcCount = 0;

int gitImported = 0;
char currentFont[50] = "Default";

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
            putchar(text[i++]);
    }
    printf("\n");
}

// --- Execute function ---
int executeFunction(const char *name, int args[], int argCount)
{
    // --- Git command support ---
    if (gitImported && strncmp(name, "git.", 4) == 0)
    {
        char cmd[256] = "git ";
        char gitCommand[50];
        strcpy(gitCommand, name + 4); // remove git.
        strcat(cmd, gitCommand);

        if (argCount > 0)
        { // currently only first argument
            // You can extend this for strings later
            // For now, assume arg[0] holds string index or int (for demo)
            // Example: git.commit -> ("message")
            // We'll use a simplified system call string
            printf("[CK-GIT] Please use system shell for arguments.\n");
        }

        printf("[CK-GIT] Running: %s\n", cmd);
        system(cmd);
        return 0;
    }

    // --- Regular CK function ---
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
            char arg[200];
            if (sscanf(line + 6, " \"%[^\"]\"", arg) == 1)
                ckPrint(arg);
            else
            {
                // check for function call
                char fname[50];
                int a1 = 0, a2 = 0;
                int n = sscanf(line + 6, "%[^'('](%d,%d)", fname, &a1, &a2);
                if (n > 0)
                {
                    int argsCall[2] = {0, 0};
                    if (n >= 2)
                        argsCall[0] = a1;
                    if (n == 3)
                        argsCall[1] = a2;
                    int ret = executeFunction(fname, argsCall, n - 1);
                    printf("%d\n", ret);
                }
                else
                {
                    int idx = findVar(line + 6);
                    if (idx != -1)
                        printf("%d\n", variables[idx].value);
                    else
                        ckPrint(line + 6);
                }
            }
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

        // import module
        if (strncmp(line, "import ", 7) == 0)
        {
            char module[50];
            if (sscanf(line + 7, " \"%[^\"]\"", module) == 1)
            {
                if (strcmp(module, "git") == 0)
                {
                    gitImported = 1;
                    printf("[CK] Git module imported!\n");
                }
                else
                {
                    printf("[CK] Unknown module: %s\n", module);
                }
            }
            continue;
        }

        // function start
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

        // global code
        if (!inFunction)
        {
            if (strncmp(line, "print ", 6) == 0)
            {
                char arg[200];
                if (sscanf(line + 6, " \"%[^\"]\"", arg) == 1)
                    ckPrint(arg);
                else
                {
                    char fname[50];
                    int a1 = 0, a2 = 0;
                    int n = sscanf(line + 6, "%[^'('](%d,%d)", fname, &a1, &a2);
                    if (n > 0)
                    {
                        int argsCall[2] = {0, 0};
                        if (n >= 2)
                            argsCall[0] = a1;
                        if (n == 3)
                            argsCall[1] = a2;
                        int ret = executeFunction(fname, argsCall, n - 1);
                        printf("%d\n", ret);
                    }
                    else
                    {
                        int idx = findVar(line + 6);
                        if (idx != -1)
                            printf("%d\n", variables[idx].value);
                        else
                            ckPrint(line + 6);
                    }
                }
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
                int a1 = 0, a2 = 0;
                int n = sscanf(line, "%[^'('](%d,%d)", fname, &a1, &a2);
                if (n > 0)
                {
                    int argsCall[2] = {0, 0};
                    if (n >= 2)
                        argsCall[0] = a1;
                    if (n == 3)
                        argsCall[1] = a2;
                    executeFunction(fname, argsCall, n - 1);
                }
            }
        }
    }
    if (inFunction)
        functions[funcCount++] = currentFunc;
    fclose(f);
}

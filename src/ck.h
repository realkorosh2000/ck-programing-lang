#ifndef CK_H
#define CK_H

#define MAX_LINE 256
#define MAX_VARS 100
#define MAX_FUNCS 50
#define MAX_CODE_LINES 50

typedef struct
{
    char name[50];
    int value;
} Variable;

typedef struct
{
    char name[50];
    char params[5][50];
    int paramCount;
    char code[MAX_CODE_LINES][MAX_LINE];
    int codeLineCount;
} Function;

extern Variable variables[MAX_VARS];
extern int varCount;
extern Function functions[MAX_FUNCS];
extern int funcCount;

extern int gitImported;
extern char currentFont[50];

void setVar(const char *name, int value);
int getVar(const char *name);
int findVar(const char *name);
void ckPrint(const char *text);
int executeFunction(const char *name, int args[], int argCount);
void executeCKFile(const char *filename);

#endif

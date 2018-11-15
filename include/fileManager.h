/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
*/

typedef struct AsmList
{
  char Program[204];
  struct AsmList *nextLine, *previousLine;
} asmList;

typedef struct TranslatedProgram
{
	char Program[204];
	struct TranslatedProgram *nextLine, *previousLine;
} translatedProgram;

asmList* CreateAsmList(char **);
FILE * OpenAsmFile(char **);
void AddAsmList(asmList **, char *);
void AddTranslatedProgram(translatedProgram **, char *);
void DeleteAsmList(asmList **);
void DeleteTranslatedProgram(translatedProgram **);
void WriteTranslatedProgram(char **, translatedProgram *);
void CopyFromTxtToList(translatedProgram *);

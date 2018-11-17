/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
Jônatas Senna - mat. 14/0090983
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

typedef struct EquTable
{
  char Label[51];
  int Value;
  struct EquTable *nextItem, *previousItem;
}equTable;

asmList* CreateAsmList(char **);
FILE * OpenAsmFile(char **);
void AddAsmList(asmList **, char *);
void AddTranslatedProgram(translatedProgram **, char *);
void DeleteAsmList(asmList **);
void DeleteTranslatedProgram(translatedProgram **);
void WriteTranslatedProgram(char **, translatedProgram *);
void CopyFromTxtToList(translatedProgram *);
void AddValueEquTable(equTable *, int); // Seta o valor do fim da lista EquTable
void IsInEqu(equTable *, char *);
int AddLabelEquTable(equTable **, char *);
int EquTableContains(equTable *, char *);
void DeleteEquTable(equTable**);

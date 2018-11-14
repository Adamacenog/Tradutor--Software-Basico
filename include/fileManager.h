/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
*/

typedef struct AsmDataHead
{
  struct AsmList *asmListHead;
	struct DataList *dataListHead;
} asmDataHead;

typedef struct AsmList
{
  char Program[204];
  struct AsmList *nextLine, *previousLine;
} asmList;

typedef struct DataList
{
  char Label[204];
  struct DataList *nextItem, *previousItem;
} dataList;

typedef struct TranslatedProgram
{
	char Program[204];
	struct TranslatedProgram *nextLine, *previousLine;
} translatedProgram;

asmDataHead* CreateAsmList(char **);
FILE * OpenAsmFile(char **);
void AddAsmList(asmList **, char *, asmDataHead *);
void AddDataList(dataList **, char *, asmDataHead *);
void AddTranslatedProgram(translatedProgram **, char *);
void InitializeAsmDataHead(asmDataHead **);
void DeleteAsmData(asmDataHead **);
void DeleteAsmList(asmList **);
void DeleteDataList(dataList **);
void DeleteTranslatedProgram(translatedProgram **);
void WriteTranslatedProgram(char **, translatedProgram *);

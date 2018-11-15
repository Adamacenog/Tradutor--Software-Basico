/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
*/

#ifndef _Primary_libraries
  #define _Primary_libraries
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <ctype.h>
#endif

#ifndef _FileManager_library
  #define _FileManager_library
    #include "fileManager.h"
#endif

#ifndef _StringFunctions_library
  #define _StringFunctions_library
    #include "stringFunctions.h"
#endif

#ifndef _TranslatorFunctions_library
  #define _TranslatorFunctions_library
    #include "translatorFunctions.h"
#endif

void TranslateToIa32(asmDataHead *contentHead, char **name)
{
	// Listas com o arquivo '.asm' e a lista de declarações de data/bss
	asmList *asmContent = contentHead->asmListHead;
	dataList *dataListHead = contentHead->dataListHead;
  translatedProgram *translatedProgramHead = NULL;

  printf("\n");

  while (dataListHead != NULL)
  {
    printf("Label: %s\n", dataListHead->Label);
    dataListHead = dataListHead->nextItem;
  }

  printf("\n");

	// Percorrimento de toda a lista 'asmContent' para tradu��o.
	while (asmContent != NULL)
	{
    printf("%s\n", asmContent->Program);
    asmContent = asmContent->nextLine;
	}

	// Adição das funções assembly de input no 'asm' final (no fim do arquivo)

	WriteTranslatedProgram(name, translatedProgramHead);
	DeleteTranslatedProgram(&translatedProgramHead);
}

// Retorna 1 se for uma declaração de label, 0 se não
int isLabelDeclaration(char *Label)
{
	return StringContainsAtEnd(Label, ':', 51);
}

// Bota 0 no section caso seja seção texto, 1 se for seção bss ou data
void isWhatSection(char *saveFile, int *section)
{
	if (strcmp(saveFile, "SECTION TEXT") == 0)
	{
		(*section) = 0;
	}

	if (strcmp(saveFile, "SECTION DATA") == 0 || strcmp(saveFile, "SECTION BSS") == 0)
	{
		(*section) = 1;
	}
}

// Busca a label na lista dataList. Caso exista a label na lista retorna 1, caso contrário 0.
int isInDataList(dataList *dataListHead, char *label)
{
	while (dataListHead != NULL)
	{
		if (strcmp(dataListHead->Label, label) == 0)
			return 1;

		dataListHead = dataListHead->nextItem;
	}

	return 0;
}

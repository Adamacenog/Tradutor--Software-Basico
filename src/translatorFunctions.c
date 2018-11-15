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

void TranslateToIa32(asmList *asmContent, char **name)
{
	// Lista com o programa traduzido para IA-32
  translatedProgram *translatedProgramHead = NULL;

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

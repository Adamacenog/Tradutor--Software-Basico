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
  translatedProgram *translatedProgramHead = NULL;

	// Percorrimento de toda a lista 'asmContent' para tradu��o.
	while (asmContent != NULL)
	{
    TranslateMnemonicToIa32(&translatedProgramHead, asmContent);
    asmContent = asmContent->nextLine;
	}

	// Adição das funções assembly de input no 'asm' final (no fim do arquivo)

	WriteTranslatedProgram(name, translatedProgramHead);
	DeleteTranslatedProgram(&translatedProgramHead);
}

// Traduz os mnemonicos de uma frase para equivalente em IA-32, assim como seus operandos/operações
void TranslateMnemonicToIa32(translatedProgram **translatedProgramHead, asmList *asmContent)
{
  char word[204];
  char program[204];

  // Limpa as strings
  ClearString(word, 204);
  ClearString(program, 204);

  while (asmContent->Program[0] != '\0')
  {
    MoveStringUntilSpace(asmContent->Program, word, 204);

    if (strcmp(word, "ADD") == 0)
    {

    }
    else if (strcmp(word, "SUB") == 0)
    {

    }
    else if (strcmp(word, "MULT") == 0)
    {

    }
    else if (strcmp(word, "DIV") == 0)
    {

    }
    else if (strcmp(word, "JMP") == 0)
    {

    }
    else if (strcmp(word, "JMPN") == 0)
    {

    }
    else if (strcmp(word, "JMPP") == 0)
    {

    }
    else if (strcmp(word, "JMPZ") == 0)
    {

    }
    else if (strcmp(word, "COPY") == 0)
    {

    }
    else if (strcmp(word, "LOAD") == 0)
    {

    }
    else if (strcmp(word, "STORE") == 0)
    {

    }
    else if (strcmp(word, "INPUT") == 0)
    {

    }
    else if (strcmp(word, "OUTPUT") == 0)
    {

    }
    else if (strcmp(word, "C_INPUT") == 0)
    {

    }
    else if (strcmp(word, "C_OUTPUT") == 0)
    {

    }
    else if (strcmp(word, "S_INPUT") == 0)
    {

    }
    else if (strcmp(word, "S_OUTPUT") == 0)
    {

    }
    else if (strcmp(word, "STOP") == 0)
    {

    }
    else if (strcmp(word, "SECTION") == 0)
    {

    }
    else if (strcmp(word, "SPACE") == 0)
    {

    }
    else if (strcmp(word, "CONST") == 0)
    {

    }
    else if (strcmp(word, "EQU") == 0)
    {

    }
    else if (strcmp(word, "IF") == 0)
    {

    }
    else
    {
      strcat(program, word);
    }
  }

  AddTranslatedProgram(translatedProgramHead, program);
}

/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
Jônatas Senna - mat. 14/0090983
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
  int wasText = 0;

	// Percorrimento de toda a lista 'asmContent' para tradu��o.
	while (asmContent != NULL)
	{
    TranslateMnemonicToIa32(&translatedProgramHead, asmContent, &wasText);
    asmContent = asmContent->nextLine;
	}

	// Adição das funções assembly de input/output no 'asm' final (no fim do arquivo)
  CopyFromTxtToList(translatedProgramHead);

	WriteTranslatedProgram(name, translatedProgramHead);
	DeleteTranslatedProgram(&translatedProgramHead);
}

// Traduz os mnemonicos de uma frase para equivalente em IA-32, assim como seus operandos/operações
void TranslateMnemonicToIa32(translatedProgram **translatedProgramHead, asmList *asmContent, int *wasText)
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
      strcat(program, word);
      strcat(program, " DWORD EAX, ");
      AdjustAdressingModes(asmContent->Program);
      strcat(program, asmContent->Program);
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "SUB") == 0)
    {
      strcat(program, word);
      strcat(program, " EAX, ");
      AdjustAdressingModes(asmContent->Program);
      strcat(program, asmContent->Program);
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "MULT") == 0)
    {

    }
    else if (strcmp(word, "DIV") == 0)
    {

    }
    else if (strcmp(word, "JMP") == 0)
    {
      strcat(program, word);
      strcat(program, " ");
      strcat(program, asmContent->Program);
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "JMPN") == 0)
    {
      strcat(program, "JL");
      strcat(program, " ");
      strcat(program, asmContent->Program);
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "JMPP") == 0)
    {
      strcat(program, "JG");
      strcat(program, " ");
      strcat(program, asmContent->Program);
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "JMPZ") == 0)
    {
      strcat(program, "JE");
      strcat(program, " ");
      strcat(program, asmContent->Program);
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "COPY") == 0)
    {
      strcat(program, "MOV DWORD EBX, ");
      MoveStringUntilSpace(asmContent->Program, word, 204);
      RemoveChar(',', word, 204, 1);
      AdjustAdressingModes(word);
      strcat(program, word);
      strcat(program, "\nMOV DWORD ");
      AdjustAdressingModes(asmContent->Program);
      strcat(program, asmContent->Program);
      strcat(program, ", EBX");
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "LOAD") == 0)
    {
      strcat(program, "MOV DWORD EAX, ");
      AdjustAdressingModes(asmContent->Program);
      strcat(program, asmContent->Program);
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "STORE") == 0)
    {
      strcat(program, "MOV DWORD ");
      AdjustAdressingModes(asmContent->Program);
      strcat(program, asmContent->Program);
      strcat(program, ", EAX");
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "INPUT") == 0)
    {
      strcat(program, "PUSH EAX\nCALL INPUT\nMOV DWORD ");
      strcat(program, asmContent->Program);
      strcat(program, ", EAX\nPOP EAX");
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "OUTPUT") == 0)
    {
      strcat(program, "PUSH EAX\nPUSH DWORD ");
      strcat(program, asmContent->Program);
      strcat(program, "\nCALL OUTPUT\nADD ESP, 4\nPOP EAX");
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "C_INPUT") == 0)
    {
      strcat(program, "PUSH EAX\nCALL C_INPUT\nMOV DWORD ");
      strcat(program, asmContent->Program);
      strcat(program, ", EAX\nPOP EAX");
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "C_OUTPUT") == 0)
    {
      strcat(program, "PUSH EAX\nPUSH DWORD ");
      strcat(program, asmContent->Program);
      strcat(program, "\nCALL C_OUTPUT\nADD ESP, 4\nPOP EAX");
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "S_INPUT") == 0)
    {
      strcat(program, "PUSH EAX\nCALL S_INPUT\nMOV DWORD ");
      strcat(program, asmContent->Program);
      strcat(program, ", EAX\nPOP EAX");
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "S_OUTPUT") == 0)
    {
      strcat(program, "PUSH EAX\nPUSH DWORD ");
      strcat(program, asmContent->Program);
      strcat(program, "\nCALL S_OUTPUT\nADD ESP, 4\nPOP EAX");
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "STOP") == 0)
    {
      strcat(program, "MOV EAX, 1\nMOV EBX, 0\nINT 80h");
    }
    else if (strcmp(word, "SECTION") == 0)
    {
      // Analisa qual é a seção
      MoveStringUntilSpace(asmContent->Program, word, 204);

      if (strcmp(word, "TEXT") == 0)
      {
        strcat(program, "SECTION .TEXT");
        if((*wasText) == 0)
          strcat(program, "\nGLOBAL _START\n_START:");
      }
      else if (strcmp(word, "DATA") == 0)
      {
        strcat(program, "SECTION .DATA");
      }
      else if (strcmp(word, "BSS") == 0)
      {
        strcat(program, "SECTION .BSS");
      }
    }
    else if (strcmp(word, "SPACE") == 0)
    {
      strcat(program, "RESD");
      MoveStringUntilSpace(asmContent->Program, word, 204);

      // Caso o space não tenha nenhum argumento, bota-se 1.
      if (word[0] != '\0')
      {
        strcat(program, " ");
        strcat(program, word);
      }
      else
      {
        strcat(program, " 1");
      }
    }
    else if (strcmp(word, "CONST") == 0)
    {
      strcat(program, "DW ");
      MoveStringUntilSpace(asmContent->Program, word, 204);
      strcat(program, word);
    }
    else
    {
      strcat(program, word);
      strcat(program, " ");
    }
  }

  AddTranslatedProgram(translatedProgramHead, program);
}

// Ajusta o modo de endereçamento de acordo com o operador (imediato, memoria (direto/indireto))
void AdjustAdressingModes(char *operator)
{
  long number;
  int i, j;
  char *temp, newOperator[204], stringNumber[10];

  number = strtol(operator, &temp, 10);

  if (strcmp(temp, "") != 0 && strcmp(temp, "H") != 0)
  {
    // Limpa a nova string por completo
    ClearString(newOperator, 204);
    ClearString(stringNumber, 10);

    // Inicia o ajuste do acesso a memoria
    newOperator[0] = '[';
    for (i = 0; i < 203; i++)
    {
      // Proximo item será um numero
      if (operator[i] == '+')
      {
        newOperator[i + 1] = '+';
        i++;

        // Pega a string do numero
        for (j = 0; j < 10 && (i + j) < 204; j++)
        {
          stringNumber[j] = operator[i + j];
        }

        // Pega o numero real e multiplica por 4 (pois é 32 bits e o deslocamento é *4)
        number = (strtol(stringNumber, &temp, 10))*4;
        ClearString(stringNumber, 10);

        // Transforma novamente para string
        sprintf(stringNumber, "%ld", number);

        // Concatena o numero final no operador junto com o colchete
        strcat(newOperator, stringNumber);
        strcat(newOperator, "]");
        break;
      }

      // Caso tenha apenas uma label e acabe a string
      if (operator[i] == '\0')
      {
        newOperator[i + 1] = ']';
        break;
      }

      // Adição dos caracteres
      newOperator[i + 1] = operator[i];
    }

    // Copia a nova string no operador
    CopyString(newOperator, operator, 204);
  }
}

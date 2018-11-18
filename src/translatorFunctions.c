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

    if (strcmp(word, "add") == 0)
    {
      strcat(program, word);
      strcat(program, " dword eax, ");
      AdjustAdressingModes(asmContent->Program);
      strcat(program, asmContent->Program);
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "sub") == 0)
    {
      strcat(program, word);
      strcat(program, " dword eax, ");
      AdjustAdressingModes(asmContent->Program);
      strcat(program, asmContent->Program);
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "mult") == 0)
    {
      strcat(program, "imul dword ");
      AdjustAdressingModes(asmContent->Program);
      strcat(program, asmContent->Program);
      ClearString(asmContent->Program, 204);
      strcat(program, "\njnc mulnotoverflow\njno mulnotoverflow");
      strcat(program, "\nmov edx, ovrflwlenth\nmov ecx, ovrflwmsg\n");
      strcat(program, "mov ebx, 1\nmov eax, 4\nint 80h\n");
      strcat(program, "mov eax, 1\nmov ebx, -1\nint 80h"); // Finaliza o programa com '-1 (erro)'
      strcat(program, "\nmulnotoverflow:");
    }
    else if (strcmp(word, "div") == 0)
    {
      strcat(program, "cdq\ni");
      strcat(program, word);
      strcat(program, " dword ");
      AdjustAdressingModes(asmContent->Program);
      strcat(program, asmContent->Program);
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "jmp") == 0)
    {
      strcat(program, word);
      strcat(program, " ");
      strcat(program, asmContent->Program);
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "jmpn") == 0)
    {
      strcat(program, "jl");
      strcat(program, " ");
      strcat(program, asmContent->Program);
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "jmpp") == 0)
    {
      strcat(program, "jg");
      strcat(program, " ");
      strcat(program, asmContent->Program);
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "jmpz") == 0)
    {
      strcat(program, "je");
      strcat(program, " ");
      strcat(program, asmContent->Program);
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "copy") == 0)
    {
      strcat(program, "mov dword ebx, ");
      MoveStringUntilSpace(asmContent->Program, word, 204);
      RemoveChar(',', word, 204, 1);
      AdjustAdressingModes(word);
      strcat(program, word);
      strcat(program, "\nmov dword ");
      RemoveChar(',', asmContent->Program, 204, 0);
      AdjustAdressingModes(asmContent->Program);
      strcat(program, asmContent->Program);
      strcat(program, ", ebx");
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "load") == 0)
    {
      strcat(program, "mov dword eax, ");
      AdjustAdressingModes(asmContent->Program);
      strcat(program, asmContent->Program);
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "store") == 0)
    {
      strcat(program, "mov dword ");
      AdjustAdressingModes(asmContent->Program);
      strcat(program, asmContent->Program);
      strcat(program, ", eax");
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "input") == 0)
    {
      strcat(program, "push eax\ncall LeerInteiro\nmov dword ");
      AdjustAdressingModes(asmContent->Program);
      strcat(program, asmContent->Program);
      strcat(program, ", eax\npop eax");
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "output") == 0)
    {
      strcat(program, "push eax\npush dword ");
      strcat(program, asmContent->Program);
      strcat(program, "\ncall EscreverInteiro\npop eax");
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "c_input") == 0)
    {
      strcat(program, "push eax\ncall LeerChar\nmov dword ");
      AdjustAdressingModes(asmContent->Program);
      strcat(program, asmContent->Program);
      strcat(program, ", eax\npop eax");
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "c_output") == 0)
    {
      strcat(program, "push eax\npush dword ");
      strcat(program, asmContent->Program);
      strcat(program, "\ncall EscreverChar\npop eax");
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "s_input") == 0)
    {
      strcat(program, "push eax\npush dword ");
      MoveStringUntilSpace(asmContent->Program, word, 204);
      RemoveChar(',', word, 204, 1);
      strcat(program, word);
      RemoveChar(',', asmContent->Program, 204, 0);
      strcat(program, "\npush dword ");
      AdjustAdressingModes(asmContent->Program);
      strcat(program, asmContent->Program);
      strcat(program, "\ncall LeerString\nadd dword esp, 8\npop eax");
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "s_output") == 0)
    {
      strcat(program, "push eax\npush dword ");
      MoveStringUntilSpace(asmContent->Program, word, 204);
      RemoveChar(',', word, 204, 1);
      strcat(program, word);
      RemoveChar(',', asmContent->Program, 204, 0);
      strcat(program, "\npush dword ");
      AdjustAdressingModes(asmContent->Program);
      strcat(program, asmContent->Program);
      strcat(program, "\ncall EscreverString\nadd esp, 8\npop eax");
      ClearString(asmContent->Program, 204);
    }
    else if (strcmp(word, "stop") == 0)
    {
      strcat(program, "mov eax, 1\nmov ebx, 0\nint 80h");
    }
    else if (strcmp(word, "section") == 0)
    {
      // Analisa qual é a seção
      MoveStringUntilSpace(asmContent->Program, word, 204);

      if (strcmp(word, "text") == 0)
      {
        strcat(program, "section .text");
        if((*wasText) == 0)
          strcat(program, "\nglobal _start\n_start:");
      }
      else if (strcmp(word, "data") == 0)
      {
        strcat(program, "section .data");
      }
      else if (strcmp(word, "bss") == 0)
      {
        strcat(program, "section .bss");
      }
    }
    else if (strcmp(word, "space") == 0)
    {
      strcat(program, "resd");
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
    else if (strcmp(word, "const") == 0)
    {
      strcat(program, "dd ");
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

  number = strtol(operator, &temp, 0);

  if (strcmp(temp, "") != 0)
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

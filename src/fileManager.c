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

asmList* CreateAsmList(char **name)
{
  FILE *asmFile;
  asmList *asmContent = NULL;
  equTable *equTableHead = NULL;
  int i = 0, removeLine = 0, wasEqu = 0, wasIf = 0, valueInt;
  char fileItem, saveFile[204], word[100], *ptr;

  // Abertura do arquivo '.asm'
  asmFile = OpenAsmFile(name);

  // Limpar por completo a string saveFile e word
  ClearString(saveFile, 204);
  ClearString(word, 100);

  // Leitura de caracter em caracter do arquivo, botando os caracteres em maiúsculo
  while ((fileItem = tolower((char) fgetc(asmFile))) != EOF)
  {
    // Remoção dos comentários
    if(fileItem == ';')
    {
      while((fileItem = fgetc(asmFile)) != '\n' && fileItem != EOF);
      word[0] = 0x20;
    }

	  // Remoção de tabs, espaços, novas linhas e 'carriage return'
	  if (fileItem != 0x20 && fileItem != 0x09 && fileItem != '\n' && fileItem != 0xD)
	  {
		  word[i] = fileItem;
		  i++;
	  }
    else
    {
      // Reseta o contador dos indexes de 'word'
		  i = 0;
    }

	  // Caso tenha sido um espaço ou tab e a string word não esteja em branco
	  if ((fileItem == 0x20 || fileItem == 0x09 || fileItem == '\n') && word[0] != '\0')
	  {
      // Caso tenha tido algum EQU no codigo
      if(wasEqu)
      {
        // Analisa o valor da linha e transforma em long int
        valueInt = (int) strtol(word,&ptr,10);

        // Adiciona o valor na lista equTable
        AddValueEquTable(equTableHead, valueInt);
        wasEqu = 0;
      }

      // Caso tenha tido algum IF no codigo
      if(wasIf)
      {
        // Verifica se o operador do if está na equTable e substitui o conteudo de 'word' pelo valor (caso esteja)
        IsInEqu(equTableHead, word);

        // Pega o valor da string
        valueInt = strtol(word,&ptr,10);

        if(valueInt == 0 && strcmp(ptr, "") == 0)
          removeLine = 2; // remove a linha do 'if' e a linha abaixo dele

        wasIf = 0;
      }

      // Caso seja encontrado um EQU
      if(strcmp(word, "equ") == 0)
      {
        // Remoção de espaços do label (que está no saveFile)
        RemoveChar(0x20, saveFile, 204, 0);

        // Adiciona a label na lista equTable (se ela não estiver sendo redefinida)
        if(AddLabelEquTable(&equTableHead, saveFile) == 1)
        {
          wasEqu = 1;
        }
        else
        {
          wasEqu = 0;
        }

        removeLine = 1;
      }

      // Caso seja encontrado um 'IF'
      if(strcmp(word, "if") == 0)
      {
        // Remoção de espaços antes do IF
        RemoveChar(0x20, saveFile, 204, 0);

        // Não pode conter labels ou qualquer coisa antes do IF
        if(strcmp(saveFile, "") == 0)
        {
          wasIf = 1;
        }
        else
        {
          wasIf = 0;
        }

        removeLine = 1;
      }

      // Verifica se a string está na equTable e precisa substituir o valor
      IsInEqu(equTableHead, word);

      // Concatenação no saveFile
		  strcat(saveFile, word);

      // Caso seja fim de linha
  	  if (fileItem == '\n')
  	  {
        if (removeLine == 0)
        {
          // Remove o ultimo espaço, caso exista.
          RemoveChar(0x20, saveFile, 204, 1);

          // Adiciona a linha do programa na lista asm
    		  AddAsmList(&asmContent, saveFile);
        }
        else
        {
          removeLine--;
        }

  		  // Limpar por completo a string saveFile e word
  		  ClearString(saveFile, 204);
  	  }
      else
      {
        strcat(saveFile, " ");
      }

		  // Limpar por completo a string word
		  ClearString(word, 100);
	  }
  }

  // Caso o arquivo '.asm' termine e não tenha um '\n' no final do arquivo (antes de terminar) e esteja no fileString
  if (saveFile[0] != '\0')
  {
	  // Concatenação no saveFile se word não estiver vazio
	  if (word[0] != '\0')
    {
      // Verifica se a string está na equTable e precisa substituir o valor
      IsInEqu(equTableHead, word);
      strcat(saveFile, word);
    }

	  // Adiciona a linha do programa na lista asm
	  AddAsmList(&asmContent, saveFile);
  }

  DeleteEquTable(&equTableHead);
  fclose(asmFile);

  return asmContent;
}

FILE * OpenAsmFile(char **name)
{
  FILE *asmFile;
  char asmFileName[100];

  // Adicionando o '.asm' no nome do arquivo
  strcpy(asmFileName,name[1]);
  strcat(asmFileName,".asm");

  // Abertura do arquivo '.asm'
  asmFile = fopen(asmFileName,"r");
  if(asmFile == NULL)
  {
    printf("ERRO: Arquivo não encontrado\n");
    exit(1);
  }
  return asmFile;
}

// Adiciona ao fim da lista AsmList (ou cria a lista caso seja NULL, e adiciona o inicio de AsmList em AsmDataHead)
void AddAsmList(asmList **asmListHead, char *programLine)
{
  asmList *contentCreator, *lastElem;

  // Criação da lista de dados das linhas do programa '.asm'
  contentCreator = (asmList *) malloc(sizeof(asmList));
  if(*asmListHead == NULL)
  {
    *asmListHead = contentCreator;
    (*asmListHead)->nextLine = NULL;
    (*asmListHead)->previousLine = NULL;
  }
  else
  {
    lastElem = *asmListHead;

    while(lastElem->nextLine != NULL)
      lastElem = lastElem->nextLine;

    contentCreator->previousLine = lastElem;
    lastElem->nextLine = contentCreator;
    contentCreator->nextLine = NULL;
  }

  strcpy(contentCreator->Program, programLine);
}

// Deleta a lista AsmList
void DeleteAsmList(asmList **asmListHead)
{
	asmList *aux;

	while (*asmListHead != NULL)
	{
		aux = *asmListHead;
		*asmListHead = (*asmListHead)->nextLine;
		free(aux);
	}
}

// Adiciona uma linha na lista translatedProgram
void AddTranslatedProgram(translatedProgram **translatedProgramHead, char *programLine)
{
	translatedProgram *contentCreator, *lastElem;

	// Criação da lista de dados das linhas do programa IA-32
	contentCreator = (translatedProgram *)malloc(sizeof(translatedProgram));
	if (*translatedProgramHead == NULL)
	{
		*translatedProgramHead = contentCreator;
		(*translatedProgramHead)->nextLine = NULL;
		(*translatedProgramHead)->previousLine = NULL;
	}
	else
	{
		lastElem = *translatedProgramHead;

		while (lastElem->nextLine != NULL)
			lastElem = lastElem->nextLine;

		contentCreator->previousLine = lastElem;
		lastElem->nextLine = contentCreator;
		contentCreator->nextLine = NULL;
	}

	strcpy(contentCreator->Program, programLine);
}

// Deleta a lista translatedProgram
void DeleteTranslatedProgram(translatedProgram **translatedProgramHead)
{
	translatedProgram *aux;

	while (*translatedProgramHead != NULL)
	{
		aux = *translatedProgramHead;
		*translatedProgramHead = (*translatedProgramHead)->nextLine;
		free(aux);
	}
}

// Coloca toda a lista de translatedProgram em um arquivo com o mesmo nome do '.asm' só que com '.s' no formato
void WriteTranslatedProgram(char **name, translatedProgram *translatedProgramHead)
{
	char asmFileName[100];
	FILE * asmFile;

	// Adicionando o '.s' no nome do arquivo
	strcpy(asmFileName, name[1]);
	strcat(asmFileName, ".s");

	// Abertura do arquivo '.s'
	asmFile = fopen(asmFileName, "w");

	if (asmFile == NULL)
	{
		printf("ERRO: Arquivo não criado\n");
		exit(1);
	}

	while (translatedProgramHead != NULL)
	{
		fprintf(asmFile, "%s\n", translatedProgramHead->Program);
		translatedProgramHead = translatedProgramHead->nextLine;
	}

	fclose(asmFile);
}

// Copia todos itens do txt 'input-output--funcs.txt' para a lista 'TranslatedProgram'
void CopyFromTxtToList(translatedProgram *translatedProgramHead)
{
  FILE *funcsFile;
  char fileItem, program[204];
  int i = 0;

  // Abertura do arquivo '.txt'
  funcsFile = fopen("input-output--funcs.txt","r");
  if(funcsFile == NULL)
  {
    printf("ERRO: Arquivo de funções input/output não encontrado\n");
    exit(1);
  }

  // Apaga todo conteudo da string program
  ClearString(program, 204);

  // Leitura de caracter em caracter do arquivo, botando os caracteres em maiúsculo
  while ((fileItem = tolower((char) fgetc(funcsFile))) != EOF)
  {
    // Remoção do carriage return e do '\n'
    if (fileItem != 0xD && fileItem != '\n')
    {
      program[i] = fileItem;
      i++;
    }
    else
    {
      i = 0;
    }

    if(fileItem == '\n')
    {
      AddTranslatedProgram(&translatedProgramHead, program);
      ClearString(program, 204);
      i = 0;
    }
  }

  // Caso o arquivo '.txt' não termine com '\n' (windows)
  if (program[0] != '\0')
    AddTranslatedProgram(&translatedProgramHead, program);

  fclose(funcsFile);
}

// Seta o valor do fim da lista EquTable
void AddValueEquTable(equTable *tableHead, int value)
{
  if(tableHead != NULL)
  {
    while(tableHead->nextItem != NULL)
      tableHead = tableHead->nextItem;

    tableHead->Value = value;
  }
}

// Verifica se a label está na equTable
void IsInEqu(equTable *EquHead, char *item)
{
  while(EquHead != NULL)
  {
    if(strcmp(EquHead->Label, item) == 0)
    {
      sprintf(item, "%d", EquHead->Value);
      break;
    }

    EquHead = EquHead->nextItem;
  }
}

// Adiciona ao fim da lista EquTable (ou cria a lista caso seja NULL). Retorna 1 se foi add com sucesso, 0 caso contrario
int AddLabelEquTable(equTable **tableHead, char *saveFile)
{
  equTable *tableCreator = NULL, *tableAux = NULL;

  // Identificação da existência de ':' APENAS no final do label
  if(StringContains(saveFile, ':', 204) == 1 && StringContainsAtEnd(saveFile, ':', 204) == 1 && EquTableContains(*tableHead, saveFile) == 0)
  {
    // Criação da tabela de itens 'equ'
    tableCreator = (equTable *) malloc(sizeof(equTable));

    if(*tableHead == NULL)
    {
      *tableHead = tableCreator;
      tableCreator->nextItem = NULL;
      tableCreator->previousItem = NULL;
    }
    else
    {
      tableAux = (*tableHead);
      while(tableAux->nextItem != NULL)
        tableAux = tableAux->nextItem;

      tableCreator->nextItem = NULL;
      tableCreator->previousItem = tableAux;
      tableAux->nextItem = tableCreator;
    }

    // Remoção de ':'
    RemoveChar(':', saveFile, 204, 1);
    strcpy(tableCreator->Label, saveFile);
    return 1;
  }
  else
  {
    return 0;
  }
}

// Verifica se label ja esta na tabela, se estiver, retorna 1, caso contrario 0
int EquTableContains(equTable *tableHead, char *string)
{
  while(tableHead != NULL)
  {
    if(StringCompareButEnd(tableHead->Label, string, 51, 51) == 1)
      return 1;

    tableHead = tableHead->nextItem;
  }

  return 0;
}

// Deleta toda a lista EquTable
void DeleteEquTable(equTable **tableHead)
{
  equTable *aux;

  while(*tableHead != NULL)
  {
    aux = *tableHead;
    *tableHead = (*tableHead)->nextItem;
    free(aux);
  }
}

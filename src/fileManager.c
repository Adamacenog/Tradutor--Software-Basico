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

asmDataHead* CreateAsmList(char **name)
{
  FILE *asmFile;
  asmDataHead *contentHead = NULL;
  asmList *asmContent = NULL;
  dataList *dataListHead = NULL;

  // section: 0 - text, 1 - data/bss
  int i = 0, section = 0, wasLabel = 0;
  char fileItem, saveFile[204], word[100];

  // Inicialização da cabeça de data/lista asm
  InitializeAsmDataHead(&contentHead);

  // Abertura do arquivo '.asm'
  asmFile = OpenAsmFile(name);

  // Limpar por completo a string saveFile e word
  ClearString(saveFile, 204);
  ClearString(word, 100);

  // Leitura de caracter em caracter do arquivo, botando os caracteres em maiúsculo
  while ((fileItem = toupper((char) fgetc(asmFile))) != EOF)
  {
	  // Remoção de tabs, espaços, novas linhas e 'carriage return'
	  if (fileItem != 0x20 && fileItem != 0x09 && fileItem != '\n' && fileItem != 0xD)
	  {
		  word[i] = fileItem;
		  i++;
	  }

	  // Caso tenha sido um espaço ou tab e a string word não esteja em branco
	  if (fileItem == 0x20 || fileItem == 0x09)
	  {
      // Finalização da string 'word'
      word[i] = '\0';

      // Reseta o contador dos indexes de 'word'
		  i = 0;

		  // Caso seja section Data ou Bss, e há uma declaração de label, add na lista de data.
		  if (section == 1 && isLabelDeclaration)
		  {
			  RemoveChar(':', word, 100, 1);
			  AddDataList(&dataListHead, word, contentHead);
			  wasLabel = 1;
		  }

		  // Concatenação no saveFile
		  strcat(saveFile, word);

      if(word[0] != '\0')
      {
        if (wasLabel == 1)
        {
          strcat(saveFile, ": ");
          wasLabel = 0;
        }
        else
        {
          strcat(saveFile, " ");
        }
      }

		  // Limpar por completo a string word
		  ClearString(word, 100);
	  }

	  // Caso seja fim de linha
	  if (fileItem == '\n')
	  {
		  // Concatenação no saveFile se word não estiver vazio
		  if (word[0] != '\0')
			   strcat(saveFile, word);

		  // Adiciona a linha do programa na lista asm
		  AddAsmList(&asmContent, saveFile, contentHead);

		  // Checa se saveFile é alguma declaração de seção (text, data, bss)
		  isWhatSection(saveFile, &section);

		  // Limpar por completo a string saveFile e word
		  ClearString(saveFile, 204);
		  ClearString(word, 100);
	  }
  }

  // Caso o arquivo '.asm' termine e não tenha um '\n' no final do arquivo (antes de terminar) e esteja no fileString
  if (saveFile[0] != '\0')
  {
	  // Concatenação no saveFile se word não estiver vazio
	  if (word[0] != '\0')
		  strcat(saveFile, word);

	  // Adiciona a linha do programa na lista asm
	  AddAsmList(&asmContent, saveFile, contentHead);
  }

  fclose(asmFile);

  return contentHead;
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
void AddAsmList(asmList **asmListHead, char *programLine, asmDataHead *contentHead)
{
  asmList *contentCreator, *lastElem;

  // Criação da lista de dados das linhas do programa '.asm'
  contentCreator = (asmList *) malloc(sizeof(asmList));
  if(*asmListHead == NULL)
  {
    *asmListHead = contentCreator;
    (*asmListHead)->nextLine = NULL;
    (*asmListHead)->previousLine = NULL;

	if(contentHead != NULL)
	{
		contentHead->asmListHead = contentCreator;
	}
	else
	{
		printf("ERRO: Cabeça da lista nula\n");
	}
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

// Adiciona ao fim da lista DataList (ou cria a lista caso seja NULL, e adiciona o inicio de DataList em AsmDataHead)
void AddDataList(dataList **dataListHead, char *label, asmDataHead *contentHead)
{
  dataList *contentCreator, *lastElem;

  // Criação da lista de dados das labels de data/bss do programa '.asm'
  contentCreator = (dataList *) malloc(sizeof(dataList));
  if(*dataListHead == NULL)
  {
    *dataListHead = contentCreator;
    (*dataListHead)->nextItem = NULL;
    (*dataListHead)->previousItem = NULL;

	if(contentHead != NULL)
	{
		contentHead->dataListHead = contentCreator;
	}
	else
	{
		printf("ERRO: Cabeça da lista nula\n");
	}
  }
  else
  {
    lastElem = *dataListHead;

    while(lastElem->nextItem != NULL)
      lastElem = lastElem->nextItem;

    contentCreator->previousItem = lastElem;
    lastElem->nextItem = contentCreator;
    contentCreator->nextItem = NULL;
  }

  strcpy(contentCreator->Label, label);
}

// Aloca espaço para a lista AsmDataHead
void InitializeAsmDataHead(asmDataHead **contentHead)
{
	(*contentHead) = (asmDataHead *) malloc(sizeof(asmDataHead));
	(*contentHead)->asmListHead = NULL;
    (*contentHead)->dataListHead = NULL;
}

// Deleta as listas AsmList, DataList e a cabeça AsmDataHead
void DeleteAsmData(asmDataHead **contentHead)
{
	DeleteAsmList(&(*contentHead)->asmListHead);
	DeleteDataList(&(*contentHead)->dataListHead);
	free(*contentHead);
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

// Deleta a lista DataList
void DeleteDataList(dataList **dataListHead)
{
	dataList *aux;

	while (*dataListHead != NULL)
	{
		aux = *dataListHead;
		*dataListHead = (*dataListHead)->nextItem;
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

// Coloca toda a lista de translatedProgram em um arquivo com o mesmo nome do '.asm' só que com '-IA32' no nome
void WriteTranslatedProgram(char **name, translatedProgram *translatedProgramHead)
{
	char asmFileName[100];
	FILE * asmFile;

	// Adicionando o '.asm' no nome do arquivo
	strcpy(asmFileName, name[1]);
	strcat(asmFileName, "-IA32.asm");

	// Abertura do arquivo '.asm'
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

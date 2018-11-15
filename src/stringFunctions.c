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

#ifndef _StringFunctions_library
  #define _StringFunctions_library
    #include "stringFunctions.h"
#endif

// Limpa a string por completo, colocando '\0'
void ClearString(char *string, int size)
{
	for (int i = 0; i<size; i++)
		string[i] = '\0';
}

// Remove apenas o char 'removeChar' que estiver no inicio e no fim da string, ou apenas checa no final caso lastOnly seja maior ou menor que 0
void RemoveChar(char removeChar, char *item, int size, int lastOnly)
{
	int i;
	if (item[0] == removeChar && lastOnly == 0)
	{
		for (i = 0; i<(size - 2); i++)
		{
			item[i] = item[i + 1];
		}
	}

	for (i = (size - 2); i > 0; i--)
	{
		if (item[i] == removeChar && item[i + 1] == '\0')
		{
			item[i] = '\0';
			break;
		}
	}

	if (item[(size - 1)] == removeChar)
		item[(size - 1)] = '\0';
}

// Verifica a quantidade de vezes que um item aparece na string
int StringContains(char *string, char item, int size)
{
	int quantity = 0;
	for (int i = 0; i < (size - 1); i++)
		if (string[i] == item)
			quantity += 1;

	return quantity;
}

// Verifica se a string contem um caracter na sua ultima posi��o preenchida (retorna 1 se tiver, 0 se n�o)
int StringContainsAtEnd(char *string, char item, int size)
{
	if (string[size - 1] == item)
		return 1;

	for (int i = (size - 2); i > 0; i--)
		if (string[i] == item && string[i + 1] == '\0')
			return 1;

	return 0;
}

// Compara todos os caracteres menos o ultimo, retorna 1 se for igual, 0 se for diferente
int StringCompareButEnd(char *item1, char *item2, int size1, int size2)
{
	int smallest = size1 > size2 ? size2 : size1;

	for (int i = 0; i < smallest; i++)
	{
		if (item1[i] != item2[i])
		{
			if (item1[i] == '\0' && item2[i + 1] == '\0' || item2[i] == '\0' && item1[i + 1] == '\0')
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}

	return 1;
}

// Pega a label de uma soma
void GetLabelFromSum(char *item, int size)
{
	char newItem[51], *ptr;
	int number, i;

	for (i = 0; i < size; i++)
	{
		if (item[i] == '+')
		{
			newItem[i] = '\0';
			break;
		}

		newItem[i] = item[i];
	}

	CopyString(newItem, item, 51);
}

// Copia o conteudo total de uma string para outra de mesmo tamannho
void CopyString(char *source, char *dest, int size)
{
	for (int i = 0; i <= size; i++)
		dest[i] = source[i];
}

// Move o conteudo da string source para a string dest até um espaço (o espaço também é removido). size(source) tem que ser igual a size(dest)
void MoveStringUntilSpace(char *source, char *dest, int size)
{
  int i, j;

  // Limpa a string dest
  ClearString(dest, size);

  if(source[0] != '\0')
  {
    for (i = 0; i <= size; i++)
    {
      if (source[i] == 0x20)
        break;

      dest[i] = source[i];
    }
    
    i++;

    for (j = 0; (j+i) <= size; j++)
  		source[j] = source[j+i];

    if (i >= size)
      ClearString(source, size);
  }
}

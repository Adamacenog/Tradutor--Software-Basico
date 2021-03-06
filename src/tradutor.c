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

int main(int argc, char *argv[])
{
  if(argc == 2)
  {
    asmList *asmContent = NULL;

    asmContent = CreateAsmList(argv);
	  TranslateToIa32(asmContent, argv);

    DeleteAsmList(&asmContent);
  }
  else
  {
    printf("ERRO: Um arquivo '.asm' é esperado.\n");
  }

  return 0;
}

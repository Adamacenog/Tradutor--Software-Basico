
section .text
;Devo separar as funções e colocar global?
leerInteiro:
  enter 16,0
  mov dword[ebp-8], 0x0
  mov  dword[ebp-12], 0x03           ;inicia o valor da flag
  mov dword[ebp-16], 0       ; contador
loop1:
  mov [ebp-4], ebp
  sub dword[ebp-4], 0x1     ;Isso aqui funciona?!
  mov ebx, 0                ;Descritor do arquivo (stdin)
  mov ecx, [ebp-4]
  mov edx, 0x1
  mov eax, 3                ;Número de chamada do sistema (sys_read)
  int 0x80                  ;Chamada do kernel
  cmp dword[ebp-4], 0x2D
  jne mark1                 ; Se não começar com - mark1
  add dword [ebp-16], 0x1     ;contador++
  mov dword[ebp-12], 0x1                 ; flag de numero negativo
  jmp loop1
mark1:
  cmp dword[ebp-12],0x3         ; Se a flag não for marcada
  jne mark2
  mov dword[ebp-12], 0x2                 ; flag de numero positivo
mark2:
  cmp dword[ebp-4], 0xA
  je  operations                      ;Se for enter
  add dword [ebp-16], 0x1     ;contador++
  sub dword[ebp-4], 0x30
  push dword[ebp-8]
  shl dword[ebp-8], 3 ;aux4 x 10
  pop eax
  add dword[ebp-8], eax
  add dword[ebp-8], eax
  mov dword eax, [ebp-4]
  add dword[ebp-8], eax
  jmp loop1
operations:
  cmp dword[ebp-12],0x2
  je retorno1 ; é um numero positivo
  ;é negativo, então complemento a 2
  not dword[ebp-8] ;inverte
  add dword[ebp-8], 0x1 ; soma 1
retorno1:
  mov dword ebx, ebp
  add dword [ebx], 0x08
  mov dword ebx, [ebp-8] ;resultado
  mov dword ebx, [ebp-16] ;contador
  leave
  ret


leerChar:
  enter 8,0
  mov [ebp-4], ebp
  sub dword [ebp-4], 0x01
  mov ebx, 0                ;Descritor do arquivo (stdin)
  mov ecx, [ebp-4]
  mov edx, 0x1
  mov eax, 3                ;Número de chamada do sistema (sys_read)
  int 0x80
  mov eax,[ebp+8]
  mov dword eax, [ebp-1] ;resultado
  mov dword [eax], 0x1; count
  leave
  ret

leerString:
  enter 4,0
  mov dword [ebp-4], 0 ; zera o contador
  mov dword ebx, [ebp+12] ;copia o endereço para ebx
  mov dword ecx, [ebp+8] ;copia o tamanho máximo
loop2:
  push ecx ;salva o tamanho máximo
  push ebx ;salva o endereço
  push ebx ;endereço de retorno do char
  call leerChar
  pop ebx ;recupera oendereço
  pop ecx ; recupera tamanho maximo
  cmp dword [ebx], 0x0A ;se for enter pula
  je retorno2
  add dword [ebx], 0x1 ;aumenta o endereço em 1B
  add dword[ebp-4], 0x1 ; contador ++
  cmp dword [ebp-4], [ecx] ;limite máximo estabelecido pelo usuário
  jb loop2
retorno2:
  mov eax, dword[ebp-4] ; move a quantidade lida para eax
  leave
  ret

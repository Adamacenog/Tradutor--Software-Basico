
section .text



leerInteiro:
  enter 14,0
  mov dword ebp-5 , 0       ;Inicia o valor da variavel local acumuladora
  mov db ebp-6 , 0x3        ;inicia o valor da flag
  mov dword ebp-10, [ebp+8] ;endereço de retorno
  mov dword ebp-14, 0       ; contador
loop1:
  mov ebx, 0                ;Descritor do arquivo (stdin)
  mov ecx, db ebp-1
  mov edx, 0x1
  mov eax, 3                ;Número de chamada do sistema (sys_read)
  int 0x80                  ;Chamada do kernel
  cmp db[ebp-1], 0x2D
  jne mark1                 ; Se não começar com - mark1
  add dword [ebp-14], 0x1     ;contador++
  mov db ebp-6, 0x1                 ; flag de numero negativo
  jmp loop1
mark1:
  cmp db[ebp-6],0x3         ; Se a flag não for marcada
  jne mark2
  mov db ebp-6, 0x2                 ; flag de numero positivo
mark2:
  cmp db[ebp-1], 0xA
  je  operations                      ;Se for enter
  add dword [ebp-14], 0x1     ;contador++
  sub db[ebp-1], 0x30
  push dword[ebp-5]
  shl dword[ebp-5], 3 ;aux4 x 10
  pop eax
  add dword[ebp-5], eax
  add dword[ebp-5], eax
  add dword[ebp-5], db[ebp-1]
  jmp loop1
operations:
  cmp db[ebp-6],0x2
  je retorno1 ; é um numero positivo
  ;é negativo, então complemento a 2
  not dword[ebp-5] ;inverte
  add dword[ebp-5], 0x1 ; soma 1
retorno1:
  mov dword [ebp-10], dword [ebp-5] ;resultado
  mov eax, dword [ebp-14] ;contador
  leave


leerChar:
  enter 5,0
  mov dword ebp-5, [ebp+8]
  mov ebx, 0                ;Descritor do arquivo (stdin)
  mov ecx, db ebp-1
  mov edx, 0x1
  mov eax, 3                ;Número de chamada do sistema (sys_read)
  int 0x80
  mov dword[ebp-5], db [ebp-1] ;resultado
  mov eax, 0x1; count
  leave

leerString:
  enter 9,0
  mov dword ebp-4, 0
  mov dword ebp-8, [ebp+8]
loop2:
  push dword ebp-9 ;endereço de retorno do char
  call leerChar
  pop eax ;só pra jogar fora
  cmp [ebp-9], 0xA ;se for enter pula
  je retorno2
  mov db [ebp-8], eax
  add [ebp-8], 0x1 ;aumenta o endereço em 1B
  add [ebp-4], 0x1 ; contador ++
  cmp [ebp-4], 0x64 ;limite máximo de 100 caracteres
  jbe loop2
retorno2:
  mov eax, dword[ebp-4]
  leave

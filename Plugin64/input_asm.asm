EXTERN	inputProc1ReturnAddress	:	QWORD
EXTERN	inputProc2ReturnAddress	:	QWORD
EXTERN	inputProc1CallAddress	:	QWORD

ESCAPE_SEQ_1	=	10h
ESCAPE_SEQ_2	=	11h
ESCAPE_SEQ_3	=	12h
ESCAPE_SEQ_4	=	13h
LOW_SHIFT		=	0Eh
HIGH_SHIFT		=	9h
SHIFT_2			=	LOW_SHIFT
SHIFT_3			=	900h
SHIFT_4			=	8F2h
NO_FONT			=	98Fh
NOT_DEF			=	2026h

.DATA
	inputProc1Var1	DB		03,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00

.CODE
inputProc1 PROC
	; eax�ɂ�IME����utf8�̕������n����Ă���
	mov		eax, dword ptr [rbp + 120h - 198h + 0Ch];
	; ah��0�ł����a-z�Ȃǂ�1byte�Ŏ��܂镶���Ȃ̂ŁA�ϊ������͕K�v�Ȃ�
	cmp		ah, 0;
	jnz		JMP_A;
	xor		bl, bl;

	; JMP_C,X,Y�ɂ��Ă̐����BMakeJMP�ŃR�[�h���j�󂳂�Ă��܂����߁A�������ۂ��ƃR�s�[���Ă��Ă���B
JMP_C:
	; ������80h�Ɣ�r���Ă���̂�UTF8��U+0000 �c U+007F���ǂ����m�F���邽��
	; https://ja.wikipedia.org/wiki/UTF-8
	cmp		al, 80h;
	jnb		JMP_X;
	mov		ebx, eax;
	jmp     JMP_Y;

JMP_X:
	cmp		al, 0E0h;
	jnb		JMP_Y;
	movzx	ebx, byte ptr [rbp + 120h - 198h + 0Dh];
	and		bl, 3Fh;
	shl		al, 6;
	or		bl, al;

JMP_Y:
	push	inputProc1ReturnAddress;
	ret;

JMP_A:
	lea		rcx,[rbp + 120h - 198h + 0Ch];
	call	inputProc1CallAddress;
	mov		ebx, dword ptr [rax];

	; ���̂܂܃R�s�[����
	mov		rax, [r14];
	lea     rdx, [rsp+200h - 1D0h];
	xorps	xmm0, xmm0;
	mov		qword ptr [rsp+200h - 1D0h], 0;
	movdqu	xmmword ptr [rsp + 200h - 1C0h], xmm0;
	mov		rcx, r14;
	mov		dword ptr [rsp + 200h + 1C8h], edi;
	movdqa  xmm0, xmmword ptr [inputProc1Var1];
	movdqu  xmmword ptr [rsp + 200h - 198h], xmm0;
	mov		byte ptr [rsp + 200h - 1C4h], bl;
	mov		qword ptr [rsp + 200h - 1B0h], 0;
	mov		dword ptr [rsp + 200h - 1A8h], edi;
	mov		qword ptr [rsp + 200h - 1A0h], rdi;
	mov		dword ptr [rsp + 200h - 188h], edi;
	mov		dword ptr [rbp + 120h - 1A0h], 2;
	mov		word ptr [rbp + 120h - 1A0h + 4], di;
	mov		byte ptr [rbp + 120h - 1A0h + 6], 0;
	call	qword ptr [rax + 18h];

JMP_B:
	push	inputProc2ReturnAddress;
	ret;

inputProc1 ENDP
END
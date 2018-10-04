#include "stdinc.h"
#include "byte_pattern.h"

/*  �ꕔ��UI�A�{�������\�� */
namespace TextView {
	/*-----------------------------------------------*/

	errno_t changeStackSize_hook(EU4Version version) {
		std::string desc = "change stack size";

		switch (version) {
		case v1_27_X:
			byte_pattern::temp_instance().find_pattern("81 EC A8 06 00 00 56 57");
			if (byte_pattern::temp_instance().has_size(1, desc)) {
				// sub esp,6A8h
				injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(3), 0xAA, true);
			}
			else return EU4_ERROR1;
			return NO_ERROR;

		case v1_25_X:
		case v1_26_X:
			byte_pattern::temp_instance().find_pattern("81 EC 20 06 00 00 56 57");
			if (byte_pattern::temp_instance().has_size(1, desc)) {
				// sub esp,620h
				injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(2), 0x24, true);
			}
			else return EU4_ERROR1;
			return NO_ERROR;
		}
		return EU4_ERROR1;
	}

	/*-----------------------------------------------*/

	uintptr_t diff;

	/*-----------------------------------------------*/
	
	errno_t prepareCopyBuff_hook(EU4Version version) {
		std::string desc = "prepare copying text to buffer";
		
		switch (version) {
		case v1_27_X:
			byte_pattern::temp_instance().find_pattern("8A 04 06 88 82 ? ? ? ? 42");
			if (byte_pattern::temp_instance().has_size(1, "v1.27.X")) {
				// mov byte_XXXXX[edx],al -> get byte_XXXXX
				char *pDST = *byte_pattern::temp_instance().get_first().pointer<char *>(5);

				// mov byte_XXXXX[edx],al -> lea edi, [byte_XXXXX]
				injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(3), 0x8D, true);
				injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(4), 0x3D, true);
				injector::WriteMemory<char *>(byte_pattern::temp_instance().get_first().address(5), pDST, true);
			}
			else return EU4_ERROR1;
			return NOERROR;

		case v1_26_X:
			byte_pattern::temp_instance().find_pattern("8A 04 0F 88 86 ? ? ? ? 46");
			if (byte_pattern::temp_instance().has_size(1, "v1.26.X")) {
				// mov byte_XXXXX[esi],al -> get byte_XXXXX
				char *pDST = *byte_pattern::temp_instance().get_first().pointer<char *>(5);

				// mov byte_XXXXX[esi],al  -> lea edx, byte_XXXXX
				injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(3), 0x8D, true);
				injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(4), 0x15, true);
				injector::WriteMemory<char *>(byte_pattern::temp_instance().get_first().address(5), pDST, true);
			}
			else return EU4_ERROR1;
			return NOERROR;

		case v1_25_X:
			byte_pattern::temp_instance().find_pattern("8A 87 ? ? ? ? 88 86 ? ? ? ? 46");
			if (byte_pattern::temp_instance().has_size(1, "v1.25.X")) {
				// TODO
				char *pSRC = *byte_pattern::temp_instance().get_first().pointer<char *>(2);
				// TODO
				char *pDST = *byte_pattern::temp_instance().get_first().pointer<char *>(8);

				diff = pDST - pSRC;

				// TODO
				injector::WriteMemory<char *>(byte_pattern::temp_instance().get_first().address(8), pSRC, true);
				injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(0), 0x8D, true);
				injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(1), 0x15, true);
				injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(6), 0x8D, true);
				injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(7), 0x15, true);
			}
			else return EU4_ERROR1;
			return NOERROR;
		}

		return EU4_ERROR1;
	}

	/*-----------------------------------------------*/

	uintptr_t text1_v125_end2;
	uintptr_t text1_v125_end;	
	__declspec(naked) void text1_v125_start()
	{
		__asm {
			push ebx;
			mov ebx, edx;
			add ebx, diff;

			mov al, [edx + edi];
			mov[ebx + esi], al;
			inc esi;

			cmp al, ESCAPE_SEQ_1;
			jz f_10;
			cmp al, ESCAPE_SEQ_2;
			jz f_11;
			cmp al, ESCAPE_SEQ_3;
			jz f_12;
			cmp al, ESCAPE_SEQ_4;
			jz f_13;
			jmp f_4;

		f_10:
			movzx eax, word ptr[edx + edi + 1];
			mov[ebx + esi], ax;
			jmp f_3;

		f_11:
			movzx eax, word ptr[edx + edi + 1];
			mov[ebx + esi], ax;
			sub eax, SHIFT_2;
			jmp f_3;

		f_12:
			movzx eax, word ptr[edx + edi + 1];
			mov[ebx + esi], ax;
			add eax, SHIFT_3;
			jmp f_3;

		f_13:
			movzx eax, word ptr[edx + edi + 1];
			mov[ebx + esi], ax;
			add eax, SHIFT_4;

		f_3:
			pop ebx;

			movzx eax, ax;
			add esi, 2;
			cmp eax, NO_FONT;

			ja f_2;
			mov eax, NOT_DEF;

		f_2:
			add edi, 2;
			mov[ebp - 0x24], edi;

			jmp loc_1996B6B_jmp;

		f_4:
			pop ebx;
			test cl, cl;
			jz f_c;

			mov edx, [ebp - 0x10];

			push text1_v125_end;
			ret;

		f_c:
			movzx eax, al;

		loc_1996B6B_jmp:

			push text1_v125_end2;
			ret;
		}
	}

	uintptr_t text1_v126_end;
	__declspec(naked) void text1_v126_start()
	{
		// edx��DST�̃A�h���X������
		__asm {
			xor eax, eax;
			mov al, byte ptr[edi + ecx];
			mov byte ptr[edx + esi], al;
			inc esi;

			cmp al, ESCAPE_SEQ_1;
			jz f_10;
			cmp al, ESCAPE_SEQ_2;
			jz f_11;
			cmp al, ESCAPE_SEQ_3;
			jz f_12;
			cmp al, ESCAPE_SEQ_4;
			jz f_13;
			jmp f_4;

		f_10:
			movzx eax, word ptr[edi + ecx + 1];
			mov[edx + esi], ax;
			jmp f_3;

		f_11:
			movzx eax, word ptr[edi + ecx + 1];
			mov[edx + esi], ax;
			sub eax, SHIFT_2;
			jmp f_3;

		f_12:
			movzx eax, word ptr[edi + ecx + 1];
			mov[edx + esi], ax;
			add eax, SHIFT_3;
			jmp f_3;

		f_13:
			movzx eax, word ptr[edi + ecx + 1];
			mov[edx + esi], ax;
			add eax, SHIFT_4;

		f_3:
			movzx eax, ax;
			add esi, 2;
			cmp eax, NO_FONT;

			ja f_2;
			mov eax, NOT_DEF;

		f_2:
			add edi, 2;
			mov[ebp - 0x24], edi;

			jmp loc_1996B6B_jmp;

		f_4:
			cmp byte ptr[ebp + 0x3C], 0; // arg_34
			jz loc_1996B6B_jmp;

			mov edx, [ebp - 0x10];

			push text1_v126_end;
			ret;

		loc_1996B6B_jmp:

			push text1_v125_end2;
			ret;
		}
	}

	uintptr_t text1_v127_end;
	__declspec(naked) void text1_v127_start()
	{
		// al�ɂ͎��o����1byte�������Ă���
		// edi��DST�̃A�h���X������
		// edi�͏㏑�����Ă��܂��Ă���̂ōŌ��[ebp-14h]�����Ė߂�
		__asm {
			// �S�~�������Ă���̂�����
			movzx eax, al;

			mov byte ptr[edi + edx], al;

			inc edx;

			cmp al, ESCAPE_SEQ_1;
			jz f_10;
			cmp al, ESCAPE_SEQ_2;
			jz f_11;
			cmp al, ESCAPE_SEQ_3;
			jz f_12;
			cmp al, ESCAPE_SEQ_4;
			jz f_13;
			jmp f_4;

		f_10:
			mov eax, [ebp - 0x18];
			movzx eax, word ptr[esi + eax + 1];
			mov[edi + edx], ax;
			jmp f_3;

		f_11:
			mov eax, [ebp - 0x18];
			movzx eax, word ptr[esi + eax + 1];
			mov[edi + edx], ax;
			sub eax, SHIFT_2;
			jmp f_3;

		f_12:
			mov eax, [ebp - 0x18];
			movzx eax, word ptr[esi + eax + 1];
			mov[edi + edx], ax;
			add eax, SHIFT_3;
			jmp f_3;

		f_13:
			mov eax, [ebp - 0x18];
			movzx eax, word ptr[esi + eax + 1];
			mov[edi + edx], ax;
			add eax, SHIFT_4;

		f_3:
			movzx eax, ax;
			add edx, 2;
			cmp eax, NO_FONT;

			ja f_2;
			mov eax, NOT_DEF;

		f_2:
			add esi, 2;
			mov[ebp - 0x28], esi;

		f_4:
			mov edi, [ebp - 0x14]; // ���Ƃɖ߂�

			cmp byte ptr[ebx + 0x3C], 0; // arg_34, ebp�ł͂Ȃ�ebx�Ȃ��Ƃɒ���

			/* 1.27.X���炻�̂܂�eax��code-point���c��Ȃ��Ȃ����̂ŁA�g�������X�^�b�N�ɕۑ� */
			mov word ptr [ebp - 0x6B8], ax; // �ۑ�

			mov eax, [ebp - 0x18];

			push text1_v127_end;
			ret;
		}
	}

	/*-----------------------------------------------*/

	errno_t copyBuff_hook(EU4Version version) {
		std::string desc = "copy text to buffer";

		switch (version) {
		case v1_27_X:
			byte_pattern::temp_instance().find_pattern("42 80 7B 3C 00 8B 45 E8");
			if (byte_pattern::temp_instance().has_size(1, desc)) {
				// inc edx
				injector::MakeJMP(byte_pattern::temp_instance().get_first().address(), text1_v127_start);
				// mov [ebp-0x30],edx
				text1_v127_end = byte_pattern::temp_instance().get_first().address(0x8);
			}
			else return EU4_ERROR1;
			return NOERROR;

		case v1_26_X:
			byte_pattern::temp_instance().find_pattern("46 80 7D 3C 00 0F 84 32");
			if (byte_pattern::temp_instance().has_size(1, desc)) {
				// inc esi
				injector::MakeJMP(byte_pattern::temp_instance().get_first().address(), text1_v126_start);
				// mov al,[edi+ecx]
				text1_v126_end = byte_pattern::temp_instance().get_first().address(0xB);
			}
			else return EU4_ERROR1;
			return NOERROR;

		case v1_25_X:
			byte_pattern::temp_instance().find_pattern("46 84 C9 0F 84 B6");
			if (byte_pattern::temp_instance().has_size(1, desc)) {
				// TODO
				injector::MakeJMP(byte_pattern::temp_instance().get_first().address(), text1_v125_start);
				// TODO
				text1_v125_end = byte_pattern::temp_instance().get_first().address(0x9);
			}
			else return EU4_ERROR1;
			return NOERROR;
		}

		return EU4_ERROR1;
	}

	/*-----------------------------------------------*/

	uintptr_t text2_v126_end;
	__declspec(naked) void text2_v126_start()
	{
		__asm {
			movzx eax, ax;
			mov edx, [ebp - 0x40];
			mov word ptr[ebp - 0x630], ax;
			mov ecx, [edx + eax * 4];

			push text2_v126_end;
			ret;
		}
	}

	uintptr_t text2_v125_end;
	__declspec(naked) void text2_v125_start()
	{
		__asm {
			movzx eax, ax;
			mov[ebp - 0x630], ax;
			mov ecx, [edx + eax * 4];

			push text2_v125_end;
			ret;
		}
	}

	uintptr_t text2_v127_end;
	__declspec(naked) void text2_v127_start()
	{
		__asm {
			// text1�łƂ��Ă�����code-point���`�F�b�N
			cmp word ptr[ebp - 0x6B8], 0xFF;
			ja f_1;

			movzx eax, byte ptr[esi + eax];
			jmp f_2;

		f_1:
			movzx eax, word ptr[ebp - 0x6B8];
		f_2:
			mov eax, [edi + eax*4 + 0xB4];

			push text2_v127_end;
			ret;
		}
	}

	/*-----------------------------------------------*/

	errno_t getCharInFont_hook(EU4Version version) {
		std::string desc = "get char in font";

		switch (version) {
		case v1_27_X:
			byte_pattern::temp_instance().find_pattern("0F B6 04 06 8B 84 87 B4 00 00 00");
			if (byte_pattern::temp_instance().has_size(1, desc)) {
				// movzx eax, byte ptr [esi+eax]
				injector::MakeJMP(byte_pattern::temp_instance().get_first().address(), text2_v127_start);
				// mov [ebp - 4Ch], eax
				text2_v127_end = byte_pattern::temp_instance().get_first().address(0xB);
			}
			else return EU4_ERROR1;
			return NOERROR;

		case v1_26_X:
			byte_pattern::temp_instance().find_pattern("0F B6 04 0F 8B 55 C0 8B");
			if (byte_pattern::temp_instance().has_size(1, "v1.26.X")) {
				// movzx eax, byte ptr [edi+ecx]
				text1_v125_end2 = byte_pattern::temp_instance().get_first().address();
				// movzx eax, byte ptr [edi+ecx]
				injector::MakeJMP(byte_pattern::temp_instance().get_first().address(), text2_v126_start);
				// mov [ebp - 4Ch], ecx
				text2_v126_end = byte_pattern::temp_instance().get_first().address(0xA);
			}
			else return EU4_ERROR1;
			return NOERROR;

		case v1_25_X:
			//1.25.1.0
			byte_pattern::temp_instance().find_pattern("8B 55 C4 0F B6 C0 8B");
			if (byte_pattern::temp_instance().has_size(1, "v1.25.X")) {
				// TODO
				text1_v125_end2 = byte_pattern::temp_instance().get_first().address();
				// TODO
				injector::MakeJMP(byte_pattern::temp_instance().get_first().address(0x3), text2_v125_start);
				// TODO
				text2_v125_end = byte_pattern::temp_instance().get_first().address(0x9);
			}
			else return EU4_ERROR1;
			return NOERROR;
		}

		return EU4_ERROR1;
	}

	/*-----------------------------------------------*/

	uintptr_t text3_v125_end;
	uintptr_t text3_v125_end2;
	__declspec(naked) void text3_v125_start()
	{
		__asm {
			cmp al, 0xA4;
			jnz f_d;
			movss xmm2, [ebp + 0x18]; //arg_10

			push text3_v125_end;
			ret;

		f_d:
			cmp al, 0xA7;
			jnz f_c_t;

			push text3_v125_end2;
			ret;

		f_c_t:
			movzx eax, al; // ���ꂪ��肽������

			push text1_v125_end2;
			ret;
		}
	}

	/*-----------------------------------------------*/

	errno_t fixCopyBuff_hook(EU4Version version) {
		std::string desc = "fix copy buff";

		switch (version) {
		case v1_27_X:
			/* text1�̏C���Ȃ̂ŃX�L�b�v���� */
			return NOERROR;
		case v1_26_X:
		case v1_25_X:
			byte_pattern::temp_instance().find_pattern("3C A4 75 17 F3");
			if (byte_pattern::temp_instance().has_size(1, desc)) {
				// cmp al,A4h
				injector::MakeJMP(byte_pattern::temp_instance().get_first().address(), text3_v125_start);
				// addss xmm2, ds:dword_XXXXX
				text3_v125_end = byte_pattern::temp_instance().get_first().address(0x9);
				// mov al, [edi+ecx+1]
				text3_v125_end2 = byte_pattern::temp_instance().get_first().address(0x1F);
			}
			else return EU4_ERROR1;
			return NOERROR;
		}

		return EU4_ERROR1;
	}

	/*-----------------------------------------------*/

	uintptr_t text4_v125_end;
	uintptr_t text4_v125_end2;
	__declspec(naked) void text4_v125_start()
	{
		__asm {
			cmp word ptr[ecx + 6], 0;
			jnz g_3_jmp;
			jmp g_5_jmp;

		g_3_jmp:
			cmp word ptr[ebp - 0x630], 0xFF;
			ja g_5_jmp;

			push text4_v125_end2;
			ret;

		g_5_jmp:
			push text4_v125_end;
			ret;
		}
	}

	uintptr_t text4_v127_end;
	uintptr_t text4_v127_end2;
	__declspec(naked) void text4_v127_start()
	{
		__asm {
			mov eax, [ebp - 0x4C];

			cmp word ptr[eax + 6], 0;
			jnz g_3_jmp;
			jmp g_5_jmp;

		g_3_jmp:
			cmp word ptr[ebp - 0x6B8], 0xFF;
			ja g_5_jmp;

			push text4_v127_end2;
			ret;

		g_5_jmp:
			push text4_v127_end;
			ret;
		}
	}

	/*-----------------------------------------------*/

	errno_t lineBreak_hook(EU4Version version) {
		std::string desc = "line break";

		switch (version) {
		case v1_27_X:
			byte_pattern::temp_instance().find_pattern("8B 45 B4 66 83 78 06 00");
			if (byte_pattern::temp_instance().has_size(1, desc)) {
				// mov eax,[ebp-4Ch]
				injector::MakeJMP(byte_pattern::temp_instance().get_first().address(), text4_v127_start);
				// mov eax, [ebp+2Ch]
				text4_v127_end = byte_pattern::temp_instance().get_first().address(0xE);
				// jnz loc_XXXXX
				text4_v127_end2 = byte_pattern::temp_instance().get_first().address(8);
				// jnz loc_XXXXX -> jmp loc_XXXXX
				injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(8), 0xE9, true);
				injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(9), 0x47, true);
				injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(10), 0x01, true);
				injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(11), 0x00, true);
			}
			else return EU4_ERROR1;

			return NOERROR;

		case v1_26_X:
		case v1_25_X:
			byte_pattern::temp_instance().find_pattern("66 83 79 06 00 0F");
			if (byte_pattern::temp_instance().has_size(1, "v1.25.X-v1.26.X")) {
				// cmp word ptr [ecx+6],0
				injector::MakeJMP(byte_pattern::temp_instance().get_first().address(), text4_v125_start);
				// mov eax, [ebp+arg_24]
				text4_v125_end = byte_pattern::temp_instance().get_first().address(0xB);
			}
			else return EU4_ERROR1;

			switch (version) {
			case v1_26_X:
				byte_pattern::temp_instance().find_pattern("80 3D ? ? ? ? 00 0F 84 98 01");
				if (byte_pattern::temp_instance().has_size(1, desc)) {
					// cmp byte_XXXXX, 0
					text4_v125_end2 = byte_pattern::temp_instance().get_first().address();
				}
				else return EU4_ERROR1;

			case v1_25_X:
				byte_pattern::temp_instance().find_pattern("80 3D ? ? ? ? 00 0F 84 A9 01 00 00 8B");
				if (byte_pattern::temp_instance().has_size(1, desc)) {
					// TODO
					text4_v125_end2 = byte_pattern::temp_instance().get_first().address();
				}
				else return EU4_ERROR1;

			default:
				return EU4_ERROR1;
			}

			return NOERROR;
		}
	}

	/*-----------------------------------------------*/

	uintptr_t text5_v125_end;
	__declspec(naked) void text5_v125_start()
	{
		__asm {
			cmp byte ptr[ecx], ESCAPE_SEQ_1;
			jz h_10;
			cmp byte ptr[ecx], ESCAPE_SEQ_2;
			jz h_11;
			cmp byte ptr[ecx], ESCAPE_SEQ_3;
			jz h_12;
			cmp byte ptr[ecx], ESCAPE_SEQ_4;
			jz h_13;
			jmp h_4;

		h_10:
			movzx eax, word ptr[ecx + 1];
			jmp h_2;

		h_11:
			movzx eax, word ptr[ecx + 1];
			sub eax, SHIFT_2;
			jmp h_2;

		h_12:
			movzx eax, word ptr[ecx + 1];
			add eax, SHIFT_3;
			jmp h_2;

		h_13:
			movzx eax, word ptr[ecx + 1];
			add eax, SHIFT_4;
		
		h_2:
			movzx eax, ax;
			add edx, 2;
			mov [ebp + 0x34], edx;
			cmp eax, NO_FONT;
			
			ja h_3;
			mov eax, NOT_DEF;

		h_3:
			jmp h_5;

		h_4:
			mov cl, [ecx];
			movzx eax, cl;

		h_5:
			movss xmm3, dword ptr[edi + 0x4DC];

			push text5_v125_end;
			ret;
		}
	}

	uintptr_t text5_v127_end;
	__declspec(naked) void text5_v127_start()
	{
		__asm {

			cmp byte ptr[eax], ESCAPE_SEQ_1;
			jz h_10;
			cmp byte ptr[eax], ESCAPE_SEQ_2;
			jz h_11;
			cmp byte ptr[eax], ESCAPE_SEQ_3;
			jz h_12;
			cmp byte ptr[eax], ESCAPE_SEQ_4;
			jz h_13;
			jmp h_4;

		h_10:
			movzx eax, word ptr[eax + 1];
			jmp h_2;

		h_11:
			movzx eax, word ptr[eax + 1];
			sub eax, SHIFT_2;
			jmp h_2;

		h_12:
			movzx eax, word ptr[eax + 1];
			add eax, SHIFT_3;
			jmp h_2;

		h_13:
			movzx eax, word ptr[eax + 1];
			add eax, SHIFT_4;

		h_2:
			movzx eax, ax;
			add edi, 2;
			cmp eax, NO_FONT;

			ja h_3;
			mov eax, NOT_DEF;

		h_3:
			jmp h_5;

		h_4:
			movzx eax, byte ptr [eax];

		h_5:
			mov ecx, [ebp - 0x54];
			mov [ebp - 0x4C], eax; // dword �ł悢�H

			push text5_v127_end;
			ret;
		}
	}

	/*-----------------------------------------------*/

	uintptr_t text6_v125_end;
	__declspec(naked) void text6_v125_start()
	{
		__asm {
			movzx eax, ax;
			mov esi, [edi + eax * 4 + 0xB4];

			push text6_v125_end;
			ret;
		}
	}

	/*-----------------------------------------------*/

	errno_t actualCharView_hook(EU4Version version) {
		std::string desc = "actual char viewing";

		switch (version) {
		case v1_27_X:
			byte_pattern::temp_instance().find_pattern("8A 87 ? ? ? ? 8B 4D AC");
			if (byte_pattern::temp_instance().has_size(1, desc)) {
				// mov al, byte_XXXXX[edi] -> lea eax, byte_XXXXX[edi]
				injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(), 0x8D, true);
				// ecx,[ebp-54h]
				injector::MakeJMP(byte_pattern::temp_instance().get_first().address(0x6), text5_v127_start);
				// xmm3, dword ptr [ecx+428h]
				text5_v127_end = byte_pattern::temp_instance().get_first().address(0xF);
				//text6��text5�ɓ�������
			}
			else return EU4_ERROR1;
			return NOERROR;

		case v1_26_X:
		case v1_25_X:
			byte_pattern::temp_instance().find_pattern("8A 8A ? ? ? ? F3 0F");
			if (byte_pattern::temp_instance().has_size(1, desc)) {
				// mov cl, byte_XXXXX[edx] -> lea ecx, byte_XXXXX[edx]
				injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(), 0x8D, true);
				// movss xmm3, dword ptr [edi+4DCh]
				injector::MakeJMP(byte_pattern::temp_instance().get_first().address(0x6), text5_v125_start);
				// movss [ebp-0xF4],xmm3
				text5_v125_end = byte_pattern::temp_instance().get_first().address(0x11);
				// mov esi,[edi+eax*4+0xB4]
				injector::MakeJMP(byte_pattern::temp_instance().get_first().address(0x19), text6_v125_start);
				// lea eax, [edi+0xB4]
				text6_v125_end = byte_pattern::temp_instance().get_first().address(0x20);
			}
			else return EU4_ERROR1;
			return NOERROR;
		}

		return EU4_ERROR1;
	}

	/*-----------------------------------------------*/

	errno_t init(EU4Version version) {
		errno_t result = 0;

		byte_pattern::temp_instance().debug_output2("main text fix");

		/* �X�^�b�N�T�C�Y�̒��� */
		result |= changeStackSize_hook(version);
		/* �]����o�b�t�@�̃|�C���^���m�� */
		result |= prepareCopyBuff_hook(version);
		/* �o�b�t�@�ɃR�s�[ */
		result |= copyBuff_hook(version);
		/* �t�H���g���當�����擾 */
		result |= getCharInFont_hook(version);
		/* �~�X�����C�� */
		result |= fixCopyBuff_hook(version);
		/* ���s�̏��� */
		result |= lineBreak_hook(version);
		/* �\�������ɓn������ */
		result |= actualCharView_hook(version);

		return result;
	}
}
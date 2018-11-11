#include "stdinc.h"
#include "byte_pattern.h"

/*  issue66 : https://github.com/matanki-saito/EU4dll/issues/66 */
namespace Issue66 {

	/*-----------------------------------------------*/
	
	uintptr_t issue66_copyBuff1_v127_start;
	uintptr_t issue66_copyBuff2_v127_start;

	/*-----------------------------------------------*/

	errno_t copyBuffFunc_hook(EU4Version version) {
		std::string desc = "copyBuffFunc";

		switch (version) {
		case v1_27_X:
			byte_pattern::temp_instance().find_pattern("83 EC 20 56 FF 75 0C 8B D1 C7 45");
			/* 2�Ƃ��擾���� */
			if (byte_pattern::temp_instance().has_size(2, desc)) {
				// sub esp,20h
				issue66_copyBuff1_v127_start = byte_pattern::temp_instance().get_first().address(-0x18);
				// sub esp,20h
				issue66_copyBuff2_v127_start = byte_pattern::temp_instance().get_first().address(-0x18);
			}
			else return EU4_ERROR1;
			return NOERROR;
		case v1_26_X:
		case v1_25_X:
			/* �Ή����Ȃ� */
			return NOERROR;
		}

		return EU4_ERROR1;
	}

	/*-----------------------------------------------*/

	uintptr_t issue66_createBuff_v127_start;

	/*-----------------------------------------------*/

	errno_t createBuffFunc_hook(EU4Version version) {
		std::string desc = "createBuffFunc";

		switch (version) {
		case v1_27_X:
			byte_pattern::temp_instance().find_pattern("83 EC 28 56 8B F1 89 75 EC 57 C7 46");
			if (byte_pattern::temp_instance().has_size(1, desc)) {
				// sub esp,28h
				issue66_createBuff_v127_start = byte_pattern::temp_instance().get_first().address(-0x18);
			}
			else return EU4_ERROR1;
			return NOERROR;
		case v1_26_X:
		case v1_25_X:
			/* �Ή����Ȃ� */
			return NOERROR;
		}

		return EU4_ERROR1;
	}

	/*-----------------------------------------------*/

	uintptr_t issue66_copyText_v127_start;

	/*-----------------------------------------------*/

	errno_t copyText_hook(EU4Version version) {
		std::string desc = "copyText";

		switch (version) {
		case v1_27_X:
			byte_pattern::temp_instance().find_pattern("55 8B EC 53 8B 5D 08 56 8B F1 85 DB 74 57");
			if (byte_pattern::temp_instance().has_size(2, desc)) {
				// push ebp
				issue66_copyText_v127_start = byte_pattern::temp_instance().get(1).address();
			}
			else return EU4_ERROR1;
			return NOERROR;
		case v1_26_X:
		case v1_25_X:
			/* �Ή����Ȃ� */
			return NOERROR;
		}

		return EU4_ERROR1;
	}

	/*-----------------------------------------------*/

	char *SPACE = " ";

	union T {
		char text[0x10];
		char* p;
	};

	typedef struct V {
		union T t;
		int len;
		int len2;
	} Vs;

	V *year;
	V *day;

	uintptr_t issue66_dateFormat_v127_end;
	__declspec(naked) void issue66_dateFormat_v127_start() {
		__asm {
			// �X�y�[�X���e�L�X�g�o�b�t�@�Ɋi�[����i�_�~�[�j
			mov     esi, eax;
			mov     byte ptr[ebp - 0x4], 2;
			push    1;
			push    SPACE;
			lea     ecx, dword ptr [ebp - 0x28];
			mov     dword ptr [ebp - 0x14], 0x0F;
			mov     dword ptr [ebp - 0x18], 0;
			mov     byte ptr[ebp - 0x28], 0;
			call    issue66_copyText_v127_start;

			// �����e�L�X�g�o�b�t�@�ɕϊ�
			push    edi;
			lea     ecx, dword ptr [ebp - 0xA0 ];
			mov     byte ptr[ebp - 0x4], 3;
			call    issue66_createBuff_v127_start;

			// �e�L�X�g�o�b�t�@��year����������
			//lea     ecx, year;
			mov     byte ptr[ebp - 0x4], 4;
			push    year;
			lea     ecx, dword ptr [ebp - 0x88];
			push    ecx;
			mov     ecx, esi;
			call    issue66_copyBuff2_v127_start;

			//�o�b�t�@�Ɍ�����������
			lea     ecx, dword ptr [ebp - 0xB8];
			mov     byte ptr[ebp - 0x4], 5;
			push    ecx;
			lea     ecx, dword ptr [ebp - 0x70];
			push    ecx;
			mov     ecx, eax;
			call    issue66_copyBuff1_v127_start;


			// �o�b�t�@�ɓ�����������
			lea ecx,    dword ptr[ebp - 0xA0];
			push ecx;
			lea     ecx, dword ptr [ebp - 0x40];
			mov     byte ptr[ebp - 0x4], 7;
			push    ecx;
			mov     ecx, eax;
			call    issue66_copyBuff2_v127_start;

			// �o�b�t�@�Ɂu���v����������
			push    day;
			lea     ecx, dword ptr[ebp - 0x58];
			mov     byte ptr[ebp - 0x4], 6;
			push    ecx;
			mov     ecx, eax;
			call    issue66_copyBuff1_v127_start;

			push issue66_dateFormat_v127_end;
			ret;
		}
	}

	/*-----------------------------------------------*/

	errno_t genDateFormat_hook(EU4Version version) {
		std::string desc = "gen date format hook";

		day = new V();
		day->t.text[0] = 0xE;
		day->t.text[1] = '\0';
		day->len = 1;
		day->len2 = 0xF;

		year = new V();
		year->t.text[0] = 0xF;
		year->t.text[1] = '\0';
		year->len = 1;
		year->len2 = 0xF;

		switch (version) {
		case v1_27_X:
			byte_pattern::temp_instance().find_pattern("8B F0 C6 45 FC 02 8D 4D D8 6A 01");
			if (byte_pattern::temp_instance().has_size(1, desc)) {
				// mov esi,eax
				injector::MakeJMP(byte_pattern::temp_instance().get_first().address(), issue66_dateFormat_v127_start);

				// push 0xFFFFFFFF
				issue66_dateFormat_v127_end = byte_pattern::temp_instance().get_first().address(0x87);
			}
			else return EU4_ERROR1;
			return NOERROR;
		case v1_26_X:
		case v1_25_X:
			/* �Ή����Ȃ� */
			return NOERROR;
		}

		return EU4_ERROR1;
	}

	/*-----------------------------------------------*/

	errno_t init(EU4Version version) {
		errno_t result = 0;

		byte_pattern::temp_instance().debug_output2("issue66");

		/* �R�s�[�o�b�t�@�֐����t�b�N */
		result |= copyBuffFunc_hook(version);

		/* �o�b�t�@�쐬�֐����t�b�N */
		result |= createBuffFunc_hook(version);

		/* �R�s�[�e�L�X�g�쐬�֐����t�b�N */
		result |= copyText_hook(version);

		/**/
		result |= genDateFormat_hook(version);

		return result;
	}
}
#include "pch.h"
#include "plugin_64.h"

namespace MainText {
	extern "C" {
		void mainTextProc1();
		void mainTextProc2();
		void mainTextProc3();
		uintptr_t mainTextProc1ReturnAddress;
		uintptr_t mainTextProc2ReturnAddress;
		uintptr_t mainTextProc2BufferAddress;
		uintptr_t mainTextProc3ReturnAddress1;
		uintptr_t mainTextProc3ReturnAddress2;
	}

	DllError mainTextProc1Injector(RunOptions options) {
		DllError e = {};

		switch (options.version) {
		case v1_29_1_0:
			// movsxd rax, edi
			BytePattern::temp_instance().find_pattern("48 63 C7 0F B6 04 18 F3 41 0F 10 9F 48 08 00 00");
			if (BytePattern::temp_instance().has_size(1, "�e�L�X�g�������[�v�Q")) {
				uintptr_t address = BytePattern::temp_instance().get_first().address();

				// movss dword ptr [rpb+108h], xmm3
				mainTextProc1ReturnAddress = address + 0x1B;

				Injector::MakeJMP(address, mainTextProc1, true);
			}
			else {
				e.unmatch.mainTextProc1Injector = true;
			}
		default:
			e.version.mainTextProc1Injector = true;
		}

		return e;
	}

	DllError mainTextProc2Injector(RunOptions options) {
		DllError e = {};

		switch (options.version) {
		case v1_29_1_0:
			// movsxd rdx, edi
			BytePattern::temp_instance().find_pattern("48 63 D7 49 63 CE 4C 8B 54 24 78");
			if (BytePattern::temp_instance().has_size(1, "�e�L�X�g�������[�v�P")) {
				uintptr_t address = BytePattern::temp_instance().get_first().address();

				// cmp byte ptr [rbp+7B0h],0
				mainTextProc2ReturnAddress = address + 0x1E;

				// lea r9, {unk_XXXXX}
				mainTextProc2BufferAddress = Injector::GetBranchDestination(address + 0x10).as_int();

				Injector::MakeJMP(address, mainTextProc2, true);
			}
			else {
				e.unmatch.mainTextProc2Injector = true;
			}
		default:
			e.version.mainTextProc2Injector = true;
		}

		return e;
	}

	DllError mainTextProc3Injector(RunOptions options) {
		DllError e = {};

		switch (options.version) {
		case v1_29_1_0:
			// cmp cs:byte_xxxxx, 0
			BytePattern::temp_instance().find_pattern("80 3D ? ? ? ? 00 0F 84 97 01 00 00");
			if (BytePattern::temp_instance().has_size(1, "�e�L�X�g�������[�v�P�̉��s�����̖߂��Q")) {
				mainTextProc3ReturnAddress2 = BytePattern::temp_instance().get_first().address();
			}
			else {
				e.unmatch.mainTextProc3Injector2 = true;
			}

			// cmp word ptr [rcx+6],0
			BytePattern::temp_instance().find_pattern("66 83 79 06 00 0F 85 15 01 00 00");
			if (BytePattern::temp_instance().has_size(1, "�e�L�X�g�������[�v�P�̉��s�������C��")) {
				uintptr_t address = BytePattern::temp_instance().get_first().address();

				// cvtdq2ps xmm1,xmm1
				mainTextProc3ReturnAddress1 = address + 0x12;

				Injector::MakeJMP(address, mainTextProc3, true);
			}
			else {
				e.unmatch.mainTextProc3Injector = true;
			}
		default:
			e.version.mainTextProc3Injector = true;
		}

		return e;
	}

	DllError Init(RunOptions options) {
		DllError result = {};

		result |= mainTextProc1Injector(options);
		result |= mainTextProc2Injector(options);
		result |= mainTextProc3Injector(options);

		return result;
	}
}
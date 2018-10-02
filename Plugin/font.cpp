#include "stdinc.h"
#include "byte_pattern.h"

namespace Test {

	/*-----------------------------------------------*/

	errno_t heepfree_hook() {
		// v1.25.X
		// v1.26.X
		// v1.27.X
		byte_pattern::temp_instance().find_pattern("59 85 C0 74 15 56 6A 00");
		if (byte_pattern::temp_instance().has_size(1)) {
			injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(0x7), 0x08, true);
		}
		else {
			return 1;
		}
	}

	/*-----------------------------------------------*/

	errno_t bufferClear_hook() {
		
		/* �����`���[ */
		// v1.25.X
		// v1.26.X
		// v1.27.X
		byte_pattern::temp_instance().find_pattern("68 20 28 00 00 8B 45");
		if (byte_pattern::temp_instance().has_size(1)) {
			injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(3), 0x04, true);
		}
		else {
			return 1;
		}

		/* �����`���[ */
		// v1.25.X
		// v1.26.X
		// v1.27.X
		byte_pattern::temp_instance().find_pattern("68 20 28 00 00 56 E8");
		if (byte_pattern::temp_instance().has_size(1)) {
			injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(3), 0x04, true);
		}
		else {
			return 1;
		}

		/* �{�� */
		// v1.25.X
		// v1.26.X
		// v1.27.X
		byte_pattern::temp_instance().find_pattern("68 50 28 00 00 8B 45");
		if (byte_pattern::temp_instance().has_size(1)) {
			injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(3), 0x04, true);
		}
		else {
			return 1;
		}

		/* �{�� */
		// v1.25.X
		// v1.26.X
		// v1.27.X
		byte_pattern::temp_instance().find_pattern("68 50 28 00 00 56 E8");
		if (byte_pattern::temp_instance().has_size(1)) {
			injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(3), 0x04, true);
		}
		else {
			return 1;
		}
	}

	/*-----------------------------------------------*/

	errno_t fontSize_hook() {
		// v1.25.X
		// v1.26.X
		// v1.27.X
		byte_pattern::temp_instance().find_pattern("81 FE 00 00 00 01");
		if (byte_pattern::temp_instance().has_size(1)) {
			injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(0x5), 0x04, true);
		}
		else {
			return 1;
		}
	}

	/*-----------------------------------------------*/

	errno_t bufferExp_hook() {

		/* �����`���[ */
		// v1.25.X
		// v1.26.X
		// v1.27.X
		byte_pattern::temp_instance().find_pattern("68 20 28 00 00 E8 ? ? ? ? 83");
		if (byte_pattern::temp_instance().has_size(1)) {
			injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(3), 0x04, true);
		}
		else {
			return 1;
		}

		/* �{�� */
		// v1.25.X
		// v1.26.X
		// v1.27.X
		byte_pattern::temp_instance().find_pattern("68 50 28 00 00 E8 ? ? ? ? 83");
		if (byte_pattern::temp_instance().has_size(1)) {
			injector::WriteMemory<uint8_t>(byte_pattern::temp_instance().get_first().address(3), 0x04, true);
		}
		else {
			return 1;
		}
	}

	errno_t InitAndPatchFont() {
		errno_t result = 0;

		/* �q�[�v�N���A�t���O�̏C�� */
		result |= heepfree_hook();
		/* �t�H���g�o�b�t�@�N���A */
		result |= bufferClear_hook();
		/* �t�H���g�o�b�t�@�g�� */
		result |= bufferExp_hook();
		/* �t�H���g�T�C�Y�̊g�� */
		result |= fontSize_hook();

		return result;
	}
}
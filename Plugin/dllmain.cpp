#include "stdinc.h"
#include "byte_pattern.h"

BOOL WINAPI DllMain(HMODULE module, DWORD reason, void *reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
		byte_pattern::start_log(L"eu4jps");

		// version�𕶎��񂩂�擾
		EU4Version version = Misc::getVersion();
		
		errno_t success = NOERROR;

		// �t�H���g�֘A�̏C��
		success |= Font::init(version);

		// �{���e�L�X�g�\���̏C��
		// TODO: �ςȉ��s��ʒu����o�O����
		success |= TextView::init(version);

		// �}�b�v�����ʒu����
		// �擪�ɃI�t�Z�b�g������o�O����
		success |= MapAdj::init(version);

		// �}�b�v����justify
		success |= MapJustify::init(version);

		// �}�b�v�����\��
		success |= MapView::init(version);

		// ���̑�
		success |= Misc::init(version);

		//���͏C��
		success |= Input::init(version);

		//IME�C��
		success |= IME::init(version);

		if (success == NOERROR) {
			byte_pattern::temp_instance().debug_output2("DLL [OK]");
		}
		else {
			byte_pattern::temp_instance().debug_output2("DLL [NG]");
			exit(-1);
		}
    }
    else if (reason == DLL_PROCESS_DETACH)
    {
        byte_pattern::shutdown_log();
    }

    return TRUE;
}

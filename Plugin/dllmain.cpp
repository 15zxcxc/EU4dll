#include "stdinc.h"
#include "byte_pattern.h"

BOOL WINAPI DllMain(HMODULE module, DWORD reason, void *reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
		byte_pattern::start_log(L"eu4jps");

		// version�𕶎��񂩂�擾
		EU4Version version = Misc::getVersion();

		// �I�v�V������ini�t�@�C������擾
		RunOptions options = RunOptions();
		Misc::getOptionsByINI(&options);
		
		errno_t success = NOERROR;

		// �t�H���g�֘A�̏C��
		success |= Font::init(version);

		// �{���e�L�X�g�\���̏C��
		success |= TextView::init(version);

		// �}�b�v�����ʒu����
		success |= MapAdj::init(version);

		// �}�b�v����justify
		success |= MapJustify::init(version);

		// �}�b�v�����\��
		success |= MapView::init(version);

		// ���̑�
		success |= Misc::init(version);

		// ���͏C��
		success |= Input::init(version);

		// IME�C��
		success |= IME::init(version);

		// �c�[���`�b�v�ƃ{�^��
		success |= ButtonAndToolTip::init(version);

		// �c�[���`�b�v�ǉ�����
		success |= ToolTipApx::init(version);

		// �}�b�v��̃|�b�v�A�b�v����
		success |= PopupCharOnMap::init(version);

		// issue19�̏C��
		success |= InputIssue19::init(version);

		// �C�x���g�_�C�A���O�̏C���ƃ}�b�v��̏C��
		success |= EventDialog::init(version);

		// �t�@�C���Z�[�u�֘A
		success |= FileSave::init(version);

		// DateFormat(issue66)�̏C��
		success |= DateFormat::init(version);

		// List�̕��������iissue-99�j
		//success |= ListChars::init(version);

		if (success == NOERROR && options.test == false) {
			//MessageBoxW(NULL, L"[OK]", L"Multibyte DLL", MB_OK);
			byte_pattern::temp_instance().debug_output2("DLL [OK]");
		}
		else {
//			const DWORD sysDefLcid = ::GetSystemDefaultLCID();
//			if (sysDefLcid == 1041) {
//				MessageBoxW(NULL, L"--> Go to https://paradoxian-japan-mod.com/ <--", L"Error", MB_OK);
//			}
//			else {
				MessageBoxW(NULL, L"[Multibyte DLL ERROR]\nThis game version is not supported by Multibyte DLL.\nPlease delete d3d9.dll and restart game.\nOr check new version dll.\n\nhttps://github.com/matanki-saito/EU4dll", L"Multibyte DLL", MB_OK);
//			}

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

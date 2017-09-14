#include "fbstdafx.h"
#include "VirtualPad.h"
#include "_Inputs\XInput.h"
#include "_Inputs\KeyBoard .h"

namespace fbEngine
{
	namespace {
		//���z�{�^����XBox�R���g���[���̃{�^���Ƃ̊֘A�t����\���\���́B
		struct VirtualPadToXPad {
			ButtonE vButton;		//!<���z�{�^���B
			DWORD	 xButton;		//!<XBox�R���g���[���̃{�^���B
		};

		const VirtualPadToXPad vPadToXPadTable[ButtonNum] = {
			{ ButtonUp		, XINPUT_GAMEPAD_DPAD_UP },
			{ ButtonDown		, XINPUT_GAMEPAD_DPAD_DOWN },
			{ ButtonLeft		, XINPUT_GAMEPAD_DPAD_LEFT },
			{ ButtonRight		, XINPUT_GAMEPAD_DPAD_RIGHT },
			{ ButtonA			, XINPUT_GAMEPAD_A },
			{ ButtonB			, XINPUT_GAMEPAD_B },
			{ ButtonY			, XINPUT_GAMEPAD_Y },
			{ ButtonX			, XINPUT_GAMEPAD_X },
			{ ButtonSelect	, XINPUT_GAMEPAD_BACK },
			{ ButtonStart		, XINPUT_GAMEPAD_START },
			{ ButtonRB1		, XINPUT_GAMEPAD_RIGHT_SHOULDER },
			{ ButtonRB2		, 0 },
			{ ButtonRB3		, XINPUT_GAMEPAD_RIGHT_THUMB },
			{ ButtonLB1		, XINPUT_GAMEPAD_LEFT_SHOULDER },
			{ ButtonLB2		, 0 },
			{ ButtonLB3		, XINPUT_GAMEPAD_LEFT_THUMB },
		};

		//���z�{�^���ƃL�[�{�[�h�Ƃ̊֘A�t����\���\���́B
		struct VirtualPadToKeyboard {
			ButtonE vButton;		//!<���z�{�^��
			DWORD keyCoord;			//!<�L�[�{�[�h�̃L�[�R�[�h�B
		};
		const VirtualPadToKeyboard vPadToKeyboardTable[ButtonNum] = {
			{ ButtonUp		, DIK_UP },
			{ ButtonDown	, DIK_DOWN },
			{ ButtonLeft	, DIK_LEFT },
			{ ButtonRight	, DIK_RIGHT },
			{ ButtonA		, DIK_Z },
			{ ButtonB		, DIK_X },
			{ ButtonY		, DIK_C },
			{ ButtonX		, DIK_V },
			{ ButtonSelect	, VK_SPACE },
			{ ButtonStart	, VK_RETURN },
			{ ButtonRB1		, DIK_0 },
			{ ButtonRB2		, DIK_1 },
			{ ButtonRB3		, DIK_2 },
			{ ButtonLB1		, DIK_3 },
			{ ButtonLB2		, DIK_4 },
			{ ButtonLB3		, DIK_5 },
		};
	}
}
VirtualPad::VirtualPad()
{
}

void VirtualPad::Initialize(KeyBoard* key, XInput* xbox)
{
	//�L�[�{�[�h������
	_Key = key;
	_Pad = xbox;
}

bool VirtualPad::IsPush(fbEngine::ButtonE button)
{
	return (_Pad->IsPushButton(fbEngine::vPadToXPadTable[button].xButton) || _Key->isPush(fbEngine::vPadToKeyboardTable[button].keyCoord));
}

bool VirtualPad::IsPress(fbEngine::ButtonE button)
{
	return (_Pad->IsPressButton(fbEngine::vPadToXPadTable[button].xButton) || _Key->isPressed(fbEngine::vPadToKeyboardTable[button].keyCoord));
}
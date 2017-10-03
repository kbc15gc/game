#include "fbstdafx.h"
#include "VirtualPad.h"
#include "_Inputs\XInput.h"
#include "_Inputs\KeyBoard .h"

namespace fbEngine
{
	namespace VPad
	{
		namespace {
			//仮想ボタンとXBoxコントローラのボタンとの関連付けを表す構造体。
			struct VirtualPadToXPad {
				ButtonE vButton;		//!<仮想ボタン。
				DWORD	 xButton;		//!<XBoxコントローラのボタン。
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

			//仮想ボタンとキーボードとの関連付けを表す構造体。
			struct VirtualPadToKeyboard {
				ButtonE vButton;		//!<仮想ボタン
				DWORD keyCoord;			//!<キーボードのキーコード。
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
				{ ButtonSelect	, DIK_SPACE },
				{ ButtonStart	, DIK_RETURN },
				{ ButtonRB1		, DIK_0 },
				{ ButtonRB2		, DIK_1 },
				{ ButtonRB3		, DIK_2 },
				{ ButtonLB1		, DIK_3 },
				{ ButtonLB2		, DIK_4 },
				{ ButtonLB3		, DIK_5 },
			};
		}
	}
}
VirtualPad::VirtualPad()
{
}

void VirtualPad::Initialize(KeyBoard* key, XInput* xbox)
{
	//キーボード初期化
	_Key = key;
	_Pad = xbox;
}

bool VirtualPad::IsPush(fbEngine::VPad::ButtonE button)
{
	return (_Pad->IsPushButton(fbEngine::VPad::vPadToXPadTable[button].xButton) || _Key->isPush(fbEngine::VPad::vPadToKeyboardTable[button].keyCoord));
}

bool VirtualPad::IsPress(fbEngine::VPad::ButtonE button)
{
	return (_Pad->IsPressButton(fbEngine::VPad::vPadToXPadTable[button].xButton) || _Key->isPressed(fbEngine::VPad::vPadToKeyboardTable[button].keyCoord));
}

bool VirtualPad::KeyRepeat(fbEngine::VPad::ButtonE button, float interval)
{
	return (_Pad->KeyRepeat(fbEngine::VPad::vPadToXPadTable[button].xButton, interval) || _Key->KeyRepeat(fbEngine::VPad::vPadToKeyboardTable[button].keyCoord, interval));
}

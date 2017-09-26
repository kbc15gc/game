#pragma once
//���̓f�o�C�X�̊Ǘ��Ƃ����Ă��

#include "KeyBoard .h"
#include "Mouse.h"
#include "XInput.h"
#include "VirtualPad.h"

//Xbox�R���g���[���[�̐�(�����ɏ����Ȃ��Ă����������B)
#define XBOX_CONTROLLER_NUM 4

class InputManager
{
public:
	HRESULT Initialize();
	void Update();
	KeyBoard * InputManager::GetKeyBoard()
	{
		return _KeyBoard;
	}

	Mouse * InputManager::GetMouse()
	{
		return _Mouse;
	}

	XInput * InputManager::GetXInput(int idx)
	{
		return _Xinput[idx];
	}

	VirtualPad * InputManager::GetVPad()
	{
		return _VirtualPad;
	}
	//�SXbox�R���g���[���[���̂ǂꂩ�Ŏw�肵���{�^����������Ă��邩�H
	bool IsPushButtonAll(int in);
	static InputManager* Instance();
private:
	// DirectInput
	LPDIRECTINPUT8 _Dinput;
	//�L�[�{�[�h�̓���
	KeyBoard* _KeyBoard;
	//�}�E�X�̓���
	Mouse* _Mouse;
	//Xbox�Q�[���p�b�h�̓���
	XInput* _Xinput[XBOX_CONTROLLER_NUM];
	//
	VirtualPad* _VirtualPad;
	static InputManager* _Instance;
};

#define KeyBoardInput INSTANCE(InputManager)->GetKeyBoard()
#define MouseInput INSTANCE(InputManager)->GetMouse()
#define XboxInput(idx) INSTANCE(InputManager)->GetXInput(idx)
#define VPadInput INSTANCE(InputManager)->GetVPad()
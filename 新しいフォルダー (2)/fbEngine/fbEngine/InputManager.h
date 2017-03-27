#pragma once
//���̓f�o�C�X�̊Ǘ��Ƃ����Ă��

#include "KeyBoard .h"
#include "Mouse.h"
#include "XInput.h"

//Xbox�R���g���[���[�̐�(�����ɏ����Ȃ��Ă����������B)
#define XBOX_CONTROLLER_NUM 4

class InputManager
{
public:
	HRESULT Initialize();
	void Update();
	KeyBoard* GetKeyBoard();
	Mouse* GetMouse();
	XInput* GetXInput(int idx);
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
	static InputManager* _Instance;
};

#define KeyBoardInput INSTANCE(InputManager)->GetKeyBoard()
#define MouseInput INSTANCE(InputManager)->GetMouse()
#define XboxInput(idx) INSTANCE(InputManager)->GetXInput(idx)
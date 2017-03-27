#include "InputManager.h"

InputManager* InputManager::_Instance = nullptr;

HRESULT InputManager::Initialize()
{
	if (FAILED(DirectInput8Create(
		GetModuleHandle(NULL),	// �C���X�^���X �n���h��
		DIRECTINPUT_VERSION,	// DirectInput�̃o�[�W����
		IID_IDirectInput8,		// �擾�C���^�[�t�F�C�X�̃^�C�v
		(VOID**)&_Dinput,		// �C���^�[�t�F�C�X�|�C���^
		NULL)))					// �g��Ȃ�
	{
		return false;
	}
	//�L�[�{�[�h������
	_KeyBoard = new KeyBoard();
	if (FAILED(_KeyBoard->Initialize(_Dinput)))
	{
		SAFE_DELETE(_KeyBoard);
		return false;
	}

	//�}�E�X������
	_Mouse = new Mouse();
	if (FAILED(_Mouse->Initialize(_Dinput)))
	{
		SAFE_DELETE(_Mouse);
		return false;
	}

	FOR(4)
	{
		_Xinput[i] = new XInput;
		_Xinput[i]->Initialize(i);
	}

	return D3D_OK;
}

void InputManager::Update()
{
	_KeyBoard->Update();
	_Mouse->Update();
	FOR(4)
		_Xinput[i]->Update();
}

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

bool InputManager::IsPushButtonAll(int in)
{
	FOR(XBOX_CONTROLLER_NUM)
	{
		if (GetXInput(i)->IsPushButton(in))
		{
			return true;
		}
	}
	return false;
}

InputManager* InputManager::Instance()
{
	if(_Instance == nullptr)
	{
		_Instance = new InputManager();
	}

	return _Instance;
}
